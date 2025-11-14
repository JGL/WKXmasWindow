/*
 *  ofxLocalFaceDisplayClient.cpp
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 09/11/2008.
 *
 *
 */

#include "ofxLocalFaceDisplayClient.h"

ofxLocalFace::ofxLocalFace(string pathToFaceImage, WKCharacterType typeOfCharacter)
{
	faceImagePath = pathToFaceImage;
	character = typeOfCharacter;
	faceImage.setUseTexture(false); ///going to be loading in a thread, so don't want any texture uploading happening, or allocation
	faceLoaded = false;
	faceResized = false;
	liveFace = false;
	liveFaceFrozen = false;
}

ofxLocalFace::~ofxLocalFace()
{
	if(isThreadRunning()) //thread safety attempt
	{
		stop();
	}
}

void ofxLocalFace::reset(string pathToFaceImage, WKCharacterType typeOfCharacter)
{
	if(isThreadRunning()) //thread safety attempt
	{
		stop();
	}	
	
	faceImagePath = pathToFaceImage;
	character = typeOfCharacter;
	faceImage.clear();
	faceImage.setUseTexture(false); ///going to be loading in a thread, so don't want any texture uploading happening, or allocation
	faceLoaded = false;
	faceResized = false;
	liveFace = false;
	liveFaceFrozen = false;	
}

ofImage* ofxLocalFace::getFaceImage()
{
	ofImage* faceImagePointer = new ofImage;
	
	//is it loaded?
	if(getFaceLoaded())
	{
		faceImagePointer = &faceImage;
	}
	else
	{
		faceImagePointer = NULL;
	}
	
	return faceImagePointer;
}
	
//-------------------------------
// blocking functions
void ofxLocalFace::loadFaceImage()
{
	//cout << "Trying to load the face image " << faceImagePath << endl;
	faceImage.setUseTexture(false);
	faceLoaded = faceImage.loadImage(faceImagePath); //load the face image
}

void ofxLocalFace::resizeFaceImage()
{
	if(faceLoaded  && !faceResized) //TODO: CHECK that if its live and frozen? 
	{
		faceImage.resize(WK_DISPLAY_FACE_WIDTH, WK_DISPLAY_FACE_HEIGHT);
		faceResized = true;
	}
}
// threading stuff
void ofxLocalFace::threadedFunction()
{
    // loop through this process whilst thread running
    if( isThreadRunning()){
		loadFaceImage();
		stop();
    }
	
}

void ofxLocalFace::start()
{
   if (isThreadRunning() == false){
	   startThread(true, false); //blocking, not so much reporting
	   //cout << "Face thread started" << endl;
   }
}
	   
void ofxLocalFace::stop()
{
	stopThread();
	//printf("face thread stopped\n");
}

ofxLocalFaceDisplayClientUtils::ofxLocalFaceDisplayClientUtils()
{
	Path characterPath = Path(ofToDataPath("characters"));
	characterDirectoryLister.setExtensionToLookFor("tif");//for loading of characters
	characterDirectoryLister.setPath(characterPath);
	
	Path facePath = Path(ofToDataPath(WK_LOCAL_COMPLETED_DIRECTORY));
	faceDirectoryLister.setExtensionToLookFor("jpg");//for saving of faces
	faceDirectoryLister.setPath(facePath);
	
	state = WINDOW_STATE_LOADING; //initially we want to get everything loaded in
	lastFaceLoaded = 0;
	charactersInited = false;
	firstFacesLoaded = false;
	frontWKFace = 0;
	backWKFace = OFX_LOCAL_NUMBER_OF_FACES-1;	
	
	//setting up the RM with the FBO
	RM.allocate(WK_INT_SCREENS_WIDTH, WK_INT_SCREENS_HEIGHT);
    RM.allocateBasedOnTwoScreenDisplay();
	
	font.loadFont("SnellRoundhandLTStd-BdScr.otf", 56);
	
	oldCharacterType = 0; //can't be a turkey the first time around
	
	loadBackground();
}

