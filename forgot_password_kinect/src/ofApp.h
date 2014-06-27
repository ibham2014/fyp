#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "imageSequenceRecorder.h"
#include "avatar.h"

#define MAX_AVATARS 3

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
    
        //---Recording controls
    
        /** Initializes frame recording. Creates new directory for saving frames. */
        void startRecording();
    
        /** Stops frame recording. */
        void endRecording();
    
        /** Manages adding frames and stopping recording / start player when done saving. */
        void updateRecording();
    
        //-- Kinect data processing
    
        /** Process the kinect frame to remove background, get threshold image */
        void processKinectFrame();
    
        /** Check if a user is present by counting number of pixels inside a defined area in the scene */
        void checkForUser();
    
        //--- Interface
        /** Set up gui. */
        void setupGui();
    
        /** Update any variables that relate to gui but not used directly from gui. */
        void updateVarsFromGui();
    
        /** Load an avatar an sequence of images from a directory*/
        void openNextAvatarFromSaved();
    
        /** Send data via osc */
        void sendOscData();
    
        
        //----- Kinect ----//
    
        /** Kinect object. */
        ofxKinect kinect;
    
        /** Color image with background removed. */
        ofxCvColorImage colorImageMasked;
    
        /** Grayscale image with depth image. */
        ofxCvGrayscaleImage depthImage;
    
        /** Grayscale image for background capture. */
        ofxCvGrayscaleImage grayBackgroundCapture;

        /** Flag if we should capture background this frame. */
        bool bCaptureBg;
    
        bool bHasDetectedUser;
    
        //----- Avatars -----//
        Avatar avatars[MAX_AVATARS];
    
        /** Total avatars made for current user. */
        int totalAvatarsThisUser;
        int currentAvatar;
    
        imageSequenceRecorder recorder;
        float avatarOffX,avatarOffY;
        float drawScale;
        bool bRecordingAvatar;
        bool bSavingRecords;
    
        //----- Application Control -----//
    
        /** Is kinect in use and connected. */
        bool bUseKinect;
    
        ofxOscSender oscSender;
        string oscHost;
        int oscPort;    
    
    
        //----- Gui and Display -----//
    
        /** Toggles display of gui on and off. */
        bool bShowGui;
        ofParameterGroup parameters;
        ofParameterGroup kinectParameters;
        ofParameterGroup avatarParameters;
        ofParameterGroup userCheckParameters;

        ofxPanel gui;
        ofParameter<int> guiXPos;
        ofParameter<int> guiYPos;
        ofParameter<float> guiScale;
        ofParameter<int> guiNearThreshold;
        ofParameter<int> guiFarThreshold;
        ofParameter<int> guiBoxCenterX;
        ofParameter<int> guiBoxCenterY;
        ofParameter<int> guiBoxCenterZ;
        ofParameter<int> guiBoxWidth;
        ofParameter<int> guiBoxHeight;
        ofParameter<int> guiBoxDepth;
        ofParameter<int> guiPresenceThreshold;
        ofParameter<bool> guiUseCheckUser;
    
        //ofxToggle    guiFullScreen;
        //ofxToggle    guiShowBox;

    
        // TODO
    /*
     - add osc
     - add kinect presence detection
     - fade out avatars
     - control timing of recording
     - threaded loading
     */
    
};
