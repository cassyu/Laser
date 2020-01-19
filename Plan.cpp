#include "image_gcode.h"
//声明

//函数定义

//-----------------------------------【路径规划】---------------------------------- 
//寻找最优点部分，正确
void plan(vector< vector<cv::Point> > &lunkuo, cv::Mat & chushitu, FILE *fp)
{

	cv::Scalar color = cv::Scalar(255, 255, 255);//黑色轮廓
	cv::Point origin;
	origin.x = 0;
	origin.y = 0;//创建原点
	cv::Point * p = new  cv::Point[2 * lunkuo.size()];//创建一个储存每条路径起点终点的二维数组，其中第一行储存的每次的起点，第二行储存每次的终点
	int * lunkuonum = new  int[lunkuo.size()];//创建储存已连接的轮廓序号的数组
	for (int i = 0; i < lunkuo.size(); i++)
		lunkuonum[i] = -1;//结合pointandlunkuo函数写的代码
	int* diannum1 = new  int[lunkuo.size()];//储存连接轮廓的终点的点序号
	int* qidianlunkuo = new  int[lunkuo.size()];//储存起点，类似上面
	int* qidiandian = new  int[lunkuo.size()];
	double *a = pointandlunkuo(origin, lunkuo, lunkuonum, lunkuo.size());//计算原点
	lunkuonum[0] = int(a[0]);
	diannum1[0] = int(a[1]);
	p[0] = origin;
	p[1] = lunkuo[lunkuonum[0]].at(diannum1[0]);//第一步的起点终点确定
	double a1 = 0.0;
	double a2 = 0.0;
	double minjuli;
	int qidianlunkuo1;
	int qidiandian1;


	//一系列定义，为后面得到的距离，点的储存开辟空间
	for (int g = 1; g <= (lunkuo.size() - 1); g++)//接下来的代码是利用最小生成树prim算法依次确定每一步连接的起点和终点连接的轮廓序号和点序号
	{
		
		int tiaoshi;



		for (int k = 0; k <g; k++)//依次计算已经连接的轮廓和未连接的轮廓
		{

			for (int i = 0; i < lunkuo[lunkuonum[k]].size(); i = i + DISTANCE)//从上述所说的储存已连接轮廓序号的数组的第一个元素开始算
			{
				double *chucun = pointandlunkuo(lunkuo[lunkuonum[k]].at(i), lunkuo, lunkuonum, lunkuo.size());//感觉错误应该出在这个地方。
				double w1 = chucun[0];
				double w2 = chucun[1];
				double w3 = chucun[2];


				if ((k == 0) && (i == 0))//第一次计算储存初始量
				{
					minjuli = chucun[2];
					a1 = chucun[0];
					a2 = chucun[1];
					qidianlunkuo1 = lunkuonum[k];//储存起点
					qidiandian1 = i;
				}
				else
				{
					double xiangbijuli = chucun[2];//之后的计算根据大小更新起点终点
					if (xiangbijuli < minjuli)
					{
						minjuli = xiangbijuli;
						a1 = chucun[0];
						a2 = chucun[1];
						qidianlunkuo1 = lunkuonum[k];
						qidiandian1 = i;
					}
				}

			}
		}


		{
			lunkuonum[g] = a1;
			diannum1[g] = a2;
			qidianlunkuo[g] = qidianlunkuo1;
			qidiandian[g] = qidiandian1;
			p[2 * g] = lunkuo[qidianlunkuo[g]].at(qidiandian[g]);

			//circle(srcImage, p[2 * g], 3, Scalar(0, 255, 0), -1, 8, 0);
			p[2 * g + 1] = lunkuo[lunkuonum[g]].at(diannum1[g]);//与原点得到的长度比较
																//circle(srcImage, p[2 * g + 1], 3, Scalar(0, 0, 255), -1, 8, 0);
		
			//printf("\n\n【Gcode如下：】\ng28\nG1 X%.3f Y%.3f F3600\nG1 Z0.300 F3600.000\nG1 E0.50000 F1800.000\n", (vContours[i].at(j).x) / 1000, (vContours[i].at(j).y) / 1000);
			//fprintf(fp, "G28\nG1 X%.3f Y%.3f F3600\nG1 Z0.30 F3600.000\nG1 E0.50000 F1800.000\n",
				//(vContours[i].at(j).x) / 1000, (vContours[i].at(j).y) / 1000);
			//line(srcImage, p[0], p[1], Scalar(255, 0, 0), 0, 8);
			//line(srcImage, p[2 * g], p[2 * g + 1], Scalar(255, 0, 0), 0, 8);
			tiaoshi = lunkuonum[g];
			;

		}


	}

	int ap = 0;
	int aq = 0;
	fuzhu(lunkuonum, lunkuo.size(), diannum1, lunkuo.size(), qidianlunkuo, lunkuo.size(), qidiandian, lunkuo.size());//
	for (int i = 0; i < lunkuo.size(); i = i + 1)
	{
		ap = qidianlunkuo[i];
		aq = qidiandian[i];
	}
	//画线生成切片图部分，正确
	cv::Mat yhqiepian = cv::Mat::zeros(chushitu.size(), CV_8UC3); //目标图像清零

	//line(yhqiepian, p[0], p[1], color, 0, 8);
	//printf("\n\n【Gcode如下：】\ng28\nG1 X%.3f Y%.3f F3600\nG1 Z0.300 F3600.000\nG1 E0.50000 F1800.000\n", (vContours[i].at(j).x) / 1000, (vContours[i].at(j).y) / 1000);
	//fprintf(fp, "G28\nG1 X%u Y%u F3600\nG1 Z0.30 F3600.000\nG1 E0.50000 F1800.000\n",
	fprintf(fp, "G28\nG1 X%u Y%u F3600\n",
		(p[1].x) /hh, (p[1].y) / hh);
	int x0 = p[1].x;
	int y0 = p[1].y;
	int qi1 = 1;
	int qi2 = 0;
	int tiaoship;
	int tiaoshipp;

	do

	{
		int  * zhong = paixu(qi1, qi2, yhqiepian, lunkuo, lunkuonum, lunkuo.size(), diannum1, lunkuo.size(), qidianlunkuo, lunkuo.size(), qidiandian, lunkuo.size());
		tiaoship = zhong[0];
		tiaoshipp = zhong[1];
		int tiaoshi3 = zhong[3];
		int tiaoshi2 = zhong[2];
		if (qi1 == 1 && tiaoshi3 == 1)//起点是终点，终点也是终点vContours[i].at(j + DISTANCE),
									  //circle(drawing, lunkuo[lunkuonum[qi2]].at(diannum1[qi2]), 3, Scalar(255, 255, 255), -1, 8, 0);
			huaxian(lunkuonum[qi2], diannum1[qi2], lunkuonum[tiaoshi2], diannum1[tiaoshi2], yhqiepian, lunkuo,fp);
		else if (qi1 == 0 && tiaoshi3 == 1)//起点是起点，终点也是终点
										   //circle(drawing, lunkuo[lunkuonum[qi2]].at(diannum1[qi2]), 3, Scalar(255, 255, 255), -1, 8, 0);
			huaxian(qidianlunkuo[qi2], qidiandian[qi2], lunkuonum[tiaoshi2], diannum1[tiaoshi2], yhqiepian, lunkuo, fp);
		else if (qi1 == 1 && tiaoshi3 == 0)//起点是终点，终点也是起点
			huaxian(lunkuonum[qi2], diannum1[qi2], qidianlunkuo[tiaoshi2], qidiandian[tiaoshi2], yhqiepian, lunkuo, fp);
		else//起点是起点，终点也是起点
			huaxian(qidianlunkuo[qi2], qidiandian[qi2], qidianlunkuo[tiaoshi2], qidiandian[tiaoshi2], yhqiepian, lunkuo, fp);



		if (tiaoship != lunkuonum[0] || tiaoshipp != diannum1[0])
		{
			if (tiaoshi3 == 1)
			{
				huaxian(tiaoship, tiaoshipp, qidianlunkuo[tiaoshi2], qidiandian[tiaoshi2], yhqiepian, lunkuo, fp);
				qi1 = 0;
				qi2 = tiaoshi2;
			}
			else
			{
				huaxian(tiaoship, tiaoshipp, lunkuonum[tiaoshi2], diannum1[tiaoshi2], yhqiepian, lunkuo, fp);
				qi1 = 1;
				qi2 = tiaoshi2;
			}
		}

	} while (tiaoship != lunkuonum[0] || tiaoshipp != diannum1[0]);
	


	delete[] lunkuonum;
	delete[] diannum1;
	delete[] qidianlunkuo;
	delete[] qidiandian;
	cv::Point tiaoshi1 = p[5];

	delete[] p;


	return;
}
void huaxian(int qdlknum, int qddnum, int zdlknum, int zddnum, cv::Mat & yhqiepian, vector< vector<cv::Point> > &lunkuo, FILE *fp)
{
	float E = 0.5;//定义基础量
	cv::Scalar color = cv::Scalar(255, 255, 255);
	if (qdlknum == zdlknum)
	{
		if (zddnum == qddnum)
		{
			for (unsigned int i = qddnum; i < (lunkuo[qdlknum].size() - DISTANCE); i = i + DISTANCE)
			{
				//line(yhqiepian, lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE), color, 0, 8);
				// 显示效果图间隔时间
				//imshow("切片图", yhqiepian);//动态显示图片
				//E = CalculateDistance(lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE));
				//printf("G1 X%.3f Y%.3f E0%.5f F600\n", vContours[i].at(j).x, vContours[i].at(j).y, E);
				fprintf(fp, "G1 X%u Y%u F600\n", (lunkuo[qdlknum].at(i))/hh, (lunkuo[qdlknum].at(i + DISTANCE))/hh, E);
				
			}
			for (int i = 0; i < (zddnum - DISTANCE); i = i + DISTANCE)
			{
				//line(yhqiepian, lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE), color, 0, 8);
				// 显示效果图间隔时间
				//imshow("切片图", yhqiepian);//动态显示图片
				//E = CalculateDistance(lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE));
				//printf("G1 X%.3f Y%.3f E0%.5f F600\n", vContours[i].at(j).x, vContours[i].at(j).y, E);
				fprintf(fp, "G1 X%u Y%u F600\n", (lunkuo[qdlknum].at(i)) / hh, (lunkuo[qdlknum].at(i + DISTANCE)) / hh, E);
				
			}
		}
		else if (zddnum < qddnum)
		{
			for (unsigned int i = qddnum; i < (lunkuo[qdlknum].size() - DISTANCE); i = i + DISTANCE)
			{
				//line(yhqiepian, lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE), color, 0, 8);
				// 显示效果图间隔时间
				//imshow("切片图", yhqiepian);//动态显示图片
				//E = CalculateDistance(lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE));
				//printf("G1 X%.3f Y%.3f E0%.5f F600\n", vContours[i].at(j).x, vContours[i].at(j).y, E);
				fprintf(fp, "G1 X%u Y%u F600\n", (lunkuo[qdlknum].at(i)) / hh, (lunkuo[qdlknum].at(i + DISTANCE)) / hh, E);
				
			}
			for (int i = 0; i <(zddnum - DISTANCE); i = i + DISTANCE)
			{
				//line(yhqiepian, lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE), color, 0, 8);
				// 显示效果图间隔时间
				//imshow("切片图", yhqiepian);//动态显示图片
				//E = CalculateDistance(lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE));
				//printf("G1 X%.3f Y%.3f E0%.5f F600\n", vContours[i].at(j).x, vContours[i].at(j).y, E);
				fprintf(fp, "G1 X%u Y%u F600\n", (lunkuo[qdlknum].at(i)) / hh, (lunkuo[qdlknum].at(i + DISTANCE)) / hh, E);
				
			}
		}
		else
			for (int i = qddnum; i <(zddnum - DISTANCE); i = i + DISTANCE)
			{
				//line(yhqiepian, lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE), color, 0, 8);
				// 显示效果图间隔时间
				//imshow("切片图", yhqiepian);//动态显示图片
				//E = CalculateDistance(lunkuo[qdlknum].at(i), lunkuo[qdlknum].at(i + DISTANCE));
				//printf("G1 X%.3f Y%.3f E0%.5f F600\n", vContours[i].at(j).x, vContours[i].at(j).y, E);
				fprintf(fp, "G1 X%u Y%u F600\n", (lunkuo[qdlknum].at(i)) / hh, (lunkuo[qdlknum].at(i + DISTANCE)) / hh, E);
		
			}
	}
	else
	{
		//line(yhqiepian, lunkuo[qdlknum].at(qddnum), lunkuo[zdlknum].at(zddnum), color, 0, 8);

		//imshow("切片图", yhqiepian);//动态显示图片
		//E = CalculateDistance(lunkuo[qdlknum].at(qddnum), lunkuo[zdlknum].at(zddnum));
		//printf("G1 F1800.000 E%.5f\nG1 Z2.300 F3600.000\nG92 E0\nG1 X%.3f Y%.3f F3600.000\nG1 Z0.300 F3600.000\nG1 E0.50000 F1800.000\n ", vContours[i].at(j).x, vContours[i].at(j).y, E);
		fprintf(fp, "G1 X%u Y%u F3600.000\n",
			(lunkuo[qdlknum].at(qddnum))/hh, (lunkuo[zdlknum].at(zddnum))/hh, E);
	
	}
	return;

}
double * pointandlunkuo(cv::Point& point, vector< vector<cv::Point> > & lunkuo, int w[], int n)//该函数是计算一个点距离未连接的轮廓的最短距离 
																					   //参数有点，轮廓的集合，储存已经连接的点的序号的数组和大小，正确
{
	double * d = new double[lunkuo.size()];//定义一个轮廓数大小的数组，用来储存点到轮廓的距离
	double a[3];//定义一个3维数组，用来储存最终得到的一个点距离最近的轮廓上的点（即连接的终点）和这个距离大小
	a[2] = 0.0;
	int k = 0;
	double dnew;
	int f = 0;
	int biaoji = 0;
	for (int i = 0; i < lunkuo.size(); i++)
	{

		for (int u = 0; u < lunkuo.size(); u++)//利用一个循环，先判断此时的i即轮廓序号是否已经连接，若连接，跳向下一个循环即i+1
			if (i == w[u])
				//goto sameend;
				biaoji = 1;
		if (biaoji == 0)
		{


			for (int j = 0; j < lunkuo[i].size(); j = j + DISTANCE)//计算出该点到第i个轮廓的最短距离和终点
			{
				if (j == 0)
				{
					d[i] = CalculateDistance(point, lunkuo[i].at(0));

					if (k == 0)
					{
						a[2] = d[i];
						a[0] = i;
						a[1] = j;
					}
				}//初始距离储存
				dnew = CalculateDistance(point, lunkuo[i].at(j));//循环得到的新距离 
				if (d[i] > dnew)//比较大小 
				{
					d[i] = dnew;
					if (k > 0)//如果k>0，则说明此时已经算完了至少一个轮廓，所以这时候将新轮廓的结果与前一个结果比较
					{
						if (d[i] < a[2])//若di小，这时候就更新终点信息
						{
							a[0] = i;
							a[1] = j;
							a[2] = d[i];
						}
					}
					else//否则，应该是i=0，那就把终点信息储存起来
					{
						a[0] = i;
						a[1] = j;
						a[2] = d[i];
					}
				}
			}
			k++;
			//if (f > 0)
			//f = 0;
		}

		biaoji = 0;
	}
	delete[] d;
	return a;
}
double CalculateDistance(cv::Point & Point1, cv::Point  &Point2)//获取距离
{
	double ds;
	ds = sqrt((double)((Point1.x - Point2.x)*(Point1.x - Point2.x) + (Point1.y - Point2.y)*(Point1.y - Point2.y)));
	return ds;
}
int * paixu(int p, int xuhao,cv::Mat & yhqiepian, vector< vector<cv::Point> > &lunkuo, int lunkuonum[], int lm, int diannum1[], int dm, int qidianlunkuo[], int qlm, int  qidiandian[], int qdm)
{
	int b[4];
	int re;
	int jilui = 0;
	int jilui1 = 0;
	int kd = 0;
	int kx = 0;
	int kx1 = 0;
	int kd1 = 0;
	int jiluii = 0;
	int jilui11 = 0;
	int qdlknum;
	int qddnum;
	if (p == 1)
	{
		qdlknum = lunkuonum[xuhao];
		qddnum = diannum1[xuhao];
	}
	else
	{
		qdlknum = qidianlunkuo[xuhao];
		qddnum = qidiandian[xuhao];
	}
	for (int i = 0; i < lm; i++)
		if (qdlknum == lunkuonum[i])

		{
			if (diannum1[i] > qddnum)
			{
				if (kd == 0)
					jilui = i;//初始化
				else if (diannum1[i] < diannum1[jilui])
					jilui = i;//更新
				kd++;
			}
			else if (diannum1[i] < qddnum)
			{
				if (kx == 0)
					jiluii = i;//初始化
				else if (diannum1[i] < diannum1[jiluii])
					jiluii = i;//更新
				kx++;
			}
		}
	for (int i = 0; i < lm; i++)
		if (qdlknum == qidianlunkuo[i])

		{
			if (qidiandian[i] >qddnum)
			{
				if (kd1 == 0)
					jilui1 = i;
				else if (qidiandian[i] < qidiandian[jilui1])
					jilui1 = i;
				kd1++;
			}
			else if (qidiandian[i] < qddnum)
			{
				if (kx1 == 0)
					jilui11 = i;
				else if (qidiandian[i] < qidiandian[jilui11])
					jilui11 = i;
				kx1++;
			}
		}
	if ((kx == 0) && (kd == 0) && (kx1 == 0) && (kd1 == 0))
	{
		if (p == 1)
		{
			b[0] = lunkuonum[xuhao];
			b[1] = diannum1[xuhao];
			b[2] = xuhao;
			b[3] = p;
		}
		else
		{
			b[0] = qidianlunkuo[xuhao];
			b[1] = qidiandian[xuhao];
			b[2] = xuhao;
			b[3] = p;
		}


	}
	else if (kd > 0 && kd1>0)
	{
		if (qidiandian[jilui1]< diannum1[jilui])
		{
			b[0] = qidianlunkuo[jilui1];
			b[1] = qidiandian[jilui1];
			b[2] = jilui1;
			b[3] = 0;
		}
		else
		{
			b[0] = lunkuonum[jilui];
			b[1] = diannum1[jilui];
			b[2] = jilui;
			b[3] = 1;
		}
	}
	else if (kd>0)
	{
		b[0] = lunkuonum[jilui];
		b[1] = diannum1[jilui];
		b[2] = jilui;
		b[3] = 1;
	}
	else if (kd1>0)
	{
		b[0] = qidianlunkuo[jilui1];
		b[1] = qidiandian[jilui1];
		b[2] = jilui1;
		b[3] = 0;
	}
	else if (kx > 0 && kx1 > 0)
	{
		if (qidiandian[jilui11] <diannum1[jiluii])
		{
			b[0] = qidianlunkuo[jilui11];
			b[1] = qidiandian[jilui11];
			b[2] = jilui11;
			b[3] = 0;
		}
		else
		{
			b[0] = lunkuonum[jiluii];
			b[1] = diannum1[jiluii];
			b[2] = jiluii;
			b[3] = 1;
		}
	}
	else if (kx>0)
	{
		b[0] = lunkuonum[jiluii];
		b[1] = diannum1[jiluii];
		b[2] = jiluii;
		b[3] = 1;
	}
	else if (kx1>0)
	{
		b[0] = qidianlunkuo[jilui11];
		b[1] = qidiandian[jilui11];
		b[2] = jilui11;
		b[3] = 0;
	}
	return b;
}
//将重合的点按一个单位分离，以便于后面的操作，会产生误差，但误差足够小，正确
void fuzhu(int lunkuonum[], int lm, int diannum1[], int dm, int qidianlunkuo[], int qlm, int  qidiandian[], int qdm)
{

	for (int i = 0; i < lm; i++)
		for (int j = i + 1; j < lm; j++)
		{
			int k = 0;
			if ((qidianlunkuo[i] == qidianlunkuo[j]) && (qidiandian[i] == qidiandian[j]))
			{
				k++;
				qidiandian[j] = qidiandian[j] + k;
			}
		}
	for (int i = 0; i < lm; i++)
		for (int j = 0; j < lm; j++)
		{
			if ((qidianlunkuo[i] == lunkuonum[j]) && (qidiandian[i] == diannum1[j]))
			{

				diannum1[j] = diannum1[j] + 1;
			}
		}


}
