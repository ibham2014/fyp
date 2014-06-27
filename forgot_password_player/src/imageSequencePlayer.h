//
//  imageSequencePlayer.h
//  forgot_password
//
//  Created by chris on 26/06/14.
//  Based on ofxImageSequencePlayer by Lukasz Karluk on 21/06/13.
//  http://julapy.com/blog
//

#pragma once

#include "ofMain.h"

class imageSequencePlayer: public ofBaseVideoPlayer {
            
    public:
        
        imageSequencePlayer();
        ~imageSequencePlayer();
        
        //void startThreadedLoading(string dirName);
        bool isMovieLoaded();
        
        //-------------------------------------- ofxImageSequencePlayer methods.
        void setFrameRate(float value);
        
        //-------------------------------------- ofBaseVideoPlayer methods.
        bool loadMovie(string name);
        bool loadMovie(vector<ofImage>&images);
        void close();
        void update();
        
        bool setPixelFormat(ofPixelFormat pixelFormat);
        ofPixelFormat getPixelFormat();
        
        void play();
        void stop();
        
        vector<ofImage> images;
        
        bool isFrameNew();
        unsigned char * getPixels();
        ofPixelsRef	getPixelsRef();
        ofTexture *	getTexture();
        
        void drawImages();
        
        float getWidth();
        float getHeight();
        
        bool isPaused();
        bool isLoaded();
        bool isPlaying();
        
        float getPosition();
        float getSpeed();
        float getDuration();
        bool getIsMovieDone();
        
        void setPaused(bool bPause);
        void setPosition(float pct);
        void setVolume(float volume); // 0..1
        void setLoopState(ofLoopType state);
        void setSpeed(float speed);
        void setFrame(int frame);  // frame 0 = first frame...
        
        int	getCurrentFrame();
        int	getTotalNumFrames();
        ofLoopType getLoopState();
        
        void firstFrame();
        void nextFrame();
        void previousFrame();
        
        void setWaitTime(float time){ waitTime = time; }
        void togglePlaying(){ bPlaying = !bPlaying;}

    protected:
        
        vector<string> imageSequencePaths;
        vector<ofTexture *> imageSequenceTextures;
        vector<ofImage *> imageSequenceImages;
        
        bool bLoaded;
        bool bPlaying;
        bool bPaused;
        bool bNewFrame;
        
        float fps;
        int frameIndex;
        int frameLastIndex;
        int framesTotal;
        float position;
        float speed;
        float duration;
        ofLoopType loopType;
        
        float lastFrameTime;
        float waitTime;
    };