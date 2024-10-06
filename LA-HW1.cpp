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
	fstream input;
	input.open(argv[1], ios::in);
	//cout << argv[1] << endl;
	string infoPath, imagePath, maskPath;
	int level1 = 0, width1 = 0, level2 = 0, width2 = 0;
	input >> infoPath >> imagePath >> maskPath;
	input >> level1 >> width1 >> level2 >> width2;

	//info儲存
	string info[20][2];
	ifstream inFile(infoPath, ios::in);
	string line;
	int counter = 0;
	while (getline(inFile, line)) {
		//分析每行數據
		stringstream ss(line);
		string sub;
		vector<string> subarray;

		while (getline(ss, sub, ','))
			subarray.push_back(sub);
		for (int i = 0; i < subarray.size(); i++) {
			info[counter][i] = subarray[i];
		}
		counter++;
	}

	fstream imageFile;
	imageFile.open(imagePath, ios::in);
	vector<vector<double>> tempimage;
	vector<vector<double>> tempimage2;
	string icline;
	while (getline(imageFile, icline)) {
		//cout << icline << endl;
		istringstream sss(icline);
		string str;
		vector<double> row;
		vector<double> row2;
		while (getline(sss, str, ',')) {
			double p = atof(str.c_str());
			double h = p * stof(info[16][1]) + stof(info[17][1]);

			double w = h;
			double w2 = h;
			if (w <= level1 - width1 / 2)
				w = 0;
			else if (w > level1 + width1 / 2)
				w = 255;
			else {
				w = ((w - level1) / width1 + 0.5) * 255;
			}
			row.push_back(w);

			if (w2 <= level2 - width2 / 2)
				w2 = 0;
			else if (w2 > level2 + width2 / 2)
				w2 = 255;
			else {
				w2 = ((w2 - level2) / width2 + 0.5) * 255;
			}
			row2.push_back(w2);
		}
		tempimage.push_back(row);
		tempimage2.push_back(row2);
	}

	Mat image;
	image.create(512, 512, CV_8U);
	for (int i = 0; i < tempimage.size(); i++) {
		for (int j = 0; j < tempimage[i].size(); j++) {
			image.at<uchar>(i, j) = (uchar)(tempimage[i][j]);
		}
	}

	Mat image2;
	image2.create(512, 512, CV_8U);
	for (int i = 0; i < tempimage2.size(); i++) {
		for (int j = 0; j < tempimage2[i].size(); j++) {
			image2.at<uchar>(i, j) = (uchar)(tempimage2[i][j]);
		}
	}

	imwrite(argv[2], image);

	fstream maskFile;
	maskFile.open(maskPath, ios::in);
	vector<vector<double>> tempmask;
	string mline;
	while (getline(maskFile, mline)) {
		istringstream iss(mline);
		string str;
		vector<double> row;
		while (getline(iss, str, ',')) {
			double p = atof(str.c_str());
			double m = floor(255 * p / 16);
			row.push_back(m);
		}
		tempmask.push_back(row);
	}

	Mat mask;
	mask.create(512, 512, CV_8U);
	for (int i = 0; i < tempmask.size(); i++) {
		for (int j = 0; j < tempmask[i].size(); j++) {
			mask.at<uchar>(i, j) = (uchar)(tempmask[i][j]);
		}
	}

	vector<Mat> newChannel;
	newChannel.push_back(mask);
	newChannel.push_back(image);
	newChannel.push_back(image2);

	Mat BGRimage;
	merge(newChannel, BGRimage);

	Mat result;
	result = mask.clone();
	inRange(mask, 40, 50, result);
	//imshow("result", result);//大腦鎌圖片
	//waitKey(0);
	//Mat edges;
	//Canny(result, edges, 150, 100);

	//fitline or houghline

	/*vector<cv::Point> points;

	for (int x = 0; x < result.cols; x++)
		for (int y = 0; y < result.rows; y++)
			points.push_back(cv::Point(x, y));

	Vec4f fitline;
	fitLine(points, fitline, DIST_L2, 0, 0.01, 0.01);
	cout << fitline << endl;*/

	vector<Vec2f> lines;
	for (int i = 512; i > 50; i--) {
		HoughLines(result, lines, 0.7, CV_PI / 180.0, i, 0, 0);
		if (lines.size() > 0) break;
	}

	double meanAngle = 0;

	Mat Redline;
	Mat Redresult;
	cvtColor(mask, Redline, COLOR_GRAY2BGR);
	cvtColor(result, Redresult, COLOR_GRAY2BGR);

	float rho = lines[0][0], theta = lines[0][1];

	meanAngle += theta;
	Point pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a * rho, y0 = b * rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));

	cv::line(Redline, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	cv::line(Redresult, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);

	meanAngle = meanAngle * 180 / CV_PI;
	if (meanAngle > 45) meanAngle -= 180;
	//cout << fixed << setprecision(6) << meanAngle << endl;

	ofstream angle;
	angle.open(argv[4], ios::out);
	angle << fixed << setprecision(6) << meanAngle << endl;

	//cv::line(Redline, total1, total2, Scalar(0, 0, 255), 1, LINE_AA);

	//imshow("redline", Redline);
	//cv::waitKey(0);
	//imshow("redresult", Redresult);//大腦鎌圖片
	//cv::waitKey(0);

	Point2f center(BGRimage.rows / 2, BGRimage.cols / 2);
	Mat rot = cv::getRotationMatrix2D(center, meanAngle, 1.0);
	Mat rotateimage;
	warpAffine(BGRimage, rotateimage, rot, BGRimage.size());
	//warpAffine(Redresult, Redresult, rot, Redresult.size());
	//imshow("redline", Redresult);//大腦鎌圖片
	//cv::waitKey(0);
	//imshow("rotate", rotateimage);
	//waitKey(0);

	imwrite(argv[3], rotateimage);
	return 0;
}