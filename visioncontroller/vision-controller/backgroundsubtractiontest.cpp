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
#include "image.h"

/*
 * Simple C++ Test Suite
 */

void test1() {

}

void test2() {
    std::cout << "backgroundsubtractiontest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (backgroundsubtractiontest) message=error message sample" << std::endl;
}


//train setup
//test limits get expanded
//test correct background returned
void test(){
    std::cout << "backgroundsubtractiontest test" << std::endl;
    
    int a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Image* img1 = new Image(a);
    
    
    int b[9] = {1, 2, 3, 6, 5, 6, 7, 8, 9};
    Image* img2 = new Image(b);
    
    Backgroundsubtraction* bs = new Backgroundsubtraction();
    bs->trainBackground(img1);
    bs->trainBackground(img2);
    
    
    int c[9] = {1, 2, 3, 5, 0, 0, 0, 0, 0};
    Image* img3 = new Image(c);
    
    Image* f = bs->doFullBackSubtract(img3);

    // 0 is where no change
    int fail = 0;
    if(f->getPixel(0,0) != 0){fail = 1;}
    if(f->getPixel(1,0) != 0){fail = 2;}
    if(f->getPixel(2,0) != 0){fail = 3;}
    if(f->getPixel(0,1) != 0){fail = 4;}
    if(f->getPixel(1,1) != 1){fail = 5;}
    if(f->getPixel(2,1) != 1){fail = 6;}
    if(f->getPixel(0,2) != 1){fail = 7;}
    if(f->getPixel(1,2) != 1){fail = 8;}
    if(f->getPixel(2,2) != 1){fail = 9;}
    
    if (fail != 0){
       std::cout << "%TEST_FAILED% time=0 testname=test (backgroundsubtractiontest) message=erm wrong" << fail << std::endl;
    }
    
}




int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% backgroundsubtractiontest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test2 (backgroundsubtractiontest)\n" << std::endl;
    test();
    std::cout << "%TEST_FINISHED% time=0 test2 (backgroundsubtractiontest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

