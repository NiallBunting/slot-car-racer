/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   backgroundsubtractiontest.cpp
 * Author: nib28
 *
 * Created on 08 February 2017, 18:26
 */

#include <stdlib.h>
#include <iostream>
#include "backgroundsubtraction.h"
#include <cv.h>

/*
 * Simple C++ Test Suite
 */

//train setup
//test limits get expanded
//test correct background returned
void testhaspixelchanged(){
    std::cout << "backgroundsubtractiontest test" << std::endl;
    
    int fail = 0;

    //Set up
    cv::Mat3f mat(20, 20);
    Backgroundsubtraction* bs = new Backgroundsubtraction();
    bs->init(20, 20);
    //Change pixel value
    cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(3,3));
    color.val[0] = color.val[1] = color.val[2] = 0;
    mat.at<cv::Vec3b>(cv::Point(0,0)) = color;
    
    std::cout << (int)mat.at<cv::Vec3b>(cv::Point(0,0)).val[2] << " A" << std::endl;
    
    bs->trainBackground(mat);
    color = mat.at<cv::Vec3b>(cv::Point(3,3));
    color.val[0] = color.val[1] = color.val[2] = 10;
    mat.at<cv::Vec3b>(cv::Point(0,0)) = color;
    
    std::cout << (int)mat.at<cv::Vec3b>(cv::Point(0,0)).val[2] << " A" << std::endl;
    
    bs->trainBackground(mat);
    //Set up mask
    color = mat.at<cv::Vec3b>(cv::Point(3,3));
    color.val[0] = color.val[1] = color.val[2] = 53;
    mat.at<cv::Vec3b>(cv::Point(0,0)) = color;
    
    std::cout << (int)mat.at<cv::Vec3b>(cv::Point(0,0)).val[2] << " A" << std::endl;
    
    bs->maskCreate(mat);
    
    color = mat.at<cv::Vec3b>(cv::Point(3,3));
    color.val[0] = color.val[1] = color.val[2] = 11;
    mat.at<cv::Vec3b>(cv::Point(0,0)) = color; 
    
    //Check pixel the same
    if(bs->hasPixelChanged(0, 0, mat) != 1){fail = 1;}
    std::cout << "hello" << bs->hasPixelChanged(3, 3, mat) << std::endl;
    
    //Change pixel value
    color = mat.at<cv::Vec3b>(cv::Point(0,0));
    color.val[0] = color.val[1] = color.val[2] = 10;
    mat.at<cv::Vec3b>(cv::Point(0,0)) = color;
    
    //Check now different
    if(bs->hasPixelChanged(0, 0, mat) != 0){fail = 2;}
    
    if (fail != 0){
       std::cout << "%TEST_FAILED% time=0 testname=test (backgroundsubtractiontest) message=erm wrong" << fail << std::endl;
    }
    
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% backgroundsubtractiontest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test2 (backgroundsubtractiontest)\n" << std::endl;
    testhaspixelchanged();
    std::cout << "%TEST_FINISHED% time=0 test2 (backgroundsubtractiontest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