ofxLocalFaceDisplayClientUtils::~ofxLocalFaceDisplayClientUtils()
{
}

void ofxLocalFaceDisplayClientUtils::initCharacters()
{
	wkCharacters.resize(OFX_LOCAL_NUMBER_OF_CHARACTERS);
	
	for(int i=0; i < OFX_LOCAL_NUMBER_OF_CHARACTERS; i++)  //really strange gl_string bug came up if i tried to do a resize and then alloacte
	{
		wkCharacters[i].allocate(WK_CHARACTER_WIDTH, WK_CHARACTER_HEIGHT, OF_IMAGE_COLOR_ALPHA);
	}	
	
	//and the head cropper
	wkHeadCropper.allocate(WK_DISPLAY_FACE_WIDTH, WK_DISPLAY_FACE_HEIGHT, OF_IMAGE_COLOR_ALPHA);
	
	loadAllCharacters();
	
	charactersInited = true;
}

void ofxLocalFaceDisplayClientUtils::loadBackground()
{
	backgroundImage.allocate(WK_BACKGROUND_WIDTH, WK_BACKGROUND_HEIGHT, OF_IMAGE_COLOR);
	backgroundImage.loadImage("background/background.tif");
}

void ofxLocalFaceDisplayClientUtils::loadNextFace() //load a new face at backWKFace, increment frontWKFace, backWKFace
{	
	if(firstFacesLoaded) //only do this if the first faces have been loaded
	{
		int numberOfFacesToDisplay = OFX_LOCAL_NUMBER_OF_FACES;
		
		if(frontWKFace >= (numberOfFacesToDisplay-1)) //i.e. the front face is at the back of the array
		{
			frontWKFace = 0; 
		}
		else
		{
			frontWKFace++;
		}
		
		if(backWKFace >= (numberOfFacesToDisplay-1)) //i.e. the front face is at the back of the array
		{
			backWKFace = 0; 
		}
		else
		{
			backWKFace++;
		}	
		
		faces[backWKFace]->stop(); //just checking
		faces[backWKFace]->setLoaded(false);
		
		long numberOfFaceFiles = faceDirectoryLister.getNumberOfFiles();
		if(numberOfFaceFiles == 0)
		{
			cout << "ERROR, no face files found in " << WK_LOCAL_COMPLETED_DIRECTORY << endl;
			//try again a few times
			while(numberOfFaceFiles == 0)
			{
				//KEEP trying? yuck this is horrible - try something better 
				//TODO: neaten this...
				numberOfFaceFiles = faceDirectoryLister.getNumberOfFiles();
				cout << "ERROR, no face files found in " << WK_LOCAL_COMPLETED_DIRECTORY << endl;
			}
		}
		
		
		Path facePath = faceDirectoryLister.getPath(lastFaceLoaded); //get the filename from the directory
			
		WKCharacterType loadingCharacter = getCharacterType(facePath.getFileName());

		faces[backWKFace]->reset(facePath.toString(), loadingCharacter);
		faces[backWKFace]->start();
		
		lastFaceLoaded++;
		
		if(lastFaceLoaded >= numberOfFaceFiles)
		{
			lastFaceLoaded = lastFaceLoaded % numberOfFaceFiles;
		}
		
		cout << "In load next face, the last face loaded is: " << lastFaceLoaded << " and the number of face files is: " << numberOfFaceFiles << endl;
	}	
}

