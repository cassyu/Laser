/*
   laser  engraving Gcode generator 
   version V0.0
   date   2019/10/18
   author Zhang Linbo
*/

//#include "pch.h"
#include "LaserGcodeGenerate.h"
#include <fstream>
#include <math.h>
#include <QMessageBox>
#include <QtCore>
//#include <QtWidgets>
#include <QLabel>
#include <QDateTime>
#include <QObject>
//#include "WinNT.h"
//#include <tchar.h>
#include "opencv2/imgproc/types_c.h"
#include <QFile>

int G_global_errorDiffusion = 0;
int G_global_ContourDetect =0;
int G_global_GetContours = 0;
int G_global_ButtonType = 0;
int G_global_LaserPower = 0;
int G_global_Threshold = 10;
//构造函数
LaserGcodeGenerate::LaserGcodeGenerate()
{
	//参数默认值
	errorDiffusion = 1;
	ImageMaxSize = 1000; //图片最大尺寸
	CannyLowThreshold = 100;
	CannyHighThreshold = 200;
	ContoursDetect = 1;//边缘检测算法 选择
	getContours = 1;

}
//析构函数
LaserGcodeGenerate::~LaserGcodeGenerate()
{


}
int LaserGcodeGenerate::getColSum(cv::Mat src, int col)
{
	int sum = 0;
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i < height; i++)
	{
		sum = sum + src.at<uchar>(i, col);
		//sum = sum + (255 -src.at<uchar>(i, col));
	}
	printf("%d\r\n", sum);
	return  sum;
}

