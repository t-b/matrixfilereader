/*
	The file brickletconverter.cpp is part of the "MatrixFileReader XOP".
	It is licensed under the LGPLv3 with additional permissions,
	see License.txt	in the source folder for details.
*/

#include "header.h"
#include "wavedata.h"

#include "brickletconverter.h"

#include <string>
#include <sstream>
#include <vector>

#include "utils.h"
#include "globaldata.h"

int createWaves(DataFolderHandle dfHandle, const char *waveBaseNameChar, int brickletID, bool resampleData, int pixelSize, std::string &fullPathOfCreatedWave){

	int dimension;
	std::vector<Vernissage::Session::ViewTypeCode> viewTypeCodes;
	Vernissage::Session *pSession;
	std::vector<std::string> allAxes;
	Vernissage::Session::AxisDescriptor triggerAxis, rootAxis;
	int numPointsTriggerAxis=-1, numPointsRootAxis=-1, ret=-1, i, j,k;
	waveHndl waveHandle;

	const int *rawBrickletDataPtr;
	int rawBrickletSize=0, waveSize=0, firstBlockOffset=0, triggerAxisBlockSize=0;

	int traceUpRawBrickletIndex, traceUpDataIndex,reTraceUpDataIndex,reTraceUpRawBrickletIndex, traceDownRawBrickletIndex,traceDownDataIndex, reTraceDownRawBrickletIndex,reTraceDownDataIndex;

	const double zeroSetScaleOffset=0.0;

	int rawValue;
	double scaledValue;

	CountInt dimensionSizes[MAX_DIMENSIONS+1], interpolatedDimSizes[MAX_DIMENSIONS+1];
	MemClear(dimensionSizes, sizeof(dimensionSizes));
	MemClear(interpolatedDimSizes, sizeof(interpolatedDimSizes));

	char cmd[ARRAY_SIZE];
	char dataFolderPath[MAXCMDLEN+1];
	int numDimensions;
	double physIncrement;

	std::string waveBaseName(waveBaseNameChar);

	BrickletClass* const bricklet = globDataPtr->getBrickletClassObject(brickletID);

	ASSERT_RETURN_ONE(bricklet);
	void *pBricklet = bricklet->getBrickletPointer();

	ASSERT_RETURN_ONE(pBricklet);
	pSession = globDataPtr->getVernissageSession();

	ASSERT_RETURN_ONE(pSession);

	dimension = bricklet->getMetaDataValueAsInt("dimension");
	bricklet->getAxes(allAxes);
	bricklet->getViewTypeCodes(viewTypeCodes);

	sprintf(globDataPtr->outputBuffer,"### BrickletID %d ###",brickletID);
	debugOutputToHistory(globDataPtr->outputBuffer);

	sprintf(globDataPtr->outputBuffer,"dimension %d",dimension);
	debugOutputToHistory(globDataPtr->outputBuffer);

	std::vector<Vernissage::Session::ViewTypeCode>::const_iterator itViewTypeCodes;
	for(itViewTypeCodes = viewTypeCodes.begin(); itViewTypeCodes != viewTypeCodes.end(); itViewTypeCodes++){
		sprintf(globDataPtr->outputBuffer,"viewType %s",viewTypeCodeToString(*itViewTypeCodes).c_str());
		debugOutputToHistory(globDataPtr->outputBuffer);
	}
	
	debugOutputToHistory("Axis order is from triggerAxis to rootAxis");

	std::vector<std::string>::const_iterator itAllAxes;	
	for(itAllAxes = allAxes.begin(); itAllAxes != allAxes.end(); itAllAxes++){
		sprintf(globDataPtr->outputBuffer,"Axis %s",itAllAxes->c_str());
		debugOutputToHistory(globDataPtr->outputBuffer);
	}

	MyWave myWaveData[MAX_NUM_TRACES];
	MyWave *traceUpData = &myWaveData[TRACE_UP];
	MyWave *reTraceUpData = &myWaveData[RE_TRACE_UP];
	MyWave *traceDownData = &myWaveData[TRACE_DOWN];
	MyWave *reTraceDownData = &myWaveData[RE_TRACE_DOWN];

	MyWave myWaveData1D;

	// pointer to raw data
	bricklet->getBrickletContentsBuffer(&rawBrickletDataPtr ,rawBrickletSize);
	if(rawBrickletSize == 0 || &rawBrickletDataPtr == NULL){
		outputToHistory("Could not load bricklet contents.");
		return UNKNOWN_ERROR;
	}

	sprintf(globDataPtr->outputBuffer,"rawBrickletDataPtr =%p,count=%d",&rawBrickletDataPtr,rawBrickletSize);
	debugOutputToHistory(globDataPtr->outputBuffer);

	ASSERT_RETURN_ONE(rawBrickletDataPtr);

	// create data for raw->scaled transformation
	// the min and max values here are for the complete bricklet data and not only for one wave
	int xOne, xTwo;
	double yOne, yTwo, slope, yIntercept;
	
	xOne = bricklet->getExtrema().getRawMin();
	xTwo = bricklet->getExtrema().getRawMax();
	yOne = bricklet->getExtrema().getPhysValRawMin();
	yTwo = bricklet->getExtrema().getPhysValRawMax();

	// usually xOne is not equal to xTwo
	if(xOne != xTwo){
		slope = (yOne - yTwo) / (xOne*1.0 - xTwo*1.0);
		yIntercept = yOne - slope*xOne;
	}
	else{
		// but if it is we have to do something different
		// xOne == xTwo means that the minimum is equal to the maximum, so the data is everywhere yOne == yTwo aka constant
		slope = 0;
		yIntercept = yOne;
	}

	sprintf(globDataPtr->outputBuffer,"raw->scaled transformation: xOne=%d,xTwo=%d,yOne=%g,yTwo=%g",xOne,xTwo,yOne,yTwo);
	debugOutputToHistory(globDataPtr->outputBuffer);

	sprintf(globDataPtr->outputBuffer,"raw->scaled transformation: slope=%g,yIntercept=%g",slope,yIntercept);
	debugOutputToHistory(globDataPtr->outputBuffer);

	if( dimension < 1 || dimension > 3 ){
		sprintf(globDataPtr->outputBuffer,"Dimension %d can not be handled. Please file a bug report and attach the measured data.",dimension);
		outputToHistory(globDataPtr->outputBuffer);
		return INTERNAL_ERROR_CONVERTING_DATA;
	}

	switch(dimension){
	
		case 1:

			triggerAxis = pSession->getAxisDescriptor(pBricklet,pSession->getTriggerAxisName(pBricklet));
			numPointsTriggerAxis = triggerAxis.clocks;
			
			if (triggerAxis.mirrored){
				numPointsTriggerAxis /= 2;
			}
			dimensionSizes[ROWS] = numPointsTriggerAxis;

			myWaveData1D.setNameAndTraceDir(waveBaseName,NO_TRACE);

			ret = MDMakeWave(&waveHandle, myWaveData1D.getWaveName(),dfHandle,dimensionSizes,globDataPtr->getIgorWaveType(),globDataPtr->overwriteEnabledAsInt());
			if(ret == NAME_WAV_CONFLICT){
				sprintf(globDataPtr->outputBuffer,"Wave %s already exists.",myWaveData1D.getWaveName());
				debugOutputToHistory(globDataPtr->outputBuffer);
				return WAVE_EXIST;
			}
			else if(ret != 0 ){
				sprintf(globDataPtr->outputBuffer,"Error %d in creating wave %s.",ret, myWaveData1D.getWaveName());
				outputToHistory(globDataPtr->outputBuffer);
				return UNKNOWN_ERROR;
			}

			ASSERT_RETURN_ONE(waveHandle);

			myWaveData1D.setWaveDataPtr(waveHandle);
			myWaveData1D.clearWave();
			waveHandle = NULL;

			for(i=0; i < MAX_NUM_TRACES; i++){
				myWaveData[i].printDebugInfo();
			}

			if(!myWaveData1D.moreData){
				return UNKNOWN_ERROR;
			}

			for(i=0; i < numPointsTriggerAxis ; i++){
				rawValue	= rawBrickletDataPtr[i];
				scaledValue = rawValue*slope + yIntercept;
				myWaveData1D.fillWave(i,rawValue,scaledValue);
			}
			setDataWaveNote(brickletID,myWaveData1D);

			MDSetWaveScaling(myWaveData1D.getWaveHandle(),ROWS,&triggerAxis.physicalIncrement,&triggerAxis.physicalStart);
			
			MDSetWaveUnits(myWaveData1D.getWaveHandle(),ROWS,WStringToString(triggerAxis.physicalUnit).c_str());
			MDSetWaveUnits(myWaveData1D.getWaveHandle(),DATA,bricklet->getMetaDataValueAsString("channelUnit").c_str());

			fullPathOfCreatedWave.append(getFullWavePath(dfHandle,myWaveData1D.getWaveHandle()));
			fullPathOfCreatedWave.append(";");

			break;

		case 2:

			// Two dimensions, probably an image
			triggerAxis = pSession->getAxisDescriptor(pBricklet,pSession->getTriggerAxisName(pBricklet));
			
			// Determine the length of one "line" of data
			numPointsTriggerAxis = triggerAxis.clocks;
			
			if (triggerAxis.mirrored)
			{
				// The axis has the "mirrored" characteristic, thus it has a
				// "forward" and a "backward" section. Thus, the length of one line
				// is only half the number of clocks that triggered the channel.
				numPointsTriggerAxis /= 2;
			}

			// There must be another axis, because the Bricklet has two dimensions:
			rootAxis = pSession->getAxisDescriptor(pBricklet,triggerAxis.triggerAxisName);

			// Determine the length of one "line" of data
			numPointsRootAxis = rootAxis.clocks;

			if (rootAxis.mirrored)
			{
				// The axis has the "mirrored" characteristic, thus it has a
				// "forward" and a "backward" section. Thus, the length of one line
				// is only half the number of clocks that triggered the channel.
				numPointsRootAxis/= 2;
			}

			sprintf(globDataPtr->outputBuffer,"numPointsRootAxis=%d",numPointsRootAxis);
			debugOutputToHistory(globDataPtr->outputBuffer);

			sprintf(globDataPtr->outputBuffer,"numPointsTriggerAxis=%d",numPointsTriggerAxis);
			debugOutputToHistory(globDataPtr->outputBuffer);

			dimensionSizes[ROWS] = numPointsTriggerAxis;
			dimensionSizes[COLUMNS] = numPointsRootAxis;
			waveSize = dimensionSizes[ROWS]*dimensionSizes[COLUMNS];

			// now we have to disinguish three cases on how many 2D waves we need
			// both are mirrored:		4
			// one mirrored, one not:	2
			// none mirrored:			1
			if( triggerAxis.mirrored && rootAxis.mirrored ){
				
				triggerAxisBlockSize = 2*numPointsTriggerAxis;

				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
				myWaveData[RE_TRACE_UP].setNameAndTraceDir(waveBaseName,RE_TRACE_UP);
				myWaveData[TRACE_DOWN].setNameAndTraceDir(waveBaseName,TRACE_DOWN);
				myWaveData[RE_TRACE_DOWN].setNameAndTraceDir(waveBaseName,RE_TRACE_DOWN);
			}
			else if( triggerAxis.mirrored ){

				triggerAxisBlockSize = 2*numPointsTriggerAxis;

				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
				myWaveData[RE_TRACE_UP].setNameAndTraceDir(waveBaseName,RE_TRACE_UP);
			}
			else if( rootAxis.mirrored ){

				triggerAxisBlockSize = numPointsTriggerAxis;

				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
				myWaveData[TRACE_DOWN].setNameAndTraceDir(waveBaseName,TRACE_DOWN);
			}
			else{
				triggerAxisBlockSize = numPointsTriggerAxis;

				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
			}

			for(i=0; i < MAX_NUM_TRACES; i++){
				// skip empty entries
				if(myWaveData[i].isEmpty()){
					continue;
				}
				ret = MDMakeWave(&waveHandle, myWaveData[i].getWaveName(),dfHandle,dimensionSizes,globDataPtr->getIgorWaveType(),globDataPtr->overwriteEnabledAsInt());

				if(ret == NAME_WAV_CONFLICT){
					sprintf(globDataPtr->outputBuffer,"Wave %s already exists.",myWaveData[i].getWaveName());
					debugOutputToHistory(globDataPtr->outputBuffer);
					return WAVE_EXIST;
				}
				else if(ret != 0 ){
					sprintf(globDataPtr->outputBuffer,"Error %d in creating wave %s.",ret, myWaveData[i].getWaveName());
					outputToHistory(globDataPtr->outputBuffer);
					return UNKNOWN_ERROR;
				}

				ASSERT_RETURN_ONE(waveHandle);
				myWaveData[i].setWaveDataPtr(waveHandle);
				myWaveData[i].clearWave();
				waveHandle=NULL;
			}

			firstBlockOffset = numPointsRootAxis * triggerAxisBlockSize;

			for(i=0; i < MAX_NUM_TRACES; i++){
				myWaveData[i].printDebugInfo();
			}

			// See also Vernissage manual page 22f
			// triggerAxisBlockSize: number of points in the raw data array which were acquired at the same root axis position
			// firstBlockOffset: offset position where the traceDown data starts

			// *RawBrickletIndex: source index into the raw data vernissage
			// *DataIndex: destination index into the igor wave (the funny index tricks are needed because of the organization of the wave in the memory)

			// data layout of igor waves in memory (Igor XOP Manual p. 238)
			// - the wave is linear in the memory
			// - going along the arrray will first fill the first column from row 0 to end and then the second column and so on

			//// TraceUp aka Forward/Up
			//// ReTraceUp aka Backward/Up
			//// TraceDown aka Forward/Down
			//// ReTraceDown aka Backward/Down
			//// horizontal axis aka X axis in Pascal's Scala Routines aka triggerAxis 		aka 	ROWS
			//// vertical   axis aka Y axis in Pascal's Scala Routines aka rootAxis 		aka		COLUMNS

			// COLUMNS
			for(i = 0; i < numPointsRootAxis; i++){
				// ROWS
				for(j=0; j < numPointsTriggerAxis; j++){

					// traceUp
					if(traceUpData->moreData){
						traceUpRawBrickletIndex			= i*triggerAxisBlockSize+ j;
						traceUpDataIndex				= i*numPointsTriggerAxis   + j;

						if(	traceUpDataIndex >= 0 &&
							traceUpDataIndex < waveSize &&
							traceUpRawBrickletIndex < rawBrickletSize &&
							traceUpRawBrickletIndex >= 0
							){
								rawValue	= rawBrickletDataPtr[traceUpRawBrickletIndex];
								scaledValue = rawValue*slope + yIntercept;

								traceUpData->fillWave(traceUpDataIndex,rawValue,scaledValue);
						}
						else{
							debugOutputToHistory("Index out of range in traceUp");

							sprintf(globDataPtr->outputBuffer,"traceUpDataIndex=%d,waveSize=%d",traceUpDataIndex,waveSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							sprintf(globDataPtr->outputBuffer,"traceUpRawBrickletIndex=%d,rawBrickletSize=%d",traceUpRawBrickletIndex,rawBrickletSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							traceUpData->moreData = false;
						}
					}

					// traceDown
					if(traceDownData->moreData){

						traceDownRawBrickletIndex	= firstBlockOffset + i*triggerAxisBlockSize + j;
						// compared to the traceUpData->dbl the index i is shifted
						// this takes into account that the data in the traceDown is aquired from the highest y value to the lowest y value
						traceDownDataIndex			= ( numPointsRootAxis -( i+1) ) * numPointsTriggerAxis   + j;

						if(	traceDownDataIndex >= 0 &&
							traceDownDataIndex < waveSize &&
							traceDownRawBrickletIndex < rawBrickletSize &&
							traceDownRawBrickletIndex >= 0
							){
								rawValue	= rawBrickletDataPtr[traceDownRawBrickletIndex];
								scaledValue = rawValue*slope + yIntercept;

								traceDownData->fillWave(traceDownDataIndex,rawValue,scaledValue);
						}
						else{
							debugOutputToHistory("Index out of range in traceDown");

							sprintf(globDataPtr->outputBuffer,"traceDownDataIndex=%d,waveSize=%d",traceDownDataIndex,waveSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							sprintf(globDataPtr->outputBuffer,"traceDownRawBrickletIndex=%d,rawBrickletSize=%d",traceDownRawBrickletIndex,rawBrickletSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							traceDownData->moreData = false;
						}
					}

					// reTraceUp
					if(reTraceUpData->moreData){

						// here we shift the j index, because the data is now acquired from high column number to low column number
						reTraceUpRawBrickletIndex	= i*triggerAxisBlockSize + triggerAxisBlockSize - (j+1);
						reTraceUpDataIndex			= i *  numPointsTriggerAxis + j;

						if(	reTraceUpDataIndex >= 0 &&
							reTraceUpDataIndex < waveSize &&
							reTraceUpRawBrickletIndex < rawBrickletSize &&
							reTraceUpRawBrickletIndex >= 0
							){
								rawValue	= rawBrickletDataPtr[reTraceUpRawBrickletIndex];
								scaledValue = rawValue*slope + yIntercept;

								reTraceUpData->fillWave(reTraceUpDataIndex,rawValue,scaledValue);
						}
						else{
							debugOutputToHistory("Index out of range in reTraceUp");

							sprintf(globDataPtr->outputBuffer,"reTraceUpDataIndex=%d,waveSize=%d",reTraceUpDataIndex,waveSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							sprintf(globDataPtr->outputBuffer,"reTraceUpRawBrickletIndex=%d,rawBrickletSize=%d",reTraceUpRawBrickletIndex,rawBrickletSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							reTraceUpData->moreData = false;
						}
					}

					// reTraceDown
					if(reTraceDownData->moreData){

						reTraceDownRawBrickletIndex		= firstBlockOffset + i*triggerAxisBlockSize + triggerAxisBlockSize - (j+1);
						reTraceDownDataIndex			= ( numPointsRootAxis -( i+1) ) * numPointsTriggerAxis   + j;

						if(	reTraceDownDataIndex >= 0 &&
							reTraceDownDataIndex < waveSize &&
							reTraceDownRawBrickletIndex < rawBrickletSize &&
							reTraceDownRawBrickletIndex >= 0
							){

								rawValue	= rawBrickletDataPtr[reTraceDownRawBrickletIndex];
								scaledValue = rawValue*slope + yIntercept;
								reTraceDownData->fillWave(reTraceDownDataIndex,rawValue,scaledValue);
						}
						else{
							debugOutputToHistory("Index out of range in reTraceDown");

							sprintf(globDataPtr->outputBuffer,"reTraceDownDataIndex=%d,waveSize=%d",reTraceDownDataIndex,waveSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							sprintf(globDataPtr->outputBuffer,"reTraceDownRawBrickletIndex=%d,rawBrickletSize=%d",reTraceDownRawBrickletIndex,rawBrickletSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							reTraceDownData->moreData = false;
						}
					}
				}
			}

			for(i=0; i < MAX_NUM_TRACES; i++){
				if(myWaveData[i].isEmpty()){
					continue;
				}

				if( resampleData ){
					myWaveData[i].pixelSize = pixelSize;

					sprintf(globDataPtr->outputBuffer,"Resampling wave %s with pixelSize=%d",myWaveData[i].getWaveName(),pixelSize);
					debugOutputToHistory(globDataPtr->outputBuffer);

					// flag=3 results in the full path being returned including a trailing colon
					ret = GetDataFolderNameOrPath(dfHandle, 3, dataFolderPath);
					if(ret != 0){
						return ret;
					}
					// The "ImageInterpolate [...] Pixelate" command is used here
					sprintf(cmd,"ImageInterpolate/PXSZ={%d,%d}/DEST=%sM_PixelatedImage Pixelate %s",\
						pixelSize,pixelSize,dataFolderPath,dataFolderPath);
					// quote waveName properly, it might be a liberal name
					CatPossiblyQuotedName(cmd,myWaveData[i].getWaveName());
					if(globDataPtr->debuggingEnabled()){
						debugOutputToHistory(cmd);
					}
					ret = XOPSilentCommand(cmd);
					if(ret != 0){
						sprintf(globDataPtr->outputBuffer,"The command _%s_ failed to execute. So the XOP has to be fixed...",cmd);
						outputToHistory(globDataPtr->outputBuffer);
						continue;
					}

					// kill the un-interpolated wave and invalidate waveHandeVector[i]
					ret = KillWave(myWaveData[i].getWaveHandle());
					if(ret != 0){
						return ret;
					}
					// rename wave from M_PixelatedImage to waveNameVector[i] both sitting in dfHandle
					ret = RenameDataFolderObject(dfHandle,WAVE_OBJECT,"M_PixelatedImage",myWaveData[i].getWaveName());
					if(ret != 0){
						return ret;
					}
					myWaveData[i].setWaveDataPtr(FetchWaveFromDataFolder(dfHandle,myWaveData[i].getWaveName()));
					ASSERT_RETURN_ONE(myWaveData[i].getWaveHandle());
					// get wave dimensions; needed for setScale below
					MDGetWaveDimensions(myWaveData[i].getWaveHandle(),&numDimensions,interpolatedDimSizes);
				}
		
				// set wave note and add info about resampling to the wave note
				setDataWaveNote(brickletID,myWaveData[i]);

				//  also the wave scaling changes if we have resampled the data
				if( resampleData ){
					physIncrement = triggerAxis.physicalIncrement*double(dimensionSizes[ROWS])/double(interpolatedDimSizes[ROWS]);
					MDSetWaveScaling(myWaveData[i].getWaveHandle(),ROWS,&physIncrement,&zeroSetScaleOffset);

					physIncrement = rootAxis.physicalIncrement*double(dimensionSizes[COLUMNS])/double(interpolatedDimSizes[COLUMNS]);
					MDSetWaveScaling(myWaveData[i].getWaveHandle(),COLUMNS,&physIncrement,&zeroSetScaleOffset);
				}
				else{// original image
					MDSetWaveScaling(myWaveData[i].getWaveHandle(),ROWS,&triggerAxis.physicalIncrement,&zeroSetScaleOffset);
					MDSetWaveScaling(myWaveData[i].getWaveHandle(),COLUMNS,&rootAxis.physicalIncrement,&zeroSetScaleOffset);
				}

				MDSetWaveUnits(myWaveData[i].getWaveHandle(),ROWS,WStringToString(triggerAxis.physicalUnit).c_str());
				MDSetWaveUnits(myWaveData[i].getWaveHandle(),COLUMNS,WStringToString(rootAxis.physicalUnit).c_str());
				MDSetWaveUnits(myWaveData[i].getWaveHandle(),DATA,bricklet->getMetaDataValueAsString("channelUnit").c_str());

				fullPathOfCreatedWave.append(getFullWavePath(dfHandle,myWaveData[i].getWaveHandle()));
				fullPathOfCreatedWave.append(";");
			}
			break;

		case 3:
			// V triggerAxis -> V is triggered by X , X is triggered by Y and Y is the root axis

			// check for correct view type codes
			int found=0;
			for(itViewTypeCodes = viewTypeCodes.begin(); itViewTypeCodes != viewTypeCodes.end(); itViewTypeCodes++){
				if(*itViewTypeCodes == Vernissage::Session::vtc_Spectroscopy){
					found +=1;
				}
				if(*itViewTypeCodes == Vernissage::Session::vtc_2Dof3D){
					found +=2;
				}
			}

			if(found != 3){
				debugOutputToHistory("The 3D data is not of the type vtc_2Dof3D and vtc_Spectroscopy.");
			}

			Vernissage::Session::AxisDescriptor specAxis = pSession->getAxisDescriptor(pBricklet,pSession->getTriggerAxisName(pBricklet));
			Vernissage::Session::AxisDescriptor xAxis = pSession->getAxisDescriptor(pBricklet, specAxis.triggerAxisName);
			Vernissage::Session::AxisDescriptor yAxis = pSession->getAxisDescriptor(pBricklet, xAxis.triggerAxisName);
			Vernissage::Session::AxisTableSets sets = pSession->getAxisTableSets(pBricklet, pSession->getTriggerAxisName(pBricklet));

			Vernissage::Session::TableSet xSet = sets[specAxis.triggerAxisName];
			Vernissage::Session::TableSet ySet = sets[xAxis.triggerAxisName];

			double xAxisIncrement = xAxis.physicalIncrement;
			double yAxisIncrement = yAxis.physicalIncrement;
			
			// normally we have table sets with some step width >1, so the physicalIncrement has to be multiplied by this factor
			// Note: this approach assumes that all axis table sets have the same physical step width, this is at least the case for Matrix 2.2-1
			if( ySet.size() > 0){
				yAxisIncrement *= ySet.begin()->step;
			}
			if( xSet.size() > 0 ){
				xAxisIncrement *= xSet.begin()->step;
			}
			//FIXME we have also a dimoffset which depends on the traceDirection

			if(globDataPtr->debuggingEnabled()){
				Vernissage::Session::TableSet::const_iterator it;

				sprintf(globDataPtr->outputBuffer,"Number of axes we have table sets for: %d",sets.size());
				debugOutputToHistory(globDataPtr->outputBuffer);

				debugOutputToHistory("Tablesets: xAxis");
				for( it= xSet.begin(); it != xSet.end(); it++){
					sprintf(globDataPtr->outputBuffer,"start=%d,step=%d,stop=%d",it->start,it->step,it->stop);
					debugOutputToHistory(globDataPtr->outputBuffer);
				}

				debugOutputToHistory("Tablesets: yAxis");
				for( it= ySet.begin(); it != ySet.end(); it++){
					sprintf(globDataPtr->outputBuffer,"start=%d,step=%d,stop=%d",it->start,it->step,it->stop);
					debugOutputToHistory(globDataPtr->outputBuffer);
				}

				sprintf(globDataPtr->outputBuffer,"xAxisIncrement=%g,yAxisIncrement=%g",xAxisIncrement,yAxisIncrement);
				debugOutputToHistory(globDataPtr->outputBuffer);
			}

			int xAxisBlockSize=0,yAxisBlockSize=0;

			// V Axis
			int numPointsVAxis = specAxis.clocks;
			
			if (specAxis.mirrored){
				numPointsVAxis /= 2;
			}

			// X Axis
			int numPointsXAxis = xAxis.clocks;

			if(xAxis.mirrored){
				numPointsXAxis /= 2;
			}

			// Y Axis
			int numPointsYAxis = yAxis.clocks;
			
			if(yAxis.mirrored){
				numPointsYAxis /= 2;
			}

			// Determine how much space the data occupies in X and Y direction
			// For that we have to take into account the table sets
			// Then we also know how many 3D cubes we have, we can have 1,2 or 4. the same as in the 2D case

			Vernissage::Session::TableSet::const_iterator yIt,xIt;
			int tablePosX,tablePosY;

			// BEGIN Borrowed from SCALA exporter plugin

			// Determine the number of different x values

			// number of y axis points with taking the axis table sets into account
			int numPointsXAxisWithTableBoth = 0;
			// the part of numPointsXAxisWithTableBoth which is used in traceUp
			int numPointsXAxisWithTableFWD = 0;
			// the part of numPointsXAxisWithTableBoth which is used in reTraceUp
			int numPointsXAxisWithTableBWD = 0;

			bool forward;

			xIt = xSet.begin();
			tablePosX = xIt->start;
	
			while (xIt != xSet.end())
			{
				numPointsXAxisWithTableBoth++;
				forward = (tablePosX <= numPointsXAxis);

				if (forward){
					numPointsXAxisWithTableFWD++;
				}
				else{
					numPointsXAxisWithTableBWD++;
				}

				tablePosX += xIt->step;

				if (tablePosX > xIt->stop){
					++xIt;

					if(xIt != xSet.end()){
						tablePosX = xIt->start;
					}
				}
			}

			// Determine the number of different y values
			
			// number of y axis points with taking the axis table sets into account
			int numPointsYAxisWithTableBoth = 0;
			// the part of numPointsYAxisWithTableBoth which is used in traceUp
			int numPointsYAxisWithTableUp   = 0;
			// the part of numPointsYAxisWithTableBoth which is used in traceDown
			int numPointsYAxisWithTableDown = 0;

			bool up;

			yIt = ySet.begin();
			tablePosY = yIt->start;
	
			while (yIt != ySet.end())
			{
				numPointsYAxisWithTableBoth++;

				up = (tablePosY <= numPointsYAxis);

				if (up){
					numPointsYAxisWithTableUp++;
				}
				else{
					numPointsYAxisWithTableDown++;
				}

				tablePosY += yIt->step;

				if (tablePosY > yIt->stop){
					++yIt;

					if(yIt != ySet.end()){
						tablePosY = yIt->start;
					}
				}
			}

			// END Borrowed from SCALA exporter plugin

			sprintf(globDataPtr->outputBuffer,"V Axis # points: %d",numPointsVAxis);
			debugOutputToHistory(globDataPtr->outputBuffer);

			sprintf(globDataPtr->outputBuffer,"X Axis # points with tableSet: Total=%d, Forward=%d, Backward=%d",
				numPointsXAxisWithTableBoth,numPointsXAxisWithTableFWD,numPointsXAxisWithTableBWD);
			debugOutputToHistory(globDataPtr->outputBuffer);

			sprintf(globDataPtr->outputBuffer,"Y Axis # points with tableSet: Total=%d, Up=%d, Down=%d",
				numPointsYAxisWithTableBoth,numPointsYAxisWithTableUp,numPointsYAxisWithTableDown);
			debugOutputToHistory(globDataPtr->outputBuffer);

			// FIXME Theoretical the sizes of the cubes could be different but we are igoring that for now
			if(numPointsXAxisWithTableBWD != 0 && numPointsXAxisWithTableFWD != 0 && numPointsXAxisWithTableFWD != numPointsXAxisWithTableBWD){
				sprintf(globDataPtr->outputBuffer,"BUG: Number of X axis points is different in forward and backward direction. Keep fingers crossed.");
				outputToHistory(globDataPtr->outputBuffer);
			}
			if(numPointsYAxisWithTableUp != 0 && numPointsYAxisWithTableDown != 0 && numPointsYAxisWithTableUp != numPointsYAxisWithTableDown){
				sprintf(globDataPtr->outputBuffer,"BUG: Number of Y axis points is different in up and down direction. Keep fingers crossed.");
				outputToHistory(globDataPtr->outputBuffer);
			}

			// dimensions of the cube
			if(numPointsXAxisWithTableFWD != 0)
				dimensionSizes[ROWS]	= numPointsXAxisWithTableFWD;
			else{
				// we only scanned in BWD direction
				dimensionSizes[ROWS]	= numPointsXAxisWithTableBWD;
			}

			//we must always scan in Up direction
			dimensionSizes[COLUMNS] = numPointsYAxisWithTableUp;
			dimensionSizes[LAYERS]  = numPointsVAxis;

			waveSize = dimensionSizes[ROWS]*dimensionSizes[COLUMNS]*dimensionSizes[LAYERS];

			sprintf(globDataPtr->outputBuffer,"dimensions of the cube: rows=%d,cols=%d,layers=%d",
				dimensionSizes[ROWS],dimensionSizes[COLUMNS],dimensionSizes[LAYERS]);
			debugOutputToHistory(globDataPtr->outputBuffer);

			// 4 cubes, TraceUp, TraceDown, ReTraceUp, ReTraceDown
			if(	numPointsXAxisWithTableFWD != 0 && numPointsXAxisWithTableBWD != 0 &&
				numPointsYAxisWithTableUp != 0 && numPointsYAxisWithTableUp != 0){
				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
				myWaveData[RE_TRACE_UP].setNameAndTraceDir(waveBaseName,RE_TRACE_UP);
				myWaveData[TRACE_DOWN].setNameAndTraceDir(waveBaseName,TRACE_DOWN);
				myWaveData[RE_TRACE_DOWN].setNameAndTraceDir(waveBaseName,RE_TRACE_DOWN);
			}
			// 2 cubes, TraceUp, TraceDown
			else if(numPointsXAxisWithTableBWD == 0 && numPointsYAxisWithTableDown != 0){
				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
				myWaveData[TRACE_DOWN].setNameAndTraceDir(waveBaseName,TRACE_DOWN);
			}
			// 2 cubes, TraceUp, ReTraceUp
			else if(numPointsXAxisWithTableBWD != 0 && numPointsYAxisWithTableDown == 0){
				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
				myWaveData[RE_TRACE_UP].setNameAndTraceDir(waveBaseName,RE_TRACE_UP);
			}
			// 2 cubes, ReTraceUp, ReTraceDown
			else if(numPointsXAxisWithTableFWD == 0 && numPointsYAxisWithTableDown != 0){
				myWaveData[RE_TRACE_UP].setNameAndTraceDir(waveBaseName,RE_TRACE_UP);
				myWaveData[RE_TRACE_DOWN].setNameAndTraceDir(waveBaseName,RE_TRACE_DOWN);
			}
			// 1 cube, TraceUp
			else if(numPointsXAxisWithTableBWD == 0 && numPointsYAxisWithTableDown == 0){
				myWaveData[TRACE_UP].setNameAndTraceDir(waveBaseName,TRACE_UP);
			}
			// 1 cube, ReTraceUp
			else if(numPointsXAxisWithTableFWD == 0 && numPointsYAxisWithTableDown == 0){
				myWaveData[RE_TRACE_UP].setNameAndTraceDir(waveBaseName,RE_TRACE_UP);
			}
			// not possible
			else{
				outputToHistory("BUG: Error in determining the number of cubes.");
				return INTERNAL_ERROR_CONVERTING_DATA;
			}
			// numPointsXAxisWithTableBWD or numPointsXAxisWithTableFWD being zero makes it correct
			xAxisBlockSize	   = ( numPointsXAxisWithTableBWD + numPointsXAxisWithTableFWD ) * numPointsVAxis;

			// data index to the start of the TraceDown data (this is the same for all combinations as xAxisBlockSize is set apropriately), in case traceDown does not exist this is no problem
			firstBlockOffset = numPointsYAxisWithTableUp*xAxisBlockSize;

			sprintf(globDataPtr->outputBuffer,"xAxisBlockSize=%d,firstBlockOffset=%d",xAxisBlockSize,firstBlockOffset);
			debugOutputToHistory(globDataPtr->outputBuffer);

			for(i=0; i < MAX_NUM_TRACES; i++){
				// skip empty entries
				if(myWaveData[i].isEmpty()){
					continue;
				}

				ret = MDMakeWave(&waveHandle, myWaveData[i].getWaveName(),dfHandle,dimensionSizes,globDataPtr->getIgorWaveType(),globDataPtr->overwriteEnabledAsInt());

				if(ret == NAME_WAV_CONFLICT){
					sprintf(globDataPtr->outputBuffer,"Wave %s already exists.",myWaveData[i].getWaveName());
					debugOutputToHistory(globDataPtr->outputBuffer);
					return WAVE_EXIST;
				}
				else if(ret != 0 ){
					sprintf(globDataPtr->outputBuffer,"Error %d in creating wave %s.",ret, myWaveData[i].getWaveName());
					outputToHistory(globDataPtr->outputBuffer);
					return UNKNOWN_ERROR;
				}
				ASSERT_RETURN_ONE(waveHandle);
				myWaveData[i].setWaveDataPtr(waveHandle);
				myWaveData[i].clearWave();
				waveHandle = NULL;
			}

			for(i=0; i < MAX_NUM_TRACES; i++){
				myWaveData[i].printDebugInfo();
			}

			// COLUMNS
			for(i = 0; i < dimensionSizes[COLUMNS]; i++){
				// ROWS
				for(j=0; j < dimensionSizes[ROWS]; j++){
					// LAYERS
					for(k=0; k < dimensionSizes[LAYERS]; k++){

						// traceUp
						if(traceUpData->moreData){
							traceUpRawBrickletIndex	= i*xAxisBlockSize + j*dimensionSizes[LAYERS] + k;
							traceUpDataIndex		= i*dimensionSizes[ROWS] + j + k*dimensionSizes[ROWS]*dimensionSizes[COLUMNS];

							if(	traceUpDataIndex >= 0 &&
								traceUpDataIndex < waveSize &&
								traceUpRawBrickletIndex < rawBrickletSize &&
								traceUpRawBrickletIndex >= 0 ){

									rawValue	= rawBrickletDataPtr[traceUpRawBrickletIndex];
									scaledValue = rawValue*slope + yIntercept;

									traceUpData->fillWave(traceUpDataIndex,rawValue,scaledValue);
							}
							else{
								debugOutputToHistory("Index out of range in traceUp");

								sprintf(globDataPtr->outputBuffer,"traceUpDataIndex=%d,waveSize=%d",traceUpDataIndex,waveSize);
								debugOutputToHistory(globDataPtr->outputBuffer);

								sprintf(globDataPtr->outputBuffer,"traceUpRawBrickletIndex=%d,rawBrickletSize=%d",traceUpRawBrickletIndex,rawBrickletSize);
								debugOutputToHistory(globDataPtr->outputBuffer);

								traceUpData->moreData = false;
							}
					}// if traceUpDataPtr

					// traceDown
					if(traceDownData->moreData){

							traceDownRawBrickletIndex = firstBlockOffset + i*xAxisBlockSize + j*dimensionSizes[LAYERS] + k;
							traceDownDataIndex		  = (dimensionSizes[COLUMNS] -(i+1))*dimensionSizes[ROWS] + j + k*dimensionSizes[ROWS]*dimensionSizes[COLUMNS];

						if(	traceDownDataIndex >= 0 &&
							traceDownDataIndex < waveSize &&
							traceDownRawBrickletIndex < rawBrickletSize &&
							traceDownRawBrickletIndex >= 0
							){
								rawValue	= rawBrickletDataPtr[traceDownRawBrickletIndex];
								scaledValue = rawValue*slope + yIntercept;

								traceDownData->fillWave(traceDownDataIndex,rawValue,scaledValue);
						}
						else{
							debugOutputToHistory("Index out of range in traceDown");

							sprintf(globDataPtr->outputBuffer,"traceDownDataIndex=%d,waveSize=%d",traceDownDataIndex,waveSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							sprintf(globDataPtr->outputBuffer,"traceDownRawBrickletIndex=%d,rawBrickletSize=%d",traceDownRawBrickletIndex,rawBrickletSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							traceDownData->moreData = false;
						}
					}// if traceDownDataPtr

					// reTraceUp
					if(reTraceUpData->moreData){

						reTraceUpRawBrickletIndex = i*xAxisBlockSize + (dimensionSizes[ROWS] - (j+1))*dimensionSizes[LAYERS] + k;
						reTraceUpDataIndex		  = i*dimensionSizes[ROWS] + j + k*dimensionSizes[ROWS]*dimensionSizes[COLUMNS];

						if(	reTraceUpDataIndex >= 0 &&
							reTraceUpDataIndex < waveSize &&
							reTraceUpRawBrickletIndex < rawBrickletSize &&
							reTraceUpRawBrickletIndex >= 0
							){
								rawValue	= rawBrickletDataPtr[reTraceUpRawBrickletIndex];
								scaledValue = rawValue*slope + yIntercept;

								reTraceUpData->fillWave(reTraceUpDataIndex,rawValue,scaledValue);
						}
						else{
							debugOutputToHistory("Index out of range in reTraceUp");

							sprintf(globDataPtr->outputBuffer,"reTraceUpDataIndex=%d,waveSize=%d",reTraceUpDataIndex,waveSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							sprintf(globDataPtr->outputBuffer,"reTraceUpRawBrickletIndex=%d,rawBrickletSize=%d",reTraceUpRawBrickletIndex,rawBrickletSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							reTraceUpData->moreData = false;
						}
					}// if reTraceUpDataPtr

					// reTraceDown
					if(reTraceDownData->moreData){

						reTraceDownRawBrickletIndex	= firstBlockOffset + i*xAxisBlockSize + (dimensionSizes[ROWS] - (j+1))*dimensionSizes[LAYERS] + k;
						reTraceDownDataIndex		= ( dimensionSizes[COLUMNS] - (i+1) )*dimensionSizes[ROWS] + j + k*dimensionSizes[ROWS]*dimensionSizes[COLUMNS];

						if(	reTraceDownDataIndex >= 0 &&
							reTraceDownDataIndex < waveSize &&
							reTraceDownRawBrickletIndex < rawBrickletSize &&
							reTraceDownRawBrickletIndex >= 0
							){

								rawValue	= rawBrickletDataPtr[reTraceDownRawBrickletIndex];
								scaledValue = rawValue*slope + yIntercept;
								reTraceDownData->fillWave(reTraceDownDataIndex,rawValue,scaledValue);

								//if(k < 10 && i < 2 && j < 2){
								//	sprintf(globDataPtr->outputBuffer,"j(rows)=%d,i(cols)=%d,k(layers)=%d,reTraceDownRawBrickletIndex=%d,reTraceDownDataIndex=%d,rawValue=%d,scaledValue=%g",
								//		j,i,k,reTraceDownRawBrickletIndex,reTraceDownDataIndex,rawValue,scaledValue);
								//	debugOutputToHistory(globDataPtr->outputBuffer);
								//}
						}
						else{
							debugOutputToHistory("Index out of range in reTraceDown");

							sprintf(globDataPtr->outputBuffer,"reTraceDownDataIndex=%d,waveSize=%d",reTraceDownDataIndex,waveSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							sprintf(globDataPtr->outputBuffer,"reTraceDownRawBrickletIndex=%d,rawBrickletSize=%d",reTraceDownRawBrickletIndex,rawBrickletSize);
							debugOutputToHistory(globDataPtr->outputBuffer);

							reTraceDownData->moreData = false;
						}
					}// if reTraceDownDataPtr
				} // for LAYERS
			} // for ROWS
		} // for COLUMNS

			for(i=0; i < MAX_NUM_TRACES; i++){
				if(myWaveData[i].isEmpty()){
					continue;
				}

				setDataWaveNote(brickletID,myWaveData[i]);

				MDSetWaveScaling(myWaveData[i].getWaveHandle(),ROWS,&xAxisIncrement,&zeroSetScaleOffset);
				MDSetWaveScaling(myWaveData[i].getWaveHandle(),COLUMNS,&yAxisIncrement,&zeroSetScaleOffset);

				// we don't use zeroSetScaleOffset=0 here
				MDSetWaveScaling(myWaveData[i].getWaveHandle(),LAYERS,&specAxis.physicalIncrement,&specAxis.physicalStart);
				MDSetWaveUnits(myWaveData[i].getWaveHandle(),ROWS,WStringToString(xAxis.physicalUnit).c_str());
				MDSetWaveUnits(myWaveData[i].getWaveHandle(),COLUMNS,WStringToString(yAxis.physicalUnit).c_str());

				MDSetWaveUnits(myWaveData[i].getWaveHandle(),LAYERS,WStringToString(specAxis.physicalUnit).c_str());
				MDSetWaveUnits(myWaveData[i].getWaveHandle(),DATA,bricklet->getMetaDataValueAsString("channelUnit").c_str());

				fullPathOfCreatedWave.append(getFullWavePath(dfHandle,myWaveData[i].getWaveHandle()));
				fullPathOfCreatedWave.append(";");
			}

			break;
	}
	return SUCCESS;
}
