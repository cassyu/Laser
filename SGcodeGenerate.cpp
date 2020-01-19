#include "SGcodeGenerate.h"
#include "opencv2/imgproc/types_c.h"
//#include "opencv2/"
#include <QObject>
#include "LaserGcodeGenerate.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <opencv2/core/matx.hpp>
#include <QDebug>
#include <math.h>
#include <QPainter>
#include "QtOpencvConvert.h"
//#include <shared/minwindef.h>

int errorDiffusion = 0;
int ImageMaxSize;
int CannyLowThreshold;
int  CannyHighThreshold;
int ContoursDetect;
int getContours;
int g_ContoursGray = 128;
int global_grayValue = 0;


GcodeStruct getTextGcode(cv::Mat src, QString filename, cv::Point offset, double scale)
{
	cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	if (src.depth() != CV_8UC1)
	{
		//printf("需要输入黑白图像\r\n");
		return  GcodeStruct();
	}
	//需要镜像 
	src = SMMirror(src);

	vector<LINE> GrayLine = getTrack(src);

	//vector<Point> GrayPoint = GetPoint(src);
	//vector<LINE> GrayLine = getLine(GrayPoint);

	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
	drawLine(GrayLine, out1);

	GcodeStruct  gvevtor;
	gvevtor.outLine = GrayLine;
	gvevtor.offset = offset;
	gvevtor.scale = scale;

	return gvevtor;
}

GcodeStruct  getBinaryImageGcode(cv::Mat src, QString filename, cv::Point offset, double scale)
{
	//先镜像 
	src = SMMirror(src);

	//第一种滤波 
	//blur(src, src, Size(3, 3));
	//高斯滤波  对图像平滑的处理  并保留图像的总体灰度分布特征 
	cv::GaussianBlur(src, src, cv::Size(9, 9), 0, 0);

	//Gray = (R*299 + G*587 + B*114 + 500) / 1000  
	//各个像素采取不同的比值  
	if (src.channels() != 1)
	{
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	}

	cv::Size size1(src.size()*LINE_DENSITY);
	cv::resize(src, src, size1);



	cv::Mat BinaryMat = cv::Mat::zeros(src.size(), CV_8UC1);
	BinaryMat = BinaryImage(src); //二值化处理图像
	vector<LINE> BinaryLine = getTrack(BinaryMat);
	//vector<Point> BinaryPoint = GetPoint(BinaryMat);//根据处理后的图片获得轨迹点
	//vector<LINE> BinaryLine = getLine(BinaryPoint);//根据轨迹点获得轨迹线

	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC3);
	drawLine(BinaryLine, out1);

	GcodeStruct  gvevtor;
	gvevtor.outLine = BinaryLine;
	gvevtor.offset = offset;
	gvevtor.scale = scale;

	return gvevtor;
    
}

cv::Mat getTextGcodeOnlyImage(cv::Mat src)
{
	cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	if (src.depth() != CV_8UC1)
	{
		printf("需要输入黑白图像\r\n");
		cv::Mat out1;
		return out1;
	}

	//在这里暂时不需要镜像 
	//src = SMMirror(src);
	vector<LINE> GrayLine = getTrack(src);

	//vector<Point> GrayPoint = GetPoint(src);
	//vector<LINE> GrayLine = getLine(GrayPoint);

	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
	drawLine(GrayLine, out1);

	return out1;
}

cv::Mat getBinaryImageGcodeOnlyImage(cv::Mat src,int thresh)
{
	//blur(src, src, Size(3, 3));
	//高斯滤波  对图像平滑的处理  并保留图像的总体灰度分布特征 
	//old used 
	//cv::GaussianBlur(src, src, cv::Size(9, 9), 0, 0);
	//src = SMMirror(src);

	//Gray = (R*299 + G*587 + B*114 + 500) / 1000  
	//各个像素采取不同的比值  
	//cv::imshow("srcre",src);
	int n = src.channels();
	if (n != 1)
	{
		//qDebug() << "this start";
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像
		//qDebug() << "this end";
	}
	//new one method
	//cv::blur(src,src,cv::Size(3,3));
	//new second method
	cv::GaussianBlur(src,src,cv::Size(9,9),0,0);

	//先简单的图像处理后  在resize图像 
	cv::Size size1(src.size()*LINE_DENSITY);
	cv::resize(src, src, size1);

	cv::Mat BinaryMat = cv::Mat::zeros(src.size(), CV_8UC1);
	BinaryMat = BinaryImage(src,thresh); //二值化处理图像
	//BinaryMat = 255 - BinaryMat;
	vector<LINE> BinaryLine = getTrack(BinaryMat);
	//vector<Point> BinaryPoint = GetPoint(BinaryMat);//根据处理后的图片获得轨迹点
	//vector<LINE> BinaryLine = getLine(BinaryPoint);//根据轨迹点获得轨迹线

	//namedWindow(u8"二值图像", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
	drawLine(BinaryLine, out1);
	//imshow("二值图像", out1);

	/*for (int i = 0; i < out1.rows; ++i)
	{
		for (int j = 0; j < out1.cols; ++j)
		{
			cv::Vec3b& color = out1.at<cv::Vec3b>(i, j);
			color[0] = 255 - color[0];
			color[1] = 255 - color[1];
			color[2] = 255 - color[2];

		}
	}*/
	//out1 = 255 - out1;
	//imshow("二值图像反色", out1);
	return out1;
}

cv::Mat getGrayImageGcodeOnlyImage(cv::Mat src,int diff)
{
	//cv::imshow("原始", src);
	//cv::Mat dst1 = DitherFloySteinBargA(src);
	//A:彩色的时候计算对比度 亮度 
	//src = DuibiDuLiangdu(src,1.5,30);
	//cv::imshow("duibidu", dst1);

	//cv::blur(src, src, cv::Size(3, 3));
	cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0);
	//cv::imwrite("in1.jpg", src);
	cv::Mat leftsrc;
	cv::Mat rightsrc;
	cv::Mat origin;
	cv::Mat Matout1 = cv::Mat::zeros(src.size(), CV_8UC1);
	//origin = BinaryImage(src);
	int n = src.channels();
	if (n != 1)
	{
		cv::cvtColor(src, Matout1, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	}
	
	//DuibiDu(src, 1.5, 30);
	//新加的2019.11.29
	//cv::resize(src, src, cv::Size(src.cols * 0.20, src.rows * 0.20));
	Matout1 = 255 - Matout1;

	//cutLeft(Matout1, leftsrc, rightsrc);
	//leftsrc = DuibiDuLiangdu(leftsrc,1.5,30);
     //B:灰色的时候计算对比度 亮度
	//Matout1 = DuibiDuLiangdu(Matout1, 1.5, 30);

	//新加的  扩充尺寸  使线条更密集
	//cv::Size size1(leftsrc.size() * LINE_DENSITY);
	//cv::resize(leftsrc, leftsrc, size1);

	//扩充 Matout1  而不是切割的leftsrc 看来扩不扩充的无所谓
	cv::Size size1(Matout1.size());
	//cv::resize(Matout1, Matout1, size1);

	//Matout1 = DuibiDuLiangdu(Matout1, 1.5, 30);

	cv::Mat midImage = GrayScale(Matout1,diff);  //灰度图像处理
	//cv::imshow("反色前", midImage);
	midImage = 255 - midImage;
	//cv::imshow("反色",midImage);

	//vector<Point> GrayPoint = GetPoint(midImage);
	//vector<LINE> GrayLine = getLine(GrayPoint);

	vector<LINE> GrayLine = getGrayTrack(midImage);
	//结果显示
	//cv::namedWindow(u8"gray", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(midImage.size(), CV_8UC4);
	//A方法：绘图
	drawLine(GrayLine, out1);
	//drawGrayLine(GrayLine,out1);
	//cv::imshow("final",out1);
	//B方法绘图
	//QPixmap pixmap(658,757);
	//
	//QPainter painter(&pixmap);
	//painter.setRenderHint(QPainter::Antialiasing,true);
	//painter.setPen(QColor(0,160,230));
	//painter.setBrush(Qt::white);
	//QVector<QLine>  linesvector;
	//for (int i = 0; i < GrayLine.size(); ++i)
	//{
	//	QLine line;
	//	QPoint st;
	//	st.setX(GrayLine[i].start.x);
	//	st.setY(GrayLine[i].start.y);
	//	QPoint ed;
	//	ed.setX(GrayLine[i].end.x);
	//	ed.setY(GrayLine[i].end.y);
	//	line.setP1(st);
	//	line.setP2(ed);
	//	linesvector.push_back(line);
	//}
	//painter.drawLines(linesvector);
	////pixmap.toImage();
	//QImage img = pixmap.toImage();
	//if (img.save("testp.png", "png"))
	//{
	//	int n = 10;
	//}
	//cv::Mat out2 = QImage2cvMat(pixmap.toImage());

	//out1 = 255 - out1;
	//resize(out1, out1, src.size());
	//cv::imshow(u8"grayImage", out1);
	//cv::imwrite("out1.jpg", out1);
	/*for (int i = 0; i < out1.rows; ++i)
	{
		for (int j = 0; j < out1.cols; ++j)
		{
			out1.at<cv::Vec3b>(i,j);
		}
	}*/

	return out1;
}

