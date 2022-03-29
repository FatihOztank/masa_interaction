# pragma once

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <vector>
#include <random>
#include <iostream>

int genRandInt(int min, int max){
    std::random_device rd; // obtain a random number from hardware
    //std::mt19937 rng(rd); // seed the generator
    std::uniform_int_distribution<int> uni(min,max);
    return uni(rd);
}

cv::Scalar getRandomColor(){
    return cv::Scalar(genRandInt(0,255),genRandInt(0,255),genRandInt(0,255));
}

struct Avatar{
    cv::Point point;
    cv::Scalar color;
    
    Avatar(int x, int y){
        point.x = x;
        point.y = y;
        color = getRandomColor();
    }
};

/*
class AvatarManager{
std::vector<Avatar> avatars;

public:



};
*/