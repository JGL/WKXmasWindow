/*
 *  ofxLocalFaceDisplayClient.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 09/11/2008.
 *
 *
 */

#ifndef _OFX_LOCAL_FACE_DISPLAY_TOOLS
#define _OFX_LOCAL_FACE_DISPLAY_TOOLS

#include "ofMain.h"

#define OF_ADDON_USING_OFXTHREAD
#define OF_ADDON_USING_OFXVECTORMATH
//#define OF_ADDON_USING_OFXDIRLIST
#include "ofAddons.h"

#include "wkConstants.h"

#include <queue>
#include <list>
#include <vector>

#include "ofFBOTexture.h"
#include "ofTextureAdv.h"
#include "renderManager.h"

#include "pocoDirectoryLister.h"

typedef enum {WINDOW_STATE_LOADING, WINDOW_STATE_IDLE, WINDOW_STATE_FACE_FOUND, WINDOW_STATE_POSE, WINDOW_STATE_GRAB_IMAGE, WINDOW_STATE_ACCELERATE_TO_IDLE, WINDOW_STATE_SLOW_TO_POSE} WKWindowState; 
typedef enum {CHARACTER_TURKEY, CHARACTER_CHOIRBOY, CHARACTER_KING, CHARACTER_SANTA, CHARACTER_ELF, CHARACTER_PUDDING, CHARACTER_TREE, CHARACTER_DAME, CHARACTER_SNOWMAN, CHARACTER_ROBIN, CHARACTER_ANGEL, CHARACTER_PIRATE} WKCharacterType;

class ofxLocalFace:public ofxThread{
public:
	
	ofxLocalFace(string pathToFaceImage, WKCharacterType typeOfCharacter);
	~ofxLocalFace();
	
	void reset(string pathToFaceImage, WKCharacterType typeOfCharacter);
	
	//-------------------------------
	// non blocking functions
	ofImage*		getFaceImage();	
	WKCharacterType	getCharacterType(){
		return character;
	}
	bool			getFaceLoaded(){
		return faceLoaded;
	}
	bool			getFaceResized(){
		return faceResized;
	}	
	
	string getFaceImagePath()
	{
		return faceImagePath;
	}
	
	void setLoaded(bool ifLoaded)
	{
		faceLoaded = ifLoaded;
	}
	
	bool isLiveFace()
	{
		return liveFace;
	}
	
	void setLiveFace(bool isLive)
	{
		liveFace = isLive;
	}
	
	bool isLiveFaceFrozen()
	{
		return liveFaceFrozen;
	}
	
	void setLiveFaceFrozen(bool isLiveFrozen)
	{
		liveFaceFrozen = isLiveFrozen;
	}	
	
	void setResized(bool isResized)
	{
		faceResized = isResized;
	}
	
	//-------------------------------
	// blocking functions
	void			loadFaceImage();
	void			resizeFaceImage(); //check for load and try a resize to the set face display size
	
	//-------------------------------
	// threading stuff
	void			threadedFunction();
	void			start();
	void			stop();
protected:
	string			faceImagePath;
	ofImage			faceImage;	
	bool			faceLoaded;
	bool			faceResized;
	WKCharacterType character;
	bool			liveFace;
	bool			liveFaceFrozen;
};

class ofxLocalFaceDisplayClientUtils{
	//}:public ofxThread{ //no longer threaded
public:
	
	ofxLocalFaceDisplayClientUtils();//(ofxPoint2f offsetScreen1, ofxPoint2f offsetScreen2);
	~ofxLocalFaceDisplayClientUtils();
	
	//-------------------------------
	// non blocking functions //not threaded at the moment
	void update();
	void init();
	void loadFirstFaces(); //load the first lot of faces in
	void loadAllCharacters(); //load all the character images in
	ofImage * getCharacterImage(WKCharacterType typeOfCharacter); //return an ofImage * for the character in question
	WKCharacterType getCharacterTypeFromCharacterName(string theCharacter); //take a string, return the character type
	string getCharacterNameFromCharacterType(WKCharacterType theCharacterType); //take a character type, return a string
	WKCharacterType getCharacterType(string filenameOfFaceImage); //take a face filename, break it up into _ separated bits and match the 0,1,2 substring 2nd substring
	// to find
	float getFaceLeftXPosition(WKCharacterType characterType);
	float getFaceTopYPosition(WKCharacterType characterType); // get the central y positon for the face
	//if the queue is empty then fill it up to ten!
	//check where we are with the images - if pos (i.e. upper right hand corner) is off screen left then we know we can pop it off the queue and create another face and start it loading
	void draw(float tweenedY);
	//void addLocalFace(string pathToFaceImage, string pathToCharacterImage);
	//-------------------------------
	// blocking functions //not threaded at the moment
	void drawLocalFace(ofxLocalFace & face);						//load into a texture and display //should this be by reference or object or pointer?
	void initCharacters();
	void loadNextFace();											//load a new face at backWKFace, increment frontWKFace, backWKFace
	void loadNextLiveFace();										//load a new live face at backWKFACE, incremement front WKFACE, backFace
	void localDrawNormal(float tweenedValue, ofImage* liveFaceImage, string theFeedbackText);						//draws the screen in the normal idle state, and others sometimes
	void localDraw(float tweenedValue, ofImage* liveFaceImage, string theFeedbackText);								//localDraw function here of the face display client which draws everything to the local clients fbo
	void previewDraw(float _previewX, float _previewY, float _previewWidth, float _previewHeight);												//the previewDraw function here of the face display client which draws the scaled fbo to the GUI screen
	void externalDraw();											//the externalDraw funciton here of the face display client, which draws the rotated and cropped FBO's to the external
	void loadBackground();											//loads the background images, should do a resize here 
	void drawBackground();											//draw the background images
	void handleMouseMove(ofRectangle r, ofPoint p);					//for handling movign of the mouse over the preview
	void handleMouseDrag(ofRectangle r, ofPoint p);					//for handling draggin of the mouse over the preview
	WKWindowState getState()
	{
		return state;
	}
	
	void setState(WKWindowState WKWS)
	{
		state = WKWS;
	}
	
	WKCharacterType getRandomCharacterType();						//returns a random character type
	WKCharacterType setLiveFaceToStillAndGetCharacterType(ofImage* faceImageToSaveFrom); //sets the face to still and returns the character type
	void drawFeedbackTextCentred(string theFeedbackText); // draw some feedback text to the top of the screen
private:
	int					frontWKFace; //these are array indexes to the front and back of the face "queue" respectively
	int					backWKFace;
	ofxLocalFace * faces[OFX_LOCAL_NUMBER_OF_FACES];
	vector <ofImage>	wkCharacters; //the characters
	ofImage				wkHeadCropper; //the head cropper, very cunning
	ofxPocoDirectoryLister			characterDirectoryLister;//for loading of characters
	ofxPocoDirectoryLister			faceDirectoryLister;//for saving of faces
	WKWindowState		state;
	long					lastFaceLoaded;
	bool				charactersInited;
	bool				firstFacesLoaded;
	renderManager		RM;
	ofImage				backgroundImage;	
	ofTrueTypeFont		font;
	int					oldCharacterType;
};


//what does this indicate precisely? class will be instantiated with the name ofxRemberFaceUploadClient? see EOF of cpp file
//extern ofxLocalFaceDisplayClientUtils ofxLocalFaceDisplayClient;
#endif