void ofxLocalFaceDisplayClientUtils::loadNextLiveFace() //load a new face at backWKFace, set it to be live, increment frontWKFace, backWKFace
{	
	if(firstFacesLoaded) //only do this if the first faces have been loaded
	{
		int numberOfFacesToDisplay = OFX_LOCAL_NUMBER_OF_FACES;
		
		if(frontWKFace >= (numberOfFacesToDisplay-1)) //i.e. the front face is at the back of the array
		{
			frontWKFace = 0; 
		}
		else
		{
			frontWKFace++;
		}
		
		if(backWKFace >= (numberOfFacesToDisplay-1)) //i.e. the front face is at the back of the array
		{
			backWKFace = 0; 
		}
		else
		{
			backWKFace++;
		}	
		
		faces[backWKFace]->stop(); //just checking
		faces[backWKFace]->setLoaded(false);
		
		std::ostringstream oss;
		oss << WK_LOCAL_COMPLETED_DIRECTORY << "/NOFILEASITSLIVE.jpg"; //just filling it for debugging
		string completePath = oss.str();
		
		WKCharacterType loadingCharacter = getRandomCharacterType();
		
		faces[backWKFace]->reset(completePath, loadingCharacter);
		faces[backWKFace]->setLiveFace(true);
		faces[backWKFace]->setLiveFaceFrozen(false);
		faces[backWKFace]->setLoaded(true);
		faces[backWKFace]->setResized(true);
		faces[backWKFace]->stop(); //just checking
	}	
}

void ofxLocalFaceDisplayClientUtils::init()
{
	state = WINDOW_STATE_LOADING;
	initCharacters();
	loadFirstFaces();
	state = WINDOW_STATE_IDLE;
}

void ofxLocalFaceDisplayClientUtils::update()
{
//	if(!charactersInited)
//		initCharacters();
//	
//	if(!firstFacesLoaded)
//		loadFirstFaces();
	
}

void ofxLocalFaceDisplayClientUtils::loadFirstFaces()
{
	long numberOfFaces = faceDirectoryLister.getNumberOfFiles();
	
	if(numberOfFaces == 0)
	{
		return;
	}
	
	for(int i=0; i< OFX_LOCAL_NUMBER_OF_FACES; i++) //we know there will be none to start with
	{
		long safeFileNumber = (long)i%numberOfFaces; //what if we have less face image files than we want in the display array?
		Path facePath = faceDirectoryLister.getPath(safeFileNumber);//get the file path from the directory
		
		WKCharacterType loadingCharacter = getCharacterType(facePath.getFileName());
		
		faces[i] = new ofxLocalFace(facePath.toString(), loadingCharacter);
		faces[i]->start();
		lastFaceLoaded = safeFileNumber;
	}
	
	firstFacesLoaded = true;
}

void ofxLocalFaceDisplayClientUtils::loadAllCharacters()
{
	int numberOfCharacters = (int)characterDirectoryLister.getNumberOfFiles();
	
	for(int i=0; i < numberOfCharacters; i++) //should have already been alocated so use the size of the array as the maxium
	{
		Path characterPath = characterDirectoryLister.getPath((long) i); //get the filename from the directory
		wkCharacters[i].loadImage(characterPath.toString());
	}
	
	wkHeadCropper.loadImage("headCropper.tif"); //an rgb b&w image of the oval mask to apply to the face images so they don't show through the edges of the 
												//character graphics
	wkHeadCropper.setImageType(OF_IMAGE_GRAYSCALE); //so we can use it as an alpha mask
}

ofImage * ofxLocalFaceDisplayClientUtils::getCharacterImage(WKCharacterType typeOfCharacter)
{
	ofImage * characterPointer = NULL;
	
	if(typeOfCharacter < wkCharacters.size())
	{
		characterPointer = &wkCharacters[(int)typeOfCharacter];
	}
	
	return characterPointer;
}

