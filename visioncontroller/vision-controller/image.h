/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   image.h
 * Author: nib28
 *
 * Created on 08 February 2017, 13:55
 */

#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"

class Image {
public:
    Image();
    Image(int image[]);
    
    int getPixel(int x, int y);
    
    int updatePixel(int x, int y, int value);
private:
    int img[IMAGEX][IMAGEY];
};

#endif /* IMAGE_H */

