/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   serialcommunicator.h
 * Author: nib28
 *
 * Created on 20 March 2017, 17:03
 */

#ifndef SERIALCOMMUNICATOR_H
#define SERIALCOMMUNICATOR_H

using namespace std;
#include <string> /* String definions */
#include <chrono> /* Time related definions */

class Car;
typedef std::chrono::high_resolution_clock clck;

const int MAXAMOUNTOFCARS = 2;

class Serialcommunicator {
public:
    Serialcommunicator();
    virtual ~Serialcommunicator();
    int init();
    int getPwm(Car* c);
    int update();
    int addCar(Car* c);
    int quit();
private:
    //set up the serial connection
    int open_port();
    int set_options(int fd);
    int fd;
    
    //parse the text received
    int parseBuffer(string str); 
    int sendData(char id, string str);
    
    //Have to know what cars to update
    Car* cars[MAXAMOUNTOFCARS];
    int carCounter;
    
    //change this value to quit
    int quitvalue;
    
    //last sent
    std::chrono::time_point<clck> timeStamp;
};

#endif /* SERIALCOMMUNICATOR_H */