WKCharacterType ofxLocalFaceDisplayClientUtils::getCharacterTypeFromCharacterName(string theCharacter)
{
	WKCharacterType characterTypeOfTheFile = CHARACTER_TURKEY; //everyone is a turkey to start with
	
	if(theCharacter=="turkey")
	{
		characterTypeOfTheFile = CHARACTER_TURKEY;
		return characterTypeOfTheFile;
	}
	else if(theCharacter=="choirboy")
	{
		characterTypeOfTheFile = CHARACTER_CHOIRBOY;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="king")
	{
		characterTypeOfTheFile = CHARACTER_KING;
		return characterTypeOfTheFile;
	}
	else if(theCharacter=="santa")
	{
		characterTypeOfTheFile = CHARACTER_SANTA;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="elf")
	{
		characterTypeOfTheFile = CHARACTER_ELF;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="pudding")
	{
		characterTypeOfTheFile = CHARACTER_PUDDING;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="tree")
	{
		characterTypeOfTheFile = CHARACTER_TREE;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="dame")
	{
		characterTypeOfTheFile = CHARACTER_DAME;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="snowman")
	{
		characterTypeOfTheFile = CHARACTER_SNOWMAN;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="robin")
	{
		characterTypeOfTheFile = CHARACTER_ROBIN;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="angel")
	{
		characterTypeOfTheFile = CHARACTER_ANGEL;
		return characterTypeOfTheFile;
	}		
	else if(theCharacter=="pirate")
	{
		characterTypeOfTheFile = CHARACTER_PIRATE;
		return characterTypeOfTheFile;
	}			
	
	return characterTypeOfTheFile;
}

string ofxLocalFaceDisplayClientUtils::getCharacterNameFromCharacterType(WKCharacterType theCharacterType)
{
	string characterTypeAsString = "turkey"; //everyone is a turkey to start with
	
	switch(theCharacterType)
	{
		case CHARACTER_TURKEY:
		{
			characterTypeAsString = "turkey";
			break;
		}
		case CHARACTER_CHOIRBOY:
		{
			characterTypeAsString = "choirboy";
			break;
		}
		case CHARACTER_KING:
		{
			characterTypeAsString = "king";
			break;
		}
		case CHARACTER_SANTA:
		{
			characterTypeAsString = "santa";
			break;
		}
		case CHARACTER_ELF:
		{
			characterTypeAsString = "elf";
			break;
		}
		case CHARACTER_PUDDING:
		{
			characterTypeAsString = "pudding";
			break;
		}
		case CHARACTER_TREE:
		{
			characterTypeAsString = "tree";
			break;
		}
		case CHARACTER_DAME:
		{
			characterTypeAsString = "dame";
			break;
		}
		case CHARACTER_SNOWMAN:
		{
			characterTypeAsString = "snowman";
			break;
		}
		case CHARACTER_ROBIN:
		{
			characterTypeAsString = "robin";
			break;
		}			
		case CHARACTER_ANGEL:
		{
			characterTypeAsString = "angel";
			break;
		}
		case CHARACTER_PIRATE:
		{
			characterTypeAsString = "pirate";
			break;
		}
		default:
		{
			characterTypeAsString = "turkey";
		}
	}
	
	return characterTypeAsString;	
}

float ofxLocalFaceDisplayClientUtils::getFaceLeftXPosition(WKCharacterType characterType)
{
	float xOffset = 0.f;	//start drawing from the left of the character image, so the will draw the image on the left most bit of the character by default
	
	xOffset -= 26.f;
	
	switch(characterType)
	{
		case CHARACTER_TURKEY:
		{
			xOffset += 319.f;
			break;
		}
		case CHARACTER_CHOIRBOY:
		{
			xOffset += 333.f;
			break;
		}
		case CHARACTER_KING:
		{
			xOffset += 336.f;
			break;
		}
		case CHARACTER_SANTA:
		{
			xOffset += 336.f;
			break;
		}
		case CHARACTER_ELF:
		{
			xOffset += 333.f;
			break;
		}
		case CHARACTER_PUDDING:
		{
			xOffset += 328.f;
			break;
		}
		case CHARACTER_TREE:
		{
			xOffset += 331.f;
			break;
		}
		case CHARACTER_DAME:
		{
			xOffset += 322.f;
			break;
		}
		case CHARACTER_SNOWMAN:
		{
			xOffset += 322.f;
			break;
		}
		case CHARACTER_ROBIN:
		{
			xOffset += 329.f;
			break;
		}			
		case CHARACTER_ANGEL:
		{
			xOffset += 312.f;
			break;
		}
		case CHARACTER_PIRATE:
		{
			xOffset += 321.f;
			break;
		}
		default:
		{
			xOffset += 0.f; //the default is not to move the face at all
		}
	}
	
	return xOffset;
}

