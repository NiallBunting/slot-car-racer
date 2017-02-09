/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gaussian.h
 * Author: nib28
 *
 * Created on 08 February 2017, 13:44
 */

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "image.h"
#include "common.h"

class gaussian {
public:
    gaussian();
    int doGaussian(Image* a);
private:
    const static float convolution[];
    int background[IMAGEX][IMAGEY];
};

#endif /* GAUSSIAN_H */