cv::Mat getContoursGcodeOnlyImage(cv::Mat src,OutLineStruct data)
{
	blur(src, src, cv::Size(3, 3));
	//界面上的显示 不需要镜像 
	//src = SMMirror(src);
	if (src.channels() != 1)
	{
		cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像
	}

	ContoursDetect = G_global_ContourDetect;
	getContours = G_global_GetContours;

	if (VECTOR_SELECT)
	{
		vector<LINE> ContoursLine = getContoursLine(src, data);
		vector<LINE> OutLine;
		//if (getContours == 1)
		if (data.border)
			//if (1)
		{
			OutLine = OptimizeLineSort(ContoursLine);
		}
		else
		{
			OutLine = ContoursLine;
		}


		//printf("contours Size;%d\r\n", OutLine.size());
		//cv::namedWindow(u8"轮廓图像", cv::WINDOW_AUTOSIZE);
		cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
		drawLine(OutLine, out1);
		//cv::imshow(u8"轮廓图像", out1);

		return out1;
	}
	else
	{
		cv::Mat outpol = OutPoline(src, data.thred);
		return outpol;
	}
}

cv::Mat getGcodeWithIdOnlyImage(cv::Mat src, int id, OutLineStruct data)
{
	cv::Mat mat;
	switch (id)
	{
	case 0:
		mat = getBinaryImageGcodeOnlyImage(src);
		break;
	case 3:
		mat = getTextGcodeOnlyImage(src);
		break;
	case 1:
		mat = getGrayImageGcodeOnlyImage(src,0);
		break;
	case 2:
		mat = getContoursGcodeOnlyImage(src,data);
		break;
	}

	return mat;
}

GcodeStruct getGrayImageGcode(cv::Mat src, QString filename, int type, cv::Point offset, double scale)
{
	//先镜像 
	src = SMMirror(src);
	//图像平滑处理方案1 
	//cv::blur(src, src, cv::Size(3, 3));
	//图像平滑处理方案2 
	cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0);
	cv::Mat leftsrc;
	cv::Mat rightsrc;
	cv::Mat origin;
	cv::Mat Matout1 = cv::Mat::zeros(src.size(), CV_8UC1);
	//origin = BinaryImage(src);
	if (src.channels() != 1)
	{
		cv::cvtColor(src, Matout1, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	}
	//cv::resize(src, src, cv::Size(src.cols * 0.20, src.rows * 0.20));
	Matout1 = 255 - Matout1;
	//cv::threshold(src, origin, 100, 255,CV_THRESH_BINARY);

	//cutLeft(Matout1, leftsrc, rightsrc);

	//新加的  扩充尺寸  使线条更密集  不需要扩充尺寸
	//cv::Size size1(leftsrc.size() * LINE_DENSITY);
	//cv::resize(leftsrc, leftsrc, size1);

	cv::Mat midImage = GrayScale(Matout1,type);  //灰度图像处理
	midImage = 255 - midImage;

	//vector<Point> GrayPoint = GetPoint(midImage);
	//vector<LINE> GrayLine = getLine(GrayPoint);

	vector<LINE> GrayLine = getTrack(midImage);
	//结果显示
	cv::Mat out1 = cv::Mat::zeros(midImage.size(), CV_8UC1);
	drawLine(GrayLine, out1);
	//out1 = 255 - out1;
    
	GcodeStruct  gvevtor;
	gvevtor.outLine = GrayLine;
	gvevtor.offset = offset;
	gvevtor.scale = scale;

	return gvevtor;
}

GcodeStruct getContoursGcode(cv::Mat src, QString filename, OutLineStruct data,cv::Point offset, double scale)
{
	//先镜像 
	src = SMMirror(src);

	//然后图像平滑处理  
	blur(src, src, cv::Size(3, 3));
	if (src.channels() != 1)
	{
		cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像
	}

	//ContoursDetect = G_global_ContourDetect;
	//getContours = G_global_GetContours;

	if (VECTOR_SELECT)
	{
		vector<LINE> ContoursLine = getContoursLine(src, data);
		vector<LINE> OutLine;
		//if (G_global_GetContours == 1)
		if (data.border == 1)
		{
			OutLine = OptimizeLineSort(ContoursLine);
		}
		else
		{
			OutLine = ContoursLine;
		}


		//printf("contours Size;%d\r\n", OutLine.size());
		//cv::namedWindow(u8"轮廓图像", cv::WINDOW_AUTOSIZE);
		cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
		drawLine(OutLine, out1);
		//cv::imshow(u8"轮廓图像", out1);

		GcodeStruct  gvevtor;
		gvevtor.outLine = OutLine;
		gvevtor.offset = offset;
		gvevtor.scale = scale;
		return gvevtor;
	}
	else
	{
		GcodeStruct  gvevtor;
		gvevtor.outLine = OutPolineGcode(src, data.thred);
		gvevtor.offset = offset;
		gvevtor.scale = scale;
		return gvevtor;
	}
    
}

vector<LINE> getTextGcodeOnlyShape(cv::Mat src)
{
	cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	if (src.depth() != CV_8UC1)
	{
		//printf("需要输入黑白图像\r\n");
		//cv::Mat out1;
		return vector<LINE>();
	}
	src = SMMirror(src);
	vector<LINE> GrayLine = getTrack(src);

	//vector<Point> GrayPoint = GetPoint(src);
	//vector<LINE> GrayLine = getLine(GrayPoint);


	//namedWindow(u8"黑白图像", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC3);
	drawLine(GrayLine, out1);

	return GrayLine;
}

vector<LINE> getBinaryImageGcodeShape(cv::Mat src, int thresh)
{
	//blur(src, src, Size(3, 3));
    //高斯滤波  对图像平滑的处理  并保留图像的总体灰度分布特征 
	cv::GaussianBlur(src, src, cv::Size(9, 9), 0, 0);
	src = SMMirror(src);

	//Gray = (R*299 + G*587 + B*114 + 500) / 1000  
	//各个像素采取不同的比值  
	int n = src.channels();
	if (n != 1)
	{
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	}


	cv::Mat BinaryMat = cv::Mat::zeros(src.size(), CV_8UC1);
	BinaryMat = BinaryImage(src, thresh); //二值化处理图像
	//BinaryMat = 255 - BinaryMat;
	vector<LINE> BinaryLine = getTrack(BinaryMat);
	//imshow("BinaryMat", BinaryMat);
	//vector<Point> BinaryPoint = GetPoint(BinaryMat);//根据处理后的图片获得轨迹点
	//vector<LINE> BinaryLine = getLine(BinaryPoint);//根据轨迹点获得轨迹线

	//namedWindow(u8"二值图像", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC3);
	drawLine(BinaryLine, out1);

	return BinaryLine;
}

vector<LINE> getGrayImageGcodeShape(cv::Mat src, int diff)
{
	blur(src, src, cv::Size(3, 3));
	//GaussianBlur(src, src, Size(9, 9), 0, 0);
	//cv::imwrite("in1.jpg", src);
	cv::Mat leftsrc;
	cv::Mat rightsrc;
	cv::Mat origin;
	cv::Mat Matout1 = cv::Mat::zeros(src.size(), CV_8UC1);
	//origin = BinaryImage(src);
	int n = src.channels();
	if (n != 1)
	{
		cv::cvtColor(src, Matout1, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	}
	//cv::resize(src, src, cv::Size(src.cols * 0.20, src.rows * 0.20));
	Matout1 = 255 - Matout1;
	//cv::threshold(src, origin, 100, 255,CV_THRESH_BINARY);

	cutLeft(Matout1, leftsrc, rightsrc);

	cv::Mat midImage = GrayScale(leftsrc, diff);  //灰度图像处理
	midImage = 255 - midImage;

	//vector<Point> GrayPoint = GetPoint(midImage);
	//vector<LINE> GrayLine = getLine(GrayPoint);

	vector<LINE> GrayLine = getGrayTrack(midImage);
	//结果显示
	//cv::namedWindow(u8"gray", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(midImage.size(), CV_8UC1);
	drawLine(GrayLine, out1);

	return GrayLine;
}

vector<LINE> getContoursGcodeShape(cv::Mat src, OutLineStruct data)
{
	blur(src, src, cv::Size(3, 3));
	//实际的打印需要 镜像 
	src = SMMirror(src);
	if (src.channels() != 1)
	{
		cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像
	}

	ContoursDetect = G_global_ContourDetect;
	getContours = G_global_GetContours;

	vector<LINE> ContoursLine = getContoursLine(src, data);
	vector<LINE> OutLine;
	//if (getContours == 1)
	if (data.border)
	{
		OutLine = OptimizeLineSort(ContoursLine);
	}
	else
	{
		OutLine = ContoursLine;
	}

    //printf("contours Size;%d\r\n", OutLine.size());
	//cv::namedWindow(u8"轮廓图像", cv::WINDOW_AUTOSIZE);
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC1);
	drawLine(OutLine, out1);

	return OutLine;
}

