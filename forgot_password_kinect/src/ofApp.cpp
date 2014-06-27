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
    
    drawScale = 1;
    
    //--------- create image for holding masked rgb pixels
    int width = 640;
    int height = 480;
    colorImageMasked.allocate(width,height);
    depthImage.allocate(width,height);
    grayBackgroundCapture.allocate(width,height);
    bCaptureBg = false;
    bHasDetectedUser = false;
    
    //--------- set avatar
    totalAvatarsThisUser = 0;
    currentAvatar = -1;
    bRecordingAvatar = false;
    bSavingRecords = false;
    avatarOffX = ofGetWidth()*.5+200;
    avatarOffY = 0;
    
    recorder.setup(640,480);
    recorder.setFormat("jpg");

    
    for(int i = 0; i < MAX_AVATARS; i++){
        avatars[i].setup();
        float yp = (i+1) * (ofGetHeight()/4.0);
        avatars[i].pos.set(avatarOffX,yp+avatarOffY);
    }
 

    
    //--------- set application settings
    ofSetFrameRate(30);
    
    //--------- gui
    setupGui();
    bShowGui = true;
    
    //--------- osc settings
    ofxXmlSettings xml;
    xml.load("oscSettings.xml");
    oscHost = xml.getValue("host", "localhost");
    oscPort = xml.getValue("port", 12345);
	oscSender.setup(oscHost, oscPort);

}
//--------------------------------------------------------------
void ofApp::exit(){
    gui.saveToFile("settings.xml");
    if(bUseKinect) recorder.waitForThread();
    if(bUseKinect) kinect.close();
}
//--------------------------------------------------------------
void ofApp::update(){
    
    
    if(bUseKinect){
        
        //--------- update kinect
        kinect.update();
    
        //--------- process frame for background subtraction and thresholding by depth
        processKinectFrame();
    
        //--------- check if user is present
        checkForUser();
        
        //--------- update recording
        updateRecording();
    
    }
    
    
    
    //--------- update avatar players
    if(totalAvatarsThisUser > 0){
        for( int i = 0; i < totalAvatarsThisUser; i++){
            avatars[i].update();
        }
    }
    
    //-------- send osc mesages
    sendOscData();
    
    //-------- update vars not directly connected to gui
    updateVarsFromGui();
    
}