int LaserGcodeGenerate::getRowSum(cv::Mat src, int row)//统计所有行的总和:
{
	//cv::imshow(u8"leftImg", src);
	int sum = 0;
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i < width; i++)
	{
		sum = sum + src.at<uchar>(row, i);
		//sum = sum + (255 -src.at<uchar>(row, i));
	}

	return sum;
}
void LaserGcodeGenerate::cutTop(cv::Mat & src, cv::Mat & dstImg)
{
	int top, bottom;
	top = 0;
	bottom = src.rows;
	int i;
	for (i = 0; i < src.rows; i++)
	{
		int colValue = getRowSum(src, i);
		if (colValue > 0)
		{
			top = i;
			break;
		}
	}

	for (; i < src.rows; i++)
	{
		int colValue = getRowSum(src, i);
		if (colValue == 0)
		{
			bottom = i;
			break;
		}
	}

	int height = bottom - top;
	cv::Rect rect(0, top, src.cols, height);
	dstImg = src(rect).clone();

	//cv::imshow("dstImg", dstImg);
}
int LaserGcodeGenerate::cutLeft(cv::Mat& src, cv::Mat& leftImg, cv::Mat& rightImg)
{
	int left, right;
	left = 0;
	right = src.cols;

	int i;
	for (i = 0; i < src.cols; i++)
	{
		int colValue = getColSum(src, i);
		if (colValue > 0)
		{
			left = i;
			break;
		}
	}
	if (left == 0)
	{
		leftImg = src;
		return 1;
	}
	for (; i < src.cols; i++)
	{
		int colValue = getColSum(src, i);
		if (colValue == 0)
		{
			right = i;
			break;
		}
	}
	int width = right - left;
	cv::Rect rect(left, 0, width, src.rows);
	leftImg = src(rect).clone();

	//cv::imshow(u8"leftImg", leftImg);

	cv::Rect rectRight(right, 0, src.cols - right, src.rows);
	rightImg = src(rectRight).clone();

	cutTop(leftImg, leftImg);
	return 0;
}
//二值图像Gcode 入口函数
void LaserGcodeGenerate::getBinaryImageGcode(cv::Mat src)
{
	//blur(src, src, Size(3, 3));
	//高斯滤波  对图像平滑的处理  并保留图像的总体灰度分布特征 
	cv::GaussianBlur(src, src, cv::Size(9, 9), 0, 0);
	src = SMMirror(src);
	//Gray = (R*299 + G*587 + B*114 + 500) / 1000  
	//各个像素采取不同的比值  
	cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	

	cv::Mat BinaryMat = cv::Mat::zeros(src.size(), CV_8UC1);
	BinaryMat = BinaryImage(src); //二值化处理图像
     vector<LINE> BinaryLine = getTrack(BinaryMat);
	//vector<Point> BinaryPoint = GetPoint(BinaryMat);//根据处理后的图片获得轨迹点
	//vector<LINE> BinaryLine = getLine(BinaryPoint);//根据轨迹点获得轨迹线

	namedWindow(u8"二值图像", cv::WINDOW_AUTOSIZE);
    cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC3);
    drawLine(BinaryLine, out1);
    imshow(u8"二值图像", out1);

	//GenerateGcode(BinaryLine); //根据轨迹线产生Gcode
	if (GeberateGcodeWithPath(BinaryLine, filename))
	{
		QMessageBox::information(nullptr, "Title", QObject::tr("gcode生成成功"));
	}
	else
	{
		QMessageBox::information(nullptr, "Title", QObject::tr("gcode生成失败"));
	}

}
//字体和黑白图像 入口函数
void LaserGcodeGenerate::getTextGcode(cv::Mat src) //输入黑白图像
{
	src = SMMirror(src);
	cvtColor(src, src,cv::COLOR_BGR2GRAY);//转化为灰度图像	
	if (src.depth() != CV_8UC1)
	{
		printf("需要输入黑白图像\r\n");
		return;
	}
	vector<LINE> GrayLine = getTrack(src);

	//vector<Point> GrayPoint = GetPoint(src);
	//vector<LINE> GrayLine = getLine(GrayPoint);


	namedWindow(u8"黑白图像", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC3);
	drawLine(GrayLine, out1);
	imshow(u8"黑白图像", out1);

	if (GeberateGcodeWithPath(GrayLine, filename))
	{
		QMessageBox::information(nullptr, "Title", QObject::tr("gcode生成成功"));
	}
	else
	{
		QMessageBox::information(nullptr, "Title", QObject::tr("gcode生成失败"));
	}

}
//灰度图像入口函数
void LaserGcodeGenerate::getGrayImageGcode(cv::Mat src)
{
	blur(src, src, cv::Size(3, 3));
	//GaussianBlur(src, src, Size(9, 9), 0, 0);
	//cv::imwrite("in1.jpg", src);
	cv::Mat leftsrc;
	cv::Mat rightsrc;
	cv::Mat origin;
	cv::Mat Matout1 = cv::Mat::zeros(src.size(), CV_8UC1);
	//origin = BinaryImage(src);
	cv::cvtColor(src, Matout1, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	Matout1 = 255 - Matout1;
	//cv::threshold(src, origin, 100, 255,CV_THRESH_BINARY);
	//cv::imshow("origin", Matout1);
	cv::imwrite("origin.jpg", Matout1);
	
	cutLeft(Matout1, leftsrc, rightsrc);

	//cutLeft(origin, leftsrc,rightsrc);
	cv::imshow("origin", leftsrc);
	//cv::imwrite("in3.jpg",leftsrc);
	//cv::imwrite("in4.jpg", rightsrc);
	//cv::imshow("rightsrc", rightsrc);
	//cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	//cv::imwrite("in2.jpg", src);
	//resize(src, src, Size(src.cols * 0.5, src.rows * 0.5));
	cv::Mat midImage = GrayScale(leftsrc);  //灰度图像处理
	midImage = 255 - midImage;

	//vector<Point> GrayPoint = GetPoint(midImage);
	//vector<LINE> GrayLine = getLine(GrayPoint);

	vector<LINE> GrayLine = getGrayTrack(midImage);
	//结果显示
	//cv::namedWindow(u8"gray", cv::WINDOW_AUTOSIZE);
    cv::Mat out1 = cv::Mat::zeros(midImage.size(),CV_8UC1);
    drawLine(GrayLine,out1);
	//out1 = 255 - out1;
	//resize(out1, out1, src.size());
    cv::imshow(u8"grayImage", out1);
	//cv::imwrite("out1.jpg", out1);


	//GenerateGcode(GrayLine);
	if (GeberateGcodeWithPath(GrayLine, filename))
	{
		QMessageBox::information(nullptr, "Title", QObject::tr(u8"gcode生成成功"));
	}
	else
	{
		QMessageBox::information(nullptr, "Title", QObject::tr("gcode生成失败"));
	}

}
//轮廓图像入口函数
void LaserGcodeGenerate::getContoursGcode(cv::Mat src)
{

	blur(src, src, cv::Size(3, 3));
	src = SMMirror(src);
	cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像

	ContoursDetect = G_global_ContourDetect;
	getContours = G_global_GetContours;

	vector<LINE> ContoursLine = getContoursLine(src);
	vector<LINE> OutLine ;
	if (getContours == 1)
	{
	     OutLine = OptimizeLineSort(ContoursLine);
	}
	else 
	{
		OutLine = ContoursLine;
	}


	printf("contours Size;%d\r\n", OutLine.size());
	cv::namedWindow(u8"轮廓图像", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC1);
    drawLine(OutLine,out1);
	cv::imshow(u8"轮廓图像", out1);

	//GenerateGcode(OutLine);
	if (GeberateGcodeWithPath(OutLine, filename))
	{
		QMessageBox::information(nullptr, "Title", QObject::tr("gcode生成成功"));
	}
	else
	{
		QMessageBox::information(nullptr, "Title", QObject::tr("gcode生成失败"));
	}


}

void LaserGcodeGenerate::getGcodeWithId(cv::Mat src, int id)
{
     switch (id)
	 {
		case 0:
			getBinaryImageGcode(src);
		 break;
		case 3:
			getTextGcode(src);
		  break;
		case 1:
			getGrayImageGcode(src);
			break;
		case 2:
			getContoursGcode(src);
			break;
	 }
}


//设置参数
void LaserGcodeGenerate::SetParameter(int errorDiffusion,int ImageMaxSize,int CannyLowThreshold,int CannyHighThreshold,
int ContoursDetect,int getContours)
{
	this->errorDiffusion = errorDiffusion;
	this->ImageMaxSize = ImageMaxSize;
	this->CannyLowThreshold = CannyLowThreshold;
	this->CannyHighThreshold = CannyHighThreshold;
	this->ContoursDetect = ContoursDetect;
	this->getContours = getContours;
	// TODO: 在此处添加实现代码.
}
void LaserGcodeGenerate::SetFilename(QString & name)
{
	filename = name;
}
// 图像灰度处理   
cv::Mat LaserGcodeGenerate::GrayScale(cv::Mat src)
{
	cv::Mat dst1=cv::Mat::zeros(src.size(), CV_8UC1);
	//半色调算法 误差消抖
	errorDiffusion = G_global_errorDiffusion;
	switch (errorDiffusion)
	{
		case 0:
		{
			dst1 = FloydSteinburg(src);
			break;
		}
		case 1:
		{
			dst1 = Burkers(src);
			break;
		}
		case 2:
		{
			dst1 = Sierra(src);
			break;
		}
		case 3:
		{
			dst1 = Stucki(src);
			break;
		}
		case 4:
		{
			dst1 = Jarvis(src);
			break;
		}
		case 5:
		{
			dst1 = Stevenson(src);
			break;
		}
	}
		

	cv::Mat dst= cv::Mat::zeros(cv::Size(src.cols*2,src.rows*2), CV_8UC1);


		for (int row = 0; row < dst1.rows; row++)
		{
		
			for (int col = 0; col < dst1.cols-4; col=col+2)
			{
			   int value = dst1.at<uchar>(row, col);
			   switch (value)
			   {  
				   case 0:
				   {
					   dst.at<uchar>(row*2 , col * 2) = 0;
					   dst.at<uchar>(row * 2, col * 2 + 1) = 0;
					   dst.at<uchar>(row * 2, col * 2 + 2) = 0;
					   dst.at<uchar>(row * 2, col * 2 + 3) = 0;

					   break;
				   }
				   case 31:
				   {
					   if (col % 2 == 0)
					   {
						   dst.at<uchar>(row * 2, col * 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 0;
					   }
					   else
					   {
						   dst.at<uchar>(row * 2, col * 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 0;
					   }

					   break;
				   }
				   case 63:
				   {
					   if (col % 2 == 0)
					   {
						   dst.at<uchar>(row * 2, col * 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 0;
					   }
					   else
					   {
						   dst.at<uchar>(row * 2, col * 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 255;
					   }

					   break;
				   }
				   case 95:
				   {
					   if (col % 2 == 0)
					   {
						   dst.at<uchar>(row * 2, col * 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 0;
					   }
					   else
					   {
						   dst.at<uchar>(row * 2, col * 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 255;
					   }

					   break;
				   }
				   case 127:
				   {
					   if (col % 2 == 0)
					   {
						   dst.at<uchar>(row * 2, col * 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 0;
					   }
					   else
					   {
						   dst.at<uchar>(row * 2, col * 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 255;
					   }

					   break;
				   }
				   case 159:
				   {
					   if (col % 2 == 0)
					   {
						   dst.at<uchar>(row * 2, col * 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 0;
					   }
					   else
					   {
						   dst.at<uchar>(row * 2, col * 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 255;
					   }

					   break;
				   }
				   case 191:
				   {
					   if (col % 2 == 0)
					   {
						   dst.at<uchar>(row * 2, col * 2) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 255;
					   }
					   else
					   {
						   dst.at<uchar>(row * 2, col * 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 1) = 0;
						   dst.at<uchar>(row * 2, col * 2 + 2) = 255;
						   dst.at<uchar>(row * 2, col * 2 + 3) = 255;
					   }

					   break;
				   }
				   case 223:
				   {
					   dst.at<uchar>(row * 2, col * 2) = 255;
					   dst.at<uchar>(row * 2, col * 2 + 1) = 255;
					   dst.at<uchar>(row * 2, col * 2 + 2) = 255;
					   dst.at<uchar>(row * 2, col * 2 + 3) = 255;
					   break;
				   }

			   }

			}

		}



	//dst = 255 - dst;

	return dst;
}
uchar LaserGcodeGenerate::valueSection(uchar input)
{
	if ((input < 0) || (input > 255))
	{
		printf("valueSection input value error!\r\n");
		return -1;
	}


	if (input < 31)
	{
		return 0;
	}
	else if ((input >= 31) && (input < 63))
	{
		return 31;
	}
	else if ((input >= 63) && (input < 95))
	{
		return 63;
	}
	else if ((input >= 95) && (input < 127))
	{
		return 95;
	}
	else if ((input >= 127) && (input < 159))
	{
		return 127;
	}
	else if ((input >= 159) && (input < 191))
	{
		return 159;
	}
	else if ((input >= 191) && (input < 223))
	{
		return 191;
	}
	else 
	{
		return 223;
	}

}

//半色调算法 中的抖动误差法 6个 误差分配的6种方式
//误差扩散法 
cv::Mat LaserGcodeGenerate::FloydSteinburg(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	for (int i = 0; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j=j+2)
		{
			dst.at<uchar>(i, j) = valueSection((src.at<uchar>(i, j)+ src.at<uchar>(i, j+1))/2);
			dst.at<uchar>(i, j + 1) = dst.at<uchar>(i, j);
			//printf("dst.at<uchar>(i, j):%d\r\n", dst.at<uchar>(i, j));
			wc = src.at<uchar>(i, j) - dst.at<uchar>(i, j);
			//printf("wc:%d\r\n", wc);
			//printf("src.at<uchar>(i, j + 1):%d\r\n", src.at<uchar>(i, j + 1));
			src.at<uchar>(i, j + 1) = ControlValue(src.at<uchar>(i, j + 1) + wc * 7 / 16);
			//printf("src.at<uchar>(i, j + 1):%d wc * 7 / 16:%d \r\n", src.at<uchar>(i, j + 1), wc * 7 / 16);
			src.at<uchar>(i + 1, j - 1) = ControlValue(src.at<uchar>(i + 1, j - 1) + wc * 3 / 16);
			src.at<uchar>(i + 1, j) = ControlValue(src.at<uchar>(i + 1, j) + wc * 5 / 16);
			src.at<uchar>(i + 1, j + 1) = ControlValue(src.at<uchar>(i + 1, j + 1) + wc * 1 / 16);
		}

	}

	return dst;
}
//误差扩散法  之 Burkers
cv::Mat LaserGcodeGenerate::Burkers(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	for (int i = 0; i < src.rows - 2; i++)
	{
		for (int j = 2; j < src.cols - 2; j=j+2)
		{
			dst.at<uchar>(i, j) = valueSection((src.at<uchar>(i, j) + src.at<uchar>(i, j + 1)) / 2);
			dst.at<uchar>(i, j + 1) = dst.at<uchar>(i, j);

			wc =src.at<uchar>(i, j) - dst.at<uchar>(i, j);
			src.at<uchar>(i, j + 1) = ControlValue((uchar)(src.at<uchar>(i, j + 1) +int( wc * 8 / 32)));
			src.at<uchar>(i, j + 2) = ControlValue((uchar)(src.at<uchar>(i, j + 2) +int( wc * 4 / 32)));	
			src.at<uchar>(i + 1, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 2) +int( wc * 2 / 32)));
			src.at<uchar>(i + 1, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 1) + int(wc * 4 / 32)));
			src.at<uchar>(i + 1, j) = ControlValue((uchar)(src.at<uchar>(i + 1, j) +int( wc * 8 / 32)));
			src.at<uchar>(i + 1, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 1) +int( wc * 4 / 32)));
			src.at<uchar>(i + 1, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 2) + int(wc * 2 / 32)));
		}

	}
	return dst;
}
//误差扩散法 之 sierra过滤器 
cv::Mat LaserGcodeGenerate::Sierra(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	for (int i = 0; i < src.rows - 2; i++)
	{
		for (int j = 2; j < src.cols - 2; j=j+2)
		{

			dst.at<uchar>(i, j) = valueSection((src.at<uchar>(i, j) + src.at<uchar>(i, j + 1)) / 2);
			dst.at<uchar>(i, j + 1) = dst.at<uchar>(i, j);

			wc = src.at<uchar>(i, j) - dst.at<uchar>(i, j);
			src.at<uchar>(i, j + 1) = ControlValue((uchar)(src.at<uchar>(i, j + 1) + wc * 5 / 32));
			src.at<uchar>(i, j + 2) = ControlValue((uchar)(src.at<uchar>(i, j + 2) + wc * 3 / 32));
			src.at<uchar>(i + 1, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 2) + wc * 2 / 32));
			src.at<uchar>(i + 1, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 1) + wc * 4 / 32));
			src.at<uchar>(i + 1, j) = ControlValue((uchar)(src.at<uchar>(i + 1, j) + wc * 5 / 32));
			src.at<uchar>(i + 1, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 1) + wc * 4 / 32));
			src.at<uchar>(i + 1, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 2) + wc * 2 / 32));
			src.at<uchar>(i + 2, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 1) + wc * 2 / 32));
			src.at<uchar>(i + 2, j) = ControlValue((uchar)(src.at<uchar>(i + 2, j) + wc * 3 / 32));
			src.at<uchar>(i + 2, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 1) + wc * 2 / 32));
		}

	}
	return dst;
}
//
//误差扩散法 之 stucki过滤器 
cv::Mat LaserGcodeGenerate::Stucki(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	for (int i = 0; i < src.rows - 2; i++)
	{
		for (int j = 2; j < src.cols - 2; j=j + 2)
		{
			dst.at<uchar>(i, j) = valueSection((src.at<uchar>(i, j) + src.at<uchar>(i, j + 1)) / 2);
			dst.at<uchar>(i, j + 1) = dst.at<uchar>(i, j);

			wc = src.at<uchar>(i, j) - dst.at<uchar>(i, j);
			src.at<uchar>(i, j + 1) = ControlValue((uchar)(src.at<uchar>(i, j + 1) + wc * 7 / 42));
			src.at<uchar>(i, j + 2) = ControlValue((uchar)(src.at<uchar>(i, j + 2) + wc * 5 / 42));
			src.at<uchar>(i + 1,j - 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 2) + wc * 3 / 42));
			src.at<uchar>(i + 1, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 1) + wc * 5 / 42));
			src.at<uchar>(i + 1, j ) = ControlValue((uchar)(src.at<uchar>(i + 1, j) + wc * 7 / 42));
			src.at<uchar>(i + 1, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 1) + wc * 5 / 42));
			src.at<uchar>(i + 1, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 2) + wc * 3 / 42));
			src.at<uchar>(i + 2, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 2) + wc * 1 / 42));
			src.at<uchar>(i + 2, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 1) + wc * 3 / 42));
			src.at<uchar>(i + 2, j ) = ControlValue((uchar)(src.at<uchar>(i + 2, j) + wc * 5 / 42));
			src.at<uchar>(i + 2, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 1) + wc * 3 / 42));
			src.at<uchar>(i + 2, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 2) + wc * 1 / 42));
		
		
		}

	}
	return dst;
}
//误差扩散法 之 jarvis过滤器 
cv::Mat LaserGcodeGenerate::Jarvis(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	for (int i = 0; i < src.rows - 2; i++)
	{
		for (int j = 2; j < src.cols - 2; j = j + 2)
		{
			dst.at<uchar>(i, j) = valueSection((src.at<uchar>(i, j) + src.at<uchar>(i, j + 1)) / 2);
			dst.at<uchar>(i, j + 1) = dst.at<uchar>(i, j);

			wc = src.at<uchar>(i, j) - dst.at<uchar>(i, j);
			src.at<uchar>(i, j + 1) = ControlValue((uchar)(src.at<uchar>(i, j + 1) + wc * 7 / 48));
			src.at<uchar>(i, j + 2) = ControlValue((uchar)(src.at<uchar>(i, j + 2) + wc * 5 / 48));
			src.at<uchar>(i + 1, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 2) + wc * 3 / 48));
			src.at<uchar>(i + 1, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 1) + wc * 5 / 48));
			src.at<uchar>(i + 1, j) = ControlValue((uchar)(src.at<uchar>(i + 1, j) + wc * 7 / 48));
			src.at<uchar>(i + 1, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 1) + wc * 5 / 48));
			src.at<uchar>(i + 1, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 2) + wc * 3 / 48));
			src.at<uchar>(i + 2, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 2) + wc * 1 / 48));
			src.at<uchar>(i + 2, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 1) + wc * 3 / 48));
			src.at<uchar>(i + 2, j) = ControlValue((uchar)(src.at<uchar>(i + 2, j) + wc * 5 / 48));
			src.at<uchar>(i + 2, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 1) + wc * 3 / 48));
			src.at<uchar>(i + 2, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 2) + wc * 1 / 48));
		}

	}
	return dst;
}
//误差扩散法 之 stevenson 过滤器 
cv::Mat LaserGcodeGenerate::Stevenson(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	for (int i = 0; i < src.rows - 3; i++)
	{
		for (int j = 3; j < src.cols - 3; j = j + 2)
		{
			dst.at<uchar>(i, j) = valueSection((src.at<uchar>(i, j) + src.at<uchar>(i, j + 1)) / 2);
			dst.at<uchar>(i, j + 1) = dst.at<uchar>(i, j);

			wc = src.at<uchar>(i, j) - dst.at<uchar>(i, j);
			src.at<uchar>(i, j + 2) = ControlValue((uchar)(src.at<uchar>(i, j + 2) + wc * 32 / 200));
			src.at<uchar>(i + 1, j - 3) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 3) + wc * 12 / 200));
			src.at<uchar>(i + 1, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 1) + wc * 26 / 200));
			src.at<uchar>(i + 1, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 1) + wc * 30 / 200));
			src.at<uchar>(i + 1, j + 3) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 3) + wc * 16 / 200));
			src.at<uchar>(i + 2, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 2) + wc * 12 / 200));
			src.at<uchar>(i + 2, j) = ControlValue((uchar)(src.at<uchar>(i + 2, j) + wc * 26 / 200));
			src.at<uchar>(i + 2, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 2) + wc * 12 / 200));
			src.at<uchar>(i + 3, j - 3) = ControlValue((uchar)(src.at<uchar>(i + 3, j - 3) + wc * 5 / 200));
			src.at<uchar>(i + 3, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 3, j - 1) + wc * 12 / 200));
			src.at<uchar>(i + 3, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 3, j + 1) + wc * 12 / 200));
			src.at<uchar>(i + 3, j + 3) = ControlValue((uchar)(src.at<uchar>(i + 3, j + 3) + wc * 5 / 200));
		}

	}
	return dst;
}

