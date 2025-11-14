#ifndef _TEST_APP
#define _TEST_APP


#define OF_ADDON_USING_OFXOPENCV
//#define OF_ADDON_USING_OFXDIRLIST
#define OF_ADDON_USING_OFXCPPGLUE_ANIMATION

#include "ofMain.h"
#include "ofAddons.h"
#include "wkConstants.h"

//this is not part of the addon yet
//so include it seperately
#include "ofxCvHaarFinder.h"

typedef Poco::Timestamp ofTimestamp;

//remote face upload client bits
#include "ofxRemoteFaceUploadClient.h"
//local face display client bits
#include "ofxLocalFaceDisplayClient.h"

//finally the pocoDirectoryLister
#include "pocoDirectoryLister.h"

class testApp : public ofSimpleApp{

	public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
private:
	
	void grabPixelsIntoImage(ofImage* grabIntoThis, ofxCvColorImage* grabFromThis, 	float x, float y, float w, float h);
	void findBiggestFaceAndCopyIntoGrabbedFace(float offSetX, float offSetY);
	void saveFaceRemotelyWithCharacterType(WKCharacterType remoteFaceCharacterType);
	void resetTweenAndLoadNext(); //reset the tween when scrolling and start the load of the next face
	void resetTweenAndLoadLiveFace(); //reset the tween when face found and set the new face to be live
	void resetTweenToSecondsForPose(); //reset the tween to a per second loop to allow for a count down text to be displayed;
	void resetTweenToSecondsForGrab(); //reset the tween to a longer loop
	void updateGUIStatusText();	//update the status text depending on the state of the window
	string updateWindowStatusText(); //update the windows text display
private:

	ofVideoGrabber			vidGrabber; //the video grabber

	ofxCvColorImage			colorImg;	//the image to put the video frame into
	
	ofxCvGrayscaleImage 	grayImage;	//the greyscale image which is fed to the HaarFinder
	ofxCvGrayscaleImage 	grayBg;		//a grey background image
	ofxCvGrayscaleImage 	grayDiff;	//the difference between the background and the livevideo image
	
	ofxCvHaarFinder			haarFinder;

	int						threshold;
	bool					bLearnBakground;
	
	ofImage					grabbedFace; //can't be bothered to fix it properly for the demo
	
	ofxPocoDirectoryLister			facesCompletedDirectoryLister;//for saving of faces
	ofxPocoDirectoryLister			facesAwaitingDirectoryLister;//for saving of faces
		
	bool					saveFaceThisFrame;
	
	ofxLocalFaceDisplayClientUtils ofxLocalFaceDisplayClient;
	ofxRemoteFaceUploadClientUtils ofxRemoteFaceUploadClient;
	
	//testing tween stuff
	Animator animator;
	void onTweenCompleted(const void* pSender, EventArgs & args);
	float leftPos, rightPos;
	uint duration;
	uint delay;
	float back;
	
	int boxW;
	int boxH;
	int loopsToPoseFromFaceFound;
	int loopsToGrabFromPose;
	int loopsToIdleFromGrab;
	
	//bool animatePeople;
	
	bool newTweenAndLoadNext; //a bool for controlling the scroll out for now
	
	bool switchToFoundFromIdle;
	
	bool switchToPoseFromFound; //a bool for switching from the found to pose states
	
	bool switchToGrabFromPose;	//a bool for switching from the post to grab states
	
	bool switchToIdleFromGrab;	//a bool for switching from the grab to idle states
	
	bool newPoseTween;			//a bool to indicate if we need a new pose tween
	
	bool newGrabTween;			//a bool to indicate if we need a new grab tween
	string statusText;			//status text for gui NOT for display TODO:make the string handling bit for the guys to write copy for...

	bool foundFaceThisFrame;	//did we find a face this frame?
	int numberOfSolidFaceFrames; //the number of consecutive faces we have found
	int numberOfDots;
};

#endif