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
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

Serialcommunicator::Serialcommunicator() {
}

int Serialcommunicator::init(){
    this->open_port();
    this->set_options(this->fd);
    
}

int Serialcommunicator::update(){

    //int n = write(this->fd, "ATZ\r", 4);
    //if (n < 0)
    //    fputs("write() of 4 bytes failed!\n", stderr);
    
    char buffer[20];
    int length;
    
    length = read(this->fd, &buffer, sizeof(buffer));
    
    std::cout << buffer << std::endl;
    //for(;;){
    
    //get time
    //if time has elapsed do the next thing - if zero just do speed
    
    //send data
    //receive data
    
    //}
    
    return 0;
}

int Serialcommunicator::setVoltage(speeds* s){
    this->voltage = s;
    this->time = 0;//TODO SET THE TIME HERE
}

int Serialcommunicator::setVoltage(int s){
    this->voltage = 0;
    this->time = 0;
}

Serialcommunicator::~Serialcommunicator(){
    close(this->fd);
}

int Serialcommunicator::open_port(){
      
  /* File descriptor for the port */


  this->fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (this->fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open /dev/ttyACM0 - ");
  }
  else
    fcntl(this->fd, F_SETFL, 0);

  return (this->fd);
}

int Serialcommunicator::set_options(int fd){
    
struct termios options;

/*
 * Get the current options for the port...
 */

tcgetattr(fd, &options);

/*
 * Set the baud rates to 19200...
 */

cfsetispeed(&options, B115200);
cfsetospeed(&options, B115200);

/*
 * Enable the receiver and set local mode...
 */

options.c_cflag |= (CLOCAL | CREAD);

options.c_cflag &= ~PARENB;
options.c_cflag &= ~CSTOPB;
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8;
/*
 * Set the new options for the port...
 */

options.c_lflag |= (ICANON | ECHO | ECHOE);
options.c_iflag |= (INPCK | ISTRIP);

tcsetattr(fd, TCSANOW, &options);  
    
fcntl(fd, F_SETFL, 0);
    
}