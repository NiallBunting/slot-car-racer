/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   serialcommunicator.cpp
 * Author: nib28
 * 
 * Created on 20 March 2017, 17:03
 */

#include "serialcommunicator.h"
#include "carcontroller.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

Serialcommunicator::Serialcommunicator() {
}

int Serialcommunicator::init(){
    this->open_port();
    this->set_options(this->fd);
    
    //Set up so can be deleted
    this->voltage = new speeds;
    this->voltageblock = false;
    
    for(int i = 0; i < AMOUNTOFCARS; i++){
        this->cars[i] = new car;
        this->cars[i]->id = 'A';
        this->cars[i]->lastLapTime = 0;
        this->cars[i]->mode = 0;
        this->cars[i]->onTrack = 1;
        this->cars[i]->passedGate = 0;        
    }
    
    //TEMORARY
    this->cars[1]->mode = 1;
    this->cars[1]->id = 'B';

}

int Serialcommunicator::update(){
    
    for(;;){
        //Collect any data
        char buffer[20];
        int bufread = read(this->fd, &buffer, sizeof(buffer));
        if(bufread < 0){
            fputs("read failed.\n", stderr);
        }else if(bufread > 0){
            std::string readstring(buffer);
            this->parseBuffer(readstring);
        }
        //can read a not present and a gate passed
        
        //send any data       
        int pwm = this->getPwm();
        
        std::string towrite = "A,P," + std::to_string(pwm) + "\r";

        int bufwrite = write(this->fd, towrite.c_str(), towrite.length());
        //write mode, pwm and to gate
        
        if (bufwrite < 0){
            fputs("write failed.\n", stderr);
        }
    }

    return 0;
}

int Serialcommunicator::parseBuffer(std::string str){
    
    int start = -1;
    int end = -1;
    
    for(int cars = 0; cars < AMOUNTOFCARS; cars++){
        for(int i = 0; i < str.length(); i++){
            if(str.at(i) == this->cars[cars]->id){
                std::cout << "hellohere" << std::endl;
                start = i;
            }
            if(str.at(i) == '\r'){
                end = i;
                break;
            }
        }

        //check if have a full message
        if(start != -1 && end != -1){
            //If the car is detected off the track
            if(str.at(start + 2) == 'N'){
                this->cars[cars]->onTrack = 0;
            }

            //On completion of the track
            if(str.at(start + 2) == 'G'){
                this->cars[cars]->onTrack = 1;
                this->cars[cars]->passedGate;
            }
        }
    }
    
    return 0;
}

int Serialcommunicator::hasPassedGate(){
    int temp = this->passedGate;
    this->passedGate = 0;
    return temp;
}

int Serialcommunicator::isOnTrack(){
    return this->onTrack;
}

int Serialcommunicator::getPwm(){
    //block while in use
    while(this->voltageblock == true){}
    this->voltageblock = true;
    
    
    //check the time and divide to get interval
    std::chrono::time_point<clck> end = clck::now();
    int msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - this->time).count();
    msec /= this->voltage->time;
        
    int pwm = 0;
    
    //if passed the pwm just use the last speed
    if(msec >= 0 && msec <= this->voltage->size){
        pwm = this->voltage->speed[msec];
    }else{
        pwm = this->voltage->speed[this->voltage->size - 1];
    }
    
    this->voltageblock = false;
    
    return pwm;
}

int Serialcommunicator::setVoltage(speeds* s){
    //block while in use
    while(this->voltageblock == true){}
    this->voltageblock = true;
    
    //Deletes current one and sets to the new one. Restarts the clock.
    delete this->voltage;
    this->voltage = s;
    this->time = clck::now();
    
    this->voltageblock = false;
    
    return 0;
}

Serialcommunicator::~Serialcommunicator(){
    close(this->fd);
    delete this->cars[0];
}

int Serialcommunicator::open_port(){
      
    //"/dev/ttyACM0",
    //"/dev/ttyUSB0"
    this->fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->fd == -1){
        perror("Unable to open serial port.");
    }else{
        fcntl(this->fd, F_SETFL, 0);
    }

    return (this->fd);
}

int Serialcommunicator::set_options(int fd){   
    struct termios options;

    //get current options
    tcgetattr(fd, &options);

    // Set the baud rate 19200
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    //enable recieve and local mode
    options.c_cflag |= (CLOCAL | CREAD);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    //port options
    options.c_lflag |= (ICANON | ECHO | ECHOE);
    options.c_iflag |= (INPCK | ISTRIP);

    tcsetattr(fd, TCSANOW, &options);  
    
    fcntl(fd, F_SETFL, 0);
    
}