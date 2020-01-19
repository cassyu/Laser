#include "mainwindow.h"
int CheckContours(vector< vector<cv::Point> > vContours, int *use_vContours, unsigned int check)
{
    cv::Rect bound = boundingRect((vContours[check]));
    cv::Point center;
    center.x = bound.x + bound.width/2;
    center.y = bound.y + bound.height/2;
    for (unsigned int i = 0; i < vContours.size(); i++)
    {
        if (i != check && use_vContours[i] == 1)
        {
            cv::Rect temp = boundingRect((vContours[i]));
            cv::Point temp_center;
            temp_center.x = temp.x + temp.width / 2;
            temp_center.y = temp.y + temp.height / 2;
            if (abs(bound.width - temp.width) / 2<7 && abs(bound.height - temp.height) / 2<7&& CalculateDistacne(center, temp_center) < 5.0)
                return 0;
        }
    }
    if (contourArea(vContours[check],true)<100 && vContours[check].size()<50)return 0;
    return 1;
}
