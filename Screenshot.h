#pragma once

#include <opencv2/opencv.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <string>
#include <algorithm>
#define ACTIVE_WINDOWS "_NET_CLIENT_LIST"

Window getWindowByName(std::string window_name, Window list[],
         unsigned long numItems, Display * display){

    Window window; 
    for (int i = 0; i < numItems; ++i) {
        XTextProperty test;
        int status = XGetWMName(display, list[i], &test);
        
        char *windowName = reinterpret_cast<char * >(test.value);
        std::string windowNameStr(windowName);
        std::for_each(windowNameStr.begin(), windowNameStr.end(), [](char & c) {
            c = ::tolower(c);
        });
        //std::cout << list[i] << " " << windowNameStr << " " << status << "\n";
        
        if (windowNameStr.find(window_name) != std::string::npos) {
            window = list[i];
            //std::cout << window << "\n";
            return window;
        }
    }
    return -1;
}


class ScreenShot
{
    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img{nullptr};
public:
    ScreenShot(int x, int y, int width, int height):
        x(x),
        y(y),
        width(width),
        height(height)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);    
    }

    ScreenShot(std::string window_name) : x(0), y(0)
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
        Atom atom = XInternAtom(display, ACTIVE_WINDOWS, true);
        Atom actualType;
        int format;
        unsigned long numItems;
        unsigned long bytesAfter;
        
        unsigned char *data = reinterpret_cast<unsigned char * >('\0');
        Window *windowList;    

        int status = XGetWindowProperty(display, root, atom, 0L, (~0L), false,
            AnyPropertyType, &actualType, &format, &numItems, &bytesAfter, &data);
        
        windowList = (Window *)data;

        Window xid = getWindowByName(window_name, windowList, numItems, display);
        if(xid == -1){
            throw "Specified window can't be found! \n";
        }
        XWindowAttributes attr;
        XGetWindowAttributes(display,xid,&attr);
        
        //std::cout << attr.x << " " << attr.y << " " << attr.width << " " << attr.height << "\n";
        this->width = attr.width;
        this->height = attr.height;
        this->root = xid;

        delete [] windowList;
    }

    void operator() (cv::Mat& cvImg)
    {
        if(img != nullptr)
            XDestroyImage(img);

        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        cvImg = cv::Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot()
    {
        if(img != nullptr)
            XDestroyImage(img);
        XCloseDisplay(display);
    }
};
