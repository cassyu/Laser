

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QString>
#include <QList>
#include "LaserGcodeGenerate.h"
#include "OutLineImage.h"

//#include <vector>

class QTextStream;

extern int errorDiffusion;//灰度算法 
extern int ImageMaxSize;
extern int CannyLowThreshold;
extern int  CannyHighThreshold;
extern int ContoursDetect;
extern int getContours;
extern int g_ContoursGray;
extern int global_grayValue;

#define  REAL_SIZE    2000
#define  RECT_SIZE    800
#define  ORI_TEST     false 
#define  VECTOR_SELECT  false
#define  LINE_DENSITY  2
#define  LIEN_BLACK_DEBNSITY  2 
constexpr auto W_RATE = ((float)2000/800);
constexpr auto Q_RATE = ((float)800/130);

//像素比率    A：3.78 B:为了扩大 除以2  C:再次为了扩大  等于1 
constexpr float PIXEL_RATE =  3.78 / 2;

struct GcodeStruct
{
	vector<LINE>    outLine;
	cv::Point       offset;
	double          scale;
	int             diandongSpeed  = 50;//  50到1000
	int             laserPower = 500;//默认500 
	int             diaokeSpeed = 50;//  50到1000
};

//输入图像  生成黑白图像 
GcodeStruct getTextGcode(cv::Mat src, QString filename, cv::Point offset = cv::Point(80, 80), double scale = 10);
GcodeStruct  getBinaryImageGcode(cv::Mat src,QString filename,cv::Point offset = cv::Point(80, 80), double scale = 10);
GcodeStruct getGrayImageGcode(cv::Mat src, QString filename,int type= 0, cv::Point offset = cv::Point(80, 80), double scale = 10);
GcodeStruct getContoursGcode(cv::Mat src, QString filename, OutLineStruct data = OutLineStruct(),cv::Point offset = cv::Point(80, 80), double scale = 10);


cv::Mat  getTextGcodeOnlyImage(cv::Mat  src);
cv::Mat  getBinaryImageGcodeOnlyImage(cv::Mat src,int thresh = 168);
cv::Mat   getGrayImageGcodeOnlyImage(cv::Mat src, int diff);
cv::Mat   getContoursGcodeOnlyImage(cv::Mat src, OutLineStruct data = OutLineStruct());
cv::Mat   getGcodeWithIdOnlyImage(cv::Mat src,int id,OutLineStruct data = OutLineStruct());


vector<LINE> getTextGcodeOnlyShape(cv::Mat  src);//文字好像不需要这样处理
vector<LINE> getBinaryImageGcodeShape(cv::Mat src, int thresh = 0);
vector<LINE> getGrayImageGcodeShape(cv::Mat src, int diff);//
vector<LINE> getContoursGcodeShape(cv::Mat src, OutLineStruct data = OutLineStruct());
vector<LINE> getGcodeWithIdShape(cv::Mat src, int id);

int getColSum(cv::Mat src, int col); //统计所有列的总和

int getRowSum(cv::Mat src, int row);//统计所有行的总和
void cutTop(cv::Mat& src, cv::Mat& dstImg); //切掉图片的上下空白
int cutLeft(cv::Mat& src, cv::Mat& leftImg, cv::Mat& rightImg); //切掉左边空白和数字切割

//二值化处理图像
cv::Mat BinaryImage(cv::Mat src,int thresh = 0);

vector<LINE> getTrack(cv::Mat src);

void drawLine(std::vector<LINE> src, cv::Mat &out);
void drawGrayLine(std::vector<LINE> src, cv::Mat &out);

cv::Mat GrayScale(cv::Mat src,int diff);

cv::Mat SMMirror(cv::Mat src);

cv::Mat  createAlpha(cv::Mat& src);

int addAlpha(cv::Mat& src, cv::Mat& dst, cv::Mat& alpha);

vector<LINE> getContoursLine(cv::Mat src, OutLineStruct data = OutLineStruct());

vector<LINE> OptimizeLineSort(vector<LINE> srcLine);

uchar valueSection(uchar input);
cv::Mat OutPoline(cv::Mat src,int thresh = 128);//新的轮廓检测
vector<LINE> OutPolineGcode(cv::Mat src,int thresh = 128);
cv::Mat ToValueGrayC(cv::Mat src);
cv::Mat FloydSteinburg(cv::Mat src);
cv::Mat FloydSteinburgO(cv::Mat src);//对原来方法的改进
cv::Mat DitherFloySteinbarg(cv::Mat src);
cv::Mat DitherFloySteinBargA(cv::Mat src);
cv::Mat Burkers(cv::Mat src);
cv::Mat DitherBurkers(cv::Mat src);
cv::Mat BurkersFinal(cv::Mat src);
cv::Mat Sierra(cv::Mat src);
cv::Mat SierraFinal(cv::Mat src);
cv::Mat Stucki(cv::Mat src);
cv::Mat StuckiFinal(cv::Mat src);
cv::Mat Jarvis(cv::Mat src);
cv::Mat JarvisFinal(cv::Mat src);
cv::Mat Stevenson(cv::Mat src);
cv::Mat StevensonFinal(cv::Mat src);

cv::Mat DuibiDuLiangdu(cv::Mat src,float alpha,float beta);

uchar ControlValue(uchar value);

double DistancePointToLine(cv::Point LinePoint1, cv::Point LinePoint2, cv::Point detectPoint);

vector<LINE> OptimizeLine(vector<LINE> srcLine);

//vector<LINE> OptimizeLineSort(vector<LINE> srcLine);


vector<LINE> getContoursLineFromPolygon(vector<vector<cv::Point>> contours);
vector<LINE> getContoursLineFromContours(vector<vector<cv::Point>> contours, cv::Mat src);
vector<LINE> getContoursLineImmediate(vector<vector<cv::Point>> contours, cv::Mat src);

double CalculateDistance(cv::Point p1, cv::Point p2);

void deleteLine(vector<LINE>&srcLine, int line);


//根据id生成对应的gcode 
GcodeStruct getGcodeWithId(cv::Mat src, int id,QString name,QVariant type= 0,cv::Point offset = cv::Point(80, 80), double scale = 10);

bool GeberateGcodeWithPath(std::vector<LINE> outLine, QString path = "gcode2.gcode", cv::Point offset = cv::Point(80, 80), double scale = 10);

void GenerateGcode(QList<GcodeStruct> &outLine,QString name);
void GenerateCcodeA(GcodeStruct outline, QTextStream*outfile);

vector<LINE> getGrayTrack(cv::Mat src);
vector<QLine>   getGrayTrackQ(cv::Mat src);
//float getGrayC();

cv::Mat  getDuibidu(cv::Mat src);

vector<LINE> ReduceDotLine(vector<LINE> input);