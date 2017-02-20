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

/*
#include <cstdlib>
#include "main.h"

using namespace std;


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
 */
#include "main.h"

#include <cv.h>
#include <highgui.h>

#include <iostream>

int selx1, sely1, selx2, sely2 = 0;

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



int main(int argc, char* argv[])
{
    //http://stackoverflow.com/questions/14833553/reading-a-video-with-opencv
    cv::VideoCapture cap("/aber/nib28/out.mp4");
    //cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Could not open stream/file." << std::endl;
        return -1;
    }

    cv::Mat frame;
    cv::Mat out;
    cv::Mat learnframe;
    Image* img = new Image();
    Image* subtract;
    Image* colimg = new Image();
    Backgroundsubtraction* bs = new Backgroundsubtraction();
    Colordetector* cd = new Colordetector();
    
    int train = 0;
    
    for(;;)
    {
        if (!cap.read(frame))
            break;
        
        //This leads to some weirdness of three repeated images.
        //cvtColor(out, out, CV_BGR2GRAY);   
        cvtColor(frame, out, cv::COLOR_BGR2HLS);
        
        GaussianBlur(out, out, cv::Size(7,7), 1.5, 1.5);
        
        for(int x = 0; x < frame.cols; x++){
            for(int y = 0; y < frame.rows; y++){
                cv::Vec3b color = out.at<cv::Vec3b>(cv::Point(x,y));
                //Takes an average currently, needs changing.
                //int c = (color[0] + color[1] + color[2])/3.0;
                int c = color[1];
                img->updatePixel(x, y, c);
            }
        }
        
        if (train == 0){
            bs->trainBackground(img);
        }else if(train == 1){
            Image* subtract = bs->maskAdd(img);
            
            for(int x = 0; x < frame.cols; x++){
                for(int y = 0; y < frame.rows; y++){
                        cv::Vec3b color = frame.at<cv::Vec3b>(cv::Point(x,y));
                        color[0] = subtract->getPixel(x,y);
                        color[1] = subtract->getPixel(x,y);
                        color[2] = subtract->getPixel(x,y);
                        frame.at<cv::Vec3b>(cv::Point(x,y)) = color;
                }
            }
            
            delete subtract;

        }
        else if (train == 2){
            train = 3;
            learnframe = out.clone();
            cv::imshow("Select car", learnframe);
            cv::setMouseCallback("Select car", CallBackFunc, NULL);

        }else if (train == 3){
            if(sely2 != 0){
                cv::rectangle(
                learnframe,
                 cv::Point(selx1, sely1),
                 cv::Point(selx2, sely2),
                 cv::Scalar(0, 255, 255)
                );
                cv::imshow("Select car", learnframe);
                train = 4;
                
                for(int x = 0; x < frame.cols; x++){
                    for(int y = 0; y < frame.rows; y++){
                        cv::Vec3b color = learnframe.at<cv::Vec3b>(cv::Point(x,y));
                        //Takes an average currently, needs changing.
                        //int c = (color[0] + color[1] + color[2])/3.0;
                        int c = color[0];
                        colimg->updatePixel(x, y, c);
                    }
                }
                
                cd->learnColor(colimg, selx1, selx2, sely1, sely2);
                
            }
        }else{
            //Then just do tracking
            Image* subtract = bs->doFullBackSubtract(img);
            for(int x = 0; x < frame.cols; x++){
                for(int y = 0; y < frame.rows; y++){
                    cv::Vec3b color = frame.at<cv::Vec3b>(cv::Point(x,y));
                    if(subtract->getPixel(x,y) == 0){
                        color[0] = 0;
                        color[1] = 0;
                        color[2] = 0;
                        frame.at<cv::Vec3b>(cv::Point(x,y)) = color;
                    }else{
                        subtract->updatePixel(x, y, color[0]);
                    }
                }
            }
            
            for(int x = 5; x < frame.cols; x+=10){
                for(int y = 5; y < frame.rows; y+=10){
                    if(subtract->getPixel(x,y) == 0){continue;}
                    std::cout << "Match: " << cd->matchColor(subtract, x, x+9, y, y+9) << std::endl;
                        if(cd->matchColor(subtract, x, x+9, y, y+9) > 1.4){
                            cv::rectangle(
                            frame,
                            cv::Point(x, y),
                            cv::Point(x+9, y+9),
                            cv::Scalar(0, 255, 255)
                            );                            
                        }
                }
            }
            
            
            

            
            
            delete subtract;
        }
        
       

        
        cv::imshow("window", frame);

        
        
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);
        
        //TODO: Mode where you select the car
        //Take key input to change the mode of what to do
        //The first mode learns the background
        //Second aims to learn the track
        //Then just track
        char key = cvWaitKey(10);
        if (key == 27){ // ESC
            break;
        }
        //48 is 0
        if (key == 48){ //0
            train = 0;
        }
        if (key == 49){ //1
            train = 1;
            std::cout << "Background Train Finished." << std::endl;
        }
        if (key == 50){ //2
            train = 2;
            std::cout << "Lap finished." << std::endl;
        }
        if (key == 51){ //3
            train = 4;
            std::cout << "Lap finished." << std::endl;
        }
    }

    return 0;
}