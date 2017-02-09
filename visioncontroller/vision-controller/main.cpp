/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: nib28
 *
 * Created on 08 February 2017, 13:37
 */

#include <cstdlib>
#include "main.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Image* i = new Image(a);
    
    
    int b[9] = {1, 2, 3, 6, 5, 6, 7, 8, 9};
    Image* j = new Image(b);
    
    cout << i->getPixel(2,2);
    i->updatePixel(2,2, 55);
    cout << i->getPixel(2,2);
    cout << "\n-----------\n";
    
    Backgroundsubtraction* bs = new Backgroundsubtraction();
    bs->trainBackground(i);
    bs->trainBackground(j);
    delete i;
    delete j;
    
    int p[9] = {1, 2, 3, 6, 0, 0, 0, 0, 0};
    Image* u = new Image(p);
    
    Image* tmp = bs->doFullBackSubtract(u);
    
    
    return 0;
}

