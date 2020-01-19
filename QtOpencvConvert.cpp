#include "QtOpencvConvert.h"
#include "opencv2/imgproc/types_c.h"
#include <QImage>

QImage cvMat2QImage(const cv::Mat & mat, bool clone, bool rb_swap)
{
	const uchar*pSrc = (const uchar*)mat.data;
	if (mat.type() == CV_8UC1)
	{
		// QImage::Format_Grayscale8     24 
		QImage image(pSrc,mat.cols,mat.rows,mat.step, QImage::Format_Grayscale8);
		//QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
		if (clone)
		{
			return image.copy();
		}
		return image;
	}
	else if (mat.type() == CV_8UC3)
	{
		// Create QImage with same dimensions as input Mat
		//old  
		QImage image(pSrc,mat.cols,mat.rows,mat.step,QImage::Format_RGB888);
		//new 
		//QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		if (clone)
		{
			if (rb_swap)
			{
				return image.rgbSwapped();
			}
			return image.copy();
		}
		else
		{
			if (rb_swap)
			{
				cv::cvtColor(mat,mat,CV_BGR2RGB);
			}
			if (image.hasAlphaChannel())
			{
				int n = 10;
			}
			//image.seta
			return image;
		}
	}
	else if (mat.type() == CV_8UC4)
	{
		QImage image(pSrc,mat.cols,mat.rows,mat.step,QImage::Format_ARGB32);
		if (clone)
		{
			return image.copy();
		}
		return image;
	}
	else
	{
		return QImage();
	}
}

cv::Mat QImage2cvMat(const QImage & image, bool clone, bool rb_swap)
{
	cv::Mat  mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(),image.width(),CV_8UC4,(void*)image.constBits(),image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(),image.width(),CV_8UC3,(void*)image.constBits(),image.bytesPerLine());
		if (clone) mat = mat.clone();
		if (rb_swap) cv::cvtColor(mat,mat,CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
	case QImage::Format_Grayscale8:
		mat = cv::Mat(image.height(),image.width(),CV_8UC1,(void*)image.bits(),image.bytesPerLine());
		if (clone)
			mat = mat.clone();
		break;
	default:
		break;
	}

	return mat;
}
