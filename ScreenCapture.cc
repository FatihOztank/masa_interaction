#include <iostream>
#include "Screenshot.h"
#include "AvatarManager.h"

int main(int argc, char const *argv[])
{
    try{
        ScreenShot screen(argv[1]);
        cv::Mat img;
        cv::Point pt;
        
        Avatar user(100,250);
        while(true) 
        {
            screen(img);
            cv::circle(img, user.point, 20,user.color,cv::FILLED); // thats how you draw shit
            cv::imshow("img", img);
            
            char k = cv::waitKey(1);
            if (k == 'q')
                break;
        }
    } catch(const char * msg){
        std::cerr << msg << std::endl;
    }

    
}