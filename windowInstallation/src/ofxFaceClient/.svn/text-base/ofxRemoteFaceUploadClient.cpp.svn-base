/*
 *  ofxRemoteFaceUploadClient.cpp
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 09/11/2008.
 *
 *
 */

#include "ofxRemoteFaceUploadClient.h"

ofxRemoteFaceUploadClientUtils::ofxRemoteFaceUploadClientUtils()
{	
	Path incompleteFacePath = Path(ofToDataPath(WK_LOCAL_AWAITING_DIRECTORY));
	incompleteFaceDirectoryLister.setExtensionToLookFor("jpg");//for saving of faces
	incompleteFaceDirectoryLister.setPath(incompleteFacePath);
	
	numberOfFacesInQueue = 0;
}

// ----------------------------------------------------------------------
ofxRemoteFaceUploadClientUtils::~ofxRemoteFaceUploadClientUtils(){
	if(isThreadRunning())
		stop();
}
//-------------------------------
// non blocking functions
void ofxRemoteFaceUploadClientUtils::addFace(ofxRemoteFace remoteFace )
{	
	checkLocalFace(remoteFace);
	faces[numberOfFacesInQueue] = &remoteFace;
	numberOfFacesInQueue++;

	if(!isThreadRunning())
		start();
}

int ofxRemoteFaceUploadClientUtils::typeOfCharacter(ofxRemoteFace* face)
{
	int theType = 0; //everyone is a turkey to start with
	
	if(face->characterType == CHARACTER_TURKEY)
	{
		theType = 0;
		return theType;
	}
	else if(face->characterType == CHARACTER_CHOIRBOY)
	{
		theType = 1;
		return theType;
	}		
	else if(face->characterType == CHARACTER_KING)
	{
		theType = 2;
		return theType;
	}
	else if(face->characterType == CHARACTER_SANTA)
	{
		theType = 3;
		return theType;
	}		
	else if(face->characterType == CHARACTER_ELF)
	{
		theType = 4;
		return theType;
	}		
	else if(face->characterType == CHARACTER_PUDDING)
	{
		theType = 5;
		return theType;
	}		
	else if(face->characterType == CHARACTER_TREE)
	{
		theType = 6;
		return theType;
	}		
	else if(face->characterType == CHARACTER_DAME)
	{
		theType = 7;
		return theType;
	}		
	else if(face->characterType == CHARACTER_SNOWMAN)
	{
		theType = 8;
		return theType;
	}		
	else if(face->characterType == CHARACTER_ROBIN)
	{
		theType = 9;
		return theType;
	}		
	else if(face->characterType == CHARACTER_ANGEL)
	{
		theType = 10;
		return theType;
	}		
	else if(face->characterType == CHARACTER_PIRATE)
	{
		theType = 11;
		return theType;
	}			
	
	return theType;
}

int ofxRemoteFaceUploadClientUtils::typeOfCharacter(ofxRemoteFace face)
{
	int theType = 0; //everyone is a turkey to start with
	
	if(face.characterType == CHARACTER_TURKEY)
	{
		theType = 0;
		return theType;
	}
	else if(face.characterType == CHARACTER_CHOIRBOY)
	{
		theType = 1;
		return theType;
	}		
	else if(face.characterType == CHARACTER_KING)
	{
		theType = 2;
		return theType;
	}
	else if(face.characterType == CHARACTER_SANTA)
	{
		theType = 3;
		return theType;
	}		
	else if(face.characterType == CHARACTER_ELF)
	{
		theType = 4;
		return theType;
	}		
	else if(face.characterType == CHARACTER_PUDDING)
	{
		theType = 5;
		return theType;
	}		
	else if(face.characterType == CHARACTER_TREE)
	{
		theType = 6;
		return theType;
	}		
	else if(face.characterType == CHARACTER_DAME)
	{
		theType = 7;
		return theType;
	}		
	else if(face.characterType == CHARACTER_SNOWMAN)
	{
		theType = 8;
		return theType;
	}		
	else if(face.characterType == CHARACTER_ROBIN)
	{
		theType = 9;
		return theType;
	}		
	else if(face.characterType == CHARACTER_ANGEL)
	{
		theType = 10;
		return theType;
	}		
	else if(face.characterType == CHARACTER_PIRATE)
	{
		theType = 11;
		return theType;
	}			
	
	return theType;
}
//-------------------------------
// blocking functions
void ofxRemoteFaceUploadClientUtils::checkLocalFace(ofxRemoteFace & face)
{
	std::ostringstream oss; //tidy way of to atoi etc, more std.
	oss << WK_LOCAL_AWAITING_DIRECTORY << "/" << face.filename;
	face.faceImage.saveImage(oss.str());  //should be some kind of IO space etc here no? filespace left, filecreation suceeded
	face.exists = true;
}

