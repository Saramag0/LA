#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
using namespace cv;



int main(int argc, char* argv[])
{
	string imagePath = argv[1];
	Mat image = imread(imagePath, IMREAD_COLOR);
	//imshow("answercard", image);
	//waitKey(0);

	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);//灰度圖
	threshold(gray, gray, 40, 255, THRESH_BINARY_INV);//二值化
	//imshow("gray", gray);
	//waitKey(0);
	//vector<vector<Point>> contour;
	//findContours(gray, contour, noArray(), RETR_LIST, CHAIN_APPROX_SIMPLE);
	//Mat contourimage = gray.clone();
	//contourimage = Scalar::all(0);
	//drawContours(contourimage, contour, -1, Scalar::all(255));
	//Mat blur;
	//GaussianBlur(gray, blur, Size(7, 7), 11, 11);
	//imshow("picture", contourimage);
	//waitKey(0);

	Mat canny;
	Canny(gray, canny, 50, 150);
	//imshow("picture", canny);
	//waitKey(0);

	//cout << canny;

	Point2f leftup;
	int temp1 = 2000;
	Point2f rightup;
	int temp2 = 2000;
	Point2f leftdown;
	int temp3 = 2000;
	Point2f rightdown;
	int temp4 = 2000;
	for (int i = 0; i < canny.rows; i++)
	{
		for (int j = 0; j < canny.cols; j++)
		{
			//int temp2i = 0, temp2j = 0;
			if (canny.at<uchar>(i, j) == 255)
			{
				if ((i + j) < temp1)
				{
					leftup.x = j;
					leftup.y = i;
					temp1 = i + j;
				}
				if (i + (canny.cols - j) < temp2)
				{
					rightup.x = j;
					rightup.y = i;
					temp2 = i + (canny.cols - j);
				}
				if ((canny.rows - i) + j < temp3)
				{
					leftdown.x = j;
					leftdown.y = i;
					temp3 = (canny.rows - i) + j;
				}
				if ((canny.rows - i) + (canny.cols - j) < temp4)
				{
					rightdown.x = j;
					rightdown.y = i;
					temp4 = (canny.rows - i) + (canny.cols - j);
				}
			}
		}
	}

	//circle(image, leftup, 3, (0, 255, 0), -1);
	//circle(image, rightup, 3, (0, 255, 0), -1);
	//circle(image, leftdown, 3, (0, 255, 0), -1);
	//circle(image, rightdown, 3, (0, 255, 0), -1);
	////imshow("picture", image);
	////waitKey(0);
	//imwrite("C:\\Users\\patri\\Desktop\\c++\\簡廷因\\Opencv\\Opencv\\testimage.jpg", image);

	Point2f src[] =
	{
		leftdown,rightdown,rightup,leftup
	};
	Point2f dst[] =
	{
		Point2f(0,image.rows - 1),
		Point2f(image.cols - 1,image.rows - 1),
		Point2f(image.cols - 1,0),
		Point2f(0,0)
	};

	Mat M = getPerspectiveTransform(src, dst);
	Mat perspective;
	warpPerspective(image, perspective, M, image.size(), INTER_LINEAR);
	//imshow("transform", perspective);
	//waitKey(0);
	//imwrite("C:\\Users\\patri\\Desktop\\c++\\簡廷因\\Opencv\\Opencv\\testimage2.jpg", perspective);

	Mat aftergray;
	cvtColor(perspective, aftergray, COLOR_BGR2GRAY);//灰度圖
	threshold(aftergray, aftergray, 115, 255, THRESH_BINARY);//二值化
	//dilate(aftergray, aftergray, Mat());
	//Canny(aftergray, aftergray, 50, 150);
	//imwrite("C:\\Users\\patri\\Desktop\\c++\\簡廷因\\Opencv\\Opencv\\testimage2.jpg", aftergray);


	float i = 545;
	for (i; i < 570; i++)
	{
		if (aftergray.at<uchar>(i, 100) == 0)
		{
			i += 4;
			break;
		}
	}
	//Point2f temp(100, i);//第一個點100 556  第一格270
 //                                   //上下差32 左右差61
 //   circle(aftergray, temp, 5, (0, 255, 0), -1);
	//imshow("transform", aftergray);
	//waitKey(0);

	vector<int> ans;
	for (i; i < 1300; i += 31.92)
	{
		int number = 1;
		int tempanswer = 0, check = 0;
		for (float j = 275; j < 950; j += 61)
		{
			//Point2f temp(j, i);
			//circle(aftergray, temp, 2, (0, 255, 0), -1);
			if (aftergray.at<uchar>(i, j) == 0 || aftergray.at<uchar>(i, j - 2) == 0
				|| aftergray.at<uchar>(i, j + 2) == 0 || aftergray.at<uchar>(i, j + 4) == 0)
			{
				if (check == 0)
				{
					tempanswer = number;
					check++;
				}
				else
					tempanswer = 20;
			}
			number++;
		}
		ans.push_back(tempanswer);
	}
	//imshow("transform", aftergray);
	//waitKey(0);

	//for (int i = 0; i < ans.size(); i++)
	//{
	//	cout << ans[i] << " ";
	//}

	ofstream output(argv[2]);
	for (int i = 0; i < ans.size(); i++)
	{
		if (ans[i] >= 1 && ans[i] <= 9)
			output << ans[i];
		else if (ans[i] == 10)
			output << "0";
		else if (ans[i] == 11)
			output << "A";
		else if (ans[i] == 12)
			output << "B";
		else if (ans[i] == 20)
			output << "M";
		else if (ans[i] == 0)
			output << "X";
	}
	output << endl;
}