vector<LINE> getGcodeWithIdShape(cv::Mat src, int id)
{
	vector<LINE> lines;

	switch (id)
	{
	case 0:
		lines = getBinaryImageGcodeShape(src);//黑白
		break;
	case 3:
		lines = getTextGcodeOnlyShape(src);//文本 
		break;
	case 1:
		lines = getGrayImageGcodeShape(src, 0);//灰度 
		break;
	case 2:
		lines = getContoursGcodeShape(src);//轮廓
		break;
	}

	return lines;
}

int getColSum(cv::Mat src, int col)
{
	int sum = 0;
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i < height; i++)
	{
		sum = sum + src.at<uchar>(i, col);
		//sum = sum + (255 -src.at<uchar>(i, col));
	}
	return  sum;
}

int getRowSum(cv::Mat src, int row)
{
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

void cutTop(cv::Mat & src, cv::Mat & dstImg)
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
}

int cutLeft(cv::Mat & src, cv::Mat & leftImg, cv::Mat & rightImg)
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

cv::Mat getBinaryImageGcode(cv::Mat src)//最终生成gcode的版本 
{

	//Gray = (R*299 + G*587 + B*114 + 500) / 1000  
	//各个像素采取不同的比值  
	int n = src.channels();
	if (n != 1)
	{
		cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);//转化为灰度图像	
	}

	//blur(src, src, Size(3, 3));
	//高斯滤波  对图像平滑的处理  并保留图像的总体灰度分布特征 
	cv::GaussianBlur(src, src, cv::Size(9, 9), 0, 0);

	//resize图像 
	cv::Size size1(src.size()*LINE_DENSITY);
	cv::resize(src, src, size1);


	cv::Mat BinaryMat = cv::Mat::zeros(src.size(), CV_8UC1);
	BinaryMat = BinaryImage(src); //二值化处理图像
	vector<LINE> BinaryLine = getTrack(BinaryMat);

	//vector<Point> BinaryPoint = GetPoint(BinaryMat);//根据处理后的图片获得轨迹点
	//vector<LINE> BinaryLine = getLine(BinaryPoint);//根据轨迹点获得轨迹线

	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
	drawLine(BinaryLine, out1);

	return out1;
}

cv::Mat BinaryImage(cv::Mat src,int thresh)
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
	int equal_sum_gray;
	if (!thresh)
	{
		equal_sum_gray = sum_gray / (height*width);
		global_grayValue = equal_sum_gray= 168;
	}
	else
	{
		equal_sum_gray = thresh;
		global_grayValue = equal_sum_gray;
	}
	// 输入图像 输出图像  阈值  向上最大值  阈值话 操作的类型 
	//THRESH_TOZERO   大于阈值 为原像素  否则为0 
	cv::threshold(src, dst, equal_sum_gray, 255, cv::THRESH_BINARY);//图像二值化
	//cvthreshold();
	//cv::threshold();

	return dst;
}

vector<LINE> getTrack(cv::Mat src)
{
	LINE L;
	vector<LINE>LineSet;
	bool LineFlag = true;
	int space = LIEN_BLACK_DEBNSITY;
	for (int row = 0; row < src.rows; row+= space)
	{
		//if (row % 2 == 1)  LINE_DENSITY
		//if (row % LIEN_BLACK_DEBNSITY == 1)
		{
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
						//L.end = cv::Point(col, row + 1);
						L.end = cv::Point(col,row);
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
		//else
		//{
		//	for (int col = 0; col < src.cols; col++)
		//	{
		//		if (src.at<uchar>(row, col) == 0)//黑色
		//		{
		//			if (LineFlag)
		//			{
		//				L.start = cv::Point(col, row);
		//				LineFlag = false;
		//			}
		//			else
		//			{

		//			}

		//		}
		//		else
		//		{
		//			if (LineFlag)
		//			{

		//			}
		//			else
		//			{
		//				L.end = cv::Point(col, row + 1);
		//				LineSet.push_back(L);
		//				LineFlag = true;
		//			}

		//		}

		//	}
		//	if (!LineFlag)
		//	{
		//		L.end = cv::Point(src.cols - 1, row);
		//		LineSet.push_back(L);
		//		LineFlag = true;
		//	}
		//}


	}
	return LineSet;
}

void drawLine(vector<LINE> src, cv::Mat &out)
{
	cv::RNG rng(12345);
	cv::String str;
	for (int j = 0; j < src.size(); j++)//
	{
		//cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar color = cv::Scalar(0, 0, 0, 255);
		/*if (j % 2 == 1)
			color = cv::Scalar(0, 255, 0, 255);*/
		//if (j % 2 == 1)
		{
			cv::line(out, src[j].start, src[j].end, color, 1);
		}
		//str = format("%d", j);
  //  putText(out, str, src[j].start, FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 0, 255), 2, 8, 0);
	//line(out, Point(0,0), Point(50,50), color, 1);
	}

}

void drawGrayLine(std::vector<LINE> src, cv::Mat & out)
{
	cv::RNG rng(12345);
	cv::String str;
	for (int j = 0; j < src.size(); j++)//
	{
		//cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar color = cv::Scalar(0, 0, 0, 255);
		/*if (j % 2 == 1)
			color = cv::Scalar(0, 255, 0, 255);*/
		if (j % 2 == 1)
		{
			cv::line(out, src[j].start, src[j].end , color, 1);
		}
	}
}

cv::Mat GrayScale(cv::Mat src,int diff)
{
	cv::Mat dst1 = cv::Mat::zeros(src.size(), CV_8UC1);
	//半色调算法 误差消抖
	//errorDiffusion = G_global_errorDiffusion;
	switch (diff)
	{
		case 0:
		{
			if (ORI_TEST)
			{
				dst1 = FloydSteinburg(src);
			}
			else
			{
				//dst1 = DitherFloySteinbarg(src);
				//dst1 = DitherFloySteinBargA(src);
				//cv::imshow("源图像",src);
				dst1 = FloydSteinburgO(src);
				//dst1 = ToValueGrayC(src);
				//cv::imshow("处理后", dst1);
			}
			//cv::imshow("FloySteinBargA", dst1);
			break;
		}
		case 1:
		{
			if (ORI_TEST)
			{
				dst1 = Burkers(src);
			}
			else
			{
				//dst1 = DitherBurkers(src);
				dst1 = BurkersFinal(src);
			}
			//cv::imshow("DitherBurkers", dst1);
			break;
		}
		case 2:
		{
			//dst1 = Sierra(src);
			dst1= SierraFinal(src);
			break;
		}
		case 3:
		{
			//dst1 = Stucki(src);
		    dst1 = StuckiFinal(src);
			break;
		}
		case 4:
		{
			//dst1 = Jarvis(src);
			dst1 = JarvisFinal(src);
			break;
		}
		case 5:
		{
			//dst1 = Stevenson(src);
			dst1 = StevensonFinal(src);
			break;
		}
	}

	if (ORI_TEST)
	{
		int alpha = 4, beta = 4;
		cv::Mat dst = cv::Mat::zeros(cv::Size(src.cols*alpha, src.rows*beta), CV_8UC1);
		cv::RNG rng;
		for (int row = 0; row < dst1.rows; row++)
		{
			for (int col = 0; col < dst1.cols - 4; col = col + 2)
			{
				int value = dst1.at<uchar>(row, col);
				switch (value)
				{
				case 0:
				{

					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;

					break;
				}
				case 32:
				{
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					break;
				}
				case 64:
				{
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					break;
				}
				case 96:
				{
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					break;
				}
				case 128:
				{
					dst.at<uchar>(row * beta, col * alpha + rng.uniform(0, 8)) = 255;
					dst.at<uchar>(row * beta, col * alpha + 1) = 255;
					dst.at<uchar>(row * beta, col * alpha + 2) = 255;
					dst.at<uchar>(row * beta, col * alpha + 3) = 0;
					dst.at<uchar>(row * beta, col * alpha + 4) = 0;
					dst.at<uchar>(row * beta, col * alpha + 5) = 0;
					dst.at<uchar>(row * beta, col * alpha + 6) = 255;
					dst.at<uchar>(row * beta, col * alpha + 7) = 255;
					break;
				}
				case 160:
				{
					dst.at<uchar>(row * beta, col * alpha) = 255;
					dst.at<uchar>(row * beta, col * alpha + 1) = 255;
					dst.at<uchar>(row * beta, col * alpha + 2) = 0;
					dst.at<uchar>(row * beta, col * alpha + 3) = 255;
					dst.at<uchar>(row * beta, col * alpha + 4) = 255;
					dst.at<uchar>(row * beta, col * alpha + 5) = 0;
					dst.at<uchar>(row * beta, col * alpha + 6) = 255;
					dst.at<uchar>(row * beta, col * alpha + 7) = 255;
					break;
				}
				case 192:
				{
					dst.at<uchar>(row * beta, col * alpha) = 255;
					dst.at<uchar>(row * beta, col * alpha + 1) = 255;
					dst.at<uchar>(row * beta, col * alpha + 2) = 255;
					dst.at<uchar>(row * beta, col * alpha + 3) = 255;
					dst.at<uchar>(row * beta, col * alpha + 4) = 255;
					dst.at<uchar>(row * beta, col * alpha + 5) = 255;
					dst.at<uchar>(row * beta, col * alpha + 6) = 255;
					dst.at<uchar>(row * beta, col * alpha + 7) = 0;
					break;
				}
				case 224:
				{
					dst.at<uchar>(row * beta, col * alpha) = 255;
					dst.at<uchar>(row * beta, col * alpha + 1) = 255;
					dst.at<uchar>(row * beta, col * alpha + 2) = 255;
					dst.at<uchar>(row * beta, col * alpha + 3) = 255;
					dst.at<uchar>(row * beta, col * alpha + 4) = 255;
					dst.at<uchar>(row * beta, col * alpha + 5) = 255;
					dst.at<uchar>(row * beta, col * alpha + 6) = 255;
					dst.at<uchar>(row * beta, col * alpha + 6) = 255;
					break;
				}



				}

			}

		}
		return dst;
	}
	else
	{
		return dst1;
	}
}

cv::Mat SMMirror(cv::Mat src)
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

cv::Mat createAlpha(cv::Mat & src)
{
	cv::Mat alpha = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			alpha.at<uchar>(i, j) = 255 - gray.at<uchar>(i, j);
		}
	}
	return alpha;
}

