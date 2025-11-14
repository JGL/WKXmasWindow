/*
 *  ofxRemoteFaceUploadClient.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 09/11/2008.
 *
 *
 */

#ifndef _OFX_REMOTE_FACE_UPLOAD_TOOLS
#define _OFX_REMOTE_FACE_UPLOAD_TOOLS

#include "ofMain.h"

#define OF_ADDON_USING_OFXTHREAD
#include "ofAddons.h"

#include "wkConstants.h"

#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/Net/FTPClientSession.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <queue>

#include "ofxLocalFaceDisplayClient.h" // so we know the character types
#include "pocoDirectoryLister.h" //for disting directories

typedef Poco::Timestamp ofTimestamp;

// these were used for POP3 client so could notify when new messages recieved, unsure if I even need this structure.
//class ofxRemoteFaceListener;				//TODO: investigate where these should be defined according to the SMTP client layout
//class ofxRemoteFaceEventManager;					//TODO: investigate where there should be defined according to the SMTP client layout 
//and if they should be named differently, ofxRemoteFaceListener and ofxRemoteFaceEventManager

//to make handling of forms easier:
/*
 ofxHttpUtils
 Art
 */
struct ofxHttpForm{
	vector <string> formIds;
	vector <string> formValues;
	
	// ----------------------------------------------------------------------
	void addFormField(string id, string value){
	    formIds.push_back( id );
	    formValues.push_back( value );
	}
	// ----------------------------------------------------------------------
	void clearFormFields(){
	    formIds.clear();
	    formValues.clear();
	}
	int method;
	string action;
	string name;
};

struct ofxHttpResponse{
	ofxHttpResponse(Poco::Net::HTTPResponse& pocoResponse, istream &bodyStream, string turl){
		status = pocoResponse.getStatus();
		timestamp = pocoResponse.getDate();
		reasonForStatus = pocoResponse.getReasonForStatus(pocoResponse.getStatus());
		contentType = pocoResponse.getContentType();
		
		Poco::StreamCopier::copyToString(bodyStream, responseBody);
		
		url = turl;
		
	}
	
	ofxHttpResponse(){}
	
	int status; 				// return code for the response ie: 200 = OK
	string reasonForStatus;		// text explaining the status
	string responseBody;		// the actual response
	string contentType;			// the mime type of the response
	ofTimestamp timestamp;		// time of the response
	string url;
};

struct ofxRemoteFace{
	//ofxRemoteFace(int id, Poco::Net::MailMessage& mailMessage){
		//may need to replicate this for stamped creation
		//try{
		//	date=mailMessage.getDate();
		//}catch(Poco::Exception){
		//
	//}
	
	ofxRemoteFace(string _filename, ofImage _faceImage, WKCharacterType _characterType){
		filename		=	_filename;
		faceImage		=	_faceImage;
		faceImage.setUseTexture(false);  // GL != threads, also we aren't using this image to be displayed, so loading it just into ram is fine
		creation		=	Poco::Timestamp();
		exists			=	false;
		uploaded		=	false;
		existsRemotely	=	false;
		copied			=	false;
		characterType	=	_characterType;
	}
	
	~ofxRemoteFace()
	{
		//faceImage.clear();
	}
	
	string filename;
	ofImage faceImage;			//should this be a pointer?
	Poco::Timestamp creation;	//creation moment
	bool exists;				//the file exists in awaitingUpload dir
	bool uploaded;				//has the image been uploaded to the remote dir
	bool existsRemotely;		//has the image been added to the remote database
	bool copied;				//has the image been copied to the addedToDatabase dir;
	WKCharacterType characterType; //the character type of the face
	//so when it begins you should check for existance, then set exists
	//then begin upload
	//then set remote database
	//then copy
	//then delete from awaitingUpload
	// should be deleted when !exists, uploaded, existsRemotely, copied
};

//do I need a remote face, or should I have a general face structure? //where do I include the filename?

class ofxRemoteFaceUploadClientUtils:public ofxThread{
public:
	
	ofxRemoteFaceUploadClientUtils();
	~ofxRemoteFaceUploadClientUtils();
	
