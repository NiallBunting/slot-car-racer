/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   image.cpp
 * Author: nib28
 * 
 * Created on 08 February 2017, 13:56
 */

#include "image.h"


Image::Image(int image[]) {
    int line = -1;
    for(int i = 0; i < (IMAGEY * IMAGEX); i++){
        if(i % IMAGEX == 0){line++;}
        this->img[i % IMAGEX][line] = image[i];
    }
    
}

Image::Image() {
    int line = -1;
    for(int i = 0; i < (IMAGEY * IMAGEX); i++){
        if(i % IMAGEX == 0){line++;}
        this->img[i % IMAGEX][line] = 0;
    }
    
}

int Image::updatePixel(int x, int y, int value){
    this->img[x][y] = value;
    return 0;
}

int Image::getPixel(int x, int y) {
    return this->img[x][y];
}