/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   slotmanager.cpp
 * Author: nib28
 * 
 * Created on 10 March 2017, 13:38
 */

#include "slotmanager.h"
#include "colordetector.h"

Slotmanager::Slotmanager(){}

//This is for collecting the mouse clicks. Calls the Slotmanager pointer.
Slotmanager* g_pointer_slotmanager;
const int BOXLENGTH = 10;
const int BOXJUMP = 11;


void CallBackFunc(int event, int x, int y, int flags, void* userdata){
    if  ( event == cv::EVENT_LBUTTONDOWN )
    {
        g_pointer_slotmanager->mouseClick(x, y);
    }
}

//Sets up the slotmanager
int Slotmanager::init(Slotmanager* sm){
    this->action = 0;
    cv::Mat frame;
    
    //Set up the video source.
    //this->cap = new cv::VideoCapture("/aber/nib28/out.mp4");
    this->cap = new cv::VideoCapture(0);
    if(!this->cap->isOpened()){
        std::cout << "Could not open stream/file." << std::endl;
        return -1;
    }
    
    //Capture a single frame for values.
    if(this->capture(frame) != 0){return -1;}
    
    //Size of the image.
    this->cols = frame.cols;
    this->rows = frame.rows;
    
    this->bs = new Backgroundsubtraction();
    this->bs->init(this->cols, this->rows);
    
    //set global pointer to this object.
    g_pointer_slotmanager = sm;
    
    //once mouse positions have been set.
    this->mousepoint = 0;
    
    //Colour detector
    this->cd = new Colordetector();
    
    //Create the car controller.
    this->cc = new Carcontroller();
    this->cc->init(this->cols, this->rows);
    
    return 0;
}

//Called on each update
int Slotmanager::update(){
    //Capture the video
    cv::Mat frame;
    if(this->capture(frame) != 0){return -1;}
    
    switch(this->action){
        case 0:
            //learn background
            cv::cvtColor(frame, frame, cv::COLOR_BGR2HLS);
            GaussianBlur(frame, frame, cv::Size(7,7), 1.5, 1.5);
            bs->trainBackground(frame);
            break;
        case 1:
            //learn mask
            cv::cvtColor(frame, frame, cv::COLOR_BGR2HLS);
            GaussianBlur(frame, frame, cv::Size(7,7), 1.5, 1.5);
            bs->maskCreate(frame);
            break;
        case 2:
            //learn learn colour
            cv::imshow("Select car", frame);
            cv::setMouseCallback("Select car", CallBackFunc, NULL);
            
            cv::cvtColor(frame, this->colframe, cv::COLOR_BGR2HLS);
            //Move to the next stage.
            this->action++;
            //Reset learning from the image.
            this->mousepoint = 0;
            break;
        case 3://This stage waits till all the mouse is inputted.
            if(this->mousepoint == 3){
                this->action++;
                this->mousepoint = 0;
                cd->learnColor(this->colframe, this->mouseselect[0], this->mouseselect[1], this->mouseselect[2], this->mouseselect[3]);
                
                cv::rectangle(
                this->colframe,
                cv::Point(this->mouseselect[0], this->mouseselect[2]),
                cv::Point(this->mouseselect[1], this->mouseselect[3]),
                cv::Scalar(0, 255, 255)
                );     
                cv::imshow("Select car", this->colframe);
            }
            break;
        default:
            cv::Point average = this->Match(frame);
            this->cc->update(average);
            break;
    }
    
    
    
    //Show the video
    cv::imshow("window", frame);
    
    //Capture the key strokes.
    char key = cvWaitKey(10);
    if (key == 27){ // ESC
        return -1;
    }
    if(key == 49){ // 1
        this->action++;
    }
    if(key == 50){ // 2
        this->action--;
    }
    
    return 0;
}

// Captures a singe frame.
int Slotmanager::capture(cv::Mat& frame){
    //Capture a single frame.
    if(!this->cap->read(frame)){
        std::cout << "Could not read frame." << std::endl;
        return -1;
    }
    return 0;
}

//Called on mouse click when selecting colours.
int Slotmanager::mouseClick(int x, int y) {
    std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
    if(this->mousepoint == 0){
        this->mouseselect[0] = x;
        this->mouseselect[2] = y;
        this->mousepoint = 2;
    }else if(this->mousepoint == 2){
        this->mouseselect[1] = x;
        this->mouseselect[3] = y;
        this->mousepoint = 3;
    }
    return 0;
}

//Called when doing the match testing.
cv::Point Slotmanager::Match(cv::Mat& frame){
    cv::Mat hlsframe;
    cv::cvtColor(frame, hlsframe, cv::COLOR_BGR2HLS);
    
    double ytotal = 0;
    double xtotal = 0;
    double boxtotal = 0;
    
    for(int x = BOXLENGTH; x < frame.cols; x+= BOXJUMP){
        for(int y = BOXLENGTH; y < frame.rows; y+= BOXJUMP){
            
            //This checks if the pixel is part of the mask or actually has changed.
            if(this->bs->hasPixelChanged(x, y, hlsframe) == 0){                
                cv::rectangle(
                frame,
                cv::Point(x, y),
                cv::Point(x - BOXLENGTH, y - BOXLENGTH),
                cv::Scalar(255, 0, 255)//Purple
                );  
                continue;
            }
            
            //Then check if the pixel matches if has changed.
            if(this->cd->matchColor(hlsframe, x-BOXLENGTH, x, y-BOXLENGTH, y) < 0.5){
                //Pixel has changed and matches colour.
                cv::rectangle(
                frame,
                cv::Point(x, y),
                cv::Point(x - BOXLENGTH, y - BOXLENGTH),
                cv::Scalar(0, 255, 255)//yellow
                );

                //Add the values to create the average.
                ytotal += y;
                xtotal += x;
                boxtotal++;
            }else{
                cv::rectangle(
                frame,
                cv::Point(x, y),
                cv::Point(x - BOXLENGTH, y - BOXLENGTH),
                cv::Scalar(255, 255, 0)//blue
                );     
            }

        }
    }
    
    //Calculate average and draw circle.
    ytotal /= boxtotal;
    xtotal /= boxtotal;
    
    cv::circle(frame, cv::Point(xtotal, ytotal), 4, cv::Scalar(255, 255, 255), 20);
    
    return cv::Point(xtotal, ytotal);
}