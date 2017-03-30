/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cardetector.h
 * Author: nib28
 *
 * Created on 30 March 2017, 11:46
 */

#ifndef CARDETECTOR_H
#define CARDETECTOR_H

#include "backgroundsubtraction.h"
#include "colordetector.h"
#include <cv.h>

const int BOXLENGTH = 6;
const int BOXJUMP = 7;

void CallBackFunc(int event, int x, int y, int flags, void* userdata);

class Cardetector {
public:
    Cardetector();
    int init(int cols, int rows);
    virtual ~Cardetector();
    
    int hasLearnt();
    int learn(cv::Mat& frame, int key, cv::Point* click);
    cv::Point* detect(cv::Mat& frame);
    
    int toggleOverlay();
private:
    Backgroundsubtraction* bs;
    Colordetector* cd;
    
    int learnStage;
    
    //Holds the colours when detecting.
    cv::Mat colFrame;
    
    cv::Point* colorBox[2];
    int colourBoxCount;
    
    int learnFinished;
    
    int overlay;
};

#endif /* CARDETECTOR_H */

