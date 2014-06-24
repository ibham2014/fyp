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
    
    nearThreshold = 200;
	farThreshold = 20;
    angle = kinect.getTargetCameraTiltAngle();
    
    //--------- create image for holding masked rgb pixels
    int width = 640;
    int height = 480;
    colorImageMasked.allocate(width,height);
    depthImage.allocate(width,height);
    grayBackgroundCapture.allocate(width,height);
    bCaptureBg = false;
    
    //--------- set avatar
    totalAvatarsThisUser = 0;
    currentAvatar = -1;
    bRecordingAvatar = false;
    bSavingRecords = false;
    recorder.setFormat("jpg");
    for(int i = 0; i < MAX_AVATARS; i++){
        avatars[i].setup();
        float yp = (i+1) * (ofGetHeight()/4.0);
        avatars[i].pos.set(ofGetWidth()*.5,yp);//xp,ofGetHeight()*.5);
    }
    
    bShowGui = true;
    
    //--------- set application settings
    ofSetFrameRate(30);
    //ofSetVerticalSync(true);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    if(bUseKinect){
        
    //--------- update kinect
	kinect.update();
    
    //--------- create masked rgb frame
    if(kinect.isFrameNew()) {
		
		depthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		colorImageMasked.setFromPixels(kinect.getPixelsRef());
        if(bCaptureBg){
            grayBackgroundCapture.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
            bCaptureBg = false;
        }
        depthImage -= grayBackgroundCapture;
        depthImage.erode_3x3();
        depthImage.dilate_3x3();
        depthImage.dilate_3x3();
        depthImage.dilate_3x3();
        depthImage.blurGaussian();
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
    if(bRecordingAvatar && ofGetFrameNum() % 2 == 0){
        recorder.addFrame(colorImageMasked.getPixelsRef());
        //cout << "add frame " << endl;
    }else if(bSavingRecords){
        
        if(recorder.q.size() == 0){
            bSavingRecords = false;
            cout << "done saving frames" << endl;
            // start new avatar
            if(!avatars[currentAvatar].isPlaying()){
                cout << "start avatar" << endl;
                //avatars[currentAvatar].pos.set( ofGetWidth()*.5,ofGetHeight()*.5 );
                avatars[currentAvatar].startAvatar();
            }
        }else{
            cout << " saving frames" << endl;
        }
    }
    
    }
    
    //--------- update avatar players
    if(totalAvatarsThisUser > 0){
        for( int i = 0; i < totalAvatarsThisUser; i++){
            avatars[i].update();
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::startRecording(){
    
    if(!bUseKinect) return;
    
    if(totalAvatarsThisUser >= MAX_AVATARS || bSavingRecords){
        bRecordingAvatar = false;
        return;
    }
    
    cout << "start recording " << endl;
    
    currentAvatar++;
    totalAvatarsThisUser++;

    // ? reset avatar
    
    // set avatar directory
    string dir = "avatar_"+ofGetTimestampString();
    avatars[currentAvatar].setDirectory(dir);
    
    // start recorder
    recorder.startThread();
    string filePrefix = dir + "/frame_";
    recorder.setPrefix(ofToDataPath(filePrefix));

    // set is recording
    bRecordingAvatar = true;
    
    
}
//--------------------------------------------------------------
void ofApp::endRecording(){
    
    if(!bUseKinect) return;
    
    if(totalAvatarsThisUser >= MAX_AVATARS+1) return;

    cout << "end recording " << endl;
    bRecordingAvatar = false;
    bSavingRecords = true;
    recorder.stopThread();
    
}

void ofApp::exit(){
    if(bUseKinect) recorder.waitForThread();
    if(bUseKinect) kinect.close();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    

    
    ofSetColor(255, 255, 255);
    //--------- draw avatar
    if(totalAvatarsThisUser > 0){
        for( int i = 0; i < totalAvatarsThisUser; i++){
            avatars[i].draw();
        }
    }
    
    if(bShowGui){
        
        if(bUseKinect){
            
            //--------- draw masked live color image
            float xp = 160*2+20*2;//(ofGetWidth() - colorImageMasked.getWidth() ) *.5;
            float yp = 20;//(ofGetHeight() - colorImageMasked.getHeight() ) *.5;
            colorImageMasked.draw(xp,yp,160,120);
            
            //--------- draw preview
            kinect.draw(20, 20, 160, 120);
            kinect.drawDepth(40+160, 20, 160, 120);
            
        }
        
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        reportStream << "set near threshold " << nearThreshold << " (press: + -)" << endl
        << "set far threshold " << farThreshold << ", fps: " << ofGetFrameRate() << endl;
        stringstream c;
        c << "Recording: " << bRecordingAvatar << "\nThread running: " << recorder.isThreadRunning() <<  "\nQueue Size: " << recorder.q.size() << "\n\nPress 'r' to toggle recording.\nPress 't' to toggle worker thread." << endl;
    
        ofDrawBitmapString(reportStream.str(), 650, 10);
        ofDrawBitmapString(c.str(), 650, 50);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'b':
            bCaptureBg = !bCaptureBg;
            break;
        case 'r':
            bRecordingAvatar = !bRecordingAvatar;
            if(bRecordingAvatar) startRecording();
            else endRecording();
            break;
        case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
        case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			if(bUseKinect) kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			if(bUseKinect) kinect.setCameraTiltAngle(angle);
			break;
        case 'x':
            for(int i = 0; i < MAX_AVATARS; i++){
                avatars[i].resetAvatar();
            }
            totalAvatarsThisUser = 0;
            currentAvatar = -1;
            recorder.q.empty();
            break;
       case '1':
            if(!bUseKinect){
                avatars[0].setDirectory("avatar_2014-06-23-18-37-35-194");
                avatars[0].startAvatar();
                totalAvatarsThisUser=1;
            }
            break;
        case '2':
            if(!bUseKinect){
                avatars[1].setDirectory("avatar_2014-06-23-18-36-38-356");
                avatars[1].startAvatar();
                totalAvatarsThisUser=2;
                
            }
            break;
        case '3':
            if(!bUseKinect){
                avatars[2].setDirectory("avatar_2014-06-23-18-36-23-113");
                avatars[2].startAvatar();
                totalAvatarsThisUser=3;
                
            }
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'g':
            bShowGui = !bShowGui;
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