int addAlpha(cv::Mat & src, cv::Mat & dst, cv::Mat & alpha)
{
	if (src.channels() == 4)
	{
		return -1;
	}
	else if (src.channels() == 1)
	{
		cv::cvtColor(src, src, cv::COLOR_GRAY2RGB);
	}
	dst = cv::Mat(src.rows, src.cols, CV_8UC4);
	std::vector<cv::Mat> srcChannels;
	std::vector<cv::Mat> dstChannels;
	//分离通道
	cv::split(src, srcChannels);
	dstChannels.push_back(srcChannels[0]);
	dstChannels.push_back(srcChannels[1]);
	dstChannels.push_back(srcChannels[2]);
	//添加透明度通道
	dstChannels.push_back(alpha);
	//合并通道
	cv::merge(dstChannels, dst);
	return 0;
}

vector<LINE> getContoursLine(cv::Mat src,OutLineStruct data)
{
	// TODO: 在此处添加实现代码.
	vector<LINE> LineSet;

	double f;
	if ((src.cols > 800) || (src.rows > 800))
	{
		if (src.cols >= src.rows)
		{
			f = 800.00 / src.cols;
		}
		else
		{
			f = 800.00 / src.rows;
		}

	}
	else
	{
		f = 1;
	}

	//old  
	/*cv::Mat gray_img = cv::Mat::zeros(cv::Size(round(f*src.cols), round(f*src.rows)), CV_8UC1);
	cv::resize(src, gray_img, cv::Size(round(f*src.cols), round(f*src.rows)), 0, 0, cv::INTER_LINEAR);*/


	//new 
	cv::Mat gray_img = src.clone();

	cv::Mat out = cv::Mat::zeros(gray_img.size(), CV_8UC1);


	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i>     hierarchy;

	//switch (ContoursDetect)
	switch (data.operar)
	{
	case 0:
	{
		//图像边缘检测 
	   //gray_image  单通道输入图像 
	   //输出图像
	   //CannyLowThreshold  第一个阈值
	   //CannyHighThreshold  第二个阈值 
	   // 3  算子内核大小 
	   // cv::Canny(gray_img, gray_img,150, 100 , 3, false);
		cv::Canny(gray_img, gray_img,3,9,3,false);
		//cv::Canny(gray_img, gray_img, CannyLowThreshold, CannyHighThreshold, 3, false);
		break;
	}
	case 1:
	{
		//边缘检测
		cv::Mat g_sobelGradient_X, g_sobelGradient_Y;
		cv::Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;

		//求
		cv::Sobel(gray_img, g_sobelGradient_X, CV_16S, 1, 0, 3, 1,1, cv::BORDER_DEFAULT);
		cv::convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);

		cv::Sobel(gray_img, g_sobelGradient_Y, CV_16S, 0, 1, 3, 1,1, cv::BORDER_DEFAULT);
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

	//switch (getContours)
	switch (data.border)
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

vector<LINE> OptimizeLine(vector<LINE> srcLine)
{
	// TODO: 在此处添加实现代码.
	vector<LINE>dstLine;
	double k1, k2;
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

		for (int j = i + 1; j < srcLine.size(); j++)
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

vector<LINE> OptimizeLineSort(vector<LINE> srcLine)
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
		if (midDist < miniDist)
		{
			miniDist = midDist;
			miniNumber = i;
		}


	}

	//重新定义最近线段的起点和终点
	if (CalculateDistance(srcLine[miniNumber].start, cv::Point(0, 0)) > CalculateDistance(srcLine[miniNumber].end, cv::Point(0, 0)))
	{
		cv::Point temp = srcLine[miniNumber].start;
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




	while (srcLine.size() > 1)
	{
		//printf("dstLine.back条线:(%d,%d) (%d,%d)\r\n", dstLine.back().start.x, dstLine.back().start.y, dstLine.back().end.x, dstLine.back().end.y);

		miniDist = CalculateDistance(srcLine[0].end, dstLine.back().end) > CalculateDistance(srcLine[0].start, dstLine.back().end) ? CalculateDistance(srcLine[0].start, dstLine.back().end) : CalculateDistance(srcLine[0].end, dstLine.back().end);

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
				if (CalculateDistance(srcLine[i].start, dstLine.back().end) <= miniDist)
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

uchar valueSection(uchar input)
{
	if ((input < 0) || (input > 255))
	{
		printf("valueSection input value error!\r\n");
		return -1;
	}


	if (input < 32)
	{
		return 0;
	}
	else if ((input >= 32) && (input < 64))
	{
		return 32;
	}
	else if ((input >= 64) && (input < 96))
	{
		return 64;
	}
	else if ((input >= 96) && (input < 128))
	{
		return 96;
	}
	else if ((input >= 128) && (input < 160))
	{
		return 128;
	}
	else if ((input >= 160) && (input < 192))
	{
		return 160;
	}
	else if ((input >= 192) && (input < 224))
	{
		return 192;
	}
	else
	{
		return 224;
	}
}

cv::Mat OutPoline(cv::Mat src,int thresh)
{
	cv::Mat  threshold_output;
	vector<vector<cv::Point>>   contours;
	vector<cv::Vec4i>   hierarchy;

	//利用Threshold 检测边缘 
	//cv::threshold(src, threshold_output,thresh,255, cv::THRESH_BINARY);
	//利用canny检测边缘
	cv::Canny(src, threshold_output, thresh, thresh * 2, 3);
	//找到轮廓
	cv::findContours(threshold_output,contours,hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	cv::Mat copy1 = threshold_output.clone();

	//将轮廓上的点绘制到图片上  
	cv::RNG rng(12345);
	for (int i = 0; i < contours.size(); ++i)
	{
		for (int j = 0; j < contours[i].size(); ++j)
		{
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//cv::circle(threshold_output,contours[i][j],2, color,-1);
		}
	}

	//获取矩形 和圆形边界框
	vector<vector<cv::Point>> contours_poly(contours.size());
	vector<cv::Rect>   boundRect(contours.size());
	vector<cv::Point2f>  center(contours.size());
	vector<float>       radius(contours.size());

	for (int i = 0; i < contours.size(); ++i)
	{
		//cv::approxPolyDP(cv::Mat(contours[i]),contours_poly[i],3,true);
		//boundRect[i] = boundingRect(cv::Mat(contours_poly[i]));
		//minEnclosingCircle(contours_poly[i],center[i],radius[i]);
	}

	for (int i = 0; i < contours_poly.size(); ++i)
	{
		for (int j = 0; j < contours_poly[i].size(); ++j)
		{
			//cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//cv::circle(copy1,contours_poly[i][j],2,color,-1);
		}
	}

	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC4);
	for (int i = 0; i < contours.size(); i++)
	{
		//cv::Scalar  color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar  color = cv::Scalar(0,0,0,255);
		//cv::drawContours(drawing,contours_poly,i,color,1,8,vector<cv::Vec4i>(),0,cv::Point());
		cv::drawContours(drawing, contours,i,color,1,8, vector<cv::Vec4i>(), 0, cv::Point());
	}

	//   getContoursLineImmediate
	//vector<LINE> OutLine;getContoursLineFromContours   getContoursLineFromPolygon
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
	vector<LINE> OutLine = getContoursLineFromContours(contours,out1);
	drawLine(OutLine, out1);

	return out1;
}

vector<LINE> OutPolineGcode(cv::Mat src, int thresh)
{
	cv::Mat  threshold_output;
	vector<vector<cv::Point>>   contours;
	vector<cv::Vec4i>   hierarchy;

	//利用Threshold 检测边缘 
	//cv::threshold(src, threshold_output,thresh,255, cv::THRESH_BINARY);
	//利用canny检测边缘
	cv::Canny(src, threshold_output, thresh, thresh * 2, 3);
	//找到轮廓
	cv::findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	cv::Mat copy1 = threshold_output.clone();

	//将轮廓上的点绘制到图片上  
	cv::RNG rng(12345);
	for (int i = 0; i < contours.size(); ++i)
	{
		for (int j = 0; j < contours[i].size(); ++j)
		{
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//cv::circle(threshold_output,contours[i][j],2, color,-1);
		}
	}

	//获取矩形 和圆形边界框
	vector<vector<cv::Point>> contours_poly(contours.size());
	vector<cv::Rect>   boundRect(contours.size());
	vector<cv::Point2f>  center(contours.size());
	vector<float>       radius(contours.size());

	for (int i = 0; i < contours.size(); ++i)
	{
		//cv::approxPolyDP(cv::Mat(contours[i]),contours_poly[i],3,true);
		//boundRect[i] = boundingRect(cv::Mat(contours_poly[i]));
		//minEnclosingCircle(contours_poly[i],center[i],radius[i]);
	}

	for (int i = 0; i < contours_poly.size(); ++i)
	{
		for (int j = 0; j < contours_poly[i].size(); ++j)
		{
			//cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			//cv::circle(copy1,contours_poly[i][j],2,color,-1);
		}
	}

	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC4);
	for (int i = 0; i < contours.size(); i++)
	{
		//cv::Scalar  color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar  color = cv::Scalar(0, 0, 0, 255);
		//cv::drawContours(drawing,contours_poly,i,color,1,8,vector<cv::Vec4i>(),0,cv::Point());
		cv::drawContours(drawing, contours, i, color, 1, 8, vector<cv::Vec4i>(), 0, cv::Point());
	}

	//   getContoursLineImmediate
	//vector<LINE> OutLine;getContoursLineFromContours   getContoursLineFromPolygon
	cv::Mat out1 = cv::Mat::zeros(src.size(), CV_8UC4);
	vector<LINE> OutLine = getContoursLineFromContours(contours, out1);
	drawLine(OutLine, out1);

	return OutLine;
}

cv::Mat ToValueGrayC(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	double error;//量化误差
	int nPixelValue;//灰度值
	int xPixelValue;
	int8_t a, b, c, d;//领域像素 

	int imgWidth = dst.cols;
	int imgHeight = dst.rows;
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			nPixelValue = src.at<uint8_t>(i, j);
			xPixelValue = nPixelValue > 128 ? 255 : 0;
			dst.at<uint8_t>(i, j) = xPixelValue;
			error = nPixelValue - xPixelValue;

			if (j + 1 < imgWidth)
				src.at<uint8_t>(i, j) += error * 7.0f / 16;
			if (j - 1 >=0 && i + 1 < imgHeight)
				src.at<uint8_t>(i + 1, j - 1) += error * 3.0f / 16;
			if (i + 1 < imgHeight)
				src.at<uint8_t>(i + 1, j ) += error * 5.0f / 16;
			if (j + 1 < imgWidth && i + 1 < imgHeight)
				src.at<uint8_t>(i + 1, j + 1) += error * 1.0 / 16;
		}
	}

	return dst;

}

cv::Mat FloydSteinburg(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);
	int wc;
	for (int i = 0; i < src.rows - 1; i++)
	{
		for (int j = 1; j < src.cols - 1; j = j + 2)
		{
			dst.at<uchar>(i, j) = valueSection((src.at<uchar>(i, j) + src.at<uchar>(i, j + 1)) / 2);
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

cv::Mat FloydSteinburgO(cv::Mat src)
{
	cv::Mat dithImg = src.clone();
	int wc;
	double error;//量化误差
	int nPixelValue;
	int8_t a, b, c, d;//领域像素 

	int imgWidth = dithImg.cols;
	int imgHeight = dithImg.rows;
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++ )
		{
			//nPixelValue = src.at<uint8_t>(i, j);
			if (dithImg.at<uint8_t>(i, j) > 128)
			{
				error = dithImg.at<uint8_t>(i, j) - 255;//计算误差
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				//dithImg.at<uint8_t>(i, j) = 0;
				error = dithImg.at<uint8_t>(i, j) - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}

			//误差传递
			a = (error * 7) / 16;
			b = (error * 1) / 16;
			c = (error * 5) / 16;
			d = (error * 3) / 16;

			if ((i != (imgHeight - 1)) && (j != 0) && (j != (imgWidth - 1)))
			{
				dithImg.at<uint8_t>(i + 0, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 0, j + 1) + a);
				dithImg.at<uint8_t>(i + 1, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 1) + b);
				dithImg.at<uint8_t>(i + 1, j + 0) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 0) + c);
				dithImg.at<uint8_t>(i + 1, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 1) + d);
			}
		}

	}

	return dithImg;
}

