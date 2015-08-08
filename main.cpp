//
//  main.cpp
//  lloydmax
//
//  Created by Abhishek Kumar Thakur on 4/28/13.
//  Copyright (c) 2013 abhishek thakur. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;




void showHistogram(Mat& img)
{
	int bins = 256;             // number of bins
	int nc = img.channels();    // number of channels
    
	vector<Mat> hist(nc);       // histogram arrays
    
	for (int i = 0; i < hist.size(); i++)
		hist[i] = Mat::zeros(1, bins, CV_32SC1);

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			for (int k = 0; k < nc; k++)
			{
				uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
				hist[k].at<int>(val) += 1;
			}
		}
	}
	int hmax[3] = {0,0,0};
	for (int i = 0; i < nc; i++)
	{
		for (int j = 0; j < bins-1; j++)
			hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
	}
    
	const char* wname[3] = { "blue", "green", "red" };
	Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };
    
	vector<Mat> canvas(nc);
	for (int i = 0; i < nc; i++)
	{
		canvas[i] = Mat::ones(500, 500, CV_8UC3);
        
		for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
		{
			line(
                 canvas[i],
                 Point(j, rows),
                 Point(j, rows - (hist[i].at<int>(j) * rows/hmax[i])),
                 nc == 1 ? Scalar(100,200,200) : colors[i],
                 4, 16, 0
                 );
		}
        
		imshow(nc == 1 ? "value" : wname[i], canvas[i]);
	}
}

int main(int argc, const char * argv[])
//int old()
{
    Mat image = imread("/Users/abhishek/Documents/image\ processing/bauckhage-gamma-1.png",CV_LOAD_IMAGE_GRAYSCALE);
    
    //showHistogram(image);
    Mat TempImage = image.clone();
    
	int nRows = TempImage.rows;
	int nCols = TempImage.cols;
    
	uchar* p;
	int i,j,k;
	float histo[256] = {0.0}; //Histogram Initialization
	for(i=0;i<nRows;++i)
	{
		p = TempImage.ptr<uchar>(i);
		for(j=0;j<nCols;++j){
			if (p[j]!= 0){
				histo[p[j]] += 1;
			}
		}
	}
    
	int number_levels = 8;
	int* RepGray = new int[number_levels];
    
	
	for(i=0;i<number_levels;i++)
		*(RepGray + i) = (i+1)*255/(number_levels+1);
    
	int* Thres = new int[number_levels + 1];
	*(Thres) = 0;					//initialize first and last threshold
	*(Thres + number_levels) = 256;
    
	int flag;
    int count = 0;
	do{
        
		cout << "Gray Values" << endl;
		for(i=0;i<number_levels;i++)
			cout << "\t" << *(RepGray + i);
		cout << endl;
        
		for(i=1;i<number_levels;i++)
			*(Thres + i) = *(RepGray + (i-1)) + floor((*(RepGray + (i-1) + 1) - *(RepGray + (i-1)))/2.0);
        
		cout << "Threshold Values" << endl;
		for(i=0;i<number_levels+1;i++)
			cout << "\t"<< *(Thres + i);
		cout << endl;
        
		// Calculate New Pixel Values
        // Minimizing MSE
		flag = 0;
        float numerator = 0;
        float denominator = 0;
        int temp;
		for(i=0;i<number_levels;i++){
			numerator = 0;
			denominator = 0;
			//int temp;
			for(j = *(Thres+i)+1; j<= *(Thres+i+1) ; j++){
				numerator += j * (histo[j]);
				denominator += histo[j];
			};
			if(denominator != 0){
				temp = round(numerator/denominator);
				if(*(RepGray + i) - temp != 0){
					flag = 1;
                    
					*(RepGray + i) = temp;
				}
			}
            
            count++;
		}
        //printf("error:%d",temp);
        	}while(flag == 1);
    printf("%d", count);
    //create image from new histogram
	for(i=0;i<nRows;++i)
	{
		p = TempImage.ptr<uchar>(i);
		for(j=0;j<nCols;++j){
			for(k=0;k<number_levels;k++)
				if(p[j]<=*(Thres +k +1)){
					p[j] = *(RepGray+k);
					break;
				}
		}
	}
	namedWindow("Output",CV_WINDOW_AUTOSIZE);
	imshow("Output", TempImage);
    
    imshow("input", image);
    
    showHistogram(TempImage);
    
    waitKey(0);
    
    return 0;
}

