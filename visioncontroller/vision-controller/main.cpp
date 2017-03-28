/* 
 * File:   main.cpp
 * Author: nib28
 *
 * Created on 08 February 2017, 13:37
 */

#include "main.h"
#include "slotmanager.h"

int main(int argc, char* argv[])
{
    
    Slotmanager* sm = new Slotmanager();
    if(sm->init(sm) != 0){return 1;}
    
    for(;;){
        if(sm->update() != 0){break;}
    }

    delete sm;
    
    return 0;
}