cv::Mat DitherFloySteinbarg(cv::Mat src)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC1);

	int lWidth = src.cols;
	int lHeight = src.rows;
	int i, j;
	//误差传播系数
	double temp, error;
	int nPixelValue;
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth; i++)
		{
			nPixelValue = src.at<uchar>(j, i);

			//128的中值 
			if (nPixelValue > 128)
			{
				dst.at<uchar>(j, i) = 255;//
				error = (double)(nPixelValue - 255.0);//计算误差
			}
			else
			{
				dst.at<uchar>(j, i) = 0;
				error = (double)nPixelValue- 0;
			}

			if (i < lWidth - 1)//如果不是边界
			{
				//向右传播
				temp = (float)src.at<uchar>(j, i + 1);
				temp = temp + error * (1.5 / 8.0);
				if (temp > 255.0)
					temp = 255.0;
				dst.at<uchar>(j,i+1) = temp;

			}
			if (j < lHeight - 1)//如果不是边界
			{
				//向下传播
				temp = (float)src.at<uchar>(j+1,i);
				temp = temp + error * (1.5/8.0);
				dst.at<uchar>(j + 1, i) = (int)temp;
				if (i < lWidth - 1)
				{
					//向右下传播
					temp = (float)src.at<uchar>(j+1,i +1);
					temp = temp + error * (2.0 / 16.0);
					src.at<uchar>(j+1,i+1) = (int)temp;
				}

			}
		}
	}

	return dst;

}

cv::Mat DitherFloySteinBargA(cv::Mat src)
{
	cv::Mat dithImg = src.clone();
	//转换为灰度图
	//cv::cvtColor(src,dithImg,CV_BGR2GRAY);

	int imgWidth = dithImg.cols;
	int imgHeight = dithImg.rows;
	int nPixelValue;
	int err;//误差
	int8_t a, b, c, d;

	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; ++j)
		{
			//nPixelValue = src.at<uint8_t>(i, j);
			if (dithImg.at<uint8_t>(i, j) > 127)
			//if (nPixelValue > 127 )
			{
				err = dithImg.at<uint8_t>(i, j) - 255;
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				err = dithImg.at<uint8_t>(i, j) - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}

			//误差传递
			a = (err * 7) / 16;
			b = (err * 1) / 16;
			c = (err * 5) / 16;
			d = (err * 3) / 16;

			if ((i != (imgHeight - 1)) && (j != 0) && (j != (imgWidth - 1)))
			{
				dithImg.at<uint8_t>(i + 0, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 0, j + 1)+ a);
				dithImg.at<uint8_t>(i+1,j+1) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 1)+ b);
				dithImg.at<uint8_t>(i + 1, j + 0) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 0) + c);
				dithImg.at<uint8_t>(i + 1, j - 1) = (uchar)(dithImg.at<uint8_t>(i+1,j-1)+d);
			}
		}
	}

	return dithImg;
}

cv::Mat Burkers(cv::Mat src)
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
			src.at<uchar>(i, j + 1) = ControlValue((uchar)(src.at<uchar>(i, j + 1) + int(wc * 8 / 32)));
			src.at<uchar>(i, j + 2) = ControlValue((uchar)(src.at<uchar>(i, j + 2) + int(wc * 4 / 32)));
			src.at<uchar>(i + 1, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 2) + int(wc * 2 / 32)));
			src.at<uchar>(i + 1, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 1) + int(wc * 4 / 32)));
			src.at<uchar>(i + 1, j) = ControlValue((uchar)(src.at<uchar>(i + 1, j) + int(wc * 8 / 32)));
			src.at<uchar>(i + 1, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 1) + int(wc * 4 / 32)));
			src.at<uchar>(i + 1, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 2) + int(wc * 2 / 32)));
		}

	}
	return dst;
}

cv::Mat DitherBurkers(cv::Mat src)
{
	cv::Mat dithImg = src.clone();

	// Bayer 表的定义 
	unsigned char BayerPattern[8][8] = { 0, 32, 8, 40, 2, 34, 10, 42,
							48, 16, 56, 24, 50, 18, 58, 26,
							12, 44, 4, 36, 14, 46, 6, 38,
							60, 28, 52, 20, 62, 30, 54, 22,
							3, 35, 11, 43, 1, 33, 9, 41,
							51, 19, 59, 27, 49, 17, 57, 25,
							15, 47, 7, 39, 13, 45, 5, 37,
							63, 31, 55, 23, 61, 29, 53, 21 };

	int lWidth = src.cols;
	int lHeight = src.rows;
	int i, j;
	// 像素的值
	int nPixelValue;

	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth; i++)
		{
			nPixelValue = dithImg.at<uint8_t>(j, i);
			if ((nPixelValue >> 2) > BayerPattern[j & 7][i & 7])
			{
				dithImg.at<uint8_t>(j, i) = 255;
			}
			else
			{
				dithImg.at<uint8_t>(j, i) = 0;
			}
		}
	}

	return dithImg;
}

