#pragma once

#include "ofMain.h"
#include "ofxImageSequenceRecorder.h"
#include "ofxImageSequencePlayer.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "avatar.h"

#define MAX_AVATARS 1

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
		
    
    
        /** Initializes frame recording. Creates new directory for saving frames. */
        void startRecording();
    
        /** Stops frame recording. */
        void endRecording();
    
    
        //----- Kinect ----//
    
        /** Kinect object. */
        ofxKinect kinect;
    
        /** Color image with background removed. */
        ofxCvColorImage colorImageMasked;
    
        /** Grayscale image with depth image. */
        ofxCvGrayscaleImage depthImage;
    
        /** Thresholds for near and far limits. */
        int nearThreshold;
        int farThreshold;
    
    
    
        //----- Avatars -----//
        Avatar avatars[MAX_AVATARS];
    
        /** Total avatars made for current user. */
        int totalAvatarsThisUser;
        int currentAvatar;
    
        bool bRecordingAvatar;
        ofxImageSequenceRecorder recorder;

    
        //----- Application Control -----//
    
        /** Is kinect in use and connected. */
        bool bUseKinect;
    
    
    
        //----- Gui and Display -----//
    
        /** Toggles display of gui on and off. */
        bool bShowGui;
    
};
