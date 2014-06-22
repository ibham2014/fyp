//
//  avatar.h
//  forgot_password
//
//  Created by chris on 22/06/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxImageSequenceRecorder.h"
#include "ofxImageSequencePlayer.h"

class Avatar{
    
    public:
    
    void setup();
    void update();
    void draw();
    void addFrame(ofPixels imageToSave);
    void setDirectory(string dir);
    void startRecording();
    void endRecording();
    
    ofxImageSequenceRecorder recorder;
    ofxImageSequencePlayer player;
    
    string myDirectory;
    
    bool bRecording;
    bool bPlaying;
    bool bDoneRecording;
    
    ofPoint pos;
    
};