void ofxRemoteFaceUploadClientUtils::uploadFace(ofxRemoteFace* face)
{
	std::ostringstream remoteOSS;
	remoteOSS << OFX_FTP_REMOTE_DIRECTORY << face->filename;
	
	std::ostringstream localOSS;
	localOSS << WK_LOCAL_AWAITING_DIRECTORY << "/" << face->filename;
	std::string localPath = ofToDataPath(localOSS.str());
	
	try{
		
		// should try to do this properly with a  session and all, in the class
		Poco::Net::FTPClientSession* ftpClient = new Poco::Net::FTPClientSession(OFX_FTP_HOST, 21); 
		ftpClient->login(OFX_FTP_USER, OFX_FTP_PASS); 
		std::ostream& ftpOStream = ftpClient->beginUpload(remoteOSS.str().c_str());  //how to make it overwrite?
		std::ifstream localIFStream(localPath.c_str(), std::ifstream::in); 
		Poco::StreamCopier::copyStream(localIFStream, ftpOStream); 
		ftpClient->endUpload(); 
		ftpClient->close(); 
		delete ftpClient; 
		ftpClient=0; 		
	} 
	catch (Poco::Exception& exc) 
	{ 
		cout << exc.displayText() << endl; 
	} 	
}

void ofxRemoteFaceUploadClientUtils::addToDatabase(ofxRemoteFace* face)
{
	ofxHttpForm form;
	
	int numberForCharacter = typeOfCharacter(face);
	
	std::ostringstream characterOSS;
	characterOSS << numberForCharacter;
	
	std::string numberForCharacterAsString = characterOSS.str();
	
	form.method= OFX_HTTP_GET;
	form.name= "sermadJGL";
	form.action= OFX_HTTP_ACTION;
	form.addFormField("path", face->filename);
	form.addFormField("costumeid", numberForCharacterAsString);
	form.addFormField("SUBMIT","SUBMIT");
	
	//if(form.method==OFX_HTTP_POST){ //strickly should do check here, but always doing get
		
	string url = generateUrl(form);
	getUrl(url);
	
	face->existsRemotely = true;
	cout << "ofxRemoteFaceUploadClientUtils: (thread running) form submitted (get): " << form.name << endl; 
}

void ofxRemoteFaceUploadClientUtils::copyLocalFace(ofxRemoteFace* face)
{
	std::ostringstream localPathBeforeCopyOSS;
	std::ostringstream localPathAfterCopyOSS;
	localPathBeforeCopyOSS << WK_LOCAL_AWAITING_DIRECTORY << "/" << face->filename;
	localPathAfterCopyOSS << WK_LOCAL_COMPLETED_DIRECTORY << "/";
	
	std::string localPathBeforeCopy = ofToDataPath(localPathBeforeCopyOSS.str());
	std::string localPathAfterCopy  = ofToDataPath(localPathAfterCopyOSS.str());	
	
	//copy the file using poco
	Poco::Path p(localPathBeforeCopy);
	Poco::File f(p);
	
	if(f.exists())
	{
		f.copyTo(localPathAfterCopy); //copy
		f.remove(); //delete original file
		face->copied = true;
		cout << "ofxRemoteFaceUploadClientUtils: copyLocalFace(thread running) to: " << localPathAfterCopy << face->filename << endl; 
	}
	else
	{
		cout << "ofxRemoteFaceUploadClientUtils::copyLocalFace failed!" << endl; 
	}
}

// ----------------------------------------------------------------------

//I've taken this function out for now whilst I make everything run ok in a thread //cometn form chris or arturo

void ofxRemoteFaceUploadClientUtils::getUrl(string url)
{
	try{
		Poco::URI uri(url.c_str());
		std::string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";
		
		Poco::Net::HTTPBasicCredentials credentials(OFX_HTTP_USER, OFX_HTTP_PASSWORD);
		
		Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
		Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
		credentials.authenticate(req); //add the username and password to the request
		session.sendRequest(req);
		Poco::Net::HTTPResponse res;
		istream& rs = session.receiveResponse(res);
		// for now just print the response and returned data from page
		// need to broadcast events and data
		
		//ofxHttpResponse response=ofxHttpResponse(res,rs);
		
		//ofxHttpEvents.notifyNewResponse(response);
		std::cout << "got the URL back successfully" << res.getStatus() << " " << res.getReason() << std::endl;
		//Poco::StreamCopier::copyStream(rs, std::cout);
		
	}catch (Poco::Exception& exc){
		std::cerr << exc.displayText() << std::endl;
	}
}

WKCharacterType ofxRemoteFaceUploadClientUtils::getCharacterTypeFromCharacterName(string theCharacter)
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

