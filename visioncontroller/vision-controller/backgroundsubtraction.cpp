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

// Which channel to use of the colour value
const int COLORCHANNEL = 2;

Backgroundsubtraction::Backgroundsubtraction(){}

int Backgroundsubtraction::init(int cols, int rows) {
    
    this->cols = cols;
    this->rows = rows;
    
    this->background = new BackgroundPixel[cols * rows];

    for(int i = 0; i < (cols * rows); i++){
        this->background[i].high = -1;
        this->background[i].low = -1;
        this->background[i].mask = 0;
    }
    
    return 0;
}

// This trains the background by increasing the min and max values for 
// a given pixel.
int Backgroundsubtraction::trainBackground(cv::Mat& frame){
    
    int x = 0;
    int y = 0;
    cv::Vec3b color;
    int colval;
    
    for(int i = 0; i < (frame.cols * frame.rows); i++){
        
        x = i % frame.cols;
        y = i / frame.cols;

        color = frame.at<cv::Vec3b>(cv::Point(x,y));
        colval = color.val[COLORCHANNEL];
        
        //Initialise on first input.
        if(this->background[i].low == -1 || this->background[i].high == -1){
            this->background[i].low = colval;
            this->background[i].high = colval;
        }
        
        //Lower that low increase
        if(this->background[i].low > colval){
            this->background[i].low = colval;
        }
        
        //Higher that high increase
        if(this->background[i].high < colval){
            this->background[i].high = colval;
        }
        
    }
    
    return 0;
}

// This creates the mask.
// 1 means the value is true and the value has changed.
int Backgroundsubtraction::maskCreate(cv::Mat& frame){
    
    int x = 0;
    int y = 0;
    cv::Vec3b color;
    int colval;
    
    for(int i = 0; i < (frame.cols * frame.rows); i++){
        
        x = i % frame.cols;
        y = i / frame.cols;

        color = frame.at<cv::Vec3b>(cv::Point(x,y));
        colval = color.val[COLORCHANNEL];
        
        if(this->background[i].high < (colval * 0.7) || this->background[i].low > (colval * 1.3) || this->background[i].mask == 1){
            this->background[i].mask = 1;
            color.val[0] = 255;
            color.val[1] = 255;
            color.val[2] = 255;
        }else{
            color.val[0] = colval;
            color.val[1] = colval;
            color.val[2] = colval;  
        }
        
        frame.at<cv::Vec3b>(cv::Point(x,y)) = color;
    }
    
    return 0;
}

//Checks if the pixel is in the mask and if value has changed.
int Backgroundsubtraction::hasPixelChanged(int x, int y, int value){
    
    int i = (y * this->rows) + x;
    
    if(this->background[i].mask == 1){
        if(this->background[i].high < value || this->background[i].low > value){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

int Backgroundsubtraction::hasPixelChanged(int x, int y, cv::Mat& frame){
        int color = frame.at<cv::Vec3b>(cv::Point(x,y)).val[COLORCHANNEL];
        return this->hasPixelChanged(x, y, color);
}

//allows the mouse to interact with the mask
int Backgroundsubtraction::maskMouse(cv::Point* point, int button){
    if(point->x < 5){
        point->x = 5;
    }
    if(point->y < 5){
        point->y = 5;
    }
    
    if(button == 1){ //Mouse click to add mask
        button = 1;
    }
    if(button == 2){ //Does the fill
        this->fillTrack(point);
        return 0;
    }
    
    int i;
    for(int x = point->x - 5; x < point->x + 5; x++){
        for(int y = point->y - 5; y < point->y + 5; y++){
            i = (y * this->rows) + x;

            this->background[i].mask = button;
        }
    }
    return 0;
}

//Removes a single point from the mask
int Backgroundsubtraction::removeMaskPoint(cv::Point* point){
    int i = (point->y * this->rows) + point->x;
    this->background[i].mask = 0;
    return 0;
}

//Does the fill mask and removes rest
int Backgroundsubtraction::fillTrack(cv::Point* point){
    //set mask to three to show it has changed
    for(int i = 0; i < (this->cols * this->rows); i++){
        if(this->background[i].mask == 1){
            this->background[i].mask = 2;
        }
    }
    
    this->floodFill(point->x, point->y);
    
    //remove all threes as not been touched
    for(int i = 0; i < (this->cols * this->rows); i++){
        if(this->background[i].mask == 2){
            this->background[i].mask = 0;
        }
    }
}

//Four way flood fill
int Backgroundsubtraction::floodFill(int x, int y){
    int i = (y * this->rows) + x;
    
    if(this->background[i].mask != 2){return 0;}
    
    this->background[i].mask = 1;
    
    this->floodFill(x+1, y);
    this->floodFill(x-1, y);
    this->floodFill(x, y+1);
    this->floodFill(x, y-1);
    
    return 0;
}