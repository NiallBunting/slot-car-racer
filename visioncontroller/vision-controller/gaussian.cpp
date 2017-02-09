/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gaussian.cpp
 * Author: nib28
 * 
 * Created on 08 February 2017, 13:44
 */

#include "gaussian.h"

const float convolution[7] = {0.006f, 0.061f, 0.242f, 0.383f, 0.242f, 0.061f, 0.006f};

gaussian::gaussian(){
}

int gaussian::doGaussian(Image* a){
    //create temp array where can add the values
    //start in left corner and add each of the values to the temp array
    //replace image a values
}

