#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    
    //--------- set up kinect
	kinect.setRegistration(true); // enable depth->video image calibration
    
	kinect.init();
	kinect.open();
	
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
        
        bUseKinect = true;
	}
    
    nearThreshold = 255;
	farThreshold = 200;
    
    //--------- create image for holding masked rgb pixels
    int width = 640;
    int height = 480;
    colorImageMasked.allocate(width,height);
    depthImage.allocate(width,height);
    
    //--------- set avatar
    totalAvatarsThisUser = 0;
    currentAvatar = -1;
    bRecordingAvatar = false;
    recorder.setFormat("jpg");
    
    //--------- set application settings
    ofSetVerticalSync(true);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //--------- update kinect
	kinect.update();
    
    //--------- create masked rgb frame
    if(kinect.isFrameNew()) {
		
		depthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		colorImageMasked.setFromPixels(kinect.getPixelsRef());
        
        unsigned char * pixG = depthImage.getPixels();
        unsigned char * pixC = colorImageMasked.getPixels();
        
        int numPixels = depthImage.getWidth() * depthImage.getHeight();
        for(int i = 0; i < numPixels; i++) {
            if(pixG[i] < nearThreshold && pixG[i] > farThreshold) {
                pixG[i] = 255;
            } else {
                pixG[i] = 0;
                pixC[i*3] = 0;
                pixC[i*3+1] = 0;
                pixC[i*3+2] = 0;
            }
        }
    }
    
    colorImageMasked.flagImageChanged();
    depthImage.flagImageChanged();
    
    
    //--------- update recording
        // this is a hack to not have too many frames, should be frame rate based
        //if( ofGetFrameNum() % 2 == 0 ){
            //recorder.addFrame(colorImageMasked.getPixelsRef());
       //     recorders[totalAvatarsThisUser].addFrame(colorImageMasked.getPixelsRef());
       // }
    
    if(bRecordingAvatar) recorder.addFrame(colorImageMasked.getPixelsRef());
    
    //--------- update avatar players
    if(totalAvatarsThisUser > 0){
        
        for( int i = 0; i < totalAvatarsThisUser; i++){
            avatars[i].update();
            avatars[i].addFrame(colorImageMasked.getPixelsRef());
            cout << "add frame " << avatars[i].myDirectory << endl;
        }
        
        // check each recorder and when it has saved out all
        /*for( int i = 0; i < totalAvatarsThisUser; i++){
            cout << avatarDirectory[i] << "avatar is playing " << avatars[i].isPlaying() << endl;
            if(!avatars[i].isPlaying() && recorders[i].q.size() == 0){
                avatars[i].startThreadedLoading(avatarDirectory[i]);
                avatars[i].play();
                cout << "start play avatar " << avatarDirectory[i] << endl;
            }
        }*/
    }
    
    
    
}
//--------------------------------------------------------------
void ofApp::startRecording(){
    
    if(totalAvatarsThisUser >= MAX_AVATARS) return;
    
    cout << "start recording " << endl;
    
    currentAvatar++;
    totalAvatarsThisUser++;

    // ? reset avatar
    
    // set avatar directory
    string dir = "avatar_"+ofGetTimestampString();
    avatars[currentAvatar].setDirectory(dir);
    
    // tell avatar to begin recording
   // avatars[currentAvatar].startRecording();
    
    
    recorder.startThread();
    string filePrefix = dir + "/frame_";
    recorder.setPrefix(ofToDataPath(filePrefix));
    
    // create new directory
    //currAvatarDir = "avatar_"+ofGetTimestampString();
    //avatarDirectory[totalAvatarsThisUser] = currAvatarDir;
    //string filePrefix = currAvatarDir + "/frame_";
    //recorders[totalAvatarsThisUser].setPrefix(ofToDataPath(filePrefix));
    //recorders[totalAvatarsThisUser].startThread();
    
    // set is recording
    //bRecordingAvatar = true;
    
    
}
//--------------------------------------------------------------
void ofApp::endRecording(){
    
    //bRecordingAvatar = false;
    //recorders[totalAvatarsThisUser].stopThread();
    //avatars[totalAvatarsThisUser].stop();
    //totalAvatarsThisUser++;
    //avatars[currentAvatar].endRecording();
    recorder.stopThread();
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    //--------- draw masked live color image
    float xp = (ofGetWidth() - colorImageMasked.getWidth() ) *.5;
    float yp = (ofGetHeight() - colorImageMasked.getHeight() ) *.5;
    colorImageMasked.draw(xp,yp);
    
    //--------- draw preview
    kinect.draw(20, 20, 160, 120);
    kinect.drawDepth(40+160, 20, 160, 120);
    
    //--------- draw avatar
    if(totalAvatarsThisUser > 0){
        for( int i = 0; i < totalAvatarsThisUser; i++){
            avatars[i].draw();
        }
    }
    
    
    //stringstream c;
    //c << "Recording: " << bRecordingAvatar << "\nThread running: " << avatars[totalAvatarsThisUser].recorder.isThreadRunning() <<  "\nQueue Size: " << avatars[totalAvatarsThisUser].recorder.q.size() << "\n\nPress 'r' to toggle recording.\nPress 't' to toggle worker thread." << endl;
    
    //ofDrawBitmapString(c.str(), 650, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'r':
            bRecordingAvatar = !bRecordingAvatar;
            if(bRecordingAvatar) startRecording();
            else endRecording();
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
