/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   colordetector.cpp
 * Author: nib28
 * 
 * Created on 18 February 2017, 15:02
 */

#include "colordetector.h"

Colordetector::Colordetector() {
    //this->distribution[51];
    //this->temp[51];
    
    this->binNum = 51; 
     
    for(int i = 0; i < this->binNum; i++){
        this->distribution[i] = (double)0;
    }
         
}


int Colordetector::learnColor(Image* img, int x1, int x2, int y1, int y2){
    this->createDistribution(img, x1, x2, y1, y2);
    for(int i = 0; i < this->binNum; i++){
        this->distribution[i] = this->temp[i];
    }
    
    return 0;
}

double Colordetector::matchColor(Image* img, int x1, int x2, int y1, int y2){
    if(this->createDistribution(img, x1, x2, y1, y2) == 1){
        return 0;
    }
    
    double match = 0.0;
    for(int i = 0; i < this->binNum; i++){
        if(this->distribution[i] == 0 && this->temp[i] == 0){continue;}
        match += ((this->distribution[i] - this->temp[i]) * (this->distribution[i] - this->temp[i])) / (this->distribution[i] + this->temp[i]);
    }
    
    return match;
}

int Colordetector::createDistribution(Image* img, int x1, int x2, int y1, int y2){
    
    int count[this->binNum];
    int zerocheck = 0;
    
    for(int i = 0; i < this->binNum; i++){
        this->temp[i] = 0.0;
        count[i] = 0;
    }
    
    int area = (x1 - x2) * (y1 - y2);
    
    for(int x = x1; x < x2; x++){
        for(int y = y1; y < y2; y++){
            if(img->getPixel(x, y) == 0){
                zerocheck++;
                continue;
            }
            count[img->getPixel(x, y)/5]++;
            //std::cout << "hello: " << count[img->getPixel(x, y)/5] << std::endl;
        }
    }
    
    if(zerocheck > (area / 4)){
        return 1;
    }
    
    for(int i = 0; i < this->binNum; i++){
        this->temp[i] = (double)count[i] / (double)area;
        //std::cout << "| i:" << i << ",count:" << count[i] << ",temp:" << this->temp[i] << std::endl;
    }
    
    return 0;
}