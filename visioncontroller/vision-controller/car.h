/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   car.h
 * Author: Niall Bunting
 *
 * Created on 30 March 2017, 16:29
 */

#ifndef CAR_H
#define CAR_H

#include "carcontroller.h"
#include "cardetector.h"
#include <chrono> /* Time related definions */

class Carcontroller;
class Cardetector;
struct dead_reckon_interval;
typedef std::chrono::high_resolution_clock clck;


class Car {
public:
    Car(char id, int mode, int rows, int cols);
    virtual ~Car();
    Carcontroller* getCarController();
    Cardetector* getCarDetector();
    dead_reckon_interval* getInterval();
    int setInterval(dead_reckon_interval* di);
    char getId();
    int getCurrentSpeed();
    int isOnTrack();
    int hasPassedGate();
    int setCurrentSpeed(int speed);
    int offTrack();
    int gatePassed();
    int getMode();
private:
    char id;//Char value used for communication
    int passedGate;
    int onTrack;
    int lastLapTime; //Lap times
    int currentSpeed; //Know the current speed
    int mode;// Computer or human
    Carcontroller* carController;
    Cardetector* carDetector;
    dead_reckon_interval* deadReckonInterval;// Future speeds
    
    int locked;
};

#endif /* CAR_H */

