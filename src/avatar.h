//
//  avatar.h
//  forgot_password
//
//  Created by chris on 22/06/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxImageSequencePlayer.h"

class Avatar{
    
    public:
    
        void setup();
        void update();
        void draw();
        void setDirectory(string dir);
        void startAvatar();
        void resetAvatar();
        
        bool isPlaying(){ return bPlaying; }
        
        ofxImageSequencePlayer player;
        ofFbo fbo;
        ofPoint pos;
        
    private:
    
        string myDirectory;
        bool bPlaying;
        bool bPortrait;
        ofImage image;
        unsigned char * imagePix;
    

};



