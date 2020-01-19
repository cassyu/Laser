#include "mainwindow.h"
#include <opencv2/imgproc/imgproc_c.h>

//void plan(vector< vector<Point> > &lunkuo, Mat & chushitu);//路径规划
//-----------------------------------【ProcesseImage( )函数】------------------------------
void ProcesseImage(cv::Mat canny_output)
{
	vector < vector <cv::Point > > vContours;
	vector<cv::Vec4i> vHierarchy;
    /*Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);//目标图像清零
    Mat gcode = Mat::zeros(canny_output.size(), CV_8UC3);//目标图像清零
    threshold(drawing, drawing, -1, 255, THRESH_BINARY);//背景变白
    threshold(gcode, gcode, -1, 255, THRESH_BINARY);

    vector< vector<Point> > vContours;
              vector<Vec4i> vHierarchy;*/
    FILE *fp = fopen("gcode.txt", "r+");
	cv::findContours(canny_output, vContours, vHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));
	plan(vContours, canny_output,fp);
        /*findContours(canny_output, vContours, vHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));	// 寻找轮廓*/
    /*int *use_vContours;//记录有效的轮廓
    use_vContours = (int *)malloc(sizeof(int)*vContours.size());
    if (!use_vContours)
    {
        cout << "Not Enough Memory!";
        exit(0) ;
    }
    for (unsigned int i = 0; i < vContours.size(); i++)
        use_vContours[i] = 0;
    for (unsigned int i = 0; i < vContours.size(); i++)
    {
        if (CheckContours(vContours, use_vContours,i) == 1)
        {
            Scalar color = Scalar(0, 0, 255);//黑色轮廓
            drawContours(drawing, vContours, i, color, 1, 8, vHierarchy, 0);
            use_vContours[i] = 1;
        }
    }
    //imshow("轮廓图", drawing);
    vector< vector<Point> > vContours_seed = vContours;
    float E = 0.5;//定义基础量
    for (unsigned int i = 0; i < vContours.size(); i++)
    {
        if (use_vContours[i] == 1)
        {
            for (unsigned int j = 0; j < vContours[i].size() - DISTANCE; j = j + DISTANCE)
            {
                Scalar color = Scalar(0, 0, 255);//红色轮廓
                if (j > vContours[i].size() - DISTANCE)
                    j =  vContours[i].size() - DISTANCE - 1;
                line(gcode, vContours[i].at(j), vContours[i].at(j + DISTANCE), color, 1, 8);
                // 显示效果图
                //imshow("切片图", gcode);
                //waitKey(10);
                if (i == 0 && j == 0)//初始点
                {
                    //printf("\n\n【Gcode如下：】\ng28\nG1 X%.3f Y%.3f F3600\nG1 Z0.300 F3600.000\nG1 E0.50000 F1800.000\n", (vContours[i].at(j).x) / 1000, (vContours[i].at(j).y) / 1000);
                    fprintf(fp, "G28\nG1 X%.3f Y%.3f F3600\nG1 Z0.30 F3600.000\nG1 E0.50000 F1800.000\n",
                        (vContours[i].at(j).x)/1000, (vContours[i].at(j).y)/1000);
                }
                if (j == DISTANCE)//F600
                {
                    E = CalculateDistacne(vContours[i].at(j), vContours[i].at(j - DISTANCE));
                    //printf("G1 X%.3f Y%.3f E0%.5f F600\n", vContours[i].at(j).x, vContours[i].at(j).y, E);
                    fprintf(fp, "G1 X%.3f Y%.3f E0%.5f F600\n",vContours[i].at(j).x, vContours[i].at(j).y, E);

                }
                if (j != 0 && j != DISTANCE)//其他点
                {
                    E = CalculateDistacne(vContours[i].at(j), vContours[i].at(j - DISTANCE));
                    //printf("G1 X%.3f Y%.3f E0%.5f\n ", vContours[i].at(j).x, vContours[i].at(j).y, E);
                    fprintf(fp, "G1 X%.3f Y%.3f E0%.5f\n ",vContours[i].at(j).x, vContours[i].at(j).y, E);

                }
                if (i != 0 && j == 0)//换轮廓空程
                {
                    int m = 0;
                    if (m>DISTANCE) m = vContours[i - 1].size() - (vContours[i - 1].size()) % (DISTANCE)-1;
                    else m = 0;
                    E = CalculateDistacne(vContours[i].at(j), vContours[i - 1].at(m));
                    //printf("G1 F1800.000 E%.5f\nG1 Z2.300 F3600.000\nG92 E0\nG1 X%.3f Y%.3f F3600.000\nG1 Z0.300 F3600.000\nG1 E0.50000 F1800.000\n ", vContours[i].at(j).x, vContours[i].at(j).y, E);
                    fprintf(fp, "G1 F1800.000 E%.5f\nG1 Z2.300 F3600.000\nG92 E0\nG1 X%.3f Y%.3f F3600.000\nG1 Z0.300 F3600.000\nG1 E0.50000 F1800.000\n ",
                        vContours[i].at(j).x, vContours[i].at(j).y, E);

                }
            }
        }

    }*/
    fclose(fp);
    
}

float CalculateDistacne(cv::Point Point1, cv::Point Point2)//获取距离
{
    float ds;
    ds = sqrt((float)((Point1.x - Point2.x)*(Point1.x - Point2.x) + (Point1.y - Point2.y)*(Point1.y - Point2.y)));
    return ds;
}
