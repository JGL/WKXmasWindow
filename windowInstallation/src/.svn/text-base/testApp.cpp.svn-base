#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofSetVerticalSync(true);
	
	//register the ftp and http factories
	Poco::Net::FTPStreamFactory::registerFactory();
	Poco::Net::HTTPStreamFactory::registerFactory();
	
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(WK_VIDEO_CAPTURE_WIDTH, WK_VIDEO_CAPTURE_HEIGHT);
        
    colorImg.allocate(WK_VIDEO_CAPTURE_WIDTH, WK_VIDEO_CAPTURE_HEIGHT);
	grayImage.allocate(WK_VIDEO_CAPTURE_WIDTH, WK_VIDEO_CAPTURE_HEIGHT);
	grayBg.allocate(WK_VIDEO_CAPTURE_WIDTH, WK_VIDEO_CAPTURE_HEIGHT);
	grayDiff.allocate(WK_VIDEO_CAPTURE_WIDTH, WK_VIDEO_CAPTURE_HEIGHT);
	bLearnBakground = false;
	threshold = 80;
	
	//lets load in our face xml file
	haarFinder.setup("haarXML/haarcascade_frontalface_default.xml");
	grabbedFace.allocate(WK_VIDEO_CAPTURE_WIDTH, WK_VIDEO_CAPTURE_HEIGHT, OF_IMAGE_COLOR);
	
	Path completeFacesPath = Path(ofToDataPath(WK_LOCAL_COMPLETED_DIRECTORY));
	facesCompletedDirectoryLister.setExtensionToLookFor("jpg");//for saving of faces
	facesCompletedDirectoryLister.setPath(completeFacesPath);	
	
	Path awaitingFacePath = Path(ofToDataPath(WK_LOCAL_AWAITING_DIRECTORY));
	facesAwaitingDirectoryLister.setExtensionToLookFor("jpg");//for saving of faces
	facesAwaitingDirectoryLister.setPath(awaitingFacePath);		
	
	saveFaceThisFrame = false;
	
	//franklinBook.loadFont("frabk.ttf", 146, true, false);

	//now some tweening bits
	boxH= 10;
	boxW= boxH;
	//leftPos = WK_INT_SCREENS_WIDTH-WK_CHARACTER_WIDTH;
	leftPos = 0;
	rightPos = WK_CHARACTER_OFFSET;
	//rightPos = WK_INT_SCREENS_WIDTH;  //TODO: this is wrong, it will actually be WK_INT_SCREENS_WIDTH or a fraction of that
	duration = 4000;
	
	// the float vars to tween, one for each easing
	back = leftPos;

	// create a tween for each easing function
	// ease from leftPos to rightPos
	// call onTweenCompleted when the tween has reached it's target. (skip the -> part if you don't need an onComplete event)
	// start the tweens with one second delay between each other
	animator.addTween(&back,leftPos,rightPos,&Expo::easeOut, duration,0)->ECompleted += Delegate<testApp, EventArgs>(this, &testApp::onTweenCompleted);
	//TODO: experiment with tweens to see which looks best, Expo::easOut seems to solve the redraw bug the best
	
	//animatePeople = true;
	ofxRemoteFaceUploadClient.start();
	ofxLocalFaceDisplayClient.init();	
	
	ofSetWindowTitle("WK 2008 Xmas Window");
	
	newTweenAndLoadNext = false;
	
	switchToFoundFromIdle = false;
	
	loopsToPoseFromFaceFound = WK_NUMBER_OF_FACES_BEFORE_START_POSE_FROM_FACE_FOUND;
	
	switchToPoseFromFound = false;
	
	loopsToGrabFromPose = WK_NUMBER_OF_SECONDS_BEFORE_START_GRAB_FROM_POSE;
	
	switchToGrabFromPose = false;	
	
	newPoseTween = false;
	
	loopsToIdleFromGrab = WK_NUMBER_OF_SECONDS_AFTER_GRAB_BEFORE_BACK_TO_IDLE;
	
	switchToIdleFromGrab = false;
	
	newGrabTween = false;
	
	statusText = "Loading...";
	
	foundFaceThisFrame = false;
	
	numberOfSolidFaceFrames = 0;
	
	numberOfDots = 0;
}

