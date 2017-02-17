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
    Image* img = new Image();
    Image* subtract;
    Backgroundsubtraction* bs = new Backgroundsubtraction();
    
    int train = 0;
    
    for(;;)
    {
        if (!cap.read(frame))
            break;
        
        //This leads to some weirdness of three repeated images.
        //cvtColor(out, out, CV_BGR2GRAY);   
        
        GaussianBlur(frame, out, cv::Size(7,7), 1.5, 1.5);
        
        for(int x = 0; x < frame.cols; x++){
            for(int y = 0; y < frame.rows; y++){
                cv::Vec3b color = out.at<cv::Vec3b>(cv::Point(x,y));
                //Takes an average currently, needs changing.
                int c = (color[0] + color[1] + color[2])/3.0;
                img->updatePixel(x, y, c);
            }
        }
        
        if (train == 0){
            bs->trainBackground(img);
        }else if(train == 1){
            //learn track
        }
        else{
            //Does subtraction and sets all pixels to black.
            Image* subtract = bs->doFullBackSubtract(img);
            for(int x = 0; x < frame.cols; x++){
                for(int y = 0; y < frame.rows; y++){
                    if(subtract->getPixel(x,y) == 0){
                        cv::Vec3b color = frame.at<cv::Vec3b>(cv::Point(x,y));
                        color[0] = 0;
                        color[1] = 0;
                        color[2] = 0;
                        frame.at<cv::Vec3b>(cv::Point(x,y)) = color;
                    }
                }
            }
            
            
            delete subtract;
        }
        
        cv::rectangle(
        frame,
        cv::Point(5, 10),
        cv::Point(20, 30),
        cv::Scalar(0, 255, 255)
        );

        
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
        if (key == 49){ //1
            train = 1;
            std::cout << "Background Train Finished." << std::endl;
        }
        //49 is 1
        if (key == 50){ //2
            train = 2;
            std::cout << "Lap finished." << std::endl;
        }
    }

    return 0;
}