float ofxLocalFaceDisplayClientUtils::getFaceTopYPosition(WKCharacterType characterType)
{
	float yOffSet = 0.f;	//start drawing from the top of the character image, taking into account the 
																//vertical size of the screen
	
	yOffSet -= 18; //hacktastic 
	//TODO:neaten this to change the constants below
	
	
	switch(characterType)
	{
		case CHARACTER_TURKEY:
		{
			yOffSet += 352.f;
			break;
		}
		case CHARACTER_CHOIRBOY:
		{
			yOffSet += 346.f;
			break;
		}
		case CHARACTER_KING:
		{
			yOffSet += 159.f;
			break;
		}
		case CHARACTER_SANTA:
		{
			yOffSet += 174.f;
			break;
		}
		case CHARACTER_ELF:
		{
			yOffSet += 352.f;
			break;
		}
		case CHARACTER_PUDDING:
		{
			yOffSet += 342.f;
			break;
		}
		case CHARACTER_TREE:
		{
			yOffSet += 215.f;
			break;
		}
		case CHARACTER_DAME:
		{
			yOffSet += 163.f;
			break;
		}
		case CHARACTER_SNOWMAN:
		{
			yOffSet += 173.f;
			break;
		}
		case CHARACTER_ROBIN:
		{
			yOffSet += 472.f;
			break;
		}			
		case CHARACTER_ANGEL:
		{
			yOffSet += 122.f;
			break;
		}
		case CHARACTER_PIRATE:
		{
			yOffSet += 159.f;
			break;
		}
		default:
		{
			yOffSet += 0.f; //the default is not to move the face at all
		}
	}
	
	return yOffSet;
}

WKCharacterType ofxLocalFaceDisplayClientUtils::getRandomCharacterType() //return a random character
{
	float random = ofRandomuf(); //random number between 0 and 1
	
	int randomIndex = (int) (random*((float)(OFX_LOCAL_NUMBER_OF_CHARACTERS)));
	
	WKCharacterType randomCharacterType = CHARACTER_TURKEY; //everyone is turkey to start with (-;
	
	randomIndex = randomIndex % OFX_LOCAL_NUMBER_OF_CHARACTERS; //for safety
	
	while(randomIndex == oldCharacterType)
	{
		//no duplicates!
		random = ofRandomuf(); //random number between 0 and 1
		randomIndex = (int) (random*((float)(OFX_LOCAL_NUMBER_OF_CHARACTERS)));
	}
	
	switch(randomIndex) 
	{
		case 0:
		{
			randomCharacterType = CHARACTER_TURKEY;
			break;
		}
		case 1:
		{
			randomCharacterType = CHARACTER_CHOIRBOY;
			break;
		}	
		case 2:
		{
			randomCharacterType = CHARACTER_KING;
			break;
		}
		case 3:
		{
			randomCharacterType = CHARACTER_SANTA;
			break;
		}
		case 4:
		{
			randomCharacterType = CHARACTER_ELF;
			break;
		}
		case 5:
		{
			randomCharacterType = CHARACTER_PUDDING;
			break;
		}
		case 6:
		{
			randomCharacterType = CHARACTER_TREE;
			break;
		}
		case 7:
		{
			randomCharacterType = CHARACTER_DAME;
			break;
		}
		case 8:
		{
			randomCharacterType = CHARACTER_SNOWMAN;
			break;
		}				
		case 9:
		{
			randomCharacterType = CHARACTER_ROBIN;
			break;
		}
		case 10:
		{
			randomCharacterType = CHARACTER_ANGEL;
			break;
		}
		case 11:
		{
			randomCharacterType = CHARACTER_PIRATE;
			break;
		}			
		default:
		{
			randomCharacterType = CHARACTER_TURKEY;
		}
	}
	
	oldCharacterType = randomIndex;
	
	return randomCharacterType;
}