//获得轨迹点  灰度值为0的点  二值化处理
vector<cv::Point> LaserGcodeGenerate::GetPoint(cv::Mat src)
{

	vector<cv::Point> PointSet;
	PointSet.clear();
	for (int row = 0; row < src.rows; row++)
	{
		for (int col = 0; col < src.cols; col++)
		{
			if (src.at<uchar>(row, col) == 0)
			{
				cv::Point p;
				p.x = col;
				p.y = row;
				PointSet.push_back(p);
			}
		    
		}
	}

	return PointSet;
}

vector<LINE> LaserGcodeGenerate::getTrack(cv::Mat src)
{
	LINE L;
	vector<LINE>LineSet;
	bool LineFlag=true;
	for (int row = 0; row < src.rows; row++)
	{
		if (row % 2 == 1)
		{
			for (int col = src.cols-1; col > 0; col--)
			{
				if (src.at<uchar>(row, col) == 0)//黑色
				{
					if (LineFlag)
					{
						L.start = cv::Point(col,row);
						LineFlag = false;
					}
					else
					{

					}

				}
				else
				{
					if (LineFlag)//没有线
					{

					}
					else//有线
					{
						L.end= cv::Point(col+1, row);
						LineSet.push_back(L);
						LineFlag = true;
					}
				}

			}
			if (!LineFlag)
			{
				L.end = cv::Point(0, row);
				LineSet.push_back(L);
				LineFlag = true;
			}
		}
		else
		{
			for (int col = 0; col < src.cols; col++)
			{
				if (src.at<uchar>(row, col) == 0)//黑色
				{
					if (LineFlag)
					{
						L.start = cv::Point(col, row);
						LineFlag = false;
					}
					else
					{

					}

				}
				else
				{
					if (LineFlag)
					{
		
					}
					else
					{
						L.end = cv::Point(col-1, row );
						LineSet.push_back(L);
						LineFlag = true;
					}

				}

			}
			if (!LineFlag)
			{
				L.end = cv::Point(src.cols-1, row);
				LineSet.push_back(L);
				LineFlag = true;
			}
		}


	}
	return LineSet;
}
vector<LINE> LaserGcodeGenerate::getGrayTrack(cv::Mat src)
{
	LINE L;
	vector<LINE>LineSet;
	bool LineFlag = true;
	bool order = true;
	for (int row = 0; row < src.rows; row = row + 2)
	{

		if (order)
		{
			order = false;
			for (int col = 0; col < src.cols; col++)
			{
				if (src.at<uchar>(row, col) == 0)//黑色
				{
					if (LineFlag)
					{
						L.start = cv::Point(col, row);
						LineFlag = false;
					}
					else
					{

					}

				}
				else
				{

					if (LineFlag)
					{

					}
					else
					{
						L.end = cv::Point(col - 1, row);
						LineSet.push_back(L);
						LineFlag = true;
					}

				}

			}
			if (!LineFlag)
			{
				L.end = cv::Point(src.cols - 1, row);
				LineSet.push_back(L);
				LineFlag = true;
			}
		}
		else
		{
			order = true;
			for (int col = src.cols - 1; col > 0; col--)
			{
				if (src.at<uchar>(row, col) == 0)//黑色
				{
					if (LineFlag)
					{
						L.start = cv::Point(col, row);
						LineFlag = false;
					}
					else
					{

					}

				}
				else
				{
					if (LineFlag)//没有线
					{

					}
					else//有线
					{
						L.end = cv::Point(col + 1, row);
						LineSet.push_back(L);
						LineFlag = true;
					}
				}

			}
			if (!LineFlag)
			{
				L.end = cv::Point(0, row);
				LineSet.push_back(L);
				LineFlag = true;
			}
		}


	}

	return LineSet;
}

