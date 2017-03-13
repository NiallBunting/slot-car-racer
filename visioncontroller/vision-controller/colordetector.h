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

#include <cv.h>

//How many bins there are for the colours.
int const BINCOUNT = 51;

class Colordetector {
public:
    Colordetector();
    int learnColor(cv::Mat& frame, int x1, int x2, int y1, int y2);
    double matchColor(cv::Mat& frame, int x1, int x2, int y1, int y2);
private:
    //255 / 5 - 5 in each bin
    double distribution[BINCOUNT];
    double temp[BINCOUNT];
    int binNum;
    
    int createDistribution(cv::Mat& frame, int x1, int x2, int y1, int y2);

};

 
#endif /* COLORDETECTOR_H */

