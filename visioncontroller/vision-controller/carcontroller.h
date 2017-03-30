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

#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

#include <cv.h> /* include openCv */
#include "serialcommunicator.h"
#include <chrono> /* Time related definions */
#include "car.h"

class Car;

typedef std::chrono::high_resolution_clock clck;

struct dead_reckon_interval{
    static const int size = 5;//The size for the array
    //1000 miliseconds. 1000/100 = 10 size of interval
    static const int time = 100;//The amount of time each is
    int speed[size];//speeds for the next time * size
    int updateCount;// counts which values needs to be updated
    int locked;
    std::chrono::time_point<clck> timeStamp;
};

class Carcontroller {
public:
    Carcontroller();
    virtual ~Carcontroller();
    int init(Car* c, int columns, int rows);
    int update(cv::Point* point);
private:
    int columns;
    int rows;
    
    Car* thisCar;
    
    //dead reckoning box
    dead_reckon_interval* boxes;
    int updateboxspeeds(dead_reckon_interval* s);
    
    int safespeed;
    int safespeedset;
    dead_reckon_interval* lastbox;
    
    int setSpeed(int speed);
    int setSpeed(dead_reckon_interval* s);
    int speedToVoltage(int speed);
};

#endif /* CARCONTROLLER_H */

