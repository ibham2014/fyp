//
//  imageSequenceRecorder.h
//  forgot_password
//
//  Created by chris on 25/06/14.
//
//


/*
 
 Based on code by Memo from this thread:
 http://forum.openframeworks.cc/index.php?topic=1687.0
 Base on ofxImageSequenceRecorder
 
 */

#pragma once

#include "ofMain.h"

typedef struct {
    string fileName;
    ofPixels image;
} QueuedImage;

class imageSequenceRecorder : public ofThread {
public:
    
    int counter;
    queue<QueuedImage> q;
    string prefix;
    string format;
    ofImage alphaImage;
    unsigned char * alphaPixels;
    
    imageSequenceRecorder(){
        counter=0;
        
    }
    
    void setup(int imageWidth, int imageHeight){
        alphaImage.allocate(imageWidth,imageHeight,OF_IMAGE_COLOR_ALPHA);
        alphaPixels = new unsigned char[imageWidth*imageHeight*4];
        for(int i = 0; i < imageWidth*imageHeight*4; i++){
            alphaPixels[i] = 0;
        }
    }
    
    void setPrefix(string pre){
        prefix = pre;
    }
    
    void setFormat(string fmt){
        format = fmt;
    }
    
    void threadedFunction() {
        while(isThreadRunning()) {
            if(!q.empty()){
                
                QueuedImage i = q.front();
                
                // convert to alpha
               /* unsigned char * pix = i.image.getPixels();
                
                for(int i = 0; i < 640*480; i++){
                    
                    if(pix[i*3]<20){
                        alphaPixels[i*4] = 0;
                        alphaPixels[i*4+1] = 0;
                        alphaPixels[i*4+2] = 0;
                        alphaPixels[i*4+3] = 0;
                        
                    }else if(pix[i*3]){
                        alphaPixels[i*4] = pix[i*3];
                        alphaPixels[i*4+1] = pix[i*3+1];
                        alphaPixels[i*4+2] = pix[i*3+2];
                        alphaPixels[i*4+3] = 255;
                        
                    }else{
                        alphaPixels[i*4] = 0;
                        alphaPixels[i*4+1] = 0;
                        alphaPixels[i*4+2] = 0;
                        alphaPixels[i*4+3] = 0;
                        
                    }
                    
                }
                alphaImage.setFromPixels(alphaPixels, 640, 480, OF_IMAGE_COLOR_ALPHA);
                */
                ofSaveImage(i.image, i.fileName);
                q.pop();
            }
        }
        
        
        
    }
    
    void addFrame(ofImage &img){
        addFrame(img.getPixelsRef());
    }
    
    void addFrame(ofVideoGrabber &cam){
        addFrame(cam.getPixelsRef());
    }
    
    void addFrame(ofVideoPlayer &player){
        addFrame(player.getPixelsRef());
    }
    
    void addFrame(ofPixels imageToSave) {
        
        cout << "add frame " << endl;
        
        char fileName[100];
        sprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());
        counter++;
        
        QueuedImage qImage;
        
        qImage.fileName = fileName;
        qImage.image = imageToSave;
        
        q.push(qImage);
        
    }    
};