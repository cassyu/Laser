#pragma once
#include <opencv2/opencv.hpp>
#include <QString>

//using namespace cv;
using namespace std;


extern int G_global_errorDiffusion;
extern int G_global_ContourDetect;
extern int G_global_GetContours;
extern int G_global_ButtonType;
extern int G_global_LaserPower;
extern int G_global_Threshold;//全局的阈值  黑与白 与 矢量的阈值都用这个

struct LINE
{
	cv::Point start;
	cv::Point end;
};
class LaserGcodeGenerate
{
	
protected:
	QString  filename;
	double Length;
	double width;
	int errorDiffusion;
	int ImageMaxSize;
	int CannyLowThreshold;
	int CannyHighThreshold;
	int ContoursDetect;
	int getContours;

public:
	LaserGcodeGenerate();
	~LaserGcodeGenerate();

	int getColSum(cv::Mat src, int col); //统计所有列的总和

	int getRowSum(cv::Mat src, int row);//统计所有行的总和
	void cutTop(cv::Mat& src,cv::Mat& dstImg); //切掉图片的上下空白
	int cutLeft(cv::Mat& src, cv::Mat& leftImg, cv::Mat& rightImg); //切掉左边空白和数字切割
	//
	void getBinaryImageGcode(cv::Mat src);
	void getTextGcode(cv::Mat src);
	void getGrayImageGcode(cv::Mat src);
	void getContoursGcode(cv::Mat src);

	void getGcodeWithId(cv::Mat src, int id);

	void SetParameter(int errorDiffusion, int ImageMaxSize, int CannyLowThreshold, int CannyHighThreshold,
		int ContoursDetect, int getContours);
	void SetFilename(QString& name);

	cv::Mat GrayScale(cv::Mat src);
	//误差扩散滤波器
	uchar valueSection(uchar input);
	cv::Mat FloydSteinburg(cv::Mat src);
	cv::Mat Burkers(cv::Mat src);
	cv::Mat Sierra(cv::Mat src);
	cv::Mat Stucki(cv::Mat src);
	cv::Mat Jarvis(cv::Mat src);
	cv::Mat Stevenson(cv::Mat src);

	vector<cv::Point> GetPoint(cv::Mat src);
	vector<LINE> getLine(vector<cv::Point> PointSet);
	cv::Mat BinaryImage(cv::Mat src);

	void GenerateGcode(vector<LINE> outLine);
	bool GeberateGcodeWithPath(vector<LINE> outLine, QString path = "gcode2.gcode");

	vector<LINE> getContoursLine(cv::Mat src);
	vector<LINE> OptimizeLine(vector<LINE> srcLine);
	vector<LINE> OptimizeLineSort(vector<LINE> srcLine);
	double CalculateDistance(cv::Point p1,cv::Point p2);
	void deleteLine(vector<LINE>&srcLine, int line);
	double DistancePointToLine(cv::Point LinePoint1, cv::Point LinePoint2, cv::Point detectPoint);
	void drawLine(vector<LINE> src, cv::Mat &out);
	cv::Mat ResizeImage(cv::Mat src);
	uchar ControlValue(uchar value);
	vector<LINE> getContoursLineFromPolygon(vector<vector<cv::Point>> contours);
	vector<LINE> getContoursLineFromContours(vector<vector<cv::Point>> contours,cv::Mat src);
	vector<LINE> getContoursLineImmediate(vector<vector<cv::Point>> contours, cv::Mat src);
	cv::Mat SMMirror(cv::Mat src);
	vector<LINE> getTrack(cv::Mat src);
	vector<LINE> getGrayTrack(cv::Mat src);
};

