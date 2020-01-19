#ifndef IMAGE_GCODE_H
#define IMAGE_GCODE_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>

//using namespace cv;
using namespace std;

#define DISTANCE 20
//-----------------------------------【全局变量声明部分】--------------------------------------
//		描述：全局变量的声明
//-----------------------------------------------------------------------------------------------


//-----------------------------------【全局函数声明部分】--------------------------------------
//		描述：全局函数的声明
//-----------------------------------------------------------------------------------------------
cv::Mat Gcode(cv::Mat intial_imge);
void ProcesseImage(cv::Mat canny_output);//阈值改变
float CalculateDistacne(cv::Point Point1, cv::Point Point2);//获取距离
int CheckContours(vector< vector<cv::Point> > vContours, int *use_vContours, unsigned int check);
void plan(vector< vector<cv::Point> > &lunkuo, cv::Mat & chushitu, FILE *fp);//路径规划
double CalculateDistance(cv::Point& Point1, cv::Point& Point2);//获取距离
double * pointandlunkuo(cv::Point& point, vector< vector<cv::Point> > & lunkuo, int w[], int n);
void huaxian(int qdlknum, int qddnum, int zdlknum, int zddnum, cv::Mat & yhqiepian, vector< vector<cv::Point> > &lunkuo, FILE *fp);
int  * paixu(int p, int xuhao, cv::Mat & yhqiepian, vector< vector<cv::Point> > &lunkuo, int lunkuonum[], int lm, int diannum1[], int dm, int qidianlunkuo[], int qlm, int  qidiandian[], int qdm);
void fuzhu(int lunkuonum[], int lm, int diannum1[], int dm, int qidianlunkuo[], int qlm, int  qidiandian[], int qdm);

#define hh 2
#endif
