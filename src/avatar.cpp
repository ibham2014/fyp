//
//  avatar.cpp
//  forgot_password
//
//  Created by chris on 22/06/14.
//
//

#include "avatar.h"

void Avatar::setup(){
    
    recorder.setFormat("jpg"); // png is really slow but high res, bmp is fast but big, jpg is just right
    bRecording = false;
    bPlaying = false;
    bDoneRecording = false;
    pos.set(0,0);
    
}

void Avatar::update(){
    
    if(bPlaying){
        player.update();
    }
    
    if( !bPlaying && bDoneRecording ){
        if(recorder.q.size() == 0){
            //bPlaying = true;
            player.startThreadedLoading(myDirectory);
            //player.play();
        }
    }
}

void Avatar::addFrame(ofPixels imageToSave){
    
    if(!bRecording) return;
    recorder.addFrame(imageToSave);
     cout << "recorder add frame " << myDirectory << endl;
}

void Avatar::setDirectory(string dir){
    
    myDirectory = dir;
    ofDirectory dirManager;
    dirManager.createDirectory(myDirectory);
}

void Avatar::startRecording(){
    
    bRecording = true;
    string filePrefix = myDirectory + "/frame_";
    recorder.setPrefix(ofToDataPath(filePrefix));
    
    recorder.startThread();
}


void Avatar::endRecording(){
    
    bRecording = false;
    recorder.stopThread();
    bDoneRecording = true;
}

void Avatar::draw(){
    
    if(bPlaying){
        player.drawImages();//pos.x,pos.y);
    }
}