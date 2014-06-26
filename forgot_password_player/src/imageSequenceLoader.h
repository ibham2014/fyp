//
//  imageSequenceLoader.h
//  forgot_password
//
//  Created by chris on 24/06/14.
//
//

#pragma once

#include "ofMain.h"

class imageSequenceLoader{
    
    public:
    
        imageSequenceLoader();
        void startLoading( string directory, bool useConvertToAlpha = true );
        void update();
        void reset();
    
        bool isLoaded(){ return allLoaded;}
        string getDirectory(){ return myDirectory; }
    
        vector<ofImage> images;
    
    private:
        int totalToLoad;
        bool bMakeAlpha;
        bool bLoading;
        bool allLoaded;
    
        string myDirectory;
        vector<string> imageSequencePaths;
    
        unsigned char * alphaPixels;
        ofImage alphaImage;
    
};