	//-------------------------------
	// non blocking functions
	void addFace(ofxRemoteFace face);
	int	typeOfCharacter(ofxRemoteFace face);
	int	typeOfCharacter(ofxRemoteFace* face);
	
	
	//-------------------------------
	// blocking functions
	void checkLocalFace(ofxRemoteFace & face);   //should this be here?
	void uploadFace(ofxRemoteFace* face);			//and this?
	void addToDatabase(ofxRemoteFace* face);		//etc
	void copyLocalFace(ofxRemoteFace* face);		//etc
	// from ofhttputils
	void submitForm(ofxHttpForm form);
	void getUrl(string url);
	
	//-------------------------------
	// threading stuff
	void threadedFunction();
	void start();
	void stop();
	
	//stuff for loading of old faces
	WKCharacterType getCharacterTypeFromCharacterName(string theCharacter);	
	WKCharacterType getCharacterType(string filenameOfFaceImage);
	void addOldFacesToQueue();
protected:
	//--------------------------------
	// http utils
	string	generateUrl(ofxHttpForm & form);
	
	ofxRemoteFace * faces[OFX_REMOTE_NUMBER_OF_FACES_TO_TRY_TO_UPLOAD]; //limited to uploading 10 old faces
	
	ofxPocoDirectoryLister	incompleteFaceDirectoryLister;//for loading  of  old faces
	
	int numberOfFacesInQueue;
};

//what does this indicate precisely? class will be instantiated with the name ofxRemberFaceUploadClient? see EOF of cpp file
//extern ofxRemoteFaceUploadClientUtils ofxRemoteFaceUploadClient;

#endif


/*
 
 email to sermad
 
 Or perhaps a naming convention for the characters used in the chain. Otherwise how will characters between the installation and the website remain consistent?
 
 E.g. the form could now be:
 
 http://www.sermad.com/wk_xmas/submit.php?path=face_0.jpg&costume=cowboy&SUBMIT=SUBMIT
 
 Or I could encode the character into the filename:
 
 http://www.sermad.com/wk_xmas/submit.php?path=face_0_cowboy.jpg&SUBMIT=SUBMIT
 
 I think that would be the better method, as it allows me to encode the types on my side using filename only. Or both - so you can easily get it from the database too?
 
 - can you guarantee that a thread will finish? apart from power loss, natch
 
 http://www.sermad.com/wk_xmas/submit.php?path=face_0_cowboy.jpg&costume=cowboy&SUBMIT=SUBMIT
 
 What do you reckon?
 
 It doesn't really matter for Monday -  but I am going to start encoding mine anyhow.
 
 */

