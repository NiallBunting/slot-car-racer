/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   colordetector.cpp
 * Author: nib28
 * 
 * Created on 18 February 2017, 15:02
 */

#include "colordetector.h"

const int COLORCHANNEL = 0;

Colordetector::Colordetector() {         
}

//Learns the colours selected.
int Colordetector::learnColor(cv::Mat& frame, int x1, int x2, int y1, int y2){
    this->createDistribution(frame, x1, x2, y1, y2);
    
    for(int i = 0; i < BINCOUNT; i++){
        this->distribution[i] = this->temp[i];
    }
    
    for(int x = 0; x < frame.cols; x++){
        for(int y = 0; y < frame.rows; y++){
               cv::Vec3b color = frame.at<cv::Vec3b>(cv::Point(x,y));

               int colval = color.val[COLORCHANNEL];

               color.val[0] = colval;
               color.val[1] = colval;
               color.val[2] = colval;  

               frame.at<cv::Vec3b>(cv::Point(x,y)) = color;   
        }
    }

}

//Matches the colours to the previous selected colours.
//Compares with chi squared.
// Lower values are better.
double Colordetector::matchColor(cv::Mat& frame, int x1, int x2, int y1, int y2){
    if(this->createDistribution(frame, x1, x2, y1, y2) == 1){
        return 5.0;
    }
    
    double match = 0.0;
    
    for(int i = 0; i < BINCOUNT; i++){
        if(this->distribution[i] == 0 && this->temp[i] == 0){continue;}
        match += ((this->temp[i] - this->distribution[i]) * (this->temp[i] - this->distribution[i])) / (this->distribution[i] + this->temp[i]);
    }
    
    return match;
}

//Creates the distribution and sorts into bins.
int Colordetector::createDistribution(cv::Mat& frame, int x1, int x2, int y1, int y2){
    int count[BINCOUNT];
    for(int i = 0; i < BINCOUNT; i++){count[i] = 0;}
    
    int area = (x1 - x2) * (y1 - y2);
    
    cv::Vec3b color;
    
    for(int x = x1; x < x2; x++){
        for(int y = y1; y < y2; y++){
            color = frame.at<cv::Vec3b>(cv::Point(x,y));
            
            count[color.val[COLORCHANNEL] / 5]++;
        }
    }
    
    for(int i = 0; i < BINCOUNT; i++){
        this->temp[i] = (double)count[i] / (double)area;
    }
    
    return 0;
}