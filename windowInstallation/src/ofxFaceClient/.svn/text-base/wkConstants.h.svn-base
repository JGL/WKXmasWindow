/*
 *  wkConstants.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 18/11/2008.
 *	A load of constants for the WK Xmas Window
 *
 */

#define WK_OF_SCREEN_WIDTH 3744		//total width of the OF graphics system 1024+1360+1360
#define WK_OF_SCREEN_HEIGHT 768		//total height...
#define WK_EXT_SCREENS_WIDTH 2720	//total width of the external screens from the EXTernal point of view, i.e. the OF graphics system and the raw monitor outputs
#define WK_EXT_SCREENS_HEIGHT 768	//total height...
#define WK_EXT_SCREEN_WIDTH 1360	//single width of an external screen from the EXTernal point of view, i.e. the OF graphics system and the raw monitor outputs
#define WK_EXT_SCREEN_HEIGHT 768	//single height...
#define WK_NUMBER_SCREENS 2		//the number of external screens
#define WK_INT_SCREENS_WIDTH 1536	//the width of the texture that the system draws into to allow for simple animation - i.e. not having to worry about external
									//	monitor orientation/real world orientation
#define WK_INT_SCREENS_HEIGHT 1360	//the height of the texture...
#define WK_INT_SCREEN_WIDTH 768		//the screen height in the internal texture
#define WK_INT_SCREEN_HEIGHT 1360	//the screen width...

#define WK_GUI_SCREEN_WIDTH 1024	//the internal gui width
#define WK_GUI_SCREEN_HEIGHT 768	//the internal gui height

#define WK_LOCAL_COMPLETED_DIRECTORY "addedToDatabase"
#define WK_LOCAL_AWAITING_DIRECTORY "awaitingUpload"

//the capture size is the KEY performance killer, but multiplied by performance of the face finder...
#define WK_VIDEO_CAPTURE_WIDTH 320
#define WK_VIDEO_CAPTURE_HEIGHT 240
//miniumum face area to be defined as a valid face that we are interested in
#define WK_MIN_FACE_AREA 1000

//FTP access details
#define OFX_FTP_HOST "127.0.0.1"
#define OFX_FTP_USER "user"
#define OFX_FTP_PASS "pass"
#define OFX_FTP_REMOTE_DIRECTORY "faces/"

//HTTP FORM constants
#define OFX_HTTP_GET  0
#define OFX_HTTP_POST 1

//HTTP FORM details
#define OFX_HTTP_ACTION "http://localhost/backend/submit.php";
#define OFX_HTTP_USER "user"
#define OFX_HTTP_PASSWORD "pass"

//sizing of the graphics for the background images, and character images
#define WK_BACKGROUND_WIDTH 768
#define WK_BACKGROUND_HEIGHT 1024
#define WK_CHARACTER_WIDTH 768
#define WK_CHARACTER_HEIGHT 1024
#define WK_CHARACTER_OFFSET 670

//for the client, defining the number of faces to keep in the array and the number of characters that we have to deal with
#define OFX_LOCAL_NUMBER_OF_FACES 5 //no more than 7 in the queue overall at any one time
#define OFX_LOCAL_NUMBER_OF_CHARACTERS 12 //12 characters

#define OFX_REMOTE_NUMBER_OF_FACES_TO_TRY_TO_UPLOAD 100 //100 faces in the remote queue, if it gets worse than this then there is really trouble 
//TODO: neaten this in some way?

#define OFX_PREVIEW_SCREENS_X 0
#define OFX_PREVIEW_SCREENS_Y 0
#define OFX_PREVIEW_SCREENS_WIDTH 512
#define OFX_PREVIEW_SCREENS_HEIGHT 384

//for the display on the screens
#define WK_DISPLAY_FACE_WIDTH 155
#define WK_DISPLAY_FACE_HEIGHT 167

//TODO:acutally set up the live face in the system, not just counting down
#define WK_NUMBER_OF_FACES_BEFORE_START_POSE_FROM_FACE_FOUND 1 //i think this is right, with number of faces set to 7, that is what this is dependent on JUST SET QUICK FOR NOW
#define WK_NUMBER_OF_SECONDS_BEFORE_START_GRAB_FROM_POSE 3 //this is the number of seconds of countdown we have before taking a picture
#define WK_NUMBER_OF_SECONDS_AFTER_GRAB_BEFORE_BACK_TO_IDLE 4 //this is the number of seconds after grabbing the new face before going back to idle

#define WK_ONE_SECOND 1000 //a second in milis

#define WK_NUMBER_OF_CONSECUTIVE_FACES_FOR_GRAB 20 //the number of consecutive faces to get to grabbing