/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   carcontroller.h
 * Author: nib28
 *
 * Created on 14 March 2017, 11:06
 */

#include <cv.h> /* include openCv */
#include "serialcommunicator.h"

#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

struct speeds{
    static const int size = 5;
    //1000 miliseconds. 1000/100 = 10 size of interval
    static const int time = 100;
    int speed[size];
    int increaser;
    int locked;
};

class Carcontroller {
public:
    Carcontroller();
    virtual ~Carcontroller();
    int init(int columns, int rows);
    int update(cv::Point point);
private:
    int columns;
    int rows;
    
    //dead reckoning box
    speeds* boxes;
    int updateboxspeeds(speeds* s);
    
    int safespeed;
    int safespeedset;
    speeds* lastbox;
    
    int setSpeed(int speed);
    int setSpeed(speeds* s);
    int speedToVoltage(int speed);
    
    //serial communicator
    Serialcommunicator* sc;
    pthread_t serialThread;
};

#endif /* CARCONTROLLER_H */

