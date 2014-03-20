//
//  ofxThreadedFTPClient.h
//
//  Created by enrico<nausea>viola on 3/20/14.
//
//

#pragma once
#include "ofxFTP.h"
#include "ofThread.h"

class ofxThreadedFTPClient : public ofThread{
public:
    ofxThreadedFTPClient();
    void setup(string _host, string username, string password, int _port=21);
    //use this to send a single file
    void send(string fileName, string localFolder, string remoteFolder, bool isFolderLocal=true);
    void send(string fileName, string remoteFolder, bool isFolderLocal=true);
    //void this to send all files in folder
    void sendFolder(string localFolder, string remoteFolder, string fileExt="", bool skipDuplicates=true ,bool isFolderLocal=true);
    bool isReady();
    void threadedFunction();
    void goOut();
    //register to this to get messages when a ftp action starts or ends
    template<class ListenerClass, typename ListenerMethod>
    void addListener(ListenerClass * listener, ListenerMethod method){
        ofAddListener(ftpEvent, listener, method);
    }
    
    template<class ListenerClass, typename ListenerMethod>
    void removeListener(ListenerClass * listener, ListenerMethod method){
        ofRemoveListener(ftpEvent, listener, method);
    }
        
    enum ThreadedAction{
        SEND_FILE1,
        SEND_FILE2,
        SEND_FOLDER
    };
    
    enum ActionStatusInfo{
        ACTION_BEGIN,
        ACTION_ENDED
    };
    
    struct FTPStatusInfo{
        ThreadedAction action;
        ActionStatusInfo status;
    };
    
    ofEvent<FTPStatusInfo>ftpEvent;
    
protected:
    void createFileList();
    
    ThreadedAction ta;
    ofxFTPClient ftp;
    vector<string>files;
    string lDir;
    string rDir;
    string fName;
    string fExt;
    bool bSkip;
    bool bDirLoc;
    bool bReady;
};