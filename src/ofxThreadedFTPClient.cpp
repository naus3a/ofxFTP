//
//  ofxThreadedFTPClient.cpp
//
//  Created by enrico<nausea>viola on 3/20/14.
//
//

#include "ofxThreadedFTPClient.h"

ofxThreadedFTPClient::ofxThreadedFTPClient(){
    bReady = true;
    ta = SEND_FILE1;
    fName="";
    lDir="";
    rDir="";
    files.clear();
    bDirLoc=true;
}

void ofxThreadedFTPClient::setup(string _host, string username, string password, int _port){
    ftp.setup(_host, username,password,_port);
    ftp.setVerbose(true);
    
    bReady=true;
}

void ofxThreadedFTPClient::send(string fileName, string localFolder, string remoteFolder, bool isFolderLocal){
    if(isReady()){
        bReady=false;
        fName=fileName;
        lDir = localFolder;
        rDir = remoteFolder;
        bDirLoc = isFolderLocal;
        ta = SEND_FILE1;
        FTPStatusInfo info;
        info.action = ta;
        info.status = ACTION_BEGIN;
        ofNotifyEvent(ftpEvent, info);
        startThread(true,false);
    }
}

void ofxThreadedFTPClient::send(string fileName, string remoteFolder, bool isFolderLocal){
    if(bReady){
        bReady=false;
        fName=fileName;
        rDir = remoteFolder;
        bDirLoc = isFolderLocal;
        ta = SEND_FILE2;
        FTPStatusInfo info;
        info.action = ta;
        info.status = ACTION_BEGIN;
        ofNotifyEvent(ftpEvent, info);
        startThread(true,false);
    }
}

void ofxThreadedFTPClient::sendFolder(string localFolder, string remoteFolder, string fileExt, bool skipDuplicates ,bool isFolderLocal){
    if(isReady()){
        bReady=false;
        lDir = localFolder;
        rDir = remoteFolder;
        fExt = fileExt;
        bSkip = skipDuplicates;
        bDirLoc = isFolderLocal;
        ta = SEND_FOLDER;
        FTPStatusInfo info;
        info.action = ta;
        info.status = ACTION_BEGIN;
        ofNotifyEvent(ftpEvent, info);
        startThread(true,false);
    }
}

bool ofxThreadedFTPClient::isReady(){
    return bReady;
}

void ofxThreadedFTPClient::threadedFunction(){
    while (isThreadRunning()!=0) {
        if(lock()){
            if(ta==SEND_FILE1){
                ftp.send(fName, lDir, rDir, bDirLoc);
            }else if(ta==SEND_FILE2){
                ftp.send(fName, rDir, bDirLoc);
            }else if(ta==SEND_FOLDER){
                createFileList();
                for(int i=0;i<files.size();i++){
                    ftp.send(files[i], lDir, rDir, bDirLoc);
                }
            }
            unlock();
            goOut();
        }
    }
}

void ofxThreadedFTPClient::createFileList(){
    files.clear();
    ofDirectory dir;
    int nf = dir.listDir(ofFilePath::getAbsolutePath(lDir));
    cout<<lDir<<endl;
    if(nf>0){
        //vector<string>ll;
        //ll.clear();
        for(int i=0;i<nf;i++){
            string s = dir.getName(i);
            if(fExt!=""){
                if(dir.getFile(i).getExtension()==fExt){
                    string s = dir.getName(i);
                    files.push_back(s);
                    cout << s<<endl;
                }
            }else{
                string s = dir.getName(i);
                files.push_back(s);
                cout << s<<endl;
            }
        }
        
        if(bSkip){
            vector<string>rr = ftp.list(rDir);
            if(rr.size()>0){
                for(int i=0;i<rr.size();i++){
                    for(int j=0;j<files.size();j++){
                        size_t found = rr[i].find(files[j]);
                        if(found!=string::npos){
                            files.erase(files.begin()+j);
                            j=files.size()+2;
                        }
                    }
                }
            }
        }
        
        cout<<files.size()<<endl;
    }
}

void ofxThreadedFTPClient::goOut(){
    stopThread();
    bReady=true;
    FTPStatusInfo info;
    info.action = ta;
    info.status = ACTION_ENDED;
    ofNotifyEvent(ftpEvent, info);
}