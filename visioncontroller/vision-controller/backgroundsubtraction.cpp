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
Image* Backgroundsubtraction::doFullBackSubtract(Image* img){
    Image* bsimg = new Image();
    
    for(int i = 0; i < IMAGEX; i++){
        for(int j = 0; j < IMAGEY; j++){
            
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
    // if lower than add as the low boundary
    if(value < this->background[x][y]->low){
        return -1;
    }
    // if higher add as the high boundary
    if(value > this->background[x][y]->high){
        return 1;
    }
    return 0;
}
