/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   backgroundsubtraction.h
 * Author: nib28
 *
 * Created on 08 February 2017, 13:53
 */

#ifndef BACKGROUNDSUBTRACTION_H
#define BACKGROUNDSUBTRACTION_H

#include "image.h"
#include "common.h"

struct BackgroundPixel{
    int high;
    int low;
};

class Backgroundsubtraction {
public:
    Backgroundsubtraction();
    //Trains the backround on the images.
    int trainBackground(Image* img);
    //This checks each pixel if within bounds.
    Image* doFullBackSubtract(Image* img);
    //Spread only checks the value every so many pixels
    //and only then fills in between the pixels if match
    Image* doSparseBackSubtract(Image* img, int spread);
private:
    BackgroundPixel* background[IMAGEX][IMAGEY];
    int boundCheck(int x, int y, int value);
};

#endif /* BACKGROUNDSUBTRACTION_H */