//获得直线  二值化处理

vector<LINE> LaserGcodeGenerate::getLine(vector<cv::Point> PointSet)
{
	//生成轨迹	
	LINE L;
	vector<LINE>  LineSet;
	L.start = PointSet[0];
	for (int i = 0; i < (PointSet.size() - 1); i++)
	{
		//如果是相邻点的话 不入表  更新终点  
		if ((PointSet[i].y == PointSet[i + 1].y) && (PointSet[i].x == (PointSet[i + 1].x - 1)))
		{
			L.end = PointSet[i + 1];
		}
		else
		{
			L.end = PointSet[i];
			LineSet.push_back(L);
			L.start = PointSet[i + 1];
		}
	}

	return LineSet;
}

//二值化处理图像
cv::Mat LaserGcodeGenerate::BinaryImage(cv::Mat src)
{
	// TODO: 在此处添加实现代码.
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);


	int sum_gray = 0;
	int height = src.rows;
	int width = src.cols;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			int gray = src.at<uchar>(row, col);
			sum_gray += gray;
		}
	}
	int equal_sum_gray = sum_gray / (height*width);
	// 输入图像 输出图像  阈值  向上最大值  阈值话 操作的类型 
	//THRESH_TOZERO   大于阈值 为原像素  否则为0 
	cv::threshold(src, dst, equal_sum_gray, 255, cv::THRESH_TOZERO);//图像二值化
	//cvthreshold();
	//cv::threshold();
	
	return dst;
}

