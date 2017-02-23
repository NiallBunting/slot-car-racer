/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   backgroundsubtraction.cpp
 * Author: nib28
 * 
 * Created on 08 February 2017, 13:53
 */

#include "backgroundsubtraction.h"

Backgroundsubtraction::Backgroundsubtraction() {
    for(int i = 0; i < IMAGEX; i++){
        for(int j = 0; j < IMAGEY; j++){
            BackgroundPixel* bp = new BackgroundPixel;
            bp->high = -1;
            bp->low = -1;
            bp->mask = 0;
            this->background[i][j] = bp;
        }
    }
}

//Give image and it will increase the width of the background image
int Backgroundsubtraction::trainBackground(Image* img){
    // loop through each pixel
    for(int i = 0; i < IMAGEX; i++){
        for(int j = 0; j < IMAGEY; j++){
            
            //initialise on first input
            if(this->background[i][j]->low == -1 || this->background[i][j]->high == -1){
                this->background[i][j]->low = img->getPixel(i, j);
                this->background[i][j]->high = img->getPixel(i, j);
            }
            
            // if lower than add as the low boundary
            if(this->boundCheck(i, j, img->getPixel(i, j)) == -1){
                this->background[i][j]->low = img->getPixel(i, j);
            }
            // if higher add as the high boundary
            if(this->boundCheck(i, j, img->getPixel(i, j)) == 1){
                this->background[i][j]->high = img->getPixel(i, j);
            }
            
        }
    }
    return 0;
}

Image* Backgroundsubtraction::doSparseBackSubtract(Image* img, int sparse){
//Idea to do a reduced background subtract
//only checks every 5pxs in each direction, is a match then checks in directions around
//pixel to check if matches then can do some colour checking.    
}

//set to one if out of bounds
//returns 0 when no change
Image* Backgroundsubtraction::doFullBackSubtract(Image* img){
    Image* bsimg = new Image();
    
    for(int i = 0; i < IMAGEX; i++){
        for(int j = 0; j < IMAGEY; j++){
            if(this->background[i][j]->mask == 0){
                continue;
            }
            
            //checks within limits
            if(this->boundCheck(i, j, img->getPixel(i, j)) != 0){
                //out of bounds
                bsimg->updatePixel(i, j, 1);
            }

        }
    }
    return bsimg;
}

//Checks if the pixel value is within the current bounds
//-1 lower
//0 same
// 1 higher
int Backgroundsubtraction::boundCheck(int x, int y, int value){
    return this->boundCheck(x, y, value, 0.2f);
}

int Backgroundsubtraction::boundCheck(int x, int y, int value, float threshold){
    // if lower than add as the low boundary
    if(value < (this->background[x][y]->low * (1.0f-threshold))){
        return -1;
    }
    // if higher add as the high boundary
    if(value > (this->background[x][y]->high * (1.0f+threshold))){
        return 1;
    }
    return 0;
}

//Finds large blobs and adds them to the mask.
Image* Backgroundsubtraction::maskAdd(Image* img){
    int size = 2;
    Image* bsimg = new Image();

    //loop through each pixel
    for(int i = size; i < IMAGEX - size; i++){
        for(int j = size; j < IMAGEY - size; j++){ 
            
            if(this->background[i][j]->mask == 1){
                bsimg->updatePixel(i, j, 255);
                continue;
            }
            
            //pass on 0 at end of tests.
            int test = 0;
            
            //check actual pixel first
            if(this->boundCheck(i, j, img->getPixel(i, j)) == 0){
               test = 1;
               continue;
            }
            
            //test in x direction
            for(int q = i - size; q < i + size; q++){
                //test if outside boundary
                if(this->boundCheck(q,j, img->getPixel(q,j)) == 0){
                    test = 1;
                    continue;
                }
            }

            //test in y direction
            for(int q = j - size; q < j + size; q++){
                //test if outside boundary
                if(this->boundCheck(i,q, img->getPixel(i,q)) == 0){
                    test = 1;
                    continue;
                }
            }
            
            if(test == 0){
                //test passed
                this->background[i][j]->mask = 1;
                bsimg->updatePixel(i, j, 255);
            }
        }
    }
    
    
    return bsimg;
}