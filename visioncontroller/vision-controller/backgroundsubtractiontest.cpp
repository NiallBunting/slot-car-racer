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

const static int BACKGROUNDTSIZE = 20;

Backgroundsubtraction* testCreateBackground(){
    std::cout << "backgroundsubtractiontest test" << std::endl;

    Backgroundsubtraction* bs = new Backgroundsubtraction();
    int returnVal = bs->init(BACKGROUNDTSIZE, BACKGROUNDTSIZE);
    
    if (returnVal != 0){
       std::cout << "%TEST_FAILED% time=0 testname=createBackground (backgroundsubtractiontest) message=Did not return 0." << std::endl;
    }
    
    return bs;
}


void testTrainBackground(Backgroundsubtraction* bs, int value){
    cv::Mat3f mat(BACKGROUNDTSIZE, BACKGROUNDTSIZE);
    
    for(int x = 0; x < BACKGROUNDTSIZE; x++){
        for(int y = 0; y < BACKGROUNDTSIZE; y++){
            cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(x,y));
            color.val[0] = color.val[1] = color.val[2] = value;
            mat.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
    
    cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(BACKGROUNDTSIZE/2,BACKGROUNDTSIZE/2));
    
    if (color.val[0] != value){
       std::cout << "%TEST_FAILED% time=0 testname=trainBackground (backgroundsubtractiontest) message=Colour not correct." << std::endl;
    }
    
    bs->trainBackground(mat);
}


//Got to create mask first
void testPixelChanged(Backgroundsubtraction* bs, int value, int test){
    cv::Mat3f mat(BACKGROUNDTSIZE, BACKGROUNDTSIZE);
    
    for(int x = 0; x < BACKGROUNDTSIZE; x++){
        for(int y = 0; y < BACKGROUNDTSIZE; y++){
            cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(x,y));
            color.val[0] = color.val[1] = color.val[2] = value;
            mat.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
    
    if(test == 0){
        if(bs->hasPixelChanged(BACKGROUNDTSIZE/5, BACKGROUNDTSIZE/5, mat)){
            return;
        }else{
            std::cout << "%TEST_FAILED% time=0 testname=testpixelchanged (backgroundsubtractiontest) message=Pixel not changed." << std::endl;
        }
    }else{
        if(!bs->hasPixelChanged(BACKGROUNDTSIZE/5, BACKGROUNDTSIZE/5, mat)){
            return;
        }else{
            std::cout << "%TEST_FAILED% time=0 testname=testpixelchanged2 (backgroundsubtractiontest) message=Pixel not changed." << std::endl;
        }
    }
}

void testMaskCreate(Backgroundsubtraction* bs, int value){
    cv::Mat3f mat(BACKGROUNDTSIZE, BACKGROUNDTSIZE);
    
    for(int x = 0; x < BACKGROUNDTSIZE; x++){
        for(int y = 0; y < BACKGROUNDTSIZE; y++){
            cv::Vec3b color = mat.at<cv::Vec3b>(cv::Point(x,y));
            color.val[0] = color.val[1] = color.val[2] = value;
            mat.at<cv::Vec3b>(cv::Point(x,y)) = color;
        }
    }
    
    bs->maskCreate(mat);
    
}
void testMaskMouse(Backgroundsubtraction* bs, int x, int y, int action){
    bs->maskMouse(new cv::Point(x, y), action);
}

void testRemoveMaskPoint(Backgroundsubtraction* bs){
    int size = BACKGROUNDTSIZE/5;
    bs->removeMaskPoint(new cv::Point(size, size));
}


int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% backgroundsubtractiontest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% createbackground (backgroundsubtractiontest)\n" << std::endl;
    Backgroundsubtraction* backsub = testCreateBackground();
    std::cout << "%TEST_FINISHED% time=0 createbackground (backgroundsubtractiontest)" << std::endl;

    std::cout << "%TEST_STARTED% trainbackground (backgroundsubtractiontest)\n" << std::endl;
    testTrainBackground(backsub, 6);
    std::cout << "%TEST_FINISHED% time=0 trainbackground (backgroundsubtractiontest)" << std::endl;
    
    std::cout << "%TEST_STARTED% trainbackground (backgroundsubtractiontest)\n" << std::endl;
    testTrainBackground(backsub, 8);
    std::cout << "%TEST_FINISHED% time=0 trainbackground (backgroundsubtractiontest)" << std::endl;
    
    std::cout << "%TEST_STARTED% maskcreate (backgroundsubtractiontest)\n" << std::endl;
    testMaskCreate(backsub, 1);
    std::cout << "%TEST_FINISHED% time=0 maskcreate (backgroundsubtractiontest)" << std::endl;
    
    //This one should say it has changed
    std::cout << "%TEST_STARTED% testpixelchanged (backgroundsubtractiontest)\n" << std::endl;
    testPixelChanged(backsub, 9, 0);
    std::cout << "%TEST_FINISHED% time=0 testpixelchanged (backgroundsubtractiontest)" << std::endl;
    
    //this one say not changed
    std::cout << "%TEST_STARTED% testpixelchanged2 (backgroundsubtractiontest)\n" << std::endl;
    testPixelChanged(backsub, 7, 1);
    std::cout << "%TEST_FINISHED% time=0 testpixelchanged2 (backgroundsubtractiontest)" << std::endl;
    
    testRemoveMaskPoint(backsub);
    
    //say not changed even though should have because not in mask
    std::cout << "%TEST_STARTED% testpixelchanged2 (backgroundsubtractiontest)\n" << std::endl;
    testPixelChanged(backsub, 10, 1);
    std::cout << "%TEST_FINISHED% time=0 testpixelchanged2 (backgroundsubtractiontest)" << std::endl;
    
    testMaskMouse(backsub, BACKGROUNDTSIZE/5, BACKGROUNDTSIZE/5, 1);
            
    //will say changed again
    std::cout << "%TEST_STARTED% testpixelchanged2 (backgroundsubtractiontest)\n" << std::endl;
    testPixelChanged(backsub, 10, 0);
    std::cout << "%TEST_FINISHED% time=0 testpixelchanged2 (backgroundsubtractiontest)" << std::endl;
    
    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    delete backsub;
    return (EXIT_SUCCESS);
}