//根据轨迹线 产生Gcode
void LaserGcodeGenerate::GenerateGcode(vector<LINE> outLine)
{
	// TODO: 在此处添加实现代码.
	std::ofstream outfile;
	double scale = 10;
	cv::Point offset = cv::Point(0, 0);

	outfile.open("gcode2.gcode");
	if (outfile.fail())
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QObject::tr(u8"警告"));
		msgBox.setText(QObject::tr(u8"文件不存在"));
		msgBox.exec();
	}
	else 
	{
		outfile << "; G-code for laser engraving" << endl;
		outfile << "; Generated by lotmax" << endl;
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
		//outfile << "; Fri Sep 20 2019 10:49:34" << endl;
		outfile << current_date.toStdString() << endl;
		outfile << endl;
		outfile << "G90" << endl;
		outfile << "G21" << endl;
		outfile << "G28 XY" << endl;
		outfile << "; G-code START <<<" << endl;
		outfile << "G0 F2500" << endl;
		outfile << "G1 F500" << endl;
		//outfile << "M5" << endl;
		//输出轮廓G代码 重画轮廓
		for (int k = 0; k < outLine.size(); k++)
		{
			if (k == 0)
			{
				outfile << "G0 " << "X" << outLine[0].start.x / scale + offset.x << " Y" << outLine[0].start.y / scale + offset.y << endl;
				outfile << "M3" << endl;
				outfile << "G1 " << "X" << outLine[0].end.x / scale + offset.x << " Y" << outLine[0].end.y / scale + offset.y << endl;
			}
			else
			{
				if ((outLine[k - 1].end.x == outLine[k].start.x) && (outLine[k - 1].end.y == outLine[k].start.y))
				{
					outfile << "G1 " << "X" << outLine[k].end.x / scale + offset.x << " Y" << outLine[k].end.y / scale + offset.y << endl;
				}
				else
				{
					outfile << "M5" << endl;
					outfile << "G0 " << "X" << outLine[k].start.x / scale + offset.x << " Y" << outLine[k].start.y / scale + offset.y << endl;
					outfile << "M3" << endl;
					outfile << "G1 " << "X" << outLine[k].end.x / scale + offset.x << " Y" << outLine[k].end.y / scale + offset.y << endl;

				}
			}


		}
		outfile << "M5" << endl;
		outfile << endl;
		outfile << "; G-code END <<<" << endl;
		outfile.close();
	}
}

bool LaserGcodeGenerate::GeberateGcodeWithPath(vector<LINE> outLine, QString path)
{
	// TODO: 在此处添加实现代码.

	double scale = 10;
	cv::Point offset = cv::Point(80, 80);

	//源代码 
	//std::ofstream outfile;
	//outfile.open(path.toStdString());

	//解决中文路径1   wofstream
	//std::ofstream outfile;
	//std::wofstream  outfile;
	//outfile.open(path.toUtf8());

	//解决中文路径2    失败
	//std::locale::global(locale(""));
	//std::ofstream outfile;
	//outfile.open(path.toStdString());
	//std::locale::global(locale("C"));//还原全局区域设定

	//if (outfile.fail())
	//{
	//	QMessageBox msgBox;
	//	msgBox.setWindowTitle(QObject::tr(u8"警告"));
	//	msgBox.setText(QObject::tr(u8"文件不存在"));
	//	msgBox.exec();
	//	return false;
	//}
	//else
	//{
	//	outfile << "; G-code for laser engraving" << endl;
	//	outfile << "; Generated by lotmax" << endl;
	//	QDateTime current_date_time = QDateTime::currentDateTime();
	//	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
	//	//outfile << "; Fri Sep 20 2019 10:49:34" << endl;
	//	outfile << current_date.toStdString() << endl;
	//	outfile << endl;
	//	outfile << "G90" << endl;
	//	outfile << "G21" << endl;
	//	outfile << "G28 XY" << endl;
	//	outfile << "; G-code START <<<" << endl;
	//	outfile << "G0 F2500" << endl;
	//	outfile << "G1 F500" << endl;
	//	//outfile << "M5" << endl;
	//	//输出轮廓G代码 重画轮廓
	//	for (int k = 0; k < outLine.size(); k++)
	//	{
	//		if (k == 0)
	//		{
	//			outfile << "G0 " << "X" << outLine[0].start.x / scale + offset.x << " Y" << outLine[0].start.y / scale + offset.y << endl;
	//			outfile << "M3" << endl;
	//			outfile << "G1 " << "X" << outLine[0].end.x / scale + offset.x << " Y" << outLine[0].end.y / scale + offset.y << endl;
	//		}
	//		else
	//		{
	//			if ((outLine[k - 1].end.x == outLine[k].start.x) && (outLine[k - 1].end.y == outLine[k].start.y))
	//			{
	//				outfile << "G1 " << "X" << outLine[k].end.x / scale + offset.x << " Y" << outLine[k].end.y / scale + offset.y << endl;
	//			}
	//			else
	//			{
	//				outfile << "M5" << endl;
	//				outfile << "G0 " << "X" << outLine[k].start.x / scale + offset.x << " Y" << outLine[k].start.y / scale + offset.y << endl;
	//				outfile << "M3" << endl;
	//				outfile << "G1 " << "X" << outLine[k].end.x / scale + offset.x << " Y" << outLine[k].end.y / scale + offset.y << endl;

	//			}
	//		}


	//	}
	//	outfile << "M5" << endl;
	//	outfile << endl;
	//	outfile << "; G-code END <<<" << endl;
	//	outfile.close();
	//	return true;
	//}

	//试试qt的部分
	QFile *myFile = nullptr;
	QTextStream *outfile = nullptr;
	QString filename = path;
	myFile = new QFile(filename);
	myFile->open(QIODevice::WriteOnly | QIODevice::Text);
	outfile = new QTextStream(myFile);
	{
		*outfile << "This is an example about how to output file.\n";
		*outfile << "; G-code for laser engraving" << endl;
		*outfile << "; Generated by lotmax" << endl;
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");
		//outfile << "; Fri Sep 20 2019 10:49:34" << endl;
		*outfile << current_date << endl;
		*outfile << endl;
		*outfile << "G90" << endl;
		*outfile << "G21" << endl;
		*outfile << "G28 XY" << endl;
		*outfile << "; G-code START <<<" << endl;
		*outfile << "G0 F2500" << endl;//点动速度
		*outfile << "M2 5" << endl;//激光能量 
		*outfile << "G1 F500" << endl;//雕刻速度 
		//outfile << "M5" << endl;
		//输出轮廓G代码 重画轮廓
		for (int k = 0; k < outLine.size(); k++)
		{
			if (k == 0)
			{
				*outfile << "G0 " << "X" << outLine[0].start.x / scale + offset.x << " Y" << outLine[0].start.y / scale + offset.y << endl;
				*outfile << "M3" << endl;
				*outfile << "G1 " << "X" << outLine[0].end.x / scale + offset.x << " Y" << outLine[0].end.y / scale + offset.y << endl;
			}
			else
			{
				if ((outLine[k - 1].end.x == outLine[k].start.x) && (outLine[k - 1].end.y == outLine[k].start.y))
				{
					*outfile << "G1 " << "X" << outLine[k].end.x / scale + offset.x << " Y" << outLine[k].end.y / scale + offset.y << endl;
				}
				else
				{
					*outfile << "M5" << endl;
					*outfile << "G0 " << "X" << outLine[k].start.x / scale + offset.x << " Y" << outLine[k].start.y / scale + offset.y << endl;
					*outfile << "M3" << endl;
					*outfile << "G1 " << "X" << outLine[k].end.x / scale + offset.x << " Y" << outLine[k].end.y / scale + offset.y << endl;

				}
			}


		}
		*outfile << "M5" << endl;
		*outfile << endl;
		*outfile << "; G-code END <<<" << endl;
		//outfile->close();
	}
	myFile->close();

	return true;
}