WKCharacterType ofxRemoteFaceUploadClientUtils::getCharacterType(string filenameOfFaceImage)  //filenames look like face_13_choirboy.jpg
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

void ofxRemoteFaceUploadClientUtils::addOldFacesToQueue()
{
	int numberOfOldFaceFiles = (int)incompleteFaceDirectoryLister.getNumberOfFiles();
	
	if(numberOfOldFaceFiles == 0)
	{
		cout << "No face files found in " << WK_LOCAL_AWAITING_DIRECTORY << endl;
		return;
	}
	
	for(int i=0; i< numberOfOldFaceFiles; i++)
	{
		Path incompleteFacePath = incompleteFaceDirectoryLister.getPath((long)i); //get the filename from the directory
		
		WKCharacterType oldFaceCharacterType = getCharacterType(incompleteFacePath.getFileName());
		
		ofImage oldFaceImage;
		oldFaceImage.setUseTexture(false);
		oldFaceImage.loadImage(incompleteFacePath.toString());
		
		faces[numberOfFacesInQueue] = new ofxRemoteFace(incompleteFacePath.getFileName(), oldFaceImage, oldFaceCharacterType);
		faces[numberOfFacesInQueue]->exists = true;  //it does exist as we have just loaded it
		numberOfFacesInQueue++;
	}
}

// ----------------------------------------------------------------------
void ofxRemoteFaceUploadClientUtils::start() {
	if (isThreadRunning() == false){
		//clear the queue
		numberOfFacesInQueue = 0; //TODO: check the safety of how I am dealing with remote faces in the array
		addOldFacesToQueue();
        startThread(true,false); //blocking verbose
		printf("thread started\n");
    }
}
// ----------------------------------------------------------------------
void ofxRemoteFaceUploadClientUtils::stop() {
    stopThread();
}
// ----------------------------------------------------------------------
void ofxRemoteFaceUploadClientUtils::threadedFunction()
{
    // loop through this process whilst thread running
    while( isThreadRunning() == true ){
    	if(numberOfFacesInQueue > 0){
			//do all face checking here before finally popping it
			//checkLocalFace(faces.front());  checking in add now, to prevent the problem of faces being missed...
			uploadFace(faces[numberOfFacesInQueue-1]);	 ///array access!, so -1	
			addToDatabase(faces[numberOfFacesInQueue-1]);	
			copyLocalFace(faces[numberOfFacesInQueue-1]);
    		numberOfFacesInQueue--; //TODO: call the descructor too?
			//faces.pop();
			//printf("the number of faces to upload is %i\n", faces.size());
    	}else{
    		stop();
    	}
    }
	
}

string ofxRemoteFaceUploadClientUtils::generateUrl(ofxHttpForm & form)
{
    //nicked from OFXHTTP
	
	string url;
    url =form.action+"?";
    for(unsigned int i=0;i<form.formIds.size();i++){
    	url += form.formIds[i] +"="+ form.formValues[i];
    	if(i<form.formIds.size()-1)
    		url += "&";
    }
    return url;	
}

//ofxRemoteFaceUploadClientUtils ofxRemoteFaceUploadClient;

/*
 stole structure from
 
 ofxMailUtils
 author: arturo

#include "ofxMailUtils.h"  // has structure of smtp client and pop3 client and mail message
#include "ofxMailEvents.h" //just for POP3 stuff, not needed

// ----------------------------------------------------------------------
ofxSmtpClientUtils::ofxSmtpClientUtils(){
	session=new Poco::Net::SMTPClientSession(OFX_SMTP_HOST,OFX_SMTP_PORT);
	session->login();
}
// ----------------------------------------------------------------------
ofxSmtpClientUtils::~ofxSmtpClientUtils(){
    stop();
}
//-------------------------------
// non blocking functions
void ofxSmtpClientUtils::addMessage(ofxMailMessage message){
	messages.push(message);
	if(!isThreadRunning())
		start();
}

//-------------------------------
// blocking functions
void ofxSmtpClientUtils::sendMessage(ofxMailMessage & message){
	session->sendMessage(message.getPocoMessage());
}


// ----------------------------------------------------------------------
void ofxSmtpClientUtils::start() {
	if (isThreadRunning() == false){
        printf("thread started\n");
        startThread(true,true);
    }
}
// ----------------------------------------------------------------------
void ofxSmtpClientUtils::stop() {
    stopThread();
}
// ----------------------------------------------------------------------
void ofxSmtpClientUtils::threadedFunction(){
	
    // loop through this process whilst thread running
    while( isThreadRunning() == true ){
    	if(messages.size()){
    		sendMessage(messages.front());
    		messages.pop();
    	}else{
    		stop();
    	}
		
    }
	
}


ofxSmtpClientUtils ofxSmtpClient;

*/


