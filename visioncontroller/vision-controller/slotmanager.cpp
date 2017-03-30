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

Slotmanager::Slotmanager(){}

Slotmanager::~Slotmanager(){
    this->serialCom->quit();
    pthread_join( this->serialThread, NULL);
    delete this->serialCom;
}

//This is for collecting the mouse clicks. Calls the Slotmanager pointer.
Slotmanager* g_pointer_slotmanager;

//This is called when the mouse is clicked on the frame.
void CallBackFunc(int event, int x, int y, int flags, void* userdata){
    if  ( event == cv::EVENT_LBUTTONDOWN )
    {
        g_pointer_slotmanager->mouseClick(x, y);
    }
}

//This is to start the serial thread.
static void* serialCommunicatorWrap(void* arg) {
    ((Serialcommunicator*) arg)->update();
    return 0;
}

//Sets up the slotmanager
int Slotmanager::init(Slotmanager* sm){
    cv::Mat frame;
    
    //Set up the video source.
    //this->cap = new cv::VideoCapture("/aber/nib28/out.mp4");
    this->cap = new cv::VideoCapture(0);
    //Set resolution of the camera
    this->cap->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    this->cap->set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if(!this->cap->isOpened()){
        std::cout << "Could not open stream/file - camera." << std::endl;
        return -1;
    }
    
    //Capture a single frame for values.
    if(this->capture(frame) != 0){return -1;}
    
    //Size of the image.
    this->cols = frame.cols;
    this->rows = frame.rows;
    
    //set global pointer to this object.
    g_pointer_slotmanager = sm;
    
    //setup the cars
    this->computerCar = new Car('A', 1, this->cols, this->rows);
    this->humanCar = new Car('B', 0, this->cols, this->rows);
    
    //The point that will be used
    this->mousePress = new cv::Point(0, 0);
    
    //Serial communication
    this->serialCom = new Serialcommunicator();
    this->serialCom->init();
    
    int iret1 = pthread_create( &this->serialThread, NULL, &serialCommunicatorWrap, this->serialCom);
    if(iret1){
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }
    
    return 0;
}

//Called on each update
int Slotmanager::update(){
    //Capture the video
    cv::Mat frame;
    if(this->capture(frame) != 0){return -1;}
    
    if(!this->computerCar->getCarDetector()->hasLearnt()){
        this->computerCar->getCarDetector()->learn(frame, this->keyPress, this->getMouseClick());
    }else{
        cv::Point* location = this->computerCar->getCarDetector()->detect(frame);
        
        //this->computerCar->carController;
        
        delete location;
    }
        
    //Show the video
    cv::imshow("window", frame);
    
    //Capture the key strokes.
    char key = cvWaitKey(10);
    if(key == -1){
        this->keyPress = 0;
    }
    if (key == 27){ // ESC
        return -1;
    }
    if(key == 49){ // 1
        this->keyPress = 1;
    }
    if(key == 50){ // 2
        this->keyPress = 2;
    }
    if(key == 51){//3
        this->computerCar->getCarDetector()->toggleOverlay();
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

    this->mousePress->x = x;
    this->mousePress->y = y;
    
    return 0;
}

// Returns a pointer to a point
// resets the current mouse click values
cv::Point* Slotmanager::getMouseClick(){
    cv::Point* temp = new cv::Point(this->mousePress->x, this->mousePress->y);
    this->mousePress->x = 0;
    this->mousePress->y = 0;
    return temp;
}