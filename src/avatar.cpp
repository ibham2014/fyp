//
//  avatar.cpp
//  forgot_password
//
//  Created by chris on 22/06/14.
//
//

#include "avatar.h"

void Avatar::setup(){
    
    bPlaying = false;
    bPortrait = false;
    //pos.set(0,0);
    fbo.allocate(640, 480,GL_RGBA);
    image.allocate(640,480,OF_IMAGE_COLOR_ALPHA);
    imagePix = new unsigned char[640*480*4];
    for(int i = 0; i < 640*480*4; i++){
        imagePix[i] = 0;
    }
}

void Avatar::resetAvatar(){
    
    bPlaying = false;
    player.stop();
    player.close();
    myDirectory = "";
    for(int i = 0; i < 640*480*4; i++){
        imagePix[i] = 0;
    }
}


void Avatar::update(){
    
    if(bPlaying){
        player.update();
    }
    
   
}


void Avatar::setDirectory(string dir){
    
    myDirectory = dir;
    ofDirectory dirManager;
    dirManager.createDirectory(myDirectory);
}


void Avatar::startAvatar(){
    
    if(myDirectory.length()==0) return;
    
    cout << "start play" << endl;
    player.loadMovie(myDirectory);//startThreadedLoading(myDirectory);
    player.play();
    bPlaying = true;
}


void Avatar::draw(){
    
    
    int totalLoaded = 0;
    for(int i = 0; i < player.images.size(); i++){
        if(player.images[player.getCurrentFrame()].getWidth() > 0){
            totalLoaded++;
            cout << "curre image width " << player.images[player.getCurrentFrame()].getWidth()  << endl;
        }
        else cout << "curre image width " << player.images[player.getCurrentFrame()].getWidth()  << endl;
    }
    if(totalLoaded >= player.images.size() && totalLoaded > 0){
        cout << "stop loader totalLoaded" << totalLoaded << endl;
    }else{
        return;
    }
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofEnableAlphaBlending();
    
    if(bPlaying){
        int myFrame = player.getCurrentFrame();
        if(player.images[player.getCurrentFrame()].width <= 0){
            myFrame -= 1;
        }
        
        if(myFrame >= 0 && player.images[myFrame].width > 0){
            
        unsigned char * pix = player.images[myFrame].getPixels();
       
        for(int i = 0; i < 640*480; i++){
            
            if(pix[i*3]<20){
                imagePix[i*4] = 0;
                imagePix[i*4+1] = 0;
                imagePix[i*4+2] = 0;
                imagePix[i*4+3] = 0;

            }else if(pix[i*3]){
                imagePix[i*4] = pix[i*3];
                imagePix[i*4+1] = pix[i*3+1];
                imagePix[i*4+2] = pix[i*3+2];
                imagePix[i*4+3] = 255;
                
            }else{
                imagePix[i*4] = 0;
                imagePix[i*4+1] = 0;
                imagePix[i*4+2] = 0;
                imagePix[i*4+3] = 0;

            }
        }
        image.setFromPixels(imagePix, 640, 480, OF_IMAGE_COLOR_ALPHA);
        
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofPushMatrix();
        ofTranslate(pos.x,pos.y);
        if(bPortrait) ofRotate(90);
        //player.drawImages();
        image.draw(0,0);
        ofPopMatrix();
        ofSetRectMode(OF_RECTMODE_CORNER);
        }else{
           // cout << "bad image no width " << endl;
        }
    }

    
    ofSetRectMode(OF_RECTMODE_CORNER);
    //cout << "posx " << pos.x << " posy " << pos.y << endl;
    

}