cv::Mat BurkersFinal(cv::Mat src)
{
	cv::Mat dithImg = src.clone();

	int imgWidth = dithImg.cols;
	int imgHeight = dithImg.rows;
	int nPixelValue;
	int error;//误差
	int8_t a, b, c, d, e, f, g;

	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			//nPixelValue = src.at<uint8_t>(i, j);
			if (dithImg.at<uint8_t>(i, j) > 127)
			{
				error = dithImg.at<uint8_t>(i, j) - 255;//计算误差
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				//dithImg.at<uint8_t>(i, j) = 0;
				error = dithImg.at<uint8_t>(i, j) - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}
			//计算误差 
			a = (error * 8) / 32;
			b = (error * 4) / 32;
			c = (error * 2) / 32;
			d = (error * 4) / 32;
			e = (error * 8) / 32;
			f = (error * 4) / 32;
			g = (error * 2) / 32;
			//第一排的 第一个 
			if (j + 1 < imgWidth)
			{
				dithImg.at<uint8_t>(i , j + 1) = (uchar)(dithImg.at<uint8_t>(i , j + 1) + a);
			}
			//第一排的第二个
			if (j + 2 < imgWidth)
			{
				dithImg.at<uint8_t>(i ,j + 2) = (uchar)(dithImg.at<uint8_t>(i, j + 2) + b);
			}
			if (i + 1 < imgHeight)//第二排 
			{
				//第二排 第一个
				if (j - 2 > 0)
				{
					dithImg.at<uint8_t>(i + 1, j - 2) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 2) + c);
				}
				//第二排 第二个
				if (j - 1 > 0)
				{
					dithImg.at<uint8_t>(i + 1, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 1,j - 1) + d);
				}
				//第二排 第三个
				dithImg.at<uint8_t>(i + 1, j) = (uchar)(dithImg.at<uint8_t>(i + 1,j ) + e);
				//第二排 第四个
				if (j + 1 < imgWidth)
				{
					dithImg.at<uint8_t>(i + 1, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 1) + f);
				}
				//第二排 第五个 
				if (j + 2 < imgWidth)
				{
					dithImg.at<uint8_t>(i + 1, j + 2) = (uchar)(dithImg.at<uint8_t>(i +1, j + 2) + g);
				}
			}
		}
	}

	return dithImg;
}

cv::Mat Sierra(cv::Mat src)
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

cv::Mat SierraFinal(cv::Mat src)
{
	cv::Mat dithImg = src.clone();
	//转换为灰度图
	//cv::cvtColor(src,dithImg,CV_BGR2GRAY);

	int imgWidth = dithImg.cols;
	int imgHeight = dithImg.rows;
	int nPixelValue;
	int err;//误差
	int8_t a,b,c,d,e,f,g,h,k,l;

	for (int i = 0; i < imgHeight; ++i)
	{
		for (int j = 0; j < imgWidth; ++j)
		{
			nPixelValue = dithImg.at<uint8_t>(i, j);
			if (nPixelValue > 127)
			{
				err = nPixelValue - 255;
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				err = nPixelValue - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}

			a = (err * 5) / 32;
			b = (err * 3) / 32;
			c = (err * 2) / 32;
			d = (err * 4) / 32;
			e = (err * 5) / 32;
			f = (err * 4) / 32;
			g = (err * 2) / 32;
			h = (err * 2) / 32;
			k = (err * 3) / 32;
			l = (err * 2) / 32;
			if (j + 1 < imgWidth)//右边第一个像素
			{
				dithImg.at<uint8_t>(i + 0, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 0, j + 1) + a);
			}

			if (j + 2 < imgWidth)//右边第二个像素
			{
				dithImg.at<uint8_t>(i + 0, j + 2) = (uchar)(dithImg.at<uint8_t>(i + 0, j + 2) + b);
			}

			if (i + 1 < imgHeight)//左边第一个像素 二  3个像素
			{
				if (j - 2 > 0)
				{
					dithImg.at<uint8_t>(i+1, j - 2) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 2) + c);
				}
				if (j - 1 > 0)
				{
					dithImg.at<uint8_t>(i + 1, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 1) + d);
				}
				dithImg.at<uint8_t>(i + 1, j ) = (uchar)(dithImg.at<uint8_t>(i + 1, j ) + e);
			}
			if (i + 1 < imgHeight && j + 1 < imgWidth)//右下角第一个像素
			{
				dithImg.at<uint8_t>(i + 1, j+ 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j+1) + f);
			}
			if (i + 1 < imgHeight && j + 2 < imgWidth)//右下角第二个像素
			{
				dithImg.at<uint8_t>(i + 1, j + 2) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 2) + g);
			}
			if (i + 2 < imgHeight && j - 1 > 0)//第三排第一个像素
			{
				dithImg.at<uint8_t>(i + 2, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 2, j - 1 ) + h);
			}
			if (i + 2 < imgHeight)//第三排第二个像素
			{
				dithImg.at<uint8_t>(i + 2, j) = (uchar)(dithImg.at<uint8_t>(i + 2, j) + k);
			}
			if (i+ 2 < imgHeight &&j + 1 <imgWidth)//第三排 第三个像素
			{
				dithImg.at<uint8_t>(i + 2, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 2, j + 1) + l);
			}
		}
	}

	return dithImg;
}

cv::Mat Stucki(cv::Mat src)
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
			src.at<uchar>(i, j + 1) = ControlValue((uchar)(src.at<uchar>(i, j + 1) + wc * 7 / 42));
			src.at<uchar>(i, j + 2) = ControlValue((uchar)(src.at<uchar>(i, j + 2) + wc * 5 / 42));
			src.at<uchar>(i + 1, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 2) + wc * 3 / 42));
			src.at<uchar>(i + 1, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j - 1) + wc * 5 / 42));
			src.at<uchar>(i + 1, j) = ControlValue((uchar)(src.at<uchar>(i + 1, j) + wc * 7 / 42));
			src.at<uchar>(i + 1, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 1) + wc * 5 / 42));
			src.at<uchar>(i + 1, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 1, j + 2) + wc * 3 / 42));
			src.at<uchar>(i + 2, j - 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 2) + wc * 1 / 42));
			src.at<uchar>(i + 2, j - 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j - 1) + wc * 3 / 42));
			src.at<uchar>(i + 2, j) = ControlValue((uchar)(src.at<uchar>(i + 2, j) + wc * 5 / 42));
			src.at<uchar>(i + 2, j + 1) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 1) + wc * 3 / 42));
			src.at<uchar>(i + 2, j + 2) = ControlValue((uchar)(src.at<uchar>(i + 2, j + 2) + wc * 1 / 42));


		}

	}
	return dst;
}

cv::Mat StuckiFinal(cv::Mat src)
{
	cv::Mat dithImg = src.clone();
	//转换为灰度图
	//cv::cvtColor(src,dithImg,CV_BGR2GRAY);

	int imgWidth = dithImg.cols;
	int imgHeight = dithImg.rows;
	int nPixelValue;
	int err;//误差
	int8_t a, b, c, d, e, f, g, h, k, l,m,n;

	for (int i = 0; i < imgHeight; ++i)
	{
		for (int j = 0; j < imgWidth; ++j)
		{
			nPixelValue = dithImg.at<uint8_t>(i, j);
			if (nPixelValue > 127)
			{
				err = nPixelValue - 255;
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				err = nPixelValue - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}

			a = (err * 7) / 42;
			b = (err * 5) / 42;
			c = (err * 3) / 42;
			d = (err * 5) / 42;
			e = (err * 7) / 42;
			f = (err * 5) / 42;
			g = (err * 3) / 42;
			h = (err * 1) / 42;
			k = (err * 3) / 42;
			l = (err * 5) / 42;
			m = (err * 3) / 42;
			n = (err * 1) / 42;

			if (j + 1 < imgWidth)//第一排第一个像素点
			{
				dithImg.at<uint8_t>( i , j + 1) = (uchar)(dithImg.at<uint8_t>(i , j + 1) + a);
			}
			if (j + 2 < imgWidth)//第一排第二个像素点
			{
				dithImg.at<uint8_t>(i, j + 2) = (uchar)(dithImg.at<uint8_t>(i, j + 2) + b);
			}
			if (i + 1 < imgHeight && j - 2 > 0)//第二排 第一个像素点
			{
				dithImg.at<uint8_t>(i + 1, j - 2) = (uchar)(dithImg.at<uint8_t>(i + 1,j -2) +c);
			}
			if (i + 1 < imgHeight && j - 1 > 0)//第二排  第二个像素点  第三个像素点
			{
				dithImg.at<uint8_t>(i + 1, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 1) + d);
				dithImg.at<uint8_t>(i + 1, j ) = (uchar)(dithImg.at<uint8_t>(i + 1, j ) + e);
			}
			if (i + 1 < imgHeight && j + 1 < imgWidth)//第二排第四个像素点
			{
				dithImg.at<uint8_t>(i + 1, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 1) + f);
			}
			if (i + 1 < imgHeight && j + 2 < imgWidth)//第二排 第五个像素点
			{
				dithImg.at<uint8_t>(i + 1, j + 2) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 2) + g);
			}
			if (i + 2 < imgHeight)//第三排的像素点
			{
				if (j - 2 > 0)//第三排第一个像素点
				{
					dithImg.at<uint8_t>(i + 2, j - 2) = (uchar)(dithImg.at<uint8_t>(i + 2, j - 2) + h);
				}
				if (j - 1 > 0)//第三排第二个像素点
				{
					dithImg.at<uint8_t>(i + 2, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 2, j - 1) + k);
				}
				//第三排 第三个像素点
				dithImg.at<uint8_t>(i + 2, j ) = (uchar)(dithImg.at<uint8_t>(i + 2, j ) + l);
				if (j + 1 < imgWidth)//第三排 第四个像素点
				{
					dithImg.at<uint8_t>(i + 2, j +1 ) = (uchar)(dithImg.at<uint8_t>(i + 2, j + 1) + m);
				}
				if (j + 2 < imgWidth)//第三排  第五个像素点
				{
					dithImg.at<uint8_t>(i + 2, j + 2) = (uchar)(dithImg.at<uint8_t>(i + 2, j + 2) + n);
				}

			}
		}
	}
	return dithImg;
}

