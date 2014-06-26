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
    
    
    
        /** Initializes frame recording. Creates new directory for saving frames. */
        void startRecording();
    
        /** Stops frame recording. */
        void endRecording();
    
         /** Set up gui. */
        void setupGui();
    
        /** Load an avatar an sequence of images from a directory*/
        void openNextAvatarFromSaved();
    
        //----- Kinect ----//
    
        /** Kinect object. */
        ofxKinect kinect;
    
        /** Color image with background removed. */
        ofxCvColorImage colorImageMasked;
        ofxCvGrayscaleImage grayBackgroundCapture;
        bool bCaptureBg;
    
        ofImage colorImage;
    
        /** Grayscale image with depth image. */
        ofxCvGrayscaleImage depthImage;
    
        /** Thresholds for near and far limits. */
        int nearThreshold;
        int farThreshold;
        float angle;
    
    
        //----- Avatars -----//
        Avatar avatars[MAX_AVATARS];
    
        /** Total avatars made for current user. */
        int totalAvatarsThisUser;
        int currentAvatar;
    
        bool bRecordingAvatar;
        bool bSavingRecords;
        imageSequenceRecorder recorder;
        float avatarOffX,avatarOffY;
        float drawScale;
    
        //----- Application Control -----//
    
        /** Is kinect in use and connected. */
        bool bUseKinect;
    
    
    
        //----- Gui and Display -----//
    
        /** Toggles display of gui on and off. */
        bool bShowGui;
        ofxPanel gui;
        ofxIntSlider guiNearThreshold;
        ofxIntSlider guiFarThreshold;
        ofxIntSlider guiBoxCenter;
        ofxIntSlider guiBoxWidth;
        ofxIntSlider guiBoxHeight;
        ofxIntSlider guiBoxDepth;
        ofxToggle    guiFullScreen;
        ofxToggle    guiShowBox;
    

    
        // TODO
    /*
     - add osc
     - check saving out png frames
     - add kinect presence detection
     - fade out avatars
     - control timing of recording
     - threaded loading
     */
    
};
