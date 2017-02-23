/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   colordetector.h
 * Author: nib28
 *
 * Created on 18 February 2017, 15:02
 */

#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include "image.h"
#include "common.h"


class Colordetector {
public:
    Colordetector();
    int learnColor(Image* img, int x1, int x2, int y1, int y2);
    double matchColor(Image* img, int x1, int x2, int y1, int y2);
private:
    //255 / 5 - 5 in each bin
    double distribution[51];
    double temp[51];
    int binNum;
    
    int createDistribution(Image* img, int x1, int x2, int y1, int y2);

};

#endif /* COLORDETECTOR_H */