cv::Mat Jarvis(cv::Mat src)
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

cv::Mat JarvisFinal(cv::Mat src)
{
	cv::Mat dithImg = src.clone();
	//转换为灰度图
	//cv::cvtColor(src,dithImg,CV_BGR2GRAY);

	int imgWidth = dithImg.cols;
	int imgHeight = dithImg.rows;
	int nPixelValue;
	int err;//误差
	int8_t a, b, c, d, e, f, g, h, k, l, m, n;

	for (int i = 0; i < imgHeight; ++i)
	{
		for (int j = 0; j < imgWidth; ++j)
		{
			nPixelValue = dithImg.at<uint8_t>(i, j);
			if (nPixelValue > 127)
			{
				err = nPixelValue - 255;
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				err = nPixelValue - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}

			//计算误差 
			a = (err * 7) / 48;
			b = (err * 5) / 48;
			c = (err * 3) / 48;
			d = (err * 5) / 48;
			e = (err * 7) / 48;
			f = (err * 5) / 48;
			g = (err * 3) / 48;
			h = (err * 1) / 48;
			k = (err * 3) / 48;
			l = (err * 5) / 48;
			m = (err * 3) / 48;
			n = (err * 1) / 48;
			if (j + 1 < imgWidth)//第一排第一个像素点
			{
				dithImg.at<uint8_t>(i, j + 1) = (uchar)(dithImg.at<uint8_t>(i, j + 1) + a);
			}
			if (j + 2 < imgWidth)//第一排第二个像素点
			{
				dithImg.at<uint8_t>(i, j + 2) = (uchar)(dithImg.at<uint8_t>(i, j + 2) + b);
			}
			if (i + 1 < imgHeight && j - 2 > 0)//第二排 第一个像素点
			{
				dithImg.at<uint8_t>(i + 1, j - 2) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 2) + c);
			}
			if (i + 1 < imgHeight && j - 1 > 0)//第二排  第二个像素点  第三个像素点
			{
				dithImg.at<uint8_t>(i + 1, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 1) + d);
				dithImg.at<uint8_t>(i + 1, j) = (uchar)(dithImg.at<uint8_t>(i + 1, j) + e);
			}
			if (i + 1 < imgHeight && j + 1 < imgWidth)//第二排第四个像素点
			{
				dithImg.at<uint8_t>(i + 1, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 1) + f);
			}
			if (i + 1 < imgHeight && j + 2 < imgWidth)//第二排 第五个像素点
			{
				dithImg.at<uint8_t>(i + 1, j + 2) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 2) + g);
			}
			if (i + 2 < imgHeight)//第三排的像素点
			{
				if (j - 2 > 0)//第三排第一个像素点
				{
					dithImg.at<uint8_t>(i + 2, j - 2) = (uchar)(dithImg.at<uint8_t>(i + 2, j - 2) + h);
				}
				if (j - 1 > 0)//第三排第二个像素点
				{
					dithImg.at<uint8_t>(i + 2, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 2, j - 1) + k);
				}
				//第三排 第三个像素点
				dithImg.at<uint8_t>(i + 2, j) = (uchar)(dithImg.at<uint8_t>(i + 2, j) + l);
				if (j + 1 < imgWidth)//第三排 第四个像素点
				{
					dithImg.at<uint8_t>(i + 2, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 2, j + 1) + m);
				}
				if (j + 2 < imgWidth)//第三排  第五个像素点
				{
					dithImg.at<uint8_t>(i + 2, j + 2) = (uchar)(dithImg.at<uint8_t>(i + 2, j + 2) + n);
				}

			}
		}
	}


	return dithImg;
}

cv::Mat Stevenson(cv::Mat src)
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

cv::Mat StevensonFinal(cv::Mat src)
{
	cv::Mat dithImg = src.clone();
	//转换为灰度图
	//cv::cvtColor(src,dithImg,CV_BGR2GRAY);

	int imgWidth = dithImg.cols;
	int imgHeight = dithImg.rows;
	int nPixelValue;
	int err;//误差
	int8_t a, b, c, d, e, f, g, h, k, l, m, n;

	for (int i = 0; i < imgHeight; ++i)
	{
		for (int j = 0; j < imgWidth; ++j)
		{
			nPixelValue = dithImg.at<uint8_t>(i, j);
			if (nPixelValue > 127)
			{
				err = nPixelValue - 255;
				dithImg.at<uint8_t>(i, j) = 255;
			}
			else
			{
				err = nPixelValue - 0;
				dithImg.at<uint8_t>(i, j) = 0;
			}

			//计算误差 
			a = (err * 32) / 200;
			b = (err * 12) / 200;
			c = (err * 26) / 200;
			d = (err * 30) / 200;
			e = (err * 16) / 200;
			f = (err * 12) / 200;
			g = (err * 26) / 200;
			h = (err * 12) / 200;
			k = (err * 5 ) / 200;
			l = (err * 12) / 200;
			m = (err * 12) / 200;
			n = (err * 5 ) / 200;
			//第一排的像素点 +2 
			if (j + 2 < imgWidth)
			{
				dithImg.at<uint8_t>(i, j + 2) = (uchar)(dithImg.at<uint8_t>(i, j + 2) + a);
			}
			//第二排的像素点
			if (i + 1 < imgHeight)
			{
				//第二排 第一个像素点
				if (j - 3 > 0)
				{
					dithImg.at<uint8_t>(i + 1, j - 3) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 3) + b);
				}
				//第二个像素点
				if (j - 1 > 0)
				{
					dithImg.at<uint8_t>(i + 1, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j - 1) + c);
				}
				//第三个像素
				if (j + 1 < imgWidth)
				{
					dithImg.at<uint8_t>(i + 1, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 1) + d);
				}
				//第四个像素点
				if (j + 3 < imgWidth)
				{
					dithImg.at<uint8_t>(i + 1, j + 3) = (uchar)(dithImg.at<uint8_t>(i + 1, j + 3) + e);
				}

			}
			//第三排的像素点 
			if (i + 2 < imgHeight)
			{
				//第一个像素点
				if (j - 2 > 0)
				{
					dithImg.at<uint8_t>(i + 2, j - 2) = (uchar)(dithImg.at<uint8_t>(i + 2, j - 2) + f);
				}
				//第二个像素点
				dithImg.at<uint8_t>(i + 2, j ) = (uchar)(dithImg.at<uint8_t>(i + 2, j ) + g);
				//第三个像素点
				if (j + 2 < imgWidth)
				{
					dithImg.at<uint8_t>(i + 2, j + 2) = (uchar)(dithImg.at<uint8_t>(i + 2, j+ 2) + h);
				}
			}
			//第四排的像素点
			if (i + 3 < imgHeight)
			{
				//第一个像素点
				if (j - 3 > 0)
				{
					dithImg.at<uint8_t>(i + 3, j - 3) = (uchar)(dithImg.at<uint8_t>(i + 3, j - 3) + k);
				}
				if (j - 1 > 0)
				{
					dithImg.at<uint8_t>(i + 3, j - 1) = (uchar)(dithImg.at<uint8_t>(i + 3, j - 1) + l);
				}
				if (j + 1 < imgWidth)
				{
					dithImg.at<uint8_t>(i + 3, j + 1) = (uchar)(dithImg.at<uint8_t>(i + 3, j + 1) + m);
				}
				if (j + 3 < imgWidth)
				{
					dithImg.at<uint8_t>(i + 3, j + 3) = (uchar)(dithImg.at<uint8_t>(i + 3, j + 3) + n);
				}
			}
		}

	}

	return dithImg;
    
}

