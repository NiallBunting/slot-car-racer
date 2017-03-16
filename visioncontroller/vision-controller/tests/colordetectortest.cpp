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

void test1() {
    int fail = 0;

    //Set up
    cv::Mat3f mat(20, 20);
    Colordetector* cd = new Colordetector();
    
    //Change pixel value
    cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(3,3));
    color.val[0] = color.val[1] = color.val[2] = 0;
    mat.at<cv::Vec3b>(cv::Point(0,0)) = color;
    
    
    
    cd->learnColor(mat, 0, 5, 0, 5);
    
    cd->matchColor(mat, 0, 5, 0, 5);
    cd->matchColor(mat 10, 15, 10, 15);
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