WKCharacterType ofxLocalFaceDisplayClientUtils::getCharacterType(string filenameOfFaceImage)  //filenames look like face_13_choirboy.jpg
{
	WKCharacterType characterTypeOfTheFile = CHARACTER_TURKEY; //everyone is a turkey to start with
	
	string underscore = "_";
	string extension = ".jpg";
	
	size_t found = filenameOfFaceImage.find(underscore);
	
	if( found == string::npos)
	{
		cout << "Error, character filename malformed " << filenameOfFaceImage << endl;
		return characterTypeOfTheFile;
	}
	
	found = filenameOfFaceImage.find(underscore, found+1); //find the next underscore, starting one character on from where the first was found
	
	size_t foundEnd = filenameOfFaceImage.find(extension);
	
	string theCharacter = filenameOfFaceImage.substr(found+1, foundEnd-found-1);
	
	//cout << "The character type is " << theCharacter << endl;
	
	characterTypeOfTheFile = getCharacterTypeFromCharacterName(theCharacter);
	
	return characterTypeOfTheFile;
}

void ofxLocalFaceDisplayClientUtils::drawBackground()
{
	//thanks to the magic of FBO's, I can just draw a single image now
	
	backgroundImage.draw(0,0); //0,0 as we are drawing in the fbo
	
//	float xPos = 0.f;
//	float yPos = WK_INT_SCREENS_HEIGHT - backgroundScreen1Image.height; //always draw them from the bottom up
//	
//	backgroundScreen1Image.draw(xPos,yPos);
//	backgroundScreen2Image.draw(xPos+WK_INT_SCREEN_WIDTH,yPos);	
}