cv::Mat DuibiDuLiangdu(cv::Mat src, float alpha, float beta)
{
	cv::Mat dst;
	if (!src.data)
	{
		return dst;
	}
	//src = cv::imread("test.jpg");
	//imshow("input image", src);
	int height = src.rows;
	int width = src.cols;
	dst = cv::Mat::zeros(src.size(),src.type());
	int channels = src.channels();
	if (channels == 3)
	{
		//src.convertTo(src,CV_32F);
	}

	for (size_t row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (channels == 3)
			{
				/*float b = src.at<cv::Vec3f>(row, col)[0];
				float g = src.at<cv::Vec3f>(row, col)[1];
				float r = src.at<cv::Vec3f>(row, col)[2];*/
				float b = src.at<cv::Vec3b>(row, col)[0];
				float g = src.at<cv::Vec3b>(row, col)[1];
				float r = src.at<cv::Vec3b>(row, col)[2];

				dst.at<cv::Vec3b>(row, col)[0] = cv::saturate_cast<uchar>(b*alpha + beta);
				dst.at<cv::Vec3b>(row, col)[1] = cv::saturate_cast<uchar>(b*alpha + beta);
				dst.at<cv::Vec3b>(row, col)[2] = cv::saturate_cast<uchar>(b*alpha + beta);
			}
			else if (channels == 1)
			{
				float v = src.at<uchar>(row, col);
				dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(v*alpha + beta);
			}
		}
	}

	//imshow("output image", dst);



	return dst;
}

uchar ControlValue(uchar value)
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

double DistancePointToLine(cv::Point LinePoint1, cv::Point LinePoint2, cv::Point detectPoint)
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

vector<LINE> getContoursLineFromPolygon(vector<vector<cv::Point>> contours)
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
	//printf("ContoursLine:%d\r\n", drawLine.size());

	vector<LINE> OpLine = OptimizeLine(drawLine);
	//printf("OpLine:%d\r\n", OpLine.size());
	return OpLine;
}

vector<LINE> getContoursLineFromContours(vector<vector<cv::Point>> contours, cv::Mat src)
{
	vector<LINE>dstLine;

	cv::Mat conMat = cv::Mat::zeros(src.size(), CV_8UC1);
	vector<cv::Vec4i>hierarchy;
	for (int i = 0; i < contours.size(); i++)
	{
		//if (i != 0)
		{
			drawContours(conMat, contours, i, cv::Scalar(255, 255, 255), 1, 8, hierarchy, 0, cv::Point(0, 0));
		}
	}
	//imshow("conMat",conMat);

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
						L.end = cv::Point(col, row - 1);
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
	int col = 1;
	for (int i = 0; i < (conMat.cols + conMat.rows - 3); i++)
	{
		flag = true;
		LINE L;
		while ((col > 0) && (row < conMat.rows))
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
		if (i < conMat.cols - 1)
		{
			row = 0;
			col = i + 1;
		}
		else
		{
			col = conMat.cols - 1;
			row = i - conMat.cols + 2;
		}
	}


	//-45°直线

	row = 0;
	col = conMat.cols - 2;
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
		if (i < conMat.cols - 1)
		{
			row = 0;
			col = conMat.cols - i - 2;
		}
		else
		{
			col = 0;
			row = i - conMat.cols + 2;
		}
	}

	return dstLine;
}

vector<LINE> getContoursLineImmediate(vector<vector<cv::Point>> contours, cv::Mat src)
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

double CalculateDistance(cv::Point p1, cv::Point p2)
{
	//printf("(%d,%d)(%d,%d)距离为%f\r\n", p1.x, p1.y, p2.x, p2.y, sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)));
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

void deleteLine(vector<LINE>& srcLine, int line)
{
	vector<LINE>::iterator it = srcLine.begin();
	for (int i = 0; i < line; i++)
	{
		it++;
	}
	srcLine.erase(it);
}

GcodeStruct getGcodeWithId(cv::Mat src, int id, QString name, QVariant type,cv::Point offset, double scale)
{
	GcodeStruct  gvevtor;
	switch (id)
	{
	case 0:
		gvevtor = getBinaryImageGcode(src, name, offset, scale);
		break;
	case 3:
		gvevtor = getTextGcode(src, name, offset, scale);
		break;
	case 1:
	{
		int type1 = type.toInt();
		gvevtor = getGrayImageGcode(src, name, type1, offset, scale);
		break;
	}
	case 2:
	{
		OutLineStruct  data = type.value<OutLineStruct>();
		gvevtor = getContoursGcode(src, name,data,offset, scale);
		break;
	}
	}

	return gvevtor;
}

bool GeberateGcodeWithPath(std::vector<LINE> outLine, QString path, cv::Point offset, double scale)
{

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
		*outfile << "G0 F2500" << endl;
		*outfile << "G1 F500" << endl;
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

void GenerateGcode(QList<GcodeStruct> &outLine, QString path)
{
	QFile *myFile = nullptr;
	QTextStream *outfile = nullptr;
	QString filename = path;
	myFile = new QFile(filename);
	myFile->open(QIODevice::WriteOnly | QIODevice::Text);
	outfile = new QTextStream(myFile);

	//开头 
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

	//for (QGraphicsItem *item : items)
	//int i = 0;
	for (GcodeStruct item : outLine)
	{
		GenerateCcodeA(item, outfile);
		//i++;
	}
	//int n = i;
	myFile->close();
	//GenerateCcodeA();
}

void GenerateCcodeA(GcodeStruct itemline, QTextStream* outfile)
{
	*outfile << "G0 F" << QString::number(itemline.diandongSpeed) << endl;//点动速度
	*outfile << "M2 "<< QString::number(itemline.laserPower)<< endl;//激光能量 
	*outfile << "G1 F"<< QString::number(itemline.diaokeSpeed) << endl;//雕刻速度 

	//double scale = itemline.scale;
	double scale = 1600 / 200;
	cv::Point offset = itemline.offset;

	for (int k = 0; k < itemline.outLine.size(); ++k)
	{
		if (k == 0)
		{
			*outfile << "G0 " << "X" << itemline.outLine[0].start.x / scale + offset.x/scale << " Y" << itemline.outLine[0].start.y / scale + offset.y/scale << endl;
			*outfile << "M3" << endl;
			*outfile << "G1 " << "X" << itemline.outLine[0].end.x / scale + offset.x/ scale << " Y" << itemline.outLine[0].end.y / scale + offset.y/scale << endl;
		}
		else
		{
			if ((itemline.outLine[k - 1].end.x == itemline.outLine[k].start.x) && (itemline.outLine[k - 1].end.y == itemline.outLine[k].start.y))
			{
				*outfile << "G1 " << "X" << itemline.outLine[k].end.x / scale + offset.x/scale<< " Y" << itemline.outLine[k].end.y / scale + offset.y/scale << endl;
			}
			else
			{
				*outfile << "M5" << endl;
				*outfile << "G0 " << "X" << itemline.outLine[k].start.x / scale + offset.x /scale<< " Y" << itemline.outLine[k].start.y / scale + offset.y/scale << endl;
				*outfile << "M3" << endl;
				*outfile << "G1 " << "X" << itemline.outLine[k].end.x / scale + offset.x/scale << " Y" << itemline.outLine[k].end.y / scale + offset.y /scale<< endl;

			}
		}


	}
	*outfile << "M5" << endl;
	*outfile << endl;
	*outfile << "; G-code END <<<" << endl;
	
}

vector<LINE> getGrayTrack(cv::Mat src)
{
	LINE L;
	vector<LINE>LineSet;
	bool LineFlag = true;
	bool order = true;
	int space = 1;
	for (int row = 0; row < src.rows; row = row + space)
	{

		//if (row % 2 == 1)
		{
			if (order)
				//if (row % 2 == 1)
				//if (1)
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
							//L.end = cv::Point(col,row);
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
	}

	return LineSet;
}

vector<QLine> getGrayTrackQ(cv::Mat src)
{
	QLine L;
	vector<QLine>LineSet;
	bool LineFlag = true;
	bool order = true;
	for (int row = 0; row < src.rows; row = row + 1)
	{

		//if (row % 2 == 1)
		{
			if (order)
				//if (row % 2 == 1)
				//if (1)
			{
				order = false;
				for (int col = 0; col < src.cols; col++)
				{
					if (src.at<uchar>(row, col) == 0)//黑色
					{
						if (LineFlag)
						{
							L.setP1(QPoint(col, row));
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
							L.setP2(QPoint(col - 1, row));
							//L.end = cv::Point(col,row);
							LineSet.push_back(L);
							LineFlag = true;
						}

					}

				}
				if (!LineFlag)
				{
					L.setP1( QPoint(src.cols - 1, row));
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
							L.setP1(QPoint(col, row));
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
							L.setP2( QPoint(col + 1, row));
							LineSet.push_back(L);
							LineFlag = true;
						}
					}

				}
				if (!LineFlag)
				{
					L.setP2(QPoint(0, row));
					LineSet.push_back(L);
					LineFlag = true;
				}
			}


		}
	}

	return LineSet;
}

cv::Mat getDuibidu(cv::Mat src)
{
	cv::Mat dst;
	cv::Mat  kernel = (cv::Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	cv::filter2D(src, dst, -1, kernel);
	return dst;
}

vector<LINE> ReduceDotLine(vector<LINE> input)
{
	vector<LINE> dst;
	dst.clear();
	for (int i = 0; i < input.size(); i++)
	{
		if ((input[i].start.x == input[i].end.x) && (input[i].start.y == input[i].end.y))
		{

		}
		else
		{
			dst.push_back(input[i]);
		}
	}
	return dst;
}
