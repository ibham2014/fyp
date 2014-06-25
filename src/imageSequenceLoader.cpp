//
//  imageSequenceLoader.cpp
//  forgot_password
//
//  Created by chris on 24/06/14.
//
//

#include "imageSequenceLoader.h"

imageSequenceLoader::imageSequenceLoader(){
    
}

void imageSequenceLoader::reset(){
    images.clear();
    imageSequencePaths.clear();
    bLoading = false;
    allLoaded = false;
    
}

void imageSequenceLoader::startLoading( string directory, bool useConvertToAlpha ){
    
    myDirectory = directory;
    bMakeAlpha = useConvertToAlpha;
    
    images.clear();
    
    // read directory images
    ofDirectory dir(directory);
    totalToLoad = dir.listDir();
    for(int i=0; i<totalToLoad; i++) {
        imageSequencePaths.push_back(dir.getPath(i));
    }
    
    // set to load
    bLoading = true;
    
    if(bMakeAlpha){
        alphaImage.allocate(640,480,OF_IMAGE_COLOR_ALPHA);
        alphaPixels = new unsigned char[640*480*4];
        for(int i = 0; i < 640*480*4; i++){
            alphaPixels[i] = 0;
        }
    }

}

void imageSequenceLoader::update(){
    
    if(bLoading){
        // check if all are loaded
        if(images.size() == totalToLoad){
            bLoading = false;
            allLoaded = true;
            return;
        }
        
        // load next
        string nextToLoad = imageSequencePaths[images.size()];
        ofImage imageLoader;
        bool bLoaded = imageLoader.loadImage(nextToLoad);
        
        // convert to alpha
        if(bMakeAlpha){
            unsigned char * pix = imageLoader.getPixels();
            
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
            images.push_back(alphaImage);

        }else{
            images.push_back(imageLoader);
        }

    }

}