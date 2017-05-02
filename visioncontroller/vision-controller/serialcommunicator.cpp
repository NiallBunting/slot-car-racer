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

//Sets up the serial connection.
int Serialcommunicator::init(){
    this->open_port();
    this->set_options(this->fd);
    
    this->carCounter = 0;
    
    this->quitvalue = 0;
    
    sleep(3);
    this->timeStamp = clck::now();
    
    return 0;
    
}

//Adds a car to the list and sets the respective mode of the car.
int Serialcommunicator::addCar(Car* c){
    this->cars[this->carCounter] = c;
    this->carCounter++;
    
    //Sets the mode of the car
    this->sendData(c->getId(), std::string("M," + std::to_string(c->getMode())));
    return 0;
}

//The main loop of the serial communicator.
int Serialcommunicator::update(){
    
    for(;;){        
        //Read any data
        char buffer[50];
        int bufread = read(this->fd, &buffer, sizeof(buffer));
        if(bufread < 0){
            fputs("read failed.\n", stderr);
        }else if(bufread > 0){
            std::string readstring(buffer);
            this->parseBuffer(readstring);
        }
        
        //Send any data for each car
        for(int i = 0; i < this->carCounter; i++){
            if(this->cars[i]->getMode() == 1){
                int pwm = this->getPwm(this->cars[i]);

                if(pwm != this->cars[i]->getCurrentSpeed()){
                    this->cars[i]->setCurrentSpeed(pwm);

                    this->sendData(this->cars[i]->getId(), std::string("P," + std::to_string(pwm)));
                }
            }

        }
        
        //Updated if the program has ended.
        if(this->quitvalue){
            return 0;
        }
    }

    return 0;
}

//Sends the data to the arduino.
//Takes a car id and a string to send.
int Serialcommunicator::sendData(char id, std::string str){
    //Create message
    std::string carIdChar(1, id);
    std::string towrite = carIdChar + "," + str + "\n";
    
    std::cout << "<< " << towrite.c_str() << std::endl;    
    int msec = 0;
    
    //Wait 20 millisecs if updating too fast
    while(msec < 20){
        std::chrono::time_point<clck> now = clck::now();
        msec = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->timeStamp).count();
    }

    //Write the data
    int bufwrite = write(this->fd, towrite.c_str(), towrite.length());
    if (bufwrite < 0){
        fputs("write failed.\n", stderr);
    }
    this->timeStamp = clck::now();
    
    return 0;
}

//Parse what is returned from the serial connection.
int Serialcommunicator::parseBuffer(std::string str){
    
    int start = -1;
    int end = -1;

    //For each car.
    for(int cars = 0; cars < this->carCounter; cars++){
        for(int i = 0; i < str.length(); i++){
            //Check if starts with the char and ends with NL
            if(str.at(i) == this->cars[cars]->getId()){
                start = i;
            }
            if(str.at(i) == '\n'){
                end = i;
                break;
            }
        }

        //check if have a full message
        if(start != -1 && end != -1){
                        
            //If the car is detected off the track
            if(str.at(start + 2) == 'N'){
                this->cars[cars]->offTrack();
            }

            //On completion of the track
            if(str.at(start + 2) == 'G'){
                for(int i = start; i < end; i++){
                std::cout << str.at(i);
                }
                std::cout<< std::endl;
                this->cars[cars]->gatePassed();
            }
        }
    }
    
    return 0;
}

//Gets the pwm value the car should be using
int Serialcommunicator::getPwm(Car* c){
    
    //check the time and divide to get interval
    std::chrono::time_point<clck> end = clck::now();
    int msec = std::chrono::duration_cast<std::chrono::milliseconds>(end - c->getInterval()->timeStamp).count();
    msec /= c->getInterval()->time;
        
    int pwm = 0;
    
    //if passed the pwm just use the last speed
    if(msec >= 0 && msec < c->getInterval()->size){
        pwm = c->getInterval()->speed[msec];
    }else{
        pwm = c->getInterval()->speed[c->getInterval()->size - 1];
    }
    
    return pwm;
}


Serialcommunicator::~Serialcommunicator(){
    //Stop all cars
    for(int i = 0; i < this->carCounter; i++){
        this->sendData(this->cars[i]->getId(), std::string("P,0"));
    }
    
    close(this->fd);
}

//Opens the port and returns the file descriptor.
int Serialcommunicator::open_port(){
    //ttyACM0 is the default on mint.
    this->fd = open("/dev/ttyACM0",  O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->fd == -1){
        perror("Unable to open serial port.");
    }

    return (this->fd);
}

//Sets all the various arduino options
int Serialcommunicator::set_options(int fd){   
    struct termios options;

    //get current options
    tcgetattr(fd, &options);

    // Set the baud rate 19200
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    //enable recieve and local mode
    options.c_cflag |= (CLOCAL | CREAD);
    
    //Set the size of the byte
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_iflag &= ~(IXON | ICRNL);//Disable software flow control and disable map CR to NL
    options.c_oflag &= ~OPOST;//Raw output
    options.c_lflag &= ~IEXTEN;//Disable extended functions
    
    //port options;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | ECHOK | ECHOCTL | ECHOKE);//Switch off echoing
    options.c_iflag &= ~(INPCK | ISTRIP); //Switch off parity checks
    
    options.c_cc[VMIN]  = 0; //Minimum of chars. This means it does not block.

    tcsetattr(fd, TCSANOW, &options); //Set these options.
    
    fcntl(fd, F_SETFL, 0);
       
}

int Serialcommunicator::quit(){
    this->quitvalue = 1;
    return 0;
}