//--------------------------------------------------------------
void testApp::update()
{	
	WKWindowState windowState = ofxLocalFaceDisplayClient.getState();
	
	if(foundFaceThisFrame)
	{
		numberOfSolidFaceFrames++;
	}
	else
	{
		numberOfSolidFaceFrames = 0;
	}
	
	if((numberOfSolidFaceFrames > WK_NUMBER_OF_CONSECUTIVE_FACES_FOR_GRAB) && (windowState == WINDOW_STATE_IDLE)) 
		//WK_NUMBER_OF_CONSECUTIVE_FACES_FOR_GRAB solid face frames at least and IDLE MODE
	{
		switchToFoundFromIdle = true;
		numberOfSolidFaceFrames = 0; //reset it
	}

	if(switchToIdleFromGrab)
	{
		switchToFoundFromIdle = false; //reset it to false to be sure
		resetTweenAndLoadNext(); //going back to the start
		switchToIdleFromGrab = false;
	}	
	
	if(switchToGrabFromPose)
	{
		saveFaceThisFrame = true; //save the image!
		loopsToIdleFromGrab = WK_NUMBER_OF_SECONDS_AFTER_GRAB_BEFORE_BACK_TO_IDLE;
		resetTweenToSecondsForGrab();
		switchToGrabFromPose = false;
	}
	
	if(switchToPoseFromFound)
	{
		loopsToGrabFromPose = WK_NUMBER_OF_SECONDS_BEFORE_START_GRAB_FROM_POSE;
		resetTweenToSecondsForPose();
		switchToPoseFromFound = false;
	}
	
	if(switchToFoundFromIdle)
	{
		loopsToPoseFromFaceFound = WK_NUMBER_OF_FACES_BEFORE_START_POSE_FROM_FACE_FOUND;
		resetTweenAndLoadLiveFace();
		//switchToFoundFromIdle = false; doing this in the case below now
	}
	
	if(newGrabTween)
	{
		resetTweenToSecondsForGrab();
	}
	
	if(newPoseTween)
	{
		resetTweenToSecondsForPose();
	}
	
	//TODO: decide where to go with this for best graphical look - not really fixed it, just gone with exponential
	if(newTweenAndLoadNext)
	{
		resetTweenAndLoadNext();
	}	//just got rid of it, no didn't very useful
	

	
	bool bNewFrame = false;

	vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();

	if (bNewFrame){
		colorImg.setFromPixels(vidGrabber.getPixels(), WK_VIDEO_CAPTURE_WIDTH, WK_VIDEO_CAPTURE_HEIGHT);

		grayImage = colorImg;

		haarFinder.findHaarObjects(grayImage, WK_MIN_FACE_AREA, 99999999, 5); //the syntax is source, min area, max area number considered
	}

	animator.update();	

	ofxLocalFaceDisplayClient.update();
	
	//this must be last! or maybe above?
	
	//it is correct for this to be here because other wise would have to communicate key presses to the local face display client
	switch(windowState)
	{
		case WINDOW_STATE_LOADING:
		{
			//cout << "WKWindowLOADING in testApp..." << endl;
			break;
		}			
		case WINDOW_STATE_IDLE:
		{
			//cout << "WKWindowIDLE in testApp..." << endl;
			
			if(switchToFoundFromIdle)
			{
				//then we have found a face, so set it to the right state
				windowState = WINDOW_STATE_FACE_FOUND;
				ofxLocalFaceDisplayClient.setState(windowState);
				switchToFoundFromIdle = false;
			}
			break;			
		}
		case WINDOW_STATE_FACE_FOUND:
		{			
			//cout << "WKWindowFACE_FOUND in testApp..." << endl;
			
			if(switchToPoseFromFound)
			{
				windowState = WINDOW_STATE_POSE;
				ofxLocalFaceDisplayClient.setState(windowState);
			}
			break;
		}
		case WINDOW_STATE_POSE:
		{
			//cout << "WKWindowPOSE in testApp..." << endl;
			if(switchToGrabFromPose)
			{
				windowState = WINDOW_STATE_GRAB_IMAGE;
				ofxLocalFaceDisplayClient.setState(windowState);
			}
			
			break;			
		}
		case WINDOW_STATE_GRAB_IMAGE:
		{
			//cout << "WKWindowGRAB_IMAGE in testApp..." << endl;
			
			if(switchToIdleFromGrab)
			{
				windowState = WINDOW_STATE_IDLE;
				ofxLocalFaceDisplayClient.setState(windowState);
			}
			
			break;			
		}			
		default:
		{
			//cout << "Error - how did you get to default in the window state in update?" << endl;
			break; //don't do anything
		}
	}
	
	updateGUIStatusText();
}

