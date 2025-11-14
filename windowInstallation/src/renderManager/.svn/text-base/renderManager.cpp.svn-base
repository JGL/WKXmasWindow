

#include "renderManager.h"

//---------------------------------------------------------------------------
renderManager::renderManager(){

}

//---------------------------------------------------------------------------
void renderManager::allocate(float renderWidth, float renderHeight){
	width = renderWidth;
	height = renderHeight;
	myOffscreenTexture.allocate(renderWidth, renderHeight, true);
}

//---------------------------------------------------------------------------
void renderManager::allocateBasedOnThreeScreenDisplay(){

    nScreens            = 3;
    outputPositions     = new ofPoint*[nScreens];
    inputPositions      = new ofPoint*[nScreens];

    float w_d_3 = width / 3.0f;
    float inner_d_3 = 1.0f / 3.0f;
    for (int i = 0; i < 3; i++){

        inputPositions[i] = new ofPoint[4];
        outputPositions[i] = new ofPoint[4];

        //-----------------------------------------
        outputPositions[i][0].set(w_d_3*(i),0,0);
        outputPositions[i][1].set(w_d_3*(i+1),0,0);
        outputPositions[i][2].set(w_d_3*(i+1),height,0);
        outputPositions[i][3].set(w_d_3*(i),height,0);

        inputPositions[i][0].set(inner_d_3*(i),0,0);
        inputPositions[i][1].set(inner_d_3*(i+1),0,0);
        inputPositions[i][2].set(inner_d_3*(i+1),1-ofRandom(0.8,0.9),0);
        inputPositions[i][3].set(inner_d_3*(i),1,0);
    }
}

//---------------------------------------------------------------------------
void renderManager::allocateBasedOnTwoScreenDisplay(){
	
    nScreens            = 2;
    outputPositions     = new ofPoint*[nScreens];
    inputPositions      = new ofPoint*[nScreens];
	
    float w_d_2 = width / 2.0f;
    float inner_d_2 = 1.0f / 2.0f;
    for (int i = 0; i < 2; i++){
		
        inputPositions[i] = new ofPoint[4];
        outputPositions[i] = new ofPoint[4];
		
        //-----------------------------------------
//        outputPositions[i][0].set(w_d_2*(i),0,0);
//        outputPositions[i][1].set(w_d_2*(i+1),0,0);
//        outputPositions[i][2].set(w_d_2*(i+1),height,0);
//        outputPositions[i][3].set(w_d_2*(i),height,0); original
		
        outputPositions[i][0].set(w_d_2*(0),0,0); //setting the two output to the same place
        outputPositions[i][1].set(w_d_2*(1),0,0);
        outputPositions[i][2].set(w_d_2*(1),height,0);
        outputPositions[i][3].set(w_d_2*(0),height,0);
		
		
        inputPositions[i][0].set(inner_d_2*(i),0,0);
        inputPositions[i][1].set(inner_d_2*(i+1),0,0);
        inputPositions[i][2].set(inner_d_2*(i+1),1,0);
        inputPositions[i][3].set(inner_d_2*(i),1,0);
    }
}

//---------------------------------------------------------------------------
void renderManager::startOffscreenDraw()
{
    myOffscreenTexture.swapIn();
    myOffscreenTexture.setupScreenForMe();
}

//---------------------------------------------------------------------------
void renderManager::endOffscreenDraw()
{
    myOffscreenTexture.swapOut();
    myOffscreenTexture.setupScreenForThem();
}

//---------------------------------------------------------------------------
void renderManager::drawDiagnostically(float x, float y, float w, float h)
{
    myOffscreenTexture.draw(x,y,w,h);
	
    for (int i = 0; i < nScreens; i++){
        ofSetColor(0x00ff00);
        ofNoFill();
        ofBeginShape();
        for (int j = 0; j < 4; j++){
            ofVertex( inputPositions[i][j].x * w + x, inputPositions[i][j].y * h + x);
        }
        ofEndShape(true);

        ofFill();
        ofSetColor(0x0000ff);
        for (int j = 0; j < 4; j++){
            ofCircle(  inputPositions[i][j].x * w + x, inputPositions[i][j].y * h + x,3);
        }
    }

}

//---------------------------------------------------------------------------
void renderManager::drawScreen(int nScreen){
    myOffscreenTexture.setPoints(inputPositions[nScreen], outputPositions[nScreen]);
    myOffscreenTexture.draw();
}

//---------------------------------------------------------------------------
void renderManager::handleMouseMove(ofRectangle drawRect, ofPoint mousePoint){

    activeScreen = -1;
    activePoint = -1;

    for (int i = 0; i < nScreens; i++){
        for (int j = 0; j < 4; j++){
            ofPoint inputPt;
            inputPt.x = inputPositions[i][j].x * drawRect.width + drawRect.x;
            inputPt.y = inputPositions[i][j].y * drawRect.height + drawRect.y;
            inputPt.z = 0;
            float len = sqrt( (inputPt.x - mousePoint.x) * (inputPt.x - mousePoint.x) +
                              (inputPt.y - mousePoint.y) * (inputPt.y - mousePoint.y));
            if (len < 3){
                activeScreen = i;
                activePoint = j;
            }
        }
    }
}


void renderManager::handleMouseDrag(ofRectangle drawRect, ofPoint mousePoint){

    if (activeScreen > -1){

        float xpct = (mousePoint.x - drawRect.x) / drawRect.width;
        float ypct = (mousePoint.y - drawRect.y) / drawRect.height;
        inputPositions[activeScreen][activePoint].x = xpct;
        inputPositions[activeScreen][activePoint].y = ypct;

    }
}

