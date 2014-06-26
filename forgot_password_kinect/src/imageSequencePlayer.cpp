//
//  imageSequencePlayer.cpp
//  forgot_password
//
//  Created by chris on 26/06/14.
//
//

#include "imageSequencePlayer.h"

imageSequencePlayer::imageSequencePlayer() {
    close();
    waitTime = .05;
    
}

imageSequencePlayer::~imageSequencePlayer() {
    close();
}

/*
 void imageSequencePlayer::startThreadedLoading(string dirName){
 
 close();
 
 loader.startThread();
 
 images.clear();
 ofDirectory dir(dirName);
 int tFiles = dir.listDir();
 images.resize(tFiles);
 for(int i=0; i<tFiles; i++) {
 imageSequencePaths.push_back(dir.getPath(i));
 loader.loadFromDisk(images[i], dir.getPath(i));
 images[i].setImageType(OF_IMAGE_COLOR_ALPHA);
 }
 
 frameIndex = 0;
 framesTotal = tFiles;
 frameLastIndex = framesTotal - 1;
 bNewFrame = true;
 lastFrameTime = ofGetElapsedTimef();
 bLoaded = true;
 bPlaying = true;
 }
 */

bool imageSequencePlayer::isMovieLoaded(){
    
}

void imageSequencePlayer::setFrameRate(float value) {
    fps = value;
}

bool imageSequencePlayer::loadMovie(vector<ofImage>&_images){
    
    bLoaded = _images.size() > 0;
    if(bLoaded == false) {
        return false;
    }
    
    close();
    for(int i=0; i<_images.size(); i++) {
        //ofTexture * texture = new ofTexture();
        //texture->allocate(_images[i].getPixelsRef());
        // texture->loadData(_images[i].getPixelsRef());
        // imageSequenceTextures.push_back(texture);
        images.push_back(_images[i]);
    }
    
    frameIndex = 0;
    framesTotal = images.size();
    frameLastIndex = framesTotal - 1;
    bNewFrame = true;
    lastFrameTime = ofGetElapsedTimef();
    bLoaded = true;
    bPlaying = true;
    return true;
    
}

bool imageSequencePlayer::loadMovie(string name) {
    
    close();
    images.clear();
    // curretly this is as basic as it gets.
    // it loads all files from a directory.
    // TODO :: check if files in directory are a sequence.
    // TODO :: if a single file is passed, work out if its part of a sequence and load that sequence.
    // loader.stopThread();
    
    ofDirectory dir(name);
    int numOfFiles = dir.listDir();
    for(int i=0; i<numOfFiles; i++) {
        imageSequencePaths.push_back(dir.getPath(i));
    }
    
    bLoaded = imageSequencePaths.size() > 0;
    if(bLoaded == false) {
        return false;
    }
    
    ofImage imageLoader;
    for(int i=0; i<imageSequencePaths.size(); i++) {
        bool bLoaded = imageLoader.loadImage(imageSequencePaths[i]);
        if(bLoaded == false) {
            continue;
        }
        
        ofTexture * texture = new ofTexture();
        texture->allocate(imageLoader.getPixelsRef());
        texture->loadData(imageLoader.getPixelsRef());
        imageSequenceTextures.push_back(texture);
        
        images.push_back(imageLoader);
        imageLoader.clear();
    }
    
    bLoaded = imageSequenceTextures.size() > 0;
    if(bLoaded == false) {
        return false;
    }
    
    frameIndex = 0;
    framesTotal = imageSequenceTextures.size();
    frameLastIndex = framesTotal - 1;
    bNewFrame = true;
    lastFrameTime = ofGetElapsedTimef();
    
    return bLoaded;
}

void imageSequencePlayer::close() {
    
    //loader.stopThread();
    images.clear();
    
    fps = 30;
    bLoaded = false;
    bPlaying = false;
    bPaused = false;
    bNewFrame = false;
    frameIndex = 0;
    frameLastIndex = 0;
    framesTotal = 0;
    position = 0;
    speed = 1;
    duration = 0;
    loopType = OF_LOOP_NORMAL;//NE;
    
    imageSequencePaths.clear();
    
    for(int i=0; i<imageSequenceTextures.size(); i++) {
        delete imageSequenceTextures[i];
        imageSequenceTextures[i] = NULL;
    }
    imageSequenceTextures.clear();
}

void imageSequencePlayer::update() {
    
    bNewFrame = false;
    
    if(isLoaded() == false) {
        return;
    }
    
    if(isPlaying() == false) {
        return;
    }
    
    /* for(int i = 0; i < images.size(); i++){
     if(images[i].getWidth() == 0) return;
     }
     
     if(loader.isThreadRunning()){
     int totalLoaded = 0;
     for(int i = 0; i < images.size(); i++){
     if(images[getCurrentFrame()].getWidth() > 0){
     totalLoaded++;
     // cout << "curre image width " << images[getCurrentFrame()].getWidth()  << endl;
     }
     //else cout << "curre image width " << images[getCurrentFrame()].getWidth()  << endl;
     }
     if(totalLoaded >= images.size() && totalLoaded > 0){
     //cout << "stop loader totalLoaded" << totalLoaded << endl;
     loader.stopThread();
     }
     }
     */
    float timeDiff = ofGetElapsedTimef() - lastFrameTime;
    if(timeDiff > waitTime) nextFrame();
    
}

