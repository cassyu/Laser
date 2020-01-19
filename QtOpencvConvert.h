#pragma once
#ifndef QT_OPENCV_CONVERT_H
#define QT_OPENCV_CONVERT_H

#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

QImage cvMat2QImage(const cv::Mat& mat,bool clone = false,bool rb_swap = false);
//
cv::Mat  QImage2cvMat(const QImage& image,bool clone = false,bool rb_swap = false);


#endif 