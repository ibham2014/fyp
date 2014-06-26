#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    avatar.setup();
    avatar.pos.set(ofGetWidth()*.5,ofGetHeight()*.5);
    avatar.setUsePortrait(true);
    avatar.imageLoader.setMirrored(true);
    
    bShowGui = true;
    bOpenFromFile = false;
    bPickRandom = false;
    
    // get list of directories
    ofDirectory dir("training_avatars");
    int totalAvatars = dir.listDir();
    for(int i=0; i<totalAvatars; i++) {
        avatarDirectories.push_back(dir.getPath(i));
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(bOpenFromFile){
        bOpenFromFile = false;
        openAvatarFromSaved();
    }
    
    if(bPickRandom){
        bPickRandom = false;
        pickRandomAvatar();

    }
    
    avatar.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    ofSetColor(255, 255, 255);
    avatar.draw();
    
    if(bShowGui){
        
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    reportStream << "0 - Load from file\n" << "g - toggle this text on/off\n"
    << "f - toggle fullscreen\n" <<  "fps: " << ofGetFrameRate() << endl;
    
    ofDrawBitmapString(reportStream.str(), 650, 10);
    }
}
//--------------------------------------------------------------
void ofApp::pickRandomAvatar(){
    if(avatarDirectories.size() > 0){
        clearAvatar();
        int randomPick = ofRandom(0,avatarDirectories.size());
        string dir = avatarDirectories[randomPick];
        ofDirectory dirManager;
        dirManager.open(dir);
        if(dirManager.exists() && dirManager.isDirectory()){
            cout << "dir " << dir << endl;
            avatar.setDirectory(dir);
            avatar.startAvatar();
        }
        
    }
}
//--------------------------------------------------------------
void ofApp::openAvatarFromSaved(){
    
        clearAvatar();
    
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a directory",true);
        string path = openFileResult.getPath();
        ofDirectory dirManager;
        dirManager.open(path);
        if( dirManager.isDirectory() ){
            avatar.setDirectory(path);
            avatar.startAvatar();
        }
    
}
//--------------------------------------------------------------
void ofApp::clearAvatar(){
        avatar.resetAvatar();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch(key){
            case 'p':
                avatar.togglePlaying();
                break;
            case '0':
                bOpenFromFile = true;
                break;
            case 'x':
                clearAvatar();
                break;
            case 'g':
                bShowGui = !bShowGui;
                break;
            case 'f':
                ofToggleFullscreen();
                break;
            case ' ':
                bPickRandom = true;
                break;
            case '1':
                avatar.player.setWaitTime(.05);
                break;
            case '2':
                 avatar.player.setWaitTime(.075);
                break;
            case '3':
                 avatar.player.setWaitTime(.1);
                break;
            case 'm':
                avatar.imageLoader.toggleMirrored();
                break;
            
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