bool imageSequencePlayer::setPixelFormat(ofPixelFormat pixelFormat) {
    if(isLoaded() == false) {
        return false;
    }
    
    // TODO.
    return false;
}

ofPixelFormat imageSequencePlayer::getPixelFormat() {
    if(isLoaded() == false) {
        return OF_PIXELS_RGBA;
    }
    
    // TODO.
    return OF_PIXELS_RGBA;
}

void imageSequencePlayer::play() {
    if(isLoaded() == false) {
        return;
    }
    
    if(getIsMovieDone()) {
        setFrame(0);
    }
    
    bPlaying = true;
    bPaused = false;
}

void imageSequencePlayer::stop() {
    if(isLoaded() == false) {
        return;
    }
    
    bPlaying = false;
    bPaused = false;
}

bool imageSequencePlayer::isFrameNew() {
    if(isLoaded() == false) {
        return false;
    }
    
    return bNewFrame;
}

unsigned char * imageSequencePlayer::getPixels() {
    if(isLoaded() == false) {
        return NULL;
    }
    
    // TODO.
    return NULL;
}

ofPixelsRef	imageSequencePlayer::getPixelsRef() {
    static ofPixels dummy;
    return dummy;
}

void imageSequencePlayer::drawImages(){
    //if(isLoaded() == false) {
    //   return NULL;
    //}
    
    images[getCurrentFrame()].draw(0,0);
}

ofTexture * imageSequencePlayer::getTexture() {
    if(isLoaded() == false) {
        return NULL;
    }
    
    ofTexture * texture = imageSequenceTextures[getCurrentFrame()];
    return texture;
}

float imageSequencePlayer::getWidth() {
    if(isLoaded() == false) {
        return 0;
    }
    
    int w = getTexture()->getWidth();
    return w;
}

float imageSequencePlayer::getHeight() {
    if(isLoaded() == false) {
        return 0;
    }
    
    int h = getTexture()->getHeight();
    return h;
}

bool imageSequencePlayer::isPaused() {
    return bPaused;
}

bool imageSequencePlayer::isLoaded() {
    return bLoaded;
}

bool imageSequencePlayer::isPlaying() {
    return bPlaying;
}

float imageSequencePlayer::getPosition() {
    return position;
}

float imageSequencePlayer::getSpeed() {
    return speed;
}

float imageSequencePlayer::getDuration() {
    return duration;
}

bool imageSequencePlayer::getIsMovieDone() {
    bool bFinished = (bPlaying == false) && (getCurrentFrame() == frameLastIndex);
    return bFinished;
}

void imageSequencePlayer::setPaused(bool bPause) {
    bPaused = bPause;
}

void imageSequencePlayer::setPosition(float value) {
    int index = value * frameLastIndex;
    setFrame(index);
}

void imageSequencePlayer::setVolume(float volume) {
    // not supported.
}

void imageSequencePlayer::setLoopState(ofLoopType value) {
    loopType = value;
}

void imageSequencePlayer::setSpeed(float value) {
    speed = value;
}

void imageSequencePlayer::setFrame(int value) {
    if(isLoaded() == false) {
        return;
    }
    
    int index = ofClamp(value, 0, frameLastIndex);
    if(frameIndex == index) {
        return;
    }
    frameIndex = index;
    bNewFrame = true;
    
    position = frameIndex / (float)frameLastIndex;
}

int	imageSequencePlayer::getCurrentFrame() {
    return frameIndex;
}

int	imageSequencePlayer::getTotalNumFrames() {
    return framesTotal;
}

ofLoopType imageSequencePlayer::getLoopState() {
    return loopType;
}

void imageSequencePlayer::firstFrame() {
    setFrame(0);
}

void imageSequencePlayer::nextFrame() {
    if(isLoaded() == false) {
        return;
    }
    
    int index = getCurrentFrame() + 1;
    if(index > frameLastIndex) {
        if(loopType == OF_LOOP_NONE) {
            index = frameLastIndex;
            if(isPlaying()) {
                stop();
            }
        } else if(loopType == OF_LOOP_NORMAL) {
            index = 0;
        } else if(loopType == OF_LOOP_PALINDROME) {
            // TODO.
        }
    }
    
    setFrame(index);
    lastFrameTime = ofGetElapsedTimef();
}

void imageSequencePlayer::previousFrame() {
    if(isLoaded() == false) {
        return;
    }
    
    int index = getCurrentFrame() - 1;
    if(index < 0) {
        if(loopType == OF_LOOP_NONE) {
            index = 0;
        } else if(loopType == OF_LOOP_NORMAL) {
            index = frameLastIndex;
        } else if(loopType == OF_LOOP_PALINDROME) {
            // TODO.
        }
    }
    
    setFrame(index);
}
