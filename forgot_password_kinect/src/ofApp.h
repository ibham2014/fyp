#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxImageSequencePlayer.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"
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
    
    
    
        //----- Gui and Display -----//
    
        /** Toggles display of gui on and off. */
        bool bShowGui;
        ofxPanel gui;
        ofxIntSlider guiXPos;
        ofxIntSlider guiYPos;
        ofxFloatSlider guiScale;
        ofxIntSlider guiNearThreshold;
        ofxIntSlider guiFarThreshold;
        ofxIntSlider guiBoxCenter;
        ofxIntSlider guiBoxWidth;
        ofxIntSlider guiBoxHeight;
        ofxIntSlider guiBoxDepth;
        ofxIntSlider guiPresencePixels;
    
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