//轮廓处理中  获得轮廓线
vector<LINE> LaserGcodeGenerate::getContoursLine(cv::Mat src)
{
	// TODO: 在此处添加实现代码.
	vector<LINE> LineSet;
	
	double f;
	if ((src.cols > 800) || (src.rows > 800))
	{
		if (src.cols >= src.rows)
		{
			f = 800 / src.cols;
		}
		else
		{
			f = 800 / src.rows;
		}

	}
	else
	{
		f = 1;
	}

	cv::Mat gray_img = cv::Mat::zeros(cv::Size(round(f*src.cols), round(f*src.rows)), CV_8UC1);
	cv::resize(src, gray_img, cv::Size(round(f*src.cols), round(f*src.rows)), 0, 0, cv::INTER_LINEAR);


	cv::Mat out = cv::Mat::zeros(gray_img.size(), CV_8UC1);


	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i>     hierarchy;

	switch (ContoursDetect)
	{
	  case 0:
	  {
		 //图像边缘检测 
		//gray_image  单通道输入图像 
		//输出图像
		//CannyLowThreshold  第一个阈值
		//CannyHighThreshold  第二个阈值 
		// 3  算子内核大小 
		//Canny(gray_img, gray_img, CannyLowThreshold, CannyHighThreshold, 3, false);
		cv::Canny(gray_img, gray_img, CannyLowThreshold, CannyHighThreshold, 3, false);
		break;
	  }
	  case 1:
	  {
		  //边缘检测
		  cv::Mat g_sobelGradient_X, g_sobelGradient_Y;
		  cv::Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;

		  Sobel(gray_img, g_sobelGradient_X, CV_8U, 1, 0, 3, 1, cv::BORDER_DEFAULT);

		  cv::convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);
		  cv::Sobel(gray_img, g_sobelGradient_Y, CV_8U, 0, 1, 3, 1, cv::BORDER_DEFAULT);
		  cv::convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);
		  cv::addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, gray_img);

		  break;
	  }
	  case 2:
	  {
		  cv::Mat g_scharrGradient_x, g_scharrGradient_Y;
		  cv::Mat g_scharrAbsGradient_x, g_scharrAbsGradient_Y;

		  cv::Scharr(gray_img, g_scharrGradient_x, CV_8U, 1, 0, 1, 0, cv::BORDER_DEFAULT);
		  cv::convertScaleAbs(g_scharrGradient_x, g_scharrAbsGradient_x);

		  //求y方向的梯度
		  cv::Scharr(gray_img, g_scharrGradient_Y, CV_8U, 0, 1, 1, 0, cv::BORDER_DEFAULT);
		  cv::convertScaleAbs(g_scharrGradient_Y, g_scharrAbsGradient_Y);
		  //合并梯度
		  cv::addWeighted(g_scharrAbsGradient_x, 0.5, g_scharrAbsGradient_Y, 0.5, 0, gray_img);

		  break;
	  }
	}
	

	cv::findContours(gray_img, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));


	vector<LINE> drawLine;
	drawLine.clear();

	switch (getContours)
	{
		case 0:
		{
		  drawLine = getContoursLineFromPolygon(contours);
		  break;
		}
		case 1:
		{
		  drawLine = getContoursLineFromContours(contours, gray_img);
		  break;
		}
		case 2:
		{
			drawLine = getContoursLineImmediate(contours, gray_img);
			break;
		}
	}
	




	return drawLine;
}

//优化轨迹 直线合并重合的线段
vector<LINE> LaserGcodeGenerate::OptimizeLine(vector<LINE> srcLine)
{
	// TODO: 在此处添加实现代码.
	vector<LINE>dstLine;
	double k1,k2;
	int mindistance = 5;
	double kError = 0.1;
	//printf("srcLine.size():%d\r\n", srcLine.size());
	for (int i = 0; i < srcLine.size(); i++)
	{
		if (srcLine[i].start.x != srcLine[i].end.x)
		{
		  k1 = (srcLine[i].start.y - srcLine[i].end.y) / (srcLine[i].start.x - srcLine[i].end.x);
		}
		else
		{
			k1 = DBL_MAX;
		
		}
		
		for (int j = i+1; j < srcLine.size(); j++)
		{

				if (srcLine[j].start.x != srcLine[j].end.x)
				{
					k2 = (srcLine[j].start.y - srcLine[j].end.y) / (srcLine[j].start.x - srcLine[j].end.x);
				}
				else
				{
					k2 = DBL_MAX;
				
				}
				if (abs(k2 - k1) < kError)//斜率相同 说明两直线平行
				{
					
					if (DistancePointToLine(srcLine[i].start, srcLine[i].end, srcLine[j].end) < mindistance)//两直线距离很近  说明重合  可以合并
					{
						//printf("距离近\r\n");
						//两条直线 有交点才能合并  排除同一直线上的 不连续线段合并
						if (((srcLine[i].start.x > srcLine[j].start.x) && (srcLine[i].start.x < srcLine[j].end.x)) ||
							((srcLine[i].end.x > srcLine[j].start.x) && (srcLine[i].end.x < srcLine[j].end.x)))
						{
							//printf("(%d,%d) (%d,%d)\r\n",srcLine[i].start.x, srcLine[i].start.y, srcLine[i].end.x, srcLine[i].end.y);
							//printf("(%d,%d) (%d,%d)\r\n", srcLine[j].start.x, srcLine[j].start.y, srcLine[j].end.x, srcLine[j].end.y);
							//printf("\r\n");
							LINE L;
							L.start = srcLine[i].start;
							if (L.start.x < srcLine[i].end.x)
							{
								L.start = srcLine[i].end;
							}
							if (L.start.x < srcLine[j].start.x)
							{
								L.start = srcLine[j].start;
							}
							if (L.start.x < srcLine[j].end.x)
							{
								L.start = srcLine[j].end;
							}

							L.end = srcLine[i].end;
							if (L.end.x < srcLine[i].end.x)
							{
								L.end = srcLine[i].end;
							}
							if (L.end.x < srcLine[j].start.x)
							{
								L.end = srcLine[j].start;
							}
							if (L.end.x < srcLine[j].end.x)
							{
								L.end = srcLine[j].end;
							}

							srcLine[i] = L;

							deleteLine(srcLine, j);

						}
					}
					

						


					


				}
			
		}
	}


	dstLine = srcLine;
	return dstLine;
}
  
