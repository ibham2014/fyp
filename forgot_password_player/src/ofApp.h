#pragma once

#include "ofMain.h"
#include "avatar.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"

#define USE_OSC

#ifdef USE_OSC
#include "ofxOsc.h"
#endif

#define MAX_AVATARS 2

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
		
        void openAvatarFromSaved();
        void clearAvatar();
        void pickRandomAvatar();
        void setToLoadSequence();
        void loadNextAvatar();
        void playNextAvatar();
        void clearAllAvatars();
        void playAndSendRecord();
        void sendStopRecord();
        void sendClearUser();
        void setupGui();
    
        Avatar avatar;
        vector<string> avatarDirectories;
        Avatar avatars[MAX_AVATARS];

        int totalPreloaded; // total instructional avatars loaded so far
        int currentPlaying;
    
        bool bOpenFromFile;
        bool bPickRandom;
        bool bLoadingNewSet; // is still loading avatars
        bool bReadyToStart; // is ready to begin insrtuctions
        bool bUseSets;
        bool bRecording;
    
        bool bShowGui;
        ofxPanel gui;
        ofTrueTypeFont font;
    

#ifdef USE_OSC
        ofxOscSender oscSender;
        ofxOscReceiver oscReceiver;
        string oscHost;
        int oscPort;
#endif
    
};
