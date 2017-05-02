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
    
    //Slot manager setup
    Slotmanager* sm = new Slotmanager();
    if(sm->init(sm) != 0){return 1;}
    
    //Inf loop that updates the program.
    for(;;){
        if(sm->update() != 0){break;}
    }

    delete sm;
    
    return 0;
}