//重新排列轨迹线的顺序
vector<LINE> LaserGcodeGenerate::OptimizeLineSort(vector<LINE> srcLine)
{
	vector<LINE>dstLine;

	//for (int i = 0; i < srcLine.size(); i++)
	//{
	//	//printf("第%d条线:(%d,%d) (%d,%d)\r\n", i, srcLine[i].start.x, srcLine[i].start.y, srcLine[i].end.x, srcLine[i].end.y);
	//}

	//寻找离原点最近的端点
	dstLine.clear();
	int miniNumber;
	double miniDist;
	double midDist;
	for (int i = 0; i < srcLine.size(); i++)
	{
		
		midDist = (CalculateDistance(srcLine[i].start, cv::Point(0, 0)) + CalculateDistance(srcLine[i].end, cv::Point(0, 0))) / 2;
		if (i == 0)
		{
			miniDist = midDist;
			miniNumber = 0;
		}
		if(midDist < miniDist)
		{
			miniDist = midDist;
			miniNumber = i;
		}	


	}
	
	//重新定义最近线段的起点和终点
	if (CalculateDistance(srcLine[miniNumber].start, cv::Point(0, 0)) > CalculateDistance(srcLine[miniNumber].end, cv::Point(0, 0)))
	{
		cv::Point temp=srcLine[miniNumber].start;
		srcLine[miniNumber].start = srcLine[miniNumber].end;
		srcLine[miniNumber].end = temp;
	}
	dstLine.push_back(srcLine[miniNumber]);
	deleteLine(srcLine, miniNumber);



	//for (int i = 0; i < srcLine.size(); i++)
	//{
	//	printf("srcLine第%d条线:(%d,%d) (%d,%d)\r\n", i, srcLine[i].start.x, srcLine[i].start.y, srcLine[i].end.x, srcLine[i].end.y);
	//}

	//for (int i = 0; i < dstLine.size(); i++)
	//{
	//	printf("dstLine第%d条线:(%d,%d) (%d,%d)\r\n", i, dstLine[i].start.x, dstLine[i].start.y, dstLine[i].end.x, dstLine[i].end.y);
	//}




	while(srcLine.size()>1)
	{
		//printf("dstLine.back条线:(%d,%d) (%d,%d)\r\n", dstLine.back().start.x, dstLine.back().start.y, dstLine.back().end.x, dstLine.back().end.y);
	   
		miniDist = CalculateDistance(srcLine[0].end, dstLine.back().end)> CalculateDistance(srcLine[0].start, dstLine.back().end)? CalculateDistance(srcLine[0].start, dstLine.back().end): CalculateDistance(srcLine[0].end, dstLine.back().end);
	 
		//printf("初始距离%f;;(%d,%d) (%d,%d)\r\n", miniDist, srcLine[0].start.x, srcLine[0].start.y, srcLine[0].end.x, srcLine[0].end.y);
		 miniNumber = 0;
		for (int i = 0; i < srcLine.size(); i++)
		{
			if (CalculateDistance(srcLine[i].start, dstLine.back().end) > CalculateDistance(srcLine[i].end, dstLine.back().end))
			{  //终点最近
				//printf("%d终点近,距离为%f 最短为%f\r\n",i, CalculateDistance(srcLine[i].end, dstLine.back().end), miniDist);
				if (CalculateDistance(srcLine[i].end, dstLine.back().end) <= miniDist)
				{
					miniDist = CalculateDistance(srcLine[i].end, dstLine.back().end);

					miniNumber = i;
					//printf("交换:(%d,%d) (%d,%d)\r\n", srcLine[miniNumber].start.x, srcLine[miniNumber].start.y, srcLine[miniNumber].end.x, srcLine[miniNumber].end.y);
					cv::Point temp = srcLine[miniNumber].start;
					srcLine[miniNumber].start = srcLine[miniNumber].end;
					srcLine[miniNumber].end = temp;
					//printf("交换:(%d,%d) (%d,%d)\r\n", srcLine[miniNumber].start.x, srcLine[miniNumber].start.y, srcLine[miniNumber].end.x, srcLine[miniNumber].end.y);
				}
			}
			else
			{
				//printf("%d起点近,距离为%f 最短为%f\r\n", i, CalculateDistance(srcLine[i].start, dstLine.back().end), miniDist);
				if (CalculateDistance(srcLine[i].start, dstLine.back().end) <=miniDist)
				{
					miniDist = CalculateDistance(srcLine[i].start, dstLine.back().end);
					miniNumber = i;

				}
			}

		}
		dstLine.push_back(srcLine[miniNumber]);
	    deleteLine(srcLine, miniNumber);

	}

//	printf("start(%d,%d) end(%d,%d)\r\n", srcLine[0].start.x, srcLine[0].start.y, srcLine[0].end.x, srcLine[0].end.y);
	dstLine.push_back(srcLine[0]);


	//for (int i = 0; i < dstLine.size(); i++)
	//{
	//	printf("排序后第%d条线:(%d,%d) (%d,%d)\r\n", i, dstLine[i].start.x, dstLine[i].start.y, dstLine[i].end.x, dstLine[i].end.y);
	//}

	return dstLine;

}
//计算两点间的距离
double LaserGcodeGenerate::CalculateDistance(cv::Point p1, cv::Point p2)
{
	//printf("(%d,%d)(%d,%d)距离为%f\r\n", p1.x, p1.y, p2.x, p2.y, sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)));
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) +(p1.y - p2.y)*(p1.y - p2.y));
}
//删除一段轨迹线
void LaserGcodeGenerate::deleteLine(vector<LINE>&srcLine,int line)
{
  vector<LINE>::iterator it = srcLine.begin();
  for (int i = 0; i < line; i++)
  {
	  it++;
  }
  srcLine.erase(it);



}
//计算点到直线的距离
double LaserGcodeGenerate::DistancePointToLine(cv::Point LinePoint1, cv::Point LinePoint2, cv::Point detectPoint)
{
	double k;
	if (LinePoint2.x != LinePoint1.x)
		k = (LinePoint2.y - LinePoint1.y) / (LinePoint2.x - LinePoint1.x);
	else
		return abs(detectPoint.x - LinePoint2.x);
	double b;
	b = LinePoint1.y - k * LinePoint1.x;
	double dist;
	dist = abs((k*detectPoint.x - detectPoint.y + b) / sqrt(k*k + 1));
	return dist;
}
//画线
void LaserGcodeGenerate::drawLine(vector<LINE> src, cv::Mat &out)
{
	cv::RNG rng(12345);
	cv::String str;
	for (int j = 0; j < src.size(); j++)//
	{
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar color = cv::Scalar(255, 255, 255);
		line(out, src[j].start, src[j].end, color, 1);
			//str = format("%d", j);
	  //  putText(out, str, src[j].start, FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 0, 255), 2, 8, 0);
		//line(out, Point(0,0), Point(50,50), color, 1);
	}


}
//重新设置图像大小
cv::Mat LaserGcodeGenerate::ResizeImage(cv::Mat src)
{
	resize(src, src, cv::Size(Length * 10, width * 10));
	return src;
}
//控制灰度值在0到255之间
uchar LaserGcodeGenerate::ControlValue(uchar value)
{
	if (value > 255)
	{
		return 255;
	}
	else if (value < 0)
	{
		return 0;
	}
	else
	{
		return value;
	}

}
//多边形描述轮廓
vector<LINE> LaserGcodeGenerate::getContoursLineFromPolygon(vector<vector<cv::Point>> contours)
{
	vector<cv::Point> lineSet;
	vector<LINE> drawLine;
	drawLine.clear();
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), lineSet, 1, true);
		for (int j = 0; j < lineSet.size() - 1; j++)
		{

			LINE L;
			L.start = lineSet[j];
			L.end = lineSet[j + 1];
			for (int k = j + 1; k < lineSet.size() - 1;)
			{
				L.end = lineSet[k];
				if (DistancePointToLine(L.start, L.end, lineSet[k + 1]) < 3)
				{
					k++;
					j++;
				}
				else
				{
					break;
				}
			}



			drawLine.push_back(L);

		}
	}
	printf("ContoursLine:%d\r\n", drawLine.size());

	vector<LINE> OpLine = OptimizeLine(drawLine);
	printf("OpLine:%d\r\n", OpLine.size());
	return OpLine;
}
//轮廓骨架描述轮廓
vector<LINE> LaserGcodeGenerate::getContoursLineFromContours(vector<vector<cv::Point>> contours, cv::Mat src)
{
	vector<LINE>dstLine;

	cv::Mat conMat =cv::Mat::zeros(src.size(), CV_8UC1);
	vector<cv::Vec4i>hierarchy;
	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(conMat, contours, i, cv::Scalar(255, 255, 255), 1, 8, hierarchy, 0, cv::Point(0, 0));
	}

	bool flag;

	// 横线

	for (int row = 0; row < conMat.rows; row++)
	{
		
		LINE L;
		for (int col = 0; col < conMat.cols; col++)
		{
			
			int value = conMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{
					
					L.start = cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{
					
					if (L.start.x == col - 1)
					{
						flag = true;
					}
					else
					{
						L.end = cv::Point(col - 1, row);
						dstLine.push_back(L);
						flag = true;
					}

				}
			}

		}
		if (!flag)
		{
			if (L.start.x == conMat.cols - 1)
			{
				flag = true;
			}
			else
			{
				L.end = cv::Point(conMat.cols - 1, row);
				dstLine.push_back(L);
				flag = true;
			}
		}
	}

	//竖线

	for (int col = 0; col < conMat.cols; col++)
	{
		//flag = true;
		LINE L;
		for (int row = 0; row < conMat.rows; row++)
		{
			int value = conMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{
					L.start = cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{
					if (L.start.y == row - 1)
					{
						flag = true;
					}
					else
					{
						L.end = cv::Point(col,row - 1);
						dstLine.push_back(L);
						flag = true;
					}

				}
			}

		}
		if (!flag)
		{
			if (L.start.y == conMat.rows - 1)
			{
				flag = true;
			}
			else
			{
				L.end = cv::Point(col, conMat.rows - 1);
				dstLine.push_back(L);
				flag = true;
			}
		}
	}

	//45°直线
	int row = 0;
	int col= 1;
	for (int i = 0; i < (conMat.cols + conMat.rows - 3); i++)
	{
		flag = true;
		LINE L;
		while((col>0)&&(row < conMat.rows))
		{
			int value = conMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{
					L.start = cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{
					
					L.end = cv::Point(col+1, row - 1);
					if ((L.start.x == L.end.x) && (L.start.y == L.end.y))
					{

					}
					else
					{
						dstLine.push_back(L);
					}
					
					flag = true;
				}
			}
			row = row + 1;
			col = col - 1;
		}
		if (i < conMat.cols-1)
		{
			row = 0;
		    col = i + 1;
		}
		else
		{
			col = conMat.cols - 1;
			row = i- conMat.cols+2;
		}
	}


	//-45°直线

	 row = 0;
	 col = conMat.cols-2;
	for (int i = 0; i < (conMat.cols + conMat.rows - 3); i++)
	{
		flag = true;
		LINE L;
		while ((col < conMat.cols) && (row < conMat.rows))
		{
			int value = conMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{
					L.start = cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{
					L.end = cv::Point(col - 1, row - 1);
					if ((L.start.x == L.end.x) && (L.start.y == L.end.y))
					{

					}
					else
					{
						dstLine.push_back(L);
					}
					flag = true;
				}
			}

			row = row + 1;
			col = col + 1;
		}
		if (i < conMat.cols-1)
		{
			row = 0;
			col = conMat.cols -i - 2;
		}
		else
		{
			col = 0;
			row = i - conMat.cols+2;
		}
	}





	return dstLine;
}