//--------------------------------------------------------------
void ofApp::processKinectFrame(){
    
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
            if(pixG[i] < guiNearThreshold && pixG[i] > guiFarThreshold) {
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
}

//--------------------------------------------------------------
void ofApp::startRecording(){
    
    if(!bUseKinect) return;
    
    if(totalAvatarsThisUser >= MAX_AVATARS || bSavingRecords){
        bRecordingAvatar = false;
        return;
    }
    
    cout << "Start recording." << endl;
    
    currentAvatar++;
    totalAvatarsThisUser++;
    
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
void ofApp::updateRecording(){
    
    if(bRecordingAvatar && ofGetFrameNum() % 2 == 0){
        
        recorder.addFrame(colorImageMasked.getPixelsRef());
    
    }else if(bSavingRecords){
        
        if(recorder.q.size() == 0){
            
            bSavingRecords = false;
            cout << "Done saving frames." << endl;
            
            // start new avatar
            if(!avatars[currentAvatar].isPlaying()){
                cout << "Init avatar." << endl;
                avatars[currentAvatar].startAvatar();
            }
            
        }else{
            //cout << " saving frames" << endl;
        }
    }
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

//--------------------------------------------------------------
void ofApp::checkForUser(){
    
    if(guiUseCheckUser && bUseKinect){
        
        if(kinect.isFrameNew()){
            
            ofPoint boxCenter = ofPoint(guiBoxCenterX,guiBoxCenterY,guiBoxCenterZ);
            int depthPointsInBox = 0;
            
            int w = 640;
            int h = 480;
            
            bHasDetectedUser = false;
            
            for( int x = 0; x < w; x+=10){
                for( int y = 0; y < h; y+=10){
                    
                    ofPoint currentPoint = kinect.getWorldCoordinateAt(x, y);

                    if (currentPoint.x > boxCenter.x - guiBoxWidth/2 && currentPoint.x < boxCenter.x + guiBoxWidth/2) {
                        if (currentPoint.y > boxCenter.y - guiBoxHeight/2 && currentPoint.y < boxCenter.y + guiBoxHeight/2) {
                            if (currentPoint.z > boxCenter.z - guiBoxDepth/2 && currentPoint.z < boxCenter.z + guiBoxDepth/2) {
                                depthPointsInBox++;
                            }
                        }
                    }
                }

            }
            cout << "depthPointsInBox detected." << depthPointsInBox << endl;
            if( depthPointsInBox > guiPresenceThreshold){
                //cout << "User detected." << endl;
                bHasDetectedUser = true;
            }
        }
            
    }
    
}

//--------------------------------------------------------------
void ofApp::sendOscData(){
    
    ofxOscMessage m;
    m.setAddress("/hasUser");
    int hasUser = bHasDetectedUser ? 1 : 0;
    m.addIntArg(hasUser);
    oscSender.sendMessage(m);
    
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
            float xp = 400;
            float yp = 20;
            
            //--------- draw preview
            ofSetColor(255);
            kinect.draw(xp, 20, 160, 120);
            kinect.drawDepth(xp+160, 20, 160, 120);
            colorImageMasked.draw(xp+160*2,yp,160,120);
            
            if(bHasDetectedUser){
                ofNoFill();
                ofSetColor(255,0,0);
                ofRect(xp+160*2,yp,160,120);
            }
        }
        
        ofSetColor(255, 255, 255);
        stringstream reportStream;
        if(bUseKinect){
            reportStream << "fps: " << ofGetFrameRate() << endl <<
            "r - toggle recording" << endl << "f - toggle fullscreen" <<  endl << "g - toggle gui " << endl << "s - save settings" << endl
            << "x - clear all avatars" << endl << "recording: " << bRecordingAvatar << endl;
        }else{
            reportStream << "0 - Load prerecorded avatar.\n" << "g - toggle this text on/off\n"
            << "f - toggle fullscreen\n" <<  "fps: " << ofGetFrameRate() << endl;
        }
            ofDrawBitmapString(reportStream.str(), 10, 450);
    
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::openNextAvatarFromSaved(){
    
    if(totalAvatarsThisUser < MAX_AVATARS){
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a directory",true);
        string path = openFileResult.getPath();
        ofDirectory dirManager;
        dirManager.open(path);
        if( dirManager.isDirectory() ){
            avatars[totalAvatarsThisUser].setDirectory(openFileResult.getPath());
            avatars[totalAvatarsThisUser].startAvatar();
            totalAvatarsThisUser++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    
    // toggle fullscreen
    // show fps
    parameters.setName("Settings");
    
    kinectParameters.setName("Kinect Settings");
    kinectParameters.add(guiNearThreshold.set("Near Threshold",50,0,255));
    kinectParameters.add(guiFarThreshold.set("Far Threshold",160,0,255));
    
    avatarParameters.setName("Avatar Settings");
    avatarParameters.add(guiXPos.set("Avatar X Position",ofGetWidth()*.5+200,-1024,1024));
	avatarParameters.add(guiYPos.set( "Avatar Y Position", 0,-1024, 1024 ));
    avatarParameters.add(guiScale.set( "Avatar Scale", 1,.1, 3 ));
    
    userCheckParameters.setName("User Presense");
    userCheckParameters.add(guiBoxCenterX.set("User Box X",-500,-2000,2000));
    userCheckParameters.add(guiBoxCenterY.set("User Box Y",0,-2000,2000));
    userCheckParameters.add(guiBoxCenterZ.set("User Box Z",2800,0,8000));
    userCheckParameters.add(guiBoxWidth.set("User Box Width",800,0,2000));
    userCheckParameters.add(guiBoxHeight.set("User Box Height",400,0,2000));
    userCheckParameters.add(guiBoxDepth.set("User Box Depth",400,0,2000));
    userCheckParameters.add(guiPresenceThreshold.set("User Pixel Thresh",20,0,300));
    userCheckParameters.add(guiUseCheckUser.set("Use Presence",true));

    parameters.add(kinectParameters);
    parameters.add(avatarParameters);
    parameters.add(userCheckParameters);

    gui.setup(parameters);

}
//--------------------------------------------------------------
void ofApp::updateVarsFromGui(){
    
    if(guiScale != drawScale){
        drawScale = guiScale;
        for(int i = 0; i < MAX_AVATARS; i++) avatars[i].drawScale = drawScale;
    }else if( guiXPos != avatarOffX){
        avatarOffX = guiXPos;
        for(int i = 0; i < MAX_AVATARS; i++){
            avatars[i].pos.x = avatarOffX;
        }
    }else if( guiYPos != avatarOffY ){
        avatarOffY = guiYPos;
        for(int i = 0; i < MAX_AVATARS; i++){
            float yp = (i+1) * (ofGetHeight()/4.0);
            avatars[i].pos.y= yp+avatarOffY;
        }
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'l':
            gui.loadFromFile("settings.xml");
            break;
        case 's':
            gui.saveToFile("settings.xml");
            break;
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
			//farThreshold ++;
			//if (farThreshold > 255) farThreshold = 255;
			break;
		case OF_KEY_UP:
			/*angle++;
			if(angle>30) angle=30;
			if(bUseKinect) kinect.setCameraTiltAngle(angle);*/
            avatarOffY-=5;
            for(int i = 0; i < MAX_AVATARS; i++){
                float yp = (i+1) * (ofGetHeight()/4.0);
                avatars[i].pos.set(avatarOffX,yp+avatarOffY);
            }
			break;
			
		case OF_KEY_DOWN:
			avatarOffY+=5;
            for(int i = 0; i < MAX_AVATARS; i++){
                float yp = (i+1) * (ofGetHeight()/4.0);
                avatars[i].pos.set(avatarOffX,yp+avatarOffY);
            }/*angle--;
			if(angle<-30) angle=-30;
			if(bUseKinect) kinect.setCameraTiltAngle(angle);*/
			break;
        case OF_KEY_LEFT:
            avatarOffX+=5;
            for( int i = 0; i < totalAvatarsThisUser; i++){
                avatars[i].pos.x = avatarOffX;
            }
            break;
        case OF_KEY_RIGHT:
            avatarOffX-=5;
            for( int i = 0; i < totalAvatarsThisUser; i++){
                avatars[i].pos.x = avatarOffX;
            }
            break;
        case 'x':
            for(int i = 0; i < MAX_AVATARS; i++){
                avatars[i].resetAvatar();
            }
            totalAvatarsThisUser = 0;
            currentAvatar = -1;
            recorder.q.empty();
            break;
        case '0':
            openNextAvatarFromSaved();
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
    
    cout << "avatarx " << avatarOffX << " gui x " << guiXPos << endl;
    
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