void ofxLocalFaceDisplayClientUtils::localDrawNormal(float tweenedValue, ofImage* liveFaceImage, string theFeedbackText)
{
	//cout << "The tweened value " << tweenedValue << endl;
	float bodyOffset = WK_CHARACTER_OFFSET;
	float adjustedBack = tweenedValue+(bodyOffset*2); //two bodies of padding 
	int startingPosition = frontWKFace; //the oldest face, the one that should be most on the right
	int currentPosition = startingPosition;
	int resetPosition = OFX_LOCAL_NUMBER_OF_FACES-1;
	//float yCharacterPosition = WK_INT_SCREENS_HEIGHT-WK_CHARACTER_HEIGHT-140; //final int adjusts the position of all characters
	//draw the characters from the bottom
	//TODO: we may want the characters and all to be drawn at the top or the bottom, bottom for now, with prompt text above
	//but it might be nicer to look at your own eyes for posing...
	
	ofSetColor(255,255,255);
	
	RM.startOffscreenDraw();
	
	drawBackground();
	
	//we are all alpha'd now
	ofEnableAlphaBlending();
	
	if(firstFacesLoaded)
	{
		for(int i=0; i< OFX_LOCAL_NUMBER_OF_FACES; i++)
		{
			if(faces[currentPosition]->getFaceLoaded()) //only if it's loaded
			{
				if(faces[currentPosition]->getFaceResized()) //only if it's resized 
					//TODO: change this so the face size is consistent through capture, upload and local display, which will avoid
					//this resize loop
				{
					//draw the face
					ofImage* faceImagePointer = NULL;
					
					if((faces[currentPosition]->isLiveFace()) && !(faces[currentPosition]->isLiveFaceFrozen())) //if its live and the face hasn't been frozen
					{
						faceImagePointer = liveFaceImage;
					}
					else //its a normal face, or a liveFace that has been frozen
					{
						faceImagePointer = faces[currentPosition]->getFaceImage();
					}
					
					float xPos = adjustedBack - (bodyOffset*i);
					float yPos = WK_INT_SCREENS_HEIGHT-WK_CHARACTER_HEIGHT-150;
					float yFaceOffset = getFaceTopYPosition(faces[currentPosition]->getCharacterType());
					float xFaceOffset = getFaceLeftXPosition(faces[currentPosition]->getCharacterType());
					
					if(faceImagePointer)
					{
						//create a four channel RGBA texture, draw into it the RGB data from the face and the alpha channel from the head cropper
						ofTexture rgbaMixture;
						
						int w = faceImagePointer->width;
						int h = faceImagePointer->height;
						
						unsigned char * pixels = new unsigned char[w*h*4];
						unsigned char * colourPixels = faceImagePointer->getPixels();
						unsigned char * alphaPixels = wkHeadCropper.getPixels();
						
						for (int i = 0; i < w; i++){
							for (int j = 0; j < h; j++){
								int pos = (j * w + i);
								pixels[pos*4  ] = colourPixels[pos * 3];
								pixels[pos*4+1] = colourPixels[pos * 3+1];
								pixels[pos*4+2] = colourPixels[pos * 3+2];
								pixels[pos*4+3] = alphaPixels[pos];
							}
						}
						
						rgbaMixture.allocate(w,h,GL_RGBA);
						rgbaMixture.loadData(pixels, w,h,GL_RGBA);
						
						rgbaMixture.draw(xPos + xFaceOffset, yPos + yFaceOffset);					
						
						delete [] pixels;//MallocDebug.app rules!!!!!!!!
					}
					else
					{
						cout << "Couldn't get faceimage  for " << currentPosition << "th face, of location " << faces[currentPosition]->getFaceImagePath() << endl;
					}
					
					//draw the character finally
					ofImage* characterImagePointer = getCharacterImage(faces[currentPosition]->getCharacterType());
					if(characterImagePointer)
					{
						characterImagePointer->draw(xPos, yPos);
					}
					else
					{
						cout << "Couldn't get character image for " << currentPosition << "th face, of type " <<faces[currentPosition]->getCharacterType() << endl;
					}
					
					// debugging the order of the characters					
//					ofSetColor(255,0,0);
//					char tempStr2[255];
//					sprintf(tempStr2, "character: %i ", i);
//					font.drawString(tempStr2, xPos + xFaceOffset, yFacePos);
					ofSetColor(255,255,255);
				}
				else
				{
					//it has been loaded, but it hasn't been resized
					faces[currentPosition]->resizeFaceImage();
				}
			}
			
			//even if it isn't loaded, still move along
			if(currentPosition >= resetPosition)
			{
				currentPosition = 0;
			}
			else
			{
				currentPosition++;
			}
		}
	}
	
	ofDisableAlphaBlending();	//text is automatic
	
	drawFeedbackTextCentred(theFeedbackText);
	
	RM.endOffscreenDraw(); //can stop drawing to the FBO now	
}

void ofxLocalFaceDisplayClientUtils::localDraw(float tweenedValue, ofImage* liveFaceImage, string theFeedbackText)
{
	switch(state)
	{
		case WINDOW_STATE_LOADING:
		{
			//cout << "WKWindowLoading..." << endl;
			break;			
		}
		case WINDOW_STATE_IDLE:
		{
			//cout << "WKWindowIDLE..." << endl;
			//just animate the tweened Value as normal
			localDrawNormal(tweenedValue, liveFaceImage, theFeedbackText);
			break;
		}
		case WINDOW_STATE_FACE_FOUND:
		{
			//cout << "WKWindowFace_Found..." << endl;
			//animate the tweened value as normal for now
			localDrawNormal(tweenedValue, liveFaceImage, theFeedbackText);
			break;
		}
		case WINDOW_STATE_POSE:
		{
			//cout << "WKWindowPOSE..." << endl;
			//animate the tweened value as normal for now
			localDrawNormal(WK_CHARACTER_OFFSET, liveFaceImage, theFeedbackText); //TODO:have this as a sensible value
			break;
		}
		case WINDOW_STATE_GRAB_IMAGE:
		{
			//cout << "WKWindowGRAB_IMAGE..." << endl;
			//animate the tweened value as normal for now
			localDrawNormal(WK_CHARACTER_OFFSET, liveFaceImage, theFeedbackText); //TODO:have this as a sensible value
			break;
		}				
		default: //don't draw anything in the default
		{
			//cout << "ERROR:How am I default?BADABADABADBDBDBADAD Call Joel" << endl;
			break;			
		}
	}
}

