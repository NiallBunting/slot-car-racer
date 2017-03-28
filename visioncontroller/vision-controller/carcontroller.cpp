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
const int BOXSIZE = 10;

static void* serialCommunicatorWrap(void* arg) {
    ((Serialcommunicator*) arg)->update();
    return 0;
}

Carcontroller::Carcontroller() {
}

Carcontroller::~Carcontroller(){
    pthread_join( this->serialThread, NULL);
    delete this->sc;
}


int Carcontroller::init(int columns, int rows){
    this->columns = columns;
    this->rows = rows;
    
    this->boxes = new speeds[columns * rows];
    
    for(int i = 0; i < columns * rows; i++){
        this->boxes[i].locked = 0;
        this->boxes[i].increaser = 0;
        for(int j = 0; j < 5; j++){
            this->boxes[i].speed[j] = BASESPEED;
        }
    }
    
    this->safespeed = BASESPEED;
    this->safespeedset = 0;
    
    this->sc = new Serialcommunicator();
    this->sc->init();
    
    int iret1 = pthread_create( &this->serialThread, NULL, &serialCommunicatorWrap, this->sc);
    if(iret1){
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }
    
    //set an inital speed for learning
    this->setSpeed(BASESPEED);
}

int Carcontroller::update(cv::Point point){
    
    //is off the track
    if(!this->sc->isOnTrack()){
        // lock last box
    }


    if(this->safespeedset == 0){
    
        //Drive around slowly increasing the speed
        if(this->sc->hasPassedGate()){
            this->safespeed++;
            std::cout << "Safespeed is now: " << this->safespeed << std::endl;
            this->setSpeed(this->safespeed);
        }
        //Learn track here as well -- in box set to safe speed
        
        //Keep going till come off track
        if(!this->sc->isOnTrack()){
            this->safespeed *= 0.97;
            this->safespeedset = 1;
        }
    }else{
        //throw error
        int j = 0;
        int a = 1 / j;
        
        int i = (point.y * this->columns) + point.x;
    
        //if(//inabox){
        this->updateboxspeeds(&this->boxes[i]);
        this->setSpeed(&this->boxes[i]);
        //}else
        //this->setSpeed(this->safespeed)
        //}
        
        //work out current speed of car and what its meant to be doing
    
        this->lastbox = &this->boxes[i];
    }
    
    return 0;
}

int Carcontroller::updateboxspeeds(speeds* s){
    
    if(s->locked == 1){return 1;}
    
    //Increase speeds
    s->speed[s->increaser]++;
    s->increaser++;
    
    //Reset increaser if reached size.
    if(s->increaser >= s->size){
        s->increaser = 0;
    }
    
    return 0;
}

int Carcontroller::setSpeed(int speed){
    
    //Create new struct
    speeds* volts = new speeds();
    volts->locked = 0;
    volts->increaser = 0;
    
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
    this->sc->setVoltage(volts);
}

int Carcontroller::setSpeed(speeds* s){
    
    //Create new struct
    speeds* volts = new speeds();
    volts->locked = 0;
    volts->increaser = 0;
    
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
    this->sc->setVoltage(volts);
}

int Carcontroller::speedToVoltage(int speed){
    //This needs to know the speed error can be calculated later.    
    //PID controller
    return speed;
}