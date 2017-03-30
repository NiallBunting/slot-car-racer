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

const int BASESPEED = 150;
const int BOXSIZE = 30;

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
        
    //set an inital speed for learning
    this->setSpeed(BASESPEED);
}

int Carcontroller::update(cv::Point* point){
    
    //is off the track
    if(!this->thisCar->isOnTrack()){
        this->setSpeed(BASESPEED);
    }
    
    if(this->safespeedset == 0){
          
        //Drive around slowly increasing the speed
        if(this->thisCar->hasPassedGate()){
            this->safespeed++;
            this->setSpeed(this->safespeed);
        }
        //Learn track here as well -- in box set to safe speed
        
        //Keep going till come off track
        if(!this->thisCar->isOnTrack()){
            this->safespeed *= 0.97;
            this->safespeedset = 1;
            std::cout << "Final max speed: " << this->safespeed << std::endl;
        }
    }else{

        int i = ((point->y/BOXSIZE)  * (this->columns/BOXSIZE)) + (point->x/BOXSIZE);
    
        //Check if the box is locked
        if(this->boxes[i].locked == 0){
            this->updateboxspeeds(&this->boxes[i]);
            this->setSpeed(&this->boxes[i]);
            this->lastbox = &this->boxes[i];
        }else{
            this->setSpeed(&this->boxes[i]);
        }
        
    }
    
    return 0;
}

int Carcontroller::updateboxspeeds(dead_reckon_interval* s){
    
    if(s->locked == 1){return 1;}
    
    //Increase speeds
    s->speed[s->updateCount]++;
    s->updateCount++;
    
    //Reset increaser if reached size.
    if(s->updateCount >= s->size){
        s->updateCount = 0;
    }
    
    return 0;
}

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

int Carcontroller::setSpeed(dead_reckon_interval* s){
    
    //Create new struct
    dead_reckon_interval* volts = new dead_reckon_interval();
    
    //Copy speeds to voltage
    for(int i = 0; i < s->size; i++){
        //Make sure over minimum speed
        if(this->safespeed > s->speed[i]){
            volts->speed[i] = this->speedToVoltage(this->safespeed);
        }else{
            volts->speed[i] = this->speedToVoltage(s->speed[i]);
        }
    }

    //Set dead reckoning
    this->thisCar->setInterval(volts);
}

int Carcontroller::speedToVoltage(int speed){
    //This needs to know the speed error can be calculated later.    
    //PID controller
    return speed;
}