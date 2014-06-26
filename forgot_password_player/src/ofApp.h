#pragma once

#include "ofMain.h"
#include "avatar.h"

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
    
        Avatar avatar;
        vector<string> avatarDirectories;
        bool bShowGui;
        bool bOpenFromFile;
        bool bPickRandom;
    
};
