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
#include <pthread.h>
#include <cv.h>

const int BASESPEED = 125;
const int BOXSIZE = 30;
const int MAXSPEED = 255;
const int LEARNINGTESTS = 5; //The amount of times the car can come off

Carcontroller::Carcontroller() {
}

Carcontroller::~Carcontroller(){
}


int Carcontroller::init(Car* c, int columns, int rows){
    this->columns = columns;
    this->rows = rows;
    
    this->thisCar = c;
    
    this->boxes = new dead_reckon_interval[(columns / BOXSIZE) * (rows/ BOXSIZE)];
    
    for(int i = 0; i < ((columns / BOXSIZE) * (rows/ BOXSIZE)); i++){
        this->boxes[i].locked = 0;
        this->boxes[i].updateCount = 0;
        for(int j = 0; j < 5; j++){
            this->boxes[i].speed[j] = BASESPEED;
        }
    }
    
    this->safespeed = BASESPEED;
    this->safespeedset = 0;
    
    this->lastboxCount = 0;
    this->lastboxSet = 0;
}

int Carcontroller::update(cv::Point* point, cv::Mat& frame, mouseinteraction* mp){
    for(int x = 0; x < this->rows; x+=BOXSIZE){
        for(int y = 0; y < this->columns; y+=BOXSIZE){  

            int i = ((y/BOXSIZE) * (this->rows/BOXSIZE)) + x/BOXSIZE;
            int c = this->boxes[i].speed[0];

            if(c == BASESPEED || c == 0){continue;}

            c = (((double)c - (double)this->safespeed) / (255.0 - (double)this->safespeed)) * 255.0;

            if(this->boxes[i].locked > LEARNINGTESTS){
                cv::rectangle(
                frame,
                cv::Point(x, y),
                cv::Point(x + BOXSIZE, y + BOXSIZE),
                cv::Scalar(255, c, c)
                );
            }else{
                cv::rectangle(
                frame,
                cv::Point(x, y),
                cv::Point(x + BOXSIZE, y + BOXSIZE),
                cv::Scalar(c, c, c)
                );
            }
        }
    }
        
    this->update(point, mp);
}

int Carcontroller::update(cv::Point* point, mouseinteraction* mp){
        
    if(this->safespeedset == 0){
        
        if(this->thisCar->getInterval()->speed[0] == 0){
            this->setSpeed(BASESPEED);
        }
          
        //Drive around slowly increasing the speed
        if(this->thisCar->hasPassedGate()){
            this->safespeed+=3;
            this->setSpeed(this->safespeed);
        }
        //Learn track here as well -- in box set to safe speed
        
        //Keep going till come off track
        if(!this->thisCar->isOnTrack() || this->safespeed >= MAXSPEED){
            this->safespeed *= 0.90;
            this->safespeedset = 1;
            std::cout << "Final max speed: " << this->safespeed << std::endl;
        }
    }else{
        if(mp->mousePoint->x > 0 && mp->mousePoint->y > 0 && mp->button == 1){
            int mousei = ((mp->mousePoint->y/BOXSIZE)  * (this->rows/BOXSIZE)) + (mp->mousePoint->x/BOXSIZE);
            this->boxes[mousei].locked += 100;
        }
        
        
        //is off the track
        if(!this->thisCar->isOnTrack()){
            if(this->offTrack == 0){
                this->setSpeed(BASESPEED);
                this->lockBox();
                this->offTrack = 1;
            }
            return 0;
        }else{
            this->offTrack = 0;
        }
        
        //This means in the top corner
        if(point->x == 0 && point->y == 0){return 1;}
        
        int i = ((point->y/BOXSIZE)  * (this->rows/BOXSIZE)) + (point->x/BOXSIZE);
    
        //Check if the box is locked
        if(this->boxes[i].locked < LEARNINGTESTS){
            this->updateboxspeeds(&this->boxes[i]);
        }
        
        this->setSpeed(&this->boxes[i]);
        
        this->lastbox[this->lastboxCount] = &this->boxes[i];
        this->lastboxCount++;
        
        if(this->lastboxCount >= POINTHISTORY){
            this->lastboxCount = 0;
            this->lastboxSet = 1;
        }
        
    }
    
    return 0;
}

//locks the box and reduces the max
int Carcontroller::lockBox(){
    int location;
    
    if(this->lastboxSet == 0){return 0;}
    //loop through all the points
    for(int i = 0; i < POINTHISTORY; i++){
        location = i + this->lastboxCount;
        if(location >= POINTHISTORY){location -= 5;}
        
        //Counts back from the back, first also gets its first updated
        for(int j = this->lastbox[location]->size - 1; j >= i; j--){
            this->lastbox[location]->speed[j] *= 0.9 + (0.1 * i);
        }
        
        this->lastbox[location]->locked++;
    }
    return 0;
}

//Update box speeds
int Carcontroller::updateboxspeeds(dead_reckon_interval* s){
    
    s->speed[s->updateCount]++;
    s->updateCount++;

    //Reset increaser if reached size.
    if(s->updateCount >= s->size){
        s->updateCount = 0;
    }
    
    return 0;
}

//Sets speed taking a single value
int Carcontroller::setSpeed(int speed){
    
    //Create new struct
    dead_reckon_interval* volts = new dead_reckon_interval();
    
    //Copy speeds to voltage
    for(int i = 0; i < volts->size; i++){
        //Make sure over minimum speed
        if(this->safespeed > speed){
            volts->speed[i] = this->speedToVoltage(this->safespeed);
        }else{
            volts->speed[i] = this->speedToVoltage(speed);
        }
    }

    //Set dead reckoning
    this->thisCar->setInterval(volts);
}

//Sets the speed taking a dead reckon
int Carcontroller::setSpeed(dead_reckon_interval* s){
    
    //This has been set to be ignored
    if(s->locked == 2){return 0;}
    
    //Create new struct
    dead_reckon_interval* volts = new dead_reckon_interval();
    
    //Copy speeds to voltage
    for(int i = 0; i < s->size; i++){
        //Make sure over minimum speed
        if(this->safespeed > s->speed[i]){
            volts->speed[i] = this->speedToVoltage(this->safespeed);
            s->speed[i] = this->safespeed;
        }else{
            volts->speed[i] = this->speedToVoltage(s->speed[i]);
        }
    }

    //Set dead reckoning
    this->thisCar->setInterval(volts);
    delete volts;
    return 0;
}

//Should change the speed to a voltage value
int Carcontroller::speedToVoltage(int speed){
    //This needs to know the speed error can be calculated later.
    if(speed < 0){speed = 0;}
    if(speed > MAXSPEED){speed = MAXSPEED;}
    //PID controller
    return speed;
}

//Public allows the learning algorithm to start the car
int Carcontroller::setBaseSpeed(){
    this->setSpeed(BASESPEED);
    return 0;
}