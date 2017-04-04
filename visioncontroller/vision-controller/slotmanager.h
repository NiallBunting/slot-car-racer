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
#include "carcontroller.h"
#include "cardetector.h"
#include "serialcommunicator.h"

class Carcontroller;
struct dead_reckon_interval;
class Serialcommunicator;

struct mouseinteraction{
    cv::Point* mousePoint;
    int button;
};

class Slotmanager {
public:
    Slotmanager();
    virtual ~Slotmanager();
    int init(Slotmanager* sm);
    //Called each update.
    int update();
    int mouseClick(int x, int y, int button);
private:
    int capture(cv::Mat& frame);
    mouseinteraction* getMouseClick();
    
    //Size of image.
    int cols;
    int rows;
    cv::VideoCapture* cap;
    
    //User interaction values
    mouseinteraction* mousePress;
    int keyPress;
    
    //serial communicator
    Serialcommunicator* serialCom;
    pthread_t serialThread;
    
    Car* computerCar;
    Car* humanCar;
};

#endif /* SLOTMANAGER_H */

