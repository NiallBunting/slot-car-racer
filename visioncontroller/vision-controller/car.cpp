/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   car.cpp
 * Author: Niall Bunting
 * 
 * Created on 30 March 2017, 16:29
 */

#include "car.h"

Car::Car(char id, int mode, int rows, int cols) {
    this->locked = 0;

    this->id = id;
    this->mode = mode;
    
    this->lastLapTime = 0;
    this->onTrack = 1;
    this->passedGate = 0;
    this->currentSpeed = 0;
    
    this->deadReckonInterval = new dead_reckon_interval();
    this->deadReckonInterval->timeStamp = clck::now();
    for(int i = 0; i < this->deadReckonInterval->size; i++){
        this->deadReckonInterval->speed[i] = 0;
    }
    this->deadReckonInterval->locked = 0;
    this->deadReckonInterval->updateCount = 0;

    //Set this up if computer
    if(mode == 1){
        //A car detector
        this->carDetector = new Cardetector();
        this->carDetector->init(cols, rows);

        //Create the car controller.
        this->carController = new Carcontroller();
        this->carController->init(this, cols, rows);
    }
    
}


Car::~Car() {
}

Carcontroller* Car::getCarController(){
    return this->carController;
}

Cardetector* Car::getCarDetector(){
    return this->carDetector;
}

dead_reckon_interval* Car::getInterval(){
    while(this->locked > 0){} //Spin lock
    return this->deadReckonInterval;
}

int Car::setInterval(dead_reckon_interval* di){
    while(this->locked > 0){} // Spin lock
    this->locked++;  
    
    this->deadReckonInterval->timeStamp = clck::now();
    
    for(int i = 0; i < this->deadReckonInterval->size; i++){
        this->deadReckonInterval->speed[i] = di->speed[i];
    }
    
    this->locked--;
    return 0;
}

char Car::getId(){
    return this->id;
}

int Car::getCurrentSpeed(){
    return this->currentSpeed;
}

int Car::isOnTrack(){
    return this->onTrack;
}

int Car::hasPassedGate(){
    int temp = this->passedGate;
    this->passedGate = 0;
    return temp;
}

int Car::setCurrentSpeed(int speed){
    this->currentSpeed = speed;
    return 0;
}

int Car::gatePassed(){
    this->passedGate++;
    this->onTrack = 1;
    return 0;
}

int Car::offTrack(){
    if(this->onTrack != 0){
        std::cout << this->id << ": Is off the track." << std::endl;
    }
    this->onTrack = 0;
    return 0;
}

int Car::getMode(){
    return this->mode;
}