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

#include <string> /* String definions */
using namespace std;
#include <chrono> /* Time related definions */

typedef std::chrono::high_resolution_clock clck;

struct speeds;

struct car{
    int passedGate;
    int onTrack;
    int lastLapTime;
    int currentSpeed;
    int mode;
    char id;
};

const int AMOUNTOFCARS;

class Serialcommunicator {
public:
    Serialcommunicator();
    virtual ~Serialcommunicator();
    int init();
    int setVoltage(speeds* s);
    int getPwm();
    int update();
    int open_port();
    int set_options(int fd);
    int parseBuffer(string str);
    int hasPassedGate();
    int isOnTrack();
private:
    speeds* voltage;
    std::chrono::time_point<clck> time;
    bool voltageblock;
    
    int fd;
    
    car* cars[AMOUNTOFCARS];

};

#endif /* SERIALCOMMUNICATOR_H */

