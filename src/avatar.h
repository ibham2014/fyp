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
#include "imageSequenceLoader.h"

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
        ofPoint pos;
        imageSequenceLoader imageLoader;
    
        
    private:
    
        string myDirectory;
        bool bPlaying;
        bool bLoading;
    
        bool bPortrait;
        ofImage image;
        unsigned char * imagePix;
    

};



