/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   carcontroller.h
 * Author: nib28
 *
 * Created on 14 March 2017, 11:06
 */

#include <cv.h>

#ifndef CARCONTROLLER_H
#define CARCONTROLLER_H

struct speeds{
    int i0;
    int i1;
    int i2;
    int i4;
    int i6;
    int i8;
    
    int locked;
};

class Carcontroller {
public:
    Carcontroller();
    int init(int columns, int rows);
    int update(cv::Point point);
private:
    int columns;
    int rows;
    speeds* boxes;
    int updateboxspeeds(speeds* s);
    
    int safespeed;
    int safespeedset;
    speeds* lastbox;
};

#endif /* CARCONTROLLER_H */