/* for now this should just maintain a queue of faces that are waiting to be uploaded to the ftp site, and also added to the database, before finally being copied to the uploaded
 part of the directory structure
 
 for safety it is better to copy the file first - to have too many files - not good either - dissaopoiing if it fails, but does casue conflicts?
 
 the question is - how do we maintain integrity - what if the upload is interrupted?
 
	- taken care of by using the awaitingUpload dir
 
 what if a local file copy fails?
 
	- not deleted from awaitingupload folder until copy confirmed to addedToDatabase folder
 
 perhaps we need some more functionality on the server side to be able to query the number of faces uploaded - guaranteeing file name saftey until we have > maxInt files
 
 
 after the queue is completed, the next challenge is to complete the code that gives you a queue of faces to animate with, that also has a threaded local load functionality,
 
 similar to that of the flickr image scraper, but local
 
 the final part will be the not necessarily threaded main logic (rather main thread) that keeps a copy of both the ofxRemoteFaceUploadClient and the ofxLocalFaceDisplayClient

/*
 Stole structure from arturo's 
 
 ofxPop3Client
 arturo

#ifndef _POCO_MAIL_TOOLS
#define _POCO_MAIL_TOOLS

#include "ofMain.h"

#define OF_ADDON_USING_OFXTHREAD
#include "ofAddons.h"






#include "Poco/Net/MailMessage.h"
#include "Poco/Net/POP3ClientSession.h"
#include "Poco/Net/SMTPClientSession.h"

#include "Poco/Timestamp.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>
#include <queue.h>

#define OFX_SMTP_HOST "localhost"
#define OFX_SMTP_PORT 25

typedef Poco::Timestamp ofTimestamp;

using Poco::Net::SMTPClientSession;
using Poco::Net::POP3ClientSession;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

#define OFX_HTTP_GET  0
#define OFX_HTTP_POST 1

class ofxPop3Listener;
class ofxPop3EventManager;

struct ofxMailMessage{
	ofxMailMessage(int id, Poco::Net::MailMessage& mailMessage){
		this->id=id;
		try{
			date=mailMessage.getDate();
		}catch(Poco::Exception){
			
		}
		
		sender = mailMessage.getSender();
		subject = mailMessage.getSubject();
		contentType = mailMessage.getContentType();
		content = mailMessage.getContent();
	}
	
	ofxMailMessage(){
		date=Poco::Timestamp();
	}
	
	Poco::Net::MailMessage & getPocoMessage(){
		Poco::Net::MailMessage & message = *(new Poco::Net::MailMessage());
		message.setDate(date);
		message.setSender(sender);
		message.setSubject(subject);
		message.setContentType(contentType);
		message.setContent(content);
		for(unsigned int i=0;i<recipients.size();i++){
			message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,recipients[i]));
		}
		for(unsigned int i=0;i<cc.size();i++){
			message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,cc[i]));
		}
		for(unsigned int i=0;i<bcc.size();i++){
			message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,bcc[i]));
		}
		return message;
	}
	
	int id;
	ofTimestamp date;
	string sender;
	string subject;
	string contentType;
	string content;
	vector<string> recipients;
	vector<string> cc;
	vector<string> bcc;
};


struct ofxMailAccount{
	ofxMailAccount(){
		port=110;
	}
	int accountId;
	string accountName;
	string user;
	string password;
	string host;
	unsigned int port;
	vector<int> alreadyReaded;
};
class ofxPop3ClientUtils : public ofxThread{
	
public:
	
	ofxPop3ClientUtils();
	~ofxPop3ClientUtils();
	
	bool removeFromServer;
	
	void setCheckInterval(int seconds);
	
	//-------------------------------
	// non blocking functions
	void addAccount(ofxMailAccount account);
	
	//-------------------------------
	// blocking functions
	int checkAccount(ofxMailAccount & account);
	
	
	//-------------------------------
	// threading stuff
	void threadedFunction();
	void start();
	void stop();
	
protected:
	bool checkAlreadyReaded(ofxMailAccount account,int id);
	
	vector<ofxMailAccount> accounts;
	bool verbose;
	int checkInterval;
	
	
};


class ofxSmtpClientUtils:public ofxThread{
public:
	
	ofxSmtpClientUtils();
	~ofxSmtpClientUtils();
	
	
	//-------------------------------
	// non blocking functions
	void addMessage(ofxMailMessage message);
	
	//-------------------------------
	// blocking functions
	void sendMessage(ofxMailMessage & message);
	
	
	//-------------------------------
	// threading stuff
	void threadedFunction();
	void start();
	void stop();
protected:
	queue <ofxMailMessage> messages;
	bool verbose;
	
	SMTPClientSession * session;
};

extern ofxPop3ClientUtils ofxPop3Client;
extern ofxSmtpClientUtils ofxSmtpClient;
#endif

*/

//FTP malarky

/*
 
 from http://www.openframeworks.cc/forum/viewtopic.php?t=408&highlight=ftp
 void LogController::uploadFile(string file){ 
 string remotePath = "/"; 
 try{ 
 _ftpClient = new FTPClientSession(_ftpHost, 21); 
 _ftpClient->login(_ftpLogin, _ftpPassword); 
 std::ostream& ftpOStream=_ftpClient->beginUpload(remotePath+file); 
 std::ifstream localIFStream((_logPath+file).c_str(), ifstream::in); 
 StreamCopier::copyStream(localIFStream, ftpOStream); 
 _ftpClient->endUpload(); 
 _ftpClient->close(); 
 delete _ftpClient; 
 _ftpClient=0; 
 } 
 catch (Exception& exc) 
 { 
 cout << exc.displayText() << endl; 
 } 
 } 
 
 from http://www.openframeworks.cc/forum/viewtopic.php?t=408&postdays=0&postorder=asc&highlight=ftp&start=15
 //some poco stuff 
 HTTPStreamFactory::registerFactory(); 
 
 //specify out url and open stream 
 URI uri("http://www.coolhunting.com/images/funky2.jpg");       
 std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri)); 
 
 //create a file to save the stream to 
 ofstream myfile;       
 myfile.open(ofToDataPath("thumb.jpg").c_str()); 
 
 //copy stream to file 
 StreamCopier::copyStream(*pStr.get(), myfile); 
 myfile.close(); 
 
 //load into an ofImage! 
 img.loadImage(ofToDataPath("thumb.jpg"));
 
 */
