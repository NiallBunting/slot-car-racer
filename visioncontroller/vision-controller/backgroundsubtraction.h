/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   backgroundsubtraction.h
 * Author: nib28
 *
 * Created on 08 February 2017, 13:53
 */

#ifndef BACKGROUNDSUBTRACTION_H
#define BACKGROUNDSUBTRACTION_H

#include <cv.h>

struct BackgroundPixel{
    int high;
    int low;
    int mask;
};


class Backgroundsubtraction {
public:
    Backgroundsubtraction();
    int init(int cols, int rows);
    int hasPixelChanged(int x, int y, cv::Mat& frame);
    //Trains the background on the images.
    int trainBackground(cv::Mat& frame);
    int maskCreate(cv::Mat& frame);
    int maskMouse(cv::Point* mousePoint, int button);
    int removeMaskPoint(cv::Point* point);
private:
    int cols;
    int rows;
    BackgroundPixel* background;
    int hasPixelChanged(int x, int y, int value);
    int fillTrack(cv::Point* point);
    int floodFill(int x, int y);
};

#endif /* BACKGROUNDSUBTRACTION_H */
