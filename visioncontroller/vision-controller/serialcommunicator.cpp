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
    
    this->carCounter = 0;
    
    this->quitvalue = 0;
    
}

int Serialcommunicator::addCar(Car* c){
    this->cars[this->carCounter] = c;
    this->carCounter++;
}

int Serialcommunicator::update(){
    
    for(;;){        
        //Collect any data
        char buffer[50];
        int bufread = read(this->fd, &buffer, sizeof(buffer));
        if(bufread < 0){
            fputs("read failed.\n", stderr);
        }else if(bufread > 0){
            std::string readstring(buffer);
            this->parseBuffer(readstring);
        }
        
        for(int i = 0; i < this->carCounter; i++){
            int pwm = this->getPwm(this->cars[i]);
            
            if(pwm != this->cars[i]->getCurrentSpeed()){
                this->cars[i]->setCurrentSpeed(pwm);
                
                std::string carIdChar(1, this->cars[i]->getId());
                std::string towrite = carIdChar + ",P," + std::to_string(pwm) + '\n';
                
                int bufwrite = write(this->fd, towrite.c_str(), towrite.length());
                
                if (bufwrite < 0){
                    fputs("write failed.\n", stderr);
                }
            }

        }
        if(this->quitvalue){
            return 0;
        }
    }

    return 0;
}

int Serialcommunicator::parseBuffer(std::string str){
    
    int start = -1;
    int end = -1;

    for(int cars = 0; cars < this->carCounter; cars++){
        for(int i = 0; i < str.length(); i++){
            if(str.at(i) == this->cars[cars]->getId()){
                start = i;
            }
            if(str.at(i) == '\n'){
                end = i;
                break;
            }
        }
        std::cout << "first: " << start << " end:" << end << " char: " <<  str.at(start + 2) << std::endl;
        //check if have a full message
        if(start != -1 && end != -1){
            //If the car is detected off the track
            if(str.at(start + 2) == 'N'){
                this->cars[cars]->offTrack();
            }

            //On completion of the track
            if(str.at(start + 2) == 'G'){
                this->cars[cars]->gatePassed();
            }
        }
    }
    
    return 0;
}

/*
int Serialcommunicator::hasPassedGate(char car){
    for(int cars = 0; cars < AMOUNTOFCARS; cars++){
        if(this->cars[cars]->id == car){
            int temp = this->cars[cars]->passedGate;
            this->cars[cars]->passedGate = 0;
            return temp;
        }
    }    

    return 0;
}

int Serialcommunicator::isOnTrack(char car){
    for(int cars = 0; cars < AMOUNTOFCARS; cars++){
        if(this->cars[cars]->id == car){
            return this->cars[cars]->onTrack;
        }
    }    

    return 0;
}
*/
int Serialcommunicator::getPwm(Car* c){
 
    //check the time and divide to get interval
    //std::chrono::time_point<clck> end = clck::now();
    //int msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - this->time).count();
    //msec /= this->voltage->time;
        
    //int pwm = 0;
    
    //if passed the pwm just use the last speed
    //if(msec >= 0 && msec <= this->voltage->size){
    //    pwm = this->voltage->speed[msec];
    //}else{
    //    pwm = this->voltage->speed[this->voltage->size - 1];
    //}
    
    //this->voltageblock = false;
    
    return 100;
}


Serialcommunicator::~Serialcommunicator(){
    close(this->fd);
}

int Serialcommunicator::open_port(){
      
    //"/dev/ttyACM0",
    //"/dev/ttyUSB0"
    this->fd = open("/dev/ttyACM0", O_NONBLOCK | O_NDELAY | O_RDWR | O_NOCTTY );
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

int Serialcommunicator::quit(){
    this->quitvalue = 1;
    return 0;
}