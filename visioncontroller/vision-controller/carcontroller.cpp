/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   carcontroller.cpp
 * Author: nib28
 * 
 * Created on 14 March 2017, 11:06
 */

#include "carcontroller.h"

Carcontroller::Carcontroller() {
}

const int BOXSIZE = 10;

int Carcontroller::init(int columns, int rows){
    this->columns = columns;
    this->rows = rows;
    
    this->boxes = new speeds[columns * rows];
    
    this->safespeed = 100;
    this->safespeedset = 0;
}

int Carcontroller::update(cv::Point point){
    
    //is off?
        // lock last box
        // safespeedset = 0 then set all the minimum speeds to .90 of that

    if(safespeedset == 0){
        //Drive around slowly increasing the speed
        //Learn track here as well
        //Keep going till come off track
    }
    
    int i = (point.y * this->columns) + point.x;
    
    //Tell serial port the new speeds.
    //Adjust for estimated speed
    
    this->updateboxspeeds(&this->boxes[i]);
    
    
    this->lastbox = &this->boxes[i];
    
    return 0;
}

int Carcontroller::updateboxspeeds(speeds* s){
    
    if(s->locked == 1){return 1;}
    
    if(s->i0 > s->i1){s->i0++;}
    else if(s->i1 > s->i2){s->i1++;}
    else if(s->i2 > s->i4){s->i2++;}
    else if(s->i4 > s->i6){s->i4++;}
    else if(s->i6 > s->i8){s->i6++;}
    else {s->i8++;}
    
    return 0;
}