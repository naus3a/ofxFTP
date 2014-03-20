#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    client.setup("<filename>","<localpath>","<remotepath>");
    client.addListener(this, &testApp::onFtp);
}       

void testApp::onFtp(ofxThreadedFTPClient::FTPStatusInfo &info){
    if(info.status==ofxThreadedFTPClient::ACTION_BEGIN){
        cout << "ftp started" << endl;
    }else if(info.status==ofxThreadedFTPClient::ACTION_ENDED){
        cout<<"ftp ended"<<endl;
    }
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if(key==' '){
        client.send("<filename>","<localpath>","<remotepath>");
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    for(int i = 0 ; i < dragInfo.files.size() ; i++)
    {
        ofFile file(dragInfo.files[i]);
        string path = file.path().substr(0,file.path().length()-file.getFileName().length());
        client.send(file.getFileName(),path,"/");
    }
}