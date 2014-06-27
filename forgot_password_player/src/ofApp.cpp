#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    avatar.setup();
    avatar.pos.set(ofGetWidth()*.5,ofGetHeight()*.5);
    avatar.setUsePortrait(true);
    avatar.imageLoader.setMirrored(true);
    
    for(int i = 0; i < MAX_AVATARS; i++){
        avatars[i].setup();
        avatars[i].pos.set(ofGetWidth()*.5,ofGetHeight()*.5);
        avatars[i].setUsePortrait(true);
        avatars[i].imageLoader.setMirrored(true);
        avatars[i].player.setLoopState(OF_LOOP_NONE);
        cout << avatars[i].getDirectory() << endl;
    }
    
    bShowGui = true;
    bOpenFromFile = false;
    bPickRandom = false;
    bLoadingNewSet = false;
    bUseSets = true;
    bRecording = false;
    totalPreloaded = 0;
    currentPlaying = 0;
    
    // get list of directories
    ofDirectory dir("training_avatars");
    int totalAvatars = dir.listDir();
    for(int i=0; i<totalAvatars; i++) {
        avatarDirectories.push_back(dir.getPath(i));
    }
    
#ifdef USE_OSC
    ofxXmlSettings xml;
    xml.load("oscSettings.xml");
    oscHost = xml.getValue("host", "localhost");
    oscPort = xml.getValue("port", 12345);
	oscSender.setup(oscHost, oscPort);
#endif
    
    // fonts
    font.loadFont("frabk.ttf", 30, true, true);
	font.setLineHeight(34.0f);
	font.setLetterSpacing(1.035);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(bLoadingNewSet){
        if(totalPreloaded >= MAX_AVATARS){
            bLoadingNewSet = false;
            avatars[0].player.setFrame(1);
        }else{
            loadNextAvatar();
        }
        
    }else if(bOpenFromFile){
        bOpenFromFile = false;
        openAvatarFromSaved();
    
    }else if(bPickRandom){
        bPickRandom = false;
        pickRandomAvatar();

    }
    
    if(bUseSets){
        for(int i = 0; i < totalPreloaded; i++)
            avatars[i].update();
        
        if(avatars[currentPlaying].player.getIsMovieDone()){
            if(bRecording){
                bRecording = false;
                sendStopRecord();
            }
        }
    }else{
     avatar.update();
    }
    
    //--- osc controls
#ifdef USE_OSC
    // send and receive osc
    // if 
