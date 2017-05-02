/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   colordetectortest.cpp
 * Author: nib28
 *
 * Created on 14 March 2017, 17:41
 */

#include <stdlib.h>
#include <iostream>
#include "colordetector.h"

/*
 * Simple C++ Test Suite
 */

const static int BACKGROUNDTSIZE = 20;

Colordetector* testCreateColorDetector(){

    Colordetector* cd = new Colordetector();
    
    return cd;
}


void teachColor(Colordetector* cd){
    cv::Mat3f mat(20, 20);
    for(int x = 0; x < BACKGROUNDTSIZE; x++){
        for(int y = 0; y < BACKGROUNDTSIZE; y++){
            cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(x,y));
            int colval = ((x / 5) * (y / 5));
            color.val[0] = color.val[1] = color.val[2] = colval;
            mat.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
    
    cd->learnColor(mat, 5, 9, 5, 9);
}

void matchColor(Colordetector* cd, cv::Point* a, cv::Point* b, int fail) {
    cv::Mat3f mat(20, 20);
    for(int x = 0; x < BACKGROUNDTSIZE; x++){
        for(int y = 0; y < BACKGROUNDTSIZE; y++){
            cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(x,y));
            int colval = ((x / 5) * (y / 5));
            color.val[0] = color.val[1] = color.val[2] = colval;
            mat.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }

    if(fail == 0){
        if(cd->matchColor(mat, a->x, b->x, a->y, b->y) < 0.5){
            return;
        }else{
            std::cout << "%TEST_FAILED% time=0 testname=colourmatch1 (backgroundsubtractiontest) message=Colours did not match." << std::endl;
        }
    }else{
        if(cd->matchColor(mat, a->x, b->x, a->y, b->y) != 0.0){
            return;
        }else{
            std::cout << "%TEST_FAILED% time=0 testname=colourmatch2 (backgroundsubtractiontest) message=Colours did match." << std::endl;
        } 
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% Colourdetectortest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    Colordetector* cd = testCreateColorDetector();
    teachColor(cd);
    
    //Test the positive case
    std::cout << "%TEST_STARTED% colourmatch1 (newsimpletest)" << std::endl;
    matchColor(cd, new cv::Point(5, 5), new cv::Point(9 ,9), 0);
    std::cout << "%TEST_FINISHED% time=0 colourmatch1 (newsimpletest)" << std::endl;
    
    //Test the negative case
    std::cout << "%TEST_STARTED% colourmatch2 (newsimpletest)" << std::endl;
    matchColor(cd, new cv::Point(11, 11), new cv::Point(19 ,19), 1);
    std::cout << "%TEST_FINISHED% time=0 colourmatch2 (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    delete cd;
    return (EXIT_SUCCESS);
}