//--------------------------------------------------------------
void testApp::draw()
{		
	//TRYING NOT DRAWING THE GUI at all, as GXM seems unable to split over the internal screen and the external
	
	//TODO: set up proper gui objects, and not use ofbitmapstring
	float creditPosition = OFX_PREVIEW_SCREENS_Y + OFX_PREVIEW_SCREENS_HEIGHT+ 20 + WK_VIDEO_CAPTURE_HEIGHT + 20;
	ofSetColor(0xff0000);
	string credits = "2009 Xmas Window Installation by Joel Gethin Lewis and Wieden and Kennedy London";
	ofDrawBitmapString(credits, 20, creditPosition);	
	string framerate = "Framerate is "; 						
	framerate += ofToString(ofGetFrameRate(), 2)+"fps"; 
	ofDrawBitmapString(framerate, 20, creditPosition + 20);
	
	string fakeFace = "No fake face found, q to change";
	if(switchToFoundFromIdle)
		fakeFace = "Fake face found, q to change";
	
	ofDrawBitmapString(fakeFace, 20, creditPosition + 40 );
	
	ofDrawBitmapString(statusText, 20, creditPosition + 80);
	
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);
	vidGrabber.draw(OFX_PREVIEW_SCREENS_X , OFX_PREVIEW_SCREENS_Y + OFX_PREVIEW_SCREENS_HEIGHT+20); // this is the live video image
	
	findBiggestFaceAndCopyIntoGrabbedFace(OFX_PREVIEW_SCREENS_X , OFX_PREVIEW_SCREENS_Y + OFX_PREVIEW_SCREENS_HEIGHT+20);
	
	grabbedFace.resize(WK_DISPLAY_FACE_WIDTH, WK_DISPLAY_FACE_HEIGHT);
	grabbedFace.draw(680, 20);
	
	if(saveFaceThisFrame)
	{
		WKCharacterType liveFaceCharacterType = CHARACTER_TURKEY;
		liveFaceCharacterType = ofxLocalFaceDisplayClient.setLiveFaceToStillAndGetCharacterType(&grabbedFace);
		saveFaceRemotelyWithCharacterType(liveFaceCharacterType);
		saveFaceThisFrame = false;
	}
	
	//more tweening
	ofSetColor(0,0,0); 
	ofRect(600,back,boxW,boxH);
	
	string newStatusText = updateWindowStatusText();
	
	ofxLocalFaceDisplayClient.localDraw(back, &grabbedFace, newStatusText);
	ofxLocalFaceDisplayClient.previewDraw(OFX_PREVIEW_SCREENS_X, OFX_PREVIEW_SCREENS_Y, OFX_PREVIEW_SCREENS_WIDTH, OFX_PREVIEW_SCREENS_HEIGHT);
	ofxLocalFaceDisplayClient.externalDraw(); //changed this to not transform away from the gui at all
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{ 	
	//WKWindowState windowState = ofxLocalFaceDisplayClient.getState(); //TODO:neaten this blocking of new faces
	
	switch (key){
		case ' ':
		{
			bLearnBakground = true;			
		}
		case '+':
		{
			threshold ++;
			if (threshold > 255) threshold = 255;			
		}
		case '-':
		{
			threshold --;
			if (threshold < 0) threshold = 0;			
		}
		case 'c':
		{
			//camera settings
			vidGrabber.videoSettings();
			break;			
		}
		case 'f':
		{
			//toggle full screen!
			ofToggleFullscreen();
			break;
		}
		case 'q': //simulating a face being found for convenience when developing
		{
			//if(windowState == WINDOW_STATE_IDLE) //only if the thing is in idle mode can you switch to found
			//{TODO:danger fix and chekc this
				switchToFoundFromIdle = !switchToFoundFromIdle;
			//}			
		}
		default:
		{
			break;
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	ofxLocalFaceDisplayClient.handleMouseMove(ofRectangle(OFX_PREVIEW_SCREENS_X, OFX_PREVIEW_SCREENS_Y, OFX_PREVIEW_SCREENS_WIDTH, OFX_PREVIEW_SCREENS_HEIGHT), ofPoint(x,y,0));
	
	//TODO: will I have to translate the x,y of the mouse too by OFX_PREVIEW_SCREENS_X, OFX_PREVIEW_SCREENS_Y
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	ofxLocalFaceDisplayClient.handleMouseDrag(ofRectangle(OFX_PREVIEW_SCREENS_X, OFX_PREVIEW_SCREENS_Y, OFX_PREVIEW_SCREENS_WIDTH, OFX_PREVIEW_SCREENS_HEIGHT), ofPoint(x,y,0));

	//TODO: will I have to translate the x,y of the mouse too by OFX_PREVIEW_SCREENS_X, OFX_PREVIEW_SCREENS_Y	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}

void testApp::resetTweenToSecondsForGrab()
{
	//	//then change the tween to seconds, so change the duration, and don't actually change any of the draw stuff
	animator.addTween(&back,leftPos,rightPos,&Expo::easeOut, WK_ONE_SECOND,0)->ECompleted += Delegate<testApp, EventArgs>(this, &testApp::onTweenCompleted);
	newGrabTween = false;
}

void testApp::resetTweenToSecondsForPose()
{
	//then change the tween to seconds, so change the duration, and don't actually change any of the draw stuff
	animator.addTween(&back,leftPos,rightPos,&Expo::easeOut, WK_ONE_SECOND,0)->ECompleted += Delegate<testApp, EventArgs>(this, &testApp::onTweenCompleted);
	newPoseTween = false;
}

void testApp::resetTweenAndLoadLiveFace()
{
	//then change the tween
	animator.addTween(&back,leftPos,rightPos,&Expo::easeOut, duration,0)->ECompleted += Delegate<testApp, EventArgs>(this, &testApp::onTweenCompleted);
	//load the live face	
	ofxLocalFaceDisplayClient.loadNextLiveFace();
	newTweenAndLoadNext = false;	
}

void testApp::resetTweenAndLoadNext()
{
	//then change the tween
	animator.addTween(&back,leftPos,rightPos,&Expo::easeOut, duration,0)->ECompleted += Delegate<testApp, EventArgs>(this, &testApp::onTweenCompleted);
	//animator.addTween(&back,leftPos,rightPos,&Sine::easeIn, duration,0)->ECompleted += Delegate<testApp, EventArgs>(this, &testApp::onTweenCompleted);
	//load the face	
	ofxLocalFaceDisplayClient.loadNextFace();	
	newTweenAndLoadNext = false;
}

// this function is called each time a tween completes
void testApp::onTweenCompleted(const void* pSender, Poco::EventArgs & args)
{
	WKWindowState windowState = ofxLocalFaceDisplayClient.getState();
	
	// we know the sender is a tween, so we cast it... this lets us access the tweens properties
	Tween* sender = (Tween*)pSender;	
	
	//it is correct for this to be here because other wise would have to communicate key presses to the local face display client
	switch(windowState)
	{
		case WINDOW_STATE_LOADING:
		{
			//cout << "WKWindowLOADING in tweencompleted..." << endl;
			break;
		}			
		case WINDOW_STATE_IDLE:
		{
			//cout << "WKWindowIDLE in  tweencompleted..." << endl;
			// check which float var and direction the tween was tweening
			// add a new tween for the same var but with opposite direction to the just completed one
			// when a tween is added for a float var the old tween for the same var is overwritten, if there is one..	
			// explore the different easing functions
			
			if(sender->getTarget()==&back && back == rightPos)
			{		
				newTweenAndLoadNext = true;	//fuck this switching, no don't it's critical
			}			
			break;			
		}
		case WINDOW_STATE_FACE_FOUND:
		{			
			if(loopsToPoseFromFaceFound == 0)
			{
				//so now start to pose
				switchToPoseFromFound = true;
			}
			else
			{
				loopsToPoseFromFaceFound--;
				//this would be the place to change text being displayed, i.e. found you!
				// check which float var and direction the tween was tweening
				// add a new tween for the same var but with opposite direction to the just completed one
				// when a tween is added for a float var the old tween for the same var is overwritten, if there is one..	
				// explore the different easing functions
				
				if(sender->getTarget()==&back && back == rightPos)
				{		
					newTweenAndLoadNext = true;	//fuck this switching, no don't it's critical
				}				
			}
			//cout << "WKWindowFACE_FOUND in  tweencompleted..." << endl;
			
			break;
		}
		case WINDOW_STATE_POSE:
		{			
			if(loopsToGrabFromPose == 0)
			{
				//so now grab
				switchToGrabFromPose = true;
			}
			else
			{
				loopsToGrabFromPose--;
				//this would be the place to change text being displayed, i.e. countdown text
				// check which float var and direction the tween was tweening
				// add a new tween for the same var but with opposite direction to the just completed one
				// when a tween is added for a float var the old tween for the same var is overwritten, if there is one..	
				// explore the different easing functions
				
				if(sender->getTarget()==&back && back == rightPos)
				{		
					newPoseTween = true;	//fuck this switching, no don't it's critical//WILL NEED ANOTHER BOOLEAN HERE for the bool to indicate the setting up of a new
					//tween for counting down the seconds until posing
				}				
			}
			//cout << "WKWindowPOSE in tweencompleted..." << endl;
			break;
		}
		case WINDOW_STATE_GRAB_IMAGE:
		{			
			if(loopsToIdleFromGrab == 0)
			{
				//so now grab
				switchToIdleFromGrab = true;
			}
			else
			{
				loopsToIdleFromGrab--;
				//this would be the place to change text being displayed, i.e. countdown text
				// check which float var and direction the tween was tweening
				// add a new tween for the same var but with opposite direction to the just completed one
				// when a tween is added for a float var the old tween for the same var is overwritten, if there is one..	
				// explore the different easing functions
				
				if(sender->getTarget()==&back && back == rightPos)
				{		
					newGrabTween = true;	//fuck this switching, no don't it's critical//WILL NEED ANOTHER BOOLEAN HERE for the bool to indicate the setting up of a new
					//tween for counting down the seconds until going back to idle from grab
				}				
			}
			//cout << "WKWindowGRAB_IMAGE in tweencompleted..." << endl;
			break;
		}				
		default:
		{
			//cout << "Error - how did you get to default in the window state in tweencompleted?" << endl;
			break; //don't do anything
		}
	}	
}

void testApp::grabPixelsIntoImage(ofImage* grabIntoThis, ofxCvColorImage* grabFromThis, float x, float y, float w, float h)
{
	int subX = x;
	int subY = y;
	int subWidth = w;
	int subHeight = h;
	
	grabIntoThis->resize(subWidth, subHeight);
	
	unsigned char subRegion[ subWidth * subHeight * 3  ];  // R G B 
	unsigned char * fromPixels = grabFromThis->getPixels(); //get the pointer to the pixel array
	
	for (int i = 0; i < subWidth; i++){ 
		for (int j = 0; j < subHeight; j++){ 
			int mainPixelPos = ((j+subY) * grabFromThis->width + (i+subX)) * 3; //check the logic of this! 
			int subPixlPos = (j * subWidth + i) * 3; 
			subRegion[subPixlPos] = fromPixels[mainPixelPos];   // R 
			subRegion[subPixlPos + 1] = fromPixels[mainPixelPos + 1];  // G 
			subRegion[subPixlPos + 2] = fromPixels[mainPixelPos + 2];  // B 
		} 
	}
	
	grabIntoThis->setFromPixels(subRegion, subWidth, subHeight, OF_IMAGE_COLOR, TRUE); //set it from our new pixel array it is RGB order
}

void testApp::findBiggestFaceAndCopyIntoGrabbedFace(float offsetX, float offsetY)
{
	int numFace = haarFinder.blobs.size();
	
	glPushMatrix();
	
	glTranslatef(offsetX, offsetY, 0);
	
	float biggestAreaSoFar = 0.f;
	float indexOfFaceWithBiggestArea = 0;
	foundFaceThisFrame = false;
	
	for(int i = 0; i < numFace; i++){
		foundFaceThisFrame = true;
		float x = haarFinder.blobs[i].boundingRect.x;
		float y = haarFinder.blobs[i].boundingRect.y;
		float w = haarFinder.blobs[i].boundingRect.width;
		float h = haarFinder.blobs[i].boundingRect.height;
		
		float cx = haarFinder.blobs[i].centroid.x;
		float cy = haarFinder.blobs[i].centroid.y;
		
		float newArea = w*h;
		
		if(newArea > biggestAreaSoFar)
		{
			//then the face is bigger, so its the new good one
			biggestAreaSoFar = newArea;
			
			indexOfFaceWithBiggestArea = i;
		}
		
		ofSetColor(0xFF0000);
		ofNoFill();
		ofRect(x, y, w, h);
		
		ofSetColor(0xFFFFFF);
		ofDrawBitmapString("face "+ofToString(i), cx, cy);
	}
	
	if(foundFaceThisFrame && biggestAreaSoFar > WK_MIN_FACE_AREA)
	{
		//if there is a face found, and it's area is bigger than the required minimum, then grab the screen
		float x = haarFinder.blobs[indexOfFaceWithBiggestArea].boundingRect.x;
		float y = haarFinder.blobs[indexOfFaceWithBiggestArea].boundingRect.y;
		float w = haarFinder.blobs[indexOfFaceWithBiggestArea].boundingRect.width;
		float h = haarFinder.blobs[indexOfFaceWithBiggestArea].boundingRect.height;
		foundFaceThisFrame = true;
		
		grabPixelsIntoImage(&grabbedFace, &colorImg, x,y,w,h*1.2);
	}
	else
	{
		//we didn't find a big enough face this frame or we didn't find one at all
		foundFaceThisFrame = false;
	}
	
	
	glPopMatrix();	
}

void testApp::saveFaceRemotelyWithCharacterType(WKCharacterType remoteFaceCharacterType)
{
	//this is where we add the face to the database and ftp and upload
	//this is where we will also add the type of the face - king or turkey or whatever
	
	ofImage faceCopy;
	faceCopy.clone(grabbedFace); //duplicate it	
	
	std::string newFaceFilename;
	
	std::string typeOfCharacterString = ofxLocalFaceDisplayClient.getCharacterNameFromCharacterType(remoteFaceCharacterType);
	
	long numberCompleted = facesCompletedDirectoryLister.getNumberOfFiles();
	long numberAwaiting = facesAwaitingDirectoryLister.getNumberOfFiles();		
	
	cout << "Number completed " << numberCompleted  << endl;
	cout << "number awaiting " << numberAwaiting  << endl;
	
	//don't need to acutally increment the number as faces are numbered from 0
	long combinedNumberFacesInUploadAndCompletedDir = numberCompleted + numberAwaiting;
	
	//if you want to do this then you need to use a stream, doing leading zeros
	std::ostringstream localOSS;
	localOSS << "face_" << setw(10) << setfill('0') << combinedNumberFacesInUploadAndCompletedDir << "_" << typeOfCharacterString << ".jpg";
	
	newFaceFilename = localOSS.str();
	
	cout << "New face filename is " << newFaceFilename << endl;
	
	ofxRemoteFace face(newFaceFilename, faceCopy, remoteFaceCharacterType);
	
	ofxRemoteFaceUploadClient.addFace(face);	
}

void testApp::updateGUIStatusText()
{
	WKWindowState windowState = ofxLocalFaceDisplayClient.getState();

	switch(windowState)
	{
		case WINDOW_STATE_LOADING:
		{
			statusText="Loading Window...";
			break;
		}			
		case WINDOW_STATE_IDLE:
		{			
			statusText="Window idling, looking for faces";
			break;			
		}
		case WINDOW_STATE_FACE_FOUND:
		{			
			statusText="Face Found, scrolling in live face";
			break;
		}
		case WINDOW_STATE_POSE:
		{			
			statusText="Posing, counting down to grab";
			break;
		}
		case WINDOW_STATE_GRAB_IMAGE:
		{			
			statusText="New face grabbed, displaying and uploading";
			break;
		}				
		default:
		{
			statusText="Undefined state - call Joel - shouldn't be here";
			break; //don't do anything
		}
	}		
}

string testApp::updateWindowStatusText()
{
	WKWindowState windowState = ofxLocalFaceDisplayClient.getState();
	
	string windowText = "Looking for a face";
	
	switch(windowState)
	
	{
		case WINDOW_STATE_LOADING:
		{
			windowText="Window loading, please wait...";
			break;
		}			
		case WINDOW_STATE_IDLE:
		{			
			windowText="Come closer.";
//			switch(numberOfDots)
//			{
//				case 2:
//				{
//					windowText += "...";
//					numberOfDots = 0;
//					break;
//				}
//				case 1:
//				{
//					windowText += ".. ";
//					numberOfDots++;
//					break;
//				}
//				case 0:
//				{
//					windowText += ".  ";
//					numberOfDots++;
//					break;
//				}					
//			}
			break;			
		}
		case WINDOW_STATE_FACE_FOUND:
		{			
			windowText="One moment please.";
			break;
		}
		case WINDOW_STATE_POSE:
		{									
			windowText = "Look up!";
			
			switch(loopsToGrabFromPose)
			{
				case 3:
				{
					windowText = "3.";
					break;
				}
				case 2:
				{
					windowText = "2.";
					break;
				}
				case 1:
				{
					windowText = "1.";
					break;
				}
				case 0:
				{
					windowText = "0.";
					break;
				}					
			}
			
			break;
		}
		case WINDOW_STATE_GRAB_IMAGE:
		{			
			windowText="Ta Daa!" ; //put date and time in here, just december date and hours/mins
			//look in of utils!
			//not no more
			break;
		}				
		default:
		{
			windowText="Welcome to the Xmas Window.";
			break; //don't do anything
		}
	}
	
	return windowText;
}