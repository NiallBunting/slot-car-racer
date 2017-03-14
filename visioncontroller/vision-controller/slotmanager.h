/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   slotmanager.h
 * Author: nib28
 *
 * Created on 10 March 2017, 13:38
 */

#ifndef SLOTMANAGER_H
#define SLOTMANAGER_H

#include <cv.h>
#include <highgui.h>
#include "backgroundsubtraction.h"
#include "colordetector.h"

class Slotmanager {
public:
    Slotmanager();
    int init(Slotmanager* sm);
    //Called each update.
    int update();
    int mouseClick(int x, int y);
private:
    int capture(cv::Mat& frame);
    //Size of image.
    int cols;
    int rows;
    cv::VideoCapture* cap;
    //Which stage of the process the program is on.
    int action;
    Backgroundsubtraction* bs;
    //For selecting the car. x1, x2, y1, y2
    int mouseselect[4];
    int mousepoint;
    Colordetector* cd;
    //Frame for the colour
    cv::Mat colframe;
    cv::Point Match(cv::Mat& frame);
};

#endif /* SLOTMANAGER_H */

