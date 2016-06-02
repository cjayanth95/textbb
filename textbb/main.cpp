//
//  main.cpp
//  textbb
//
//  Created by Adobe on 02/06/16.
//  Copyright © 2016 Adobe. All rights reserved.
//
//  This is an all inclusive function to get all the images in one go.
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
std::vector<cv::Rect> detectLetters1(cv::Mat img , int x , int y,  int s)
{
    Mat bwImage;
    cv::cvtColor(img, bwImage, CV_RGB2GRAY);
    std::vector<cv::Rect> boundRect;
    cv::Mat img_gray, img_sobel, img_threshold, element;
    cvtColor(img, img_gray, CV_BGR2GRAY);
    cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(x, y) );
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element); //Does the trick
    std::vector< std::vector< cv::Point> > contours;
    //cv::findContours(img_threshold, contours, 0, 1);
    vector<Vec4i> hierarchy;
    
    cv::findContours( bwImage, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
//    for( int i = 0; i < contours.size(); i++ )
//        //if (contours[i].area()>s)
//        if (contourArea(contours[i])>s)
//        {
//            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
//            cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
//            if (appRect.width>appRect.height)
//                boundRect.push_back(appRect);
//        }
    //cout<<boundRect.size()<<endl;
    for( int i = 0; i < contours.size(); i++ ){
        cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
         boundRect.push_back(appRect);
    }
            //if (contours[i].area()>s)
    

    return boundRect;
}
std::vector<cv::Rect> detectLetters(cv::Mat img , int x , int y,  int s)
{
    //    Mat bwImage;
    //    cv::cvtColor(img, bwImage, CV_RGB2GRAY);
    std::vector<cv::Rect> boundRect;
    cv::Mat img_gray, img_sobel, img_threshold, element;
    cvtColor(img, img_gray, CV_BGR2GRAY);
    cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(x, y) );
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element); //Does the trick
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1);
    //vector<Vec4i> hierarchy;
    
    //cv::findContours( bwImage, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
        //if (contours[i].area()>s)
        if ((contours[i]).size()>s)
        {
            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
            cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
            if (appRect.width>appRect.height)
                boundRect.push_back(appRect);
        }
    //cout<<boundRect.size()<<endl;
    return boundRect;
}

int main(int argc, const char * argv[]) {
    cv::Mat img_gray, img_dilate, img_threshold ,img_dilate_gray,element;
    //Read
    cv::Mat img1=cv::imread("4.jpg");
    cv::Mat img2=cv::imread("4.jpg");
    cv::Mat img3= img1.clone();
    img3.setTo(255);
    //Detect
    std::vector<cv::Rect> letterBBoxes1=detectLetters(img1 ,17 , 3 ,100);
    //std::vector<cv::Rect> letterBBoxes2=detectLetters(img2, 17 ,3,100);
    cvtColor(img1, img_gray, CV_BGR2GRAY);
    cv::threshold(img_gray, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    //cv::threshold(img_gray,img_threshold,200,255,cv::THRESH_BINARY);
    for(int i=0; i< letterBBoxes1.size(); i++){
        img_threshold(letterBBoxes1[i] ).setTo(255);
        img3(letterBBoxes1[i] ).setTo(0);
    }
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(20, 3) );
    dilate(img_threshold, img_dilate, element, Point(-1, -1), 100, 1, 1);
    cv::imwrite( "imgdilated.jpg", img_dilate);
    cv::imwrite( "text.jpg", img3);
//    
//    for(int i=0; i< letterBBoxes2.size(); i++)
//        cv::rectangle(img2,letterBBoxes2[i],cv::Scalar(0,255,0),3,8,0);
//        cv::imwrite( "imgOut46.jpg", img3);
    cv::cvtColor(img_dilate, img_dilate_gray, CV_GRAY2BGR);
    std::vector<cv::Rect> letterBBoxes2=detectLetters(img_dilate_gray, 140,20,100);
    for(int i=0; i< letterBBoxes2.size(); i++){
        cv::rectangle(img2,letterBBoxes2[i],cv::Scalar(0,255,255),3,8,0);
        img3(letterBBoxes2[i] ).setTo(255);
    }
    cv::imwrite( "text.jpg", img3);
    //cv::imwrite( "imgOut10.jpg", img2);
    threshold(img3,img3,254,255,THRESH_BINARY_INV);
    dilate(img3, img3, Mat(), Point(-1, -1), 2, 1, 1);
    threshold(img3,img3,254,255,THRESH_BINARY_INV);
    cv::imwrite( "dilated.jpg", img3);
    std::vector<cv::Rect> letterBBoxes3=detectLetters1(img3, 350,50,10000);
    for(int i=0; i< letterBBoxes3.size(); i++)
        cv::rectangle(img2,letterBBoxes3[i],cv::Scalar(255,0,0),3,8,0);
    cv::imwrite( "imgOut10.jpg", img2);
    
    
    
    
    
    return 0;
}
























