
#ifndef _RENDER_MANAGER_H
#define _RENDER_MANAGER_H


#include "ofTextureAdv.h"

class renderManager {


    public:

        renderManager();
        void allocate(float renderWidth, float renderHeight);

        float width, height;

        void allocateBasedOnThreeScreenDisplay();   // just demo allocation;
		void allocateBasedOnTwoScreenDisplay(); //jgl addition

        int nScreens;
        ofPoint ** outputPositions;
        ofPoint ** inputPositions;

        void loadFromXml();
        void saveToXml();

        void drawDiagnostically(float x, float y, float w, float h); // can't specify abs dimensions, since might be diff apsect ratio...
        void drawScreen(int nScreen);
		void drawRotated0()
		{
			//ofSetRectMode(OF_RECTMODE_CENTER);
			glPushMatrix();
			glTranslatef(width/nScreens, 0, 0);
			glRotatef(90, 0, 0, 1); 
			drawScreen(0);
			//drawScreen(1);
			glPopMatrix();
			//ofSetRectMode(OF_RECTMODE_CORNER);
		}
		void drawRotated1()
		{
			//ofSetRectMode(OF_RECTMODE_CENTER);
			glPushMatrix();
			glTranslatef(width/nScreens, 0, 0);
			glRotatef(90, 0, 0, 1); 
			drawScreen(1);
			glPopMatrix();
			//ofSetRectMode(OF_RECTMODE_CORNER);
		}	
	

        void startOffscreenDraw();
        void endOffscreenDraw();

        ofTextureAdv myOffscreenTexture;

        void handleMouseMove(ofRectangle drawRect, ofPoint mousePoint);
        void handleMouseDrag(ofRectangle drawRect, ofPoint mousePoint);
        int    activeScreen;
        int    activePoint;

};

#endif