vector<LINE> LaserGcodeGenerate::getContoursLineImmediate(vector<vector<cv::Point>> contours, cv::Mat src)
{
	vector<LINE>dstLine;
	cv::Mat CMat = cv::Mat::zeros(src.size(), CV_8UC1);
	for (int i = 0; i < contours.size(); i++)
	{
		for (int j = 0; j < contours[i].size(); j++)
		{
			cv::Point p = contours[i].at(j);
			CMat.at<uchar>(p.y, p.x) = 255;

		}

	}

	bool flag;

	// 横线

	for (int row = 0; row < CMat.rows; row++)
	{
	
		LINE L;
		for (int col = 0; col < CMat.cols; col++)
		{

			int value = CMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{

					L.start = cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{

					if (L.start.x == col - 1)
					{
						flag = true;
					}
					else
					{
						L.end = cv::Point(col - 1, row);
						dstLine.push_back(L);
						flag = true;
					}

				}
			}

		}
		if (!flag)
		{
			if (L.start.x == CMat.cols - 1)
			{
				flag = true;
			}
			else
			{
				L.end = cv::Point(CMat.cols - 1, row);
				dstLine.push_back(L);
				flag = true;
			}
		}
	}

	//竖线

	for (int col = 0; col < CMat.cols; col++)
	{
		flag = true;
		LINE L;
		for (int row = 0; row < CMat.rows; row++)
		{
			int value = CMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{
					L.start = cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{
					if (L.start.y == row - 1)
					{
						flag = true;
					}
					else
					{
						L.end = cv::Point(col, row - 1);
						dstLine.push_back(L);
						flag = true;
					}

				}
			}

		}
		if (!flag)
		{
			if (L.start.y == CMat.rows - 1)
			{
				flag = true;
			}
			else
			{
				L.end = cv::Point(col, CMat.rows - 1);
				dstLine.push_back(L);
				flag = true;
			}
		}


	}


	//45°直线
	int row = 0;
	int col = 1;
	for (int i = 0; i < (CMat.cols + CMat.rows - 3); i++)
	{
		flag = true;
		LINE L;
		while ((col > 0) && (row < CMat.rows))
		{
			int value = CMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{
					L.start = cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{

					L.end = cv::Point(col + 1, row - 1);
					if ((L.start.x == L.end.x) && (L.start.y == L.end.y))
					{

					}
					else
					{
						dstLine.push_back(L);
					}

					flag = true;
				}
			}
			row = row + 1;
			col = col - 1;
		}
		if (i < CMat.cols - 1)
		{
			row = 0;
			col = i + 1;
		}
		else
		{
			col = CMat.cols - 1;
			row = i - CMat.cols + 2;
		}
	}


	//-45°直线

	row = 0;
	col = CMat.cols - 2;
	for (int i = 0; i < (CMat.cols + CMat.rows - 3); i++)//有漏洞
	{
		flag = true;
		LINE L;
		while ((col < CMat.cols) && (row < CMat.rows))
		{

	
			int value = CMat.at<uchar>(row, col);
			if (flag)
			{
				if (value == 255)
				{
					L.start =cv::Point(col, row);
					flag = false;
				}
			}
			else
			{
				if (value == 0)
				{
					L.end = cv::Point(col - 1, row - 1);
					if ((L.start.x == L.end.x) && (L.start.y == L.end.y))
					{

					}
					else
					{
						dstLine.push_back(L);
					}
					flag = true;
				}
			}

			row = row + 1;
			col = col + 1;
		}
		if (i < CMat.cols - 1)
		{
			row = 0;
			col = CMat.cols - i - 2;
		}
		else
		{
			col = 0;
			row = i - CMat.cols + 2;
		}
		
	}




	return dstLine;
}

//镜像图像
cv::Mat LaserGcodeGenerate::SMMirror(cv::Mat src)
{
			cv::Mat dst;	
			dst.create(src.size(), src.type());
			cv::Mat map_x;	
			cv::Mat map_y;	
			map_x.create(src.size(), CV_32FC1);
			map_y.create(src.size(), CV_32FC1);
			for (int i = 0; i < src.rows; ++i) {		
				for (int j = 0; j < src.cols; ++j) {
			     map_x.at<float>(i, j) = (float)(src.cols - j);
				 map_y.at<float>(i, j) = (float)i;//(src.rows - i) ;	
				}	
			}	
			remap(src, dst, map_x, map_y, cv::INTER_LINEAR);
			//imshow("dst", dst);		

			return dst;
			

}