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

#include "slotmanager.h"
#include <cv.h> /* include openCv */
#include "serialcommunicator.h"
#include <chrono> /* Time related definions */
#include "car.h"

class Car;
struct mouseinteraction;
typedef std::chrono::high_resolution_clock clck;
int const POINTHISTORY = 5;

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
    int update(cv::Point* point, mouseinteraction* mp);
    int update(cv::Point* point, cv::Mat& frame, mouseinteraction*  mp);
    int setBaseSpeed();
private:
    int columns;
    int rows;
    
    Car* thisCar;
    
    //dead reckoning box
    dead_reckon_interval* boxes;
    int updateboxspeeds(dead_reckon_interval* s);
    
    int safespeed;
    int safespeedset;
    
    dead_reckon_interval* lastbox[POINTHISTORY];
    int lastboxCount;
    int lastboxSet;//Makes sure these have been set
    int offTrack;
    
    int setSpeed(int speed);
    int setSpeed(dead_reckon_interval* s);
    int speedToVoltage(int speed);
    int lockBox();
};

#endif /* CARCONTROLLER_H */