void ofxLocalFaceDisplayClientUtils::previewDraw(float _previewX, float _previewY, float _previewWidth, float _previewHeight)
{
	ofSetColor(255,255,255);
	RM.drawDiagnostically(_previewX, _previewY, _previewWidth, _previewHeight);	
}

void ofxLocalFaceDisplayClientUtils::externalDraw()
{		
	ofSetColor(255,255,255);
	glPushMatrix();
	glTranslatef(WK_GUI_SCREEN_WIDTH+WK_INT_SCREENS_HEIGHT, 0, 0);
	glRotatef(90, 0, 0, 1); 
	RM.drawScreen(0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(WK_GUI_SCREEN_WIDTH+WK_INT_SCREENS_HEIGHT+WK_INT_SCREENS_HEIGHT, 0, 0);
	glRotatef(90, 0, 0, 1); 
	RM.drawScreen(1);
	glPopMatrix();	
}

void ofxLocalFaceDisplayClientUtils::handleMouseMove(ofRectangle r, ofPoint p)
{
	RM.handleMouseMove(r,p);
}


void ofxLocalFaceDisplayClientUtils::handleMouseDrag(ofRectangle r, ofPoint p)
{
	RM.handleMouseDrag(r,p);
}

WKCharacterType ofxLocalFaceDisplayClientUtils::setLiveFaceToStillAndGetCharacterType(ofImage* faceImageToSaveFrom)
{
	int indexToLiveFace = -1;
	
	WKCharacterType liveFaceCharacterType = CHARACTER_TURKEY; //all turkeys to start with
	
	if(firstFacesLoaded)
	{
		for(int i=0; i< OFX_LOCAL_NUMBER_OF_FACES; i++)
		{
			if(faces[i]->getFaceLoaded() && faces[i]->isLiveFace() && !(faces[i]->isLiveFaceFrozen())) //only if it's loaded and it's live and un frozen
			{
				indexToLiveFace = i;
				liveFaceCharacterType = faces[i]->getCharacterType();
			}
		}
	}
	
	if(indexToLiveFace > -1)
	{
		//then we have a face to freeze
		ofImage* faceImagePointer = faces[indexToLiveFace]->getFaceImage();
		
		if(faceImagePointer)
		{
			int pixSize = faceImageToSaveFrom->width*faceImageToSaveFrom->height*3; //RGB so times 3
			unsigned char * pixArr = new unsigned char[pixSize];
			memcpy(pixArr,faceImageToSaveFrom->getPixels(), pixSize);
			faceImagePointer->setFromPixels(pixArr, faceImageToSaveFrom->width, faceImageToSaveFrom->height, OF_IMAGE_COLOR); //remembering to set correct type
			delete pixArr; //MallocDebug.app rules!!!!!!!!
			faces[indexToLiveFace]->setLiveFaceFrozen(true); //we have frozen it sucessfully
		}
		else
		{
			//we couldn't get an image pointer
			cout << "Couldn't get faceimage to freeze for " << indexToLiveFace << "th face" << endl;
		}
	}
	else
	{
		//we want to freeze a face but we can't find one to freeze
		cout << "ERROR:How come we could find a live face to freeze? CAll joel." << endl;
		
	}
	
	return liveFaceCharacterType;
}

void ofxLocalFaceDisplayClientUtils::drawFeedbackTextCentred(string theFeedbackText)
{
	//ofSetColor(204,153,51); old gold
	ofSetColor(255, 0, 0);
	float halfStringWidth = (font.stringWidth(theFeedbackText))/2.f;
	float centrePosition = WK_INT_SCREEN_WIDTH/2.f;
	float feedbackHorizPos = centrePosition - halfStringWidth;
	font.drawString(theFeedbackText, feedbackHorizPos, 140);
}