/* 
 * File:   main.cpp
 * Author: nib28
 *
 * Created on 08 February 2017, 13:37
 */

#include "main.h"
#include "slotmanager.h"

/*
void CallBackFunc(int event, int x, int y, int flags, void* userdata){
    if  ( event == cv::EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        if(selx1 == 0){
            selx1 = x;
            sely1 = y;
        }else if(selx2 == 0){
            selx2 = x;
            sely2 = y;
        }
    }
}
*/


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