#endif
  
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    int totalLoaded = 0;
    for(int i = 0; i < MAX_AVATARS; i++){
        if(avatars[i].imageLoader.isLoaded()) totalLoaded++;
    }
    
    ofSetColor(255, 255, 255);
    if(bUseSets){
        
        if(avatars[currentPlaying].player.getIsMovieDone())
            ofSetColor(150,150,150);
        
        // only show if all loaded
        if(totalLoaded >= MAX_AVATARS) avatars[currentPlaying].draw();
    
    }else
        avatar.draw();
    
    //---
    // show instructions
    if(bUseSets){
        ofSetColor(255, 255, 255);
        if(totalLoaded < MAX_AVATARS){
            string readyInfo = "Please Wait";
            ofRectangle bounds = font.getStringBoundingBox(readyInfo, 0, 0);
            font.drawString(readyInfo, ofGetWidth()*.5-bounds.width*.5, ofGetHeight()*.85-bounds.height*.5);
        }else if( !avatars[currentPlaying].player.isPlaying() && !avatars[currentPlaying].player.getIsMovieDone()){
            string readyInfo = "When the video begins,";
            ofRectangle bounds = font.getStringBoundingBox(readyInfo, 0, 0);
            font.drawString(readyInfo, ofGetWidth()*.5-bounds.width*.5, ofGetHeight()*.85-bounds.height*.5);
            readyInfo = "follow the dance movements";
            ofRectangle bounds2 = font.getStringBoundingBox(readyInfo, 0, 0);
            font.drawString(readyInfo, ofGetWidth()*.5-bounds2.width*.5, ofGetHeight()*.85-bounds2.height*.5+bounds.height);
        }else if( avatars[currentPlaying].player.getIsMovieDone() ){
            string readyInfo = "Wait a moment for your avatar to generate.";
            ofRectangle bounds = font.getStringBoundingBox(readyInfo, 0, 0);
            font.drawString(readyInfo, ofGetWidth()*.5-bounds.width*.5, ofGetHeight()*.85-bounds.height*.5);
        }
    }
    
    //-----
    if(bShowGui){
        
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        reportStream << "0 - Load from file\n" << "g - toggle this text on/off\n"
        << "f - toggle fullscreen\n" <<  "fps: " << ofGetFrameRate() << endl << "RETURN - load sequence" << endl << "n - next sequence " << endl << "r - play and record" << endl;
    
        ofDrawBitmapString(reportStream.str(), 650, 10);
        
        if( (bLoadingNewSet && totalPreloaded>=0) || totalLoaded < MAX_AVATARS){
            
            cout << totalPreloaded << endl;
            stringstream reportStream2;
            if(totalPreloaded > 0) reportStream2 << "Loading file: " << avatars[totalPreloaded-1].getDirectory() << endl;
            ofDrawBitmapString(reportStream2.str(), 650, 200);
        }
    }
}
//--------------------------------------------------------------
void ofApp::playAndSendRecord(){
    
    // send record message to app
#ifdef USE_OSC
    ofxOscMessage m;
    m.setAddress("/fypRecord");
    m.addIntArg(1);
    oscSender.sendMessage(m);
#endif
    // start playing avatar
    avatars[currentPlaying].player.setFrame(0);
    avatars[currentPlaying].player.play();
}
//--------------------------------------------------------------
void ofApp::sendStopRecord(){
#ifdef USE_OSC
    ofxOscMessage m;
    m.setAddress("/fypRecord");
    m.addIntArg(0);
    oscSender.sendMessage(m);
#endif
}
//--------------------------------------------------------------
void ofApp::sendClearUser(){
#ifdef USE_OSC
    ofxOscMessage m;
    m.setAddress("/fypNewUser");
    oscSender.sendMessage(m);
#endif
}
//--------------------------------------------------------------
void ofApp::loadNextAvatar(){

    if(avatarDirectories.size() > 0){
        
        if(totalPreloaded > 0){
            if(avatars[totalPreloaded-1].isLoading()){
                return;
            }
        }
        int randomPick = ofRandom(0,avatarDirectories.size());
        string dir = avatarDirectories[randomPick];
        ofDirectory dirManager;
        dirManager.open(dir);
        if(dirManager.exists() && dirManager.isDirectory()){
            avatars[totalPreloaded].setDirectory(dir);
            avatars[totalPreloaded].startAvatar();
            totalPreloaded++;
        }
        avatarDirectories.erase( avatarDirectories.begin() + randomPick );
        cout << "load next " << endl;
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
void ofApp::clearAllAvatars(){
    for(int i = 0; i < totalPreloaded; i++) avatars[i].resetAvatar();
    totalPreloaded = 0;
    currentPlaying = 0;
    
}
//--------------------------------------------------------------
void ofApp::playNextAvatar(){
    if(currentPlaying == MAX_AVATARS-1){
        currentPlaying = 0;
    }else{
        currentPlaying++;
    }
    avatars[currentPlaying].player.setFrame(0);

}
//--------------------------------------------------------------
void ofApp::setToLoadSequence(){
    ofDirectory dir("training_avatars");
    int totalAvatars = dir.listDir();
    for(int i=0; i<totalAvatars; i++) {
        avatarDirectories.push_back(dir.getPath(i));
    }
    sendClearUser();
    clearAllAvatars();
    bLoadingNewSet = true;
}
//--------------------------------------------------------------
void ofApp::setupGui(){
    

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch(key){
        case OF_KEY_UP:
            for(int i = 0; i < MAX_AVATARS; i++)
                avatars[i].drawScale+=.01;
            break;
        case OF_KEY_DOWN:
            for(int i = 0; i < MAX_AVATARS; i++)
                avatars[i].drawScale-=.01;
            break;
        case OF_KEY_RETURN:
                setToLoadSequence();
                break;
            case 'r':
                bRecording = true;
                playAndSendRecord();
                break;
            case 'n':
                playNextAvatar();
                break;
            case 'p':
                if(bUseSets) avatars[currentPlaying].togglePlaying();
                //avatar.togglePlaying();
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
