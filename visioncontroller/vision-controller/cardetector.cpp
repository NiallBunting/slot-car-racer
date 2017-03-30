/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   slotdetector.cpp
 * Author: nib28
 * 
 * Created on 30 March 2017, 11:46
 */

#include "cardetector.h"
#include "slotmanager.h"
#include <highgui.h>

Cardetector::Cardetector() {
}

int Cardetector::init(int cols, int rows){
    //Background subtraction
    this->bs = new Backgroundsubtraction();
    this->bs->init(cols, rows);
    
    //Colour detector
    this->cd = new Colordetector();
    
    //varibles required by class
    this->learnStage = 0;
    this->colourBoxCount = 0;
    
    this->learnFinished = 0;
    this->overlay = 1;
    
    return 0;
}

Cardetector::~Cardetector() {
    delete this->bs;
    delete this->cd;
}

int Cardetector::hasLearnt(){
    return this->learnFinished;
}

int Cardetector::learn(cv::Mat& frame, int key, cv::Point* click){
    
    if(key == 1){
        this->learnStage++;
    }else if(key == 2){
        this->learnStage--;
    }
    
    switch(this->learnStage){
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
            cv::cvtColor(frame, this->colFrame, cv::COLOR_BGR2HLS);
            //Move to the next stage.
            this->learnStage++;
            
            break;
        case 3://This stage waits till all the mouse is inputted.
            if(this->colourBoxCount < 2){
                //if clicks are not finished and over 0 input them
                if(click->x > 0 && click->y > 0){
                    this->colorBox[this->colourBoxCount] = new cv::Point(click->x, click->y);
                    this->colourBoxCount++;
                }
            }else{
            
                this->learnStage++;

                cd->learnColor(this->colFrame, this->colorBox[0]->x, this->colorBox[1]->x, this->colorBox[0]->y, this->colorBox[1]->y);
                
                cv::rectangle(
                this->colFrame,
                cv::Point(this->colorBox[0]->x, this->colorBox[0]->y),
                cv::Point(this->colorBox[1]->x, this->colorBox[1]->y),
                cv::Scalar(0, 255, 255)
                );     
                cv::imshow("Select car", this->colFrame);

            }
            break;
        case 4:
            this->learnFinished = 1;
            break;
        default:
            break;
    }
    
    delete click;
    
}

cv::Point* Cardetector::detect(cv::Mat& frame){
    cv::Mat hlsframe;
    cv::cvtColor(frame, hlsframe, cv::COLOR_BGR2HLS);
    
    double ytotal = 0;
    double xtotal = 0;
    double boxtotal = 0;
    
    int boxonlastx = 0;
    int boxonlasty = 0;
    for(int x = BOXLENGTH; x < frame.cols; x+= BOXJUMP){
        for(int y = BOXLENGTH; y < frame.rows; y+= BOXJUMP){
            
            //This checks if the pixel is part of the mask or actually has changed.
            if(this->bs->hasPixelChanged(x, y, hlsframe) == 0){
                if(this->overlay){
                    cv::rectangle(
                    frame,
                    cv::Point(x, y),
                    cv::Point(x - BOXLENGTH, y - BOXLENGTH),
                    cv::Scalar(255, 0, 255)//Purple
                    );  
                }
                continue;
            }
            
            //Then check if the pixel matches if has changed.
            if(this->cd->matchColor(hlsframe, x-BOXLENGTH, x, y-BOXLENGTH, y) < 0.5){
                //Pixel has changed and matches colour.
                if(this->overlay){
                    cv::rectangle(
                    frame,
                    cv::Point(x, y),
                    cv::Point(x - BOXLENGTH, y - BOXLENGTH),
                    cv::Scalar(0, 255, 255)//yellow
                    );
                }

                // There here means that if there is a positive box to the left
                // or above this box this gets a weight rather than one.
                // This should hopefully reduce the effect of outliers.
                int multiplier = 1;
                if((boxonlastx - BOXJUMP) == x){
                    multiplier *= 100;
                }
                if((boxonlasty - BOXJUMP) == y){
                    multiplier *= 100;
                }
                
                boxonlastx = x;
                boxonlasty = y;
                
                //Add the values to create the average.
                ytotal += y * multiplier;
                xtotal += x * multiplier;
                boxtotal+=multiplier;
            }else{
                if(this->overlay){
                    cv::rectangle(
                    frame,
                    cv::Point(x, y),
                    cv::Point(x - BOXLENGTH, y - BOXLENGTH),
                    cv::Scalar(255, 255, 0)//blue
                    );
                }
            }

        }
        //
    }
    
    //Calculate average and draw circle.
    if(boxtotal != 0){
        ytotal /= boxtotal;
        xtotal /= boxtotal;
    }
    
    cv::circle(frame, cv::Point(xtotal, ytotal), 4, cv::Scalar(255, 255, 255), 3 + (this->overlay * 17));
    
    return new cv::Point(xtotal, ytotal);    
}

int Cardetector::toggleOverlay(){
    if(this->overlay == 1){
        this->overlay = 0;
    }else{
        this->overlay = 1;
    }
}
