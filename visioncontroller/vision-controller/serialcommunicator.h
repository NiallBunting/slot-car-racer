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

struct speeds;

class Serialcommunicator {
public:
    Serialcommunicator();
    virtual ~Serialcommunicator();
    int init();
    int setVoltage(speeds* s);
    int setVoltage(int s);
    int update();
    int open_port();
    int set_options(int fd);
private:
    speeds* voltage;
    int time;
    
    int fd;
};

#endif /* SERIALCOMMUNICATOR_H */

