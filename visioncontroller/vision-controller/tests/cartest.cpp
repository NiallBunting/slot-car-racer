/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newsimpletest.cpp
 * Author: Niall Bunting
 *
 * Created on 04 April 2017, 13:50
 */

#include <stdlib.h>
#include <iostream>

#include "car.h"

/*
 * Car tests
 */

Car* testCarCreate() {
    std::cout << "newsimpletest carcreate" << std::endl;
    return new Car('H', 1, 20, 20);
}

void testGetId(Car* car) {
    std::cout << "newsimpletest getid" << std::endl;
    if(car->getId() != 'H'){
        std::cout << "%TEST_FAILED% time=0 testname=getid (newsimpletest) message=Id not as expected" << std::endl;
    }
}

void testGetMode(Car* car) {
    std::cout << "newsimpletest getmode" << std::endl;
    if(car->getMode() != 1){
        std::cout << "%TEST_FAILED% time=0 testname=getmode (newsimpletest) message=Mode not as expected" << std::endl;
    }
}

void testGetSpeed(Car* car, int speed) {
    std::cout << "newsimpletest getspeed" << std::endl;
    if(car->getCurrentSpeed() != speed){
        std::cout << "%TEST_FAILED% time=0 testname=getid (newsimpletest) message=Id not as expected" << std::endl;
    }
}

void testGetOnTrack(Car* car, int expected) {
    std::cout << "newsimpletest getontrack" << std::endl;
    if(car->isOnTrack() != expected){
        std::cout << "%TEST_FAILED% time=0 testname=getontrack (newsimpletest) message=On track not as expected." << std::endl;
    }
}

void testPassedGate(Car* car, int expected) {
    std::cout << "newsimpletest passedgate" << std::endl;
    if(car->hasPassedGate() != expected){
        std::cout << "%TEST_FAILED% time=0 testname=passedgate (newsimpletest) message=Passed gate not as expected." << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% carcreate (newsimpletest)" << std::endl;
    Car* c = testCarCreate();
    std::cout << "%TEST_FINISHED% time=0 carcreate (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% getid (newsimpletest)\n" << std::endl;
    testGetId(c);
    std::cout << "%TEST_FINISHED% time=0 getid (newsimpletest)" << std::endl;
    
    std::cout << "%TEST_STARTED% getmode (newsimpletest)\n" << std::endl;
    testGetMode(c);
    std::cout << "%TEST_FINISHED% time=0 getmode (newsimpletest)" << std::endl; 
    
    std::cout << "%TEST_STARTED% getspeed (newsimpletest)\n" << std::endl;
    testGetSpeed(c, 0);
    std::cout << "%TEST_FINISHED% time=0 getspeed (newsimpletest)" << std::endl;
    
    c->setCurrentSpeed(100);
    
    std::cout << "%TEST_STARTED% getspeed (newsimpletest)\n" << std::endl;
    testGetSpeed(c, 100);
    std::cout << "%TEST_FINISHED% time=0 getspeed (newsimpletest)" << std::endl;
    
    std::cout << "%TEST_STARTED% getontrack (newsimpletest)\n" << std::endl;
    testGetOnTrack(c, 1);
    std::cout << "%TEST_FINISHED% time=0 getontrack (newsimpletest)" << std::endl;
    
    c->offTrack();
    
    std::cout << "%TEST_STARTED% getontrack (newsimpletest)\n" << std::endl;
    testGetOnTrack(c, 0);
    std::cout << "%TEST_FINISHED% time=0 getontrack (newsimpletest)" << std::endl;
    
    std::cout << "%TEST_STARTED% passedgate (newsimpletest)\n" << std::endl;
    testPassedGate(c, 0);
    std::cout << "%TEST_FINISHED% time=0 passedgate (newsimpletest)" << std::endl;
    
    c->gatePassed();
    
    std::cout << "%TEST_STARTED% getontrack (newsimpletest)\n" << std::endl;
    testGetOnTrack(c, 1);
    std::cout << "%TEST_FINISHED% time=0 getontrack (newsimpletest)" << std::endl;
    
    std::cout << "%TEST_STARTED% passedgate (newsimpletest)\n" << std::endl;
    testPassedGate(c, 1);
    std::cout << "%TEST_FINISHED% time=0 passedgate (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

