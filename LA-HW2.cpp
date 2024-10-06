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

void translation(vector<Mat>& list, double c[])
{
	Mat temp = Mat::eye(4, 4, CV_64F);
	temp.at<double>(0, 3) = -c[0];
	temp.at<double>(1, 3) = -c[1];
	temp.at<double>(2, 3) = -c[2];
	list.push_back(temp);
}

void transfomation(vector<Mat>& list, string transform, ifstream& input)
{
	Mat temp = Mat::eye(4, 4, CV_64F);
	if (transform == "#T")
	{
		double txyz[3]{};
		input >> txyz[0] >> txyz[1] >> txyz[2];
		temp.at<double>(0, 3) = txyz[0];
		temp.at<double>(1, 3) = txyz[1];
		temp.at<double>(2, 3) = txyz[2];
		list.push_back(temp);
	}
	else if (transform == "#Pyz")
	{
		temp.at<double>(0, 0) = 0;
		list.push_back(temp);
	}
	else if (transform == "#Pxz")
	{
		temp.at<double>(1, 1) = 0;
		list.push_back(temp);
	}
	else if (transform == "#Pxy")
	{
		temp.at<double>(2, 2) = 0;
		list.push_back(temp);
	}
	else if (transform == "#S")
	{
		double c[3]{ 0 }, k[3]{ 0 };
		input >> c[0] >> c[1] >> c[2];
		//先平移負的點c
		translation(list, c);

		input >> k[0] >> k[1] >> k[2];
		temp.at<double>(0, 0) = k[0];
		temp.at<double>(1, 1) = k[1];
		temp.at<double>(2, 2) = k[2];
		temp.at<double>(0, 3) = c[0];
		temp.at<double>(1, 3) = c[1];
		temp.at<double>(2, 3) = c[2];
		list.push_back(temp);
	}
	else if (transform == "#Rx")
	{
		double c[3]{ 0 };
		double theta = 0;
		input >> c[0] >> c[1] >> c[2];
		input >> theta;
		//先平移負的點c
		//cout << c[0] << c[1] << c[2];
		translation(list, c);

		theta = theta * CV_PI / 180;
		temp.at<double>(0, 3) = c[0];
		temp.at<double>(1, 3) = c[1];
		temp.at<double>(2, 3) = c[2];
		temp.at<double>(1, 1) = cos(theta);
		temp.at<double>(1, 2) = -sin(theta);
		temp.at<double>(2, 1) = sin(theta);
		temp.at<double>(2, 2) = cos(theta);

		list.push_back(temp);
	}
	else if (transform == "#Ry")
	{
		double c[3]{ 0 };
		double theta = 0;
		input >> c[0] >> c[1] >> c[2];
		input >> theta;
		//先平移負的點c
		translation(list, c);

		theta = theta * CV_PI / 180;
		temp.at<double>(0, 3) = c[0];
		temp.at<double>(1, 3) = c[1];
		temp.at<double>(2, 3) = c[2];
		temp.at<double>(0, 0) = cos(theta);
		temp.at<double>(2, 0) = -sin(theta);
		temp.at<double>(0, 2) = sin(theta);
		temp.at<double>(2, 2) = cos(theta);

		list.push_back(temp);
	}
	else if (transform == "#Rz")
	{
		double c[3]{ 0 };
		double theta = 0;
		input >> c[0] >> c[1] >> c[2];
		input >> theta;
		//先平移負的點c
		translation(list, c);

		theta = theta * CV_PI / 180;
		temp.at<double>(0, 3) = c[0];
		temp.at<double>(1, 3) = c[1];
		temp.at<double>(2, 3) = c[2];
		temp.at<double>(0, 0) = cos(theta);
		temp.at<double>(0, 1) = -sin(theta);
		temp.at<double>(1, 0) = sin(theta);
		temp.at<double>(1, 1) = cos(theta);

		list.push_back(temp);
	}
	else if (transform == "#Hx")
	{
		double c[3]{ 0 }, s = 0, ht = 0;
		input >> c[0] >> c[1] >> c[2];
		input >> s >> ht;
		//先平移負的點c
		translation(list, c);

		temp.at<double>(0, 3) = c[0];
		temp.at<double>(1, 3) = c[1];
		temp.at<double>(2, 3) = c[2];
		temp.at<double>(0, 1) = s;
		temp.at<double>(0, 2) = ht;

		list.push_back(temp);
	}
	else if (transform == "#Hy")
	{
		double c[3]{ 0 }, s = 0, ht = 0;
		input >> c[0] >> c[1] >> c[2];
		input >> s >> ht;
		//先平移負的點c
		translation(list, c);

		temp.at<double>(0, 3) = c[0];
		temp.at<double>(1, 3) = c[1];
		temp.at<double>(2, 3) = c[2];
		temp.at<double>(1, 0) = s;
		temp.at<double>(1, 2) = ht;

		list.push_back(temp);
	}
	else if (transform == "#Hz")
	{
		double c[3]{ 0 }, s = 0, ht = 0;
		input >> c[0] >> c[1] >> c[2];
		input >> s >> ht;
		//先平移負的點c
		translation(list, c);

		temp.at<double>(0, 3) = c[0];
		temp.at<double>(1, 3) = c[1];
		temp.at<double>(2, 3) = c[2];
		temp.at<double>(2, 0) = s;
		temp.at<double>(2, 1) = ht;

		list.push_back(temp);
	}
	else if (transform == "#M")
	{
		double m[4][4]{ 0 };
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				input >> m[x][y];
				temp.at<double>(x, y) = m[x][y];
			}
		}
		list.push_back(temp);
	}
}

vector<Vec4d> getpoint(Vec4d vec)
{
	double x = vec[0];
	double y = vec[1];
	double z = vec[2];
	int fx = floor(x), fy = floor(y), fz = floor(z);
	int cx = ceil(x), cy = ceil(y), cz = ceil(z);

	vector<Vec4d> eight;
	for (int i = 0; i < 8; i++)
	{
		Vec4d temp;
		if (i % 2 == 0)temp[0] = fx;
		else temp[0] = cx;
		if ((i / 2) % 2 == 0)temp[1] = fy;
		else temp[1] = cy;
		if ((i / 4) % 2 == 0)temp[2] = fz;
		else temp[2] = cz;
		temp[3] = 1;
		eight.push_back(temp);
	}

	return eight;
}

double getvalue(Vec4d vec, vector<Mat> table)
{
	int x = vec[0], y = vec[1], z = vec[2];
	if (z >= table.size() || z < 0)
		return 0;
	if (y > table.at(0).cols - 1 || y < 0)
		return 0;
	if (x > table.at(0).rows - 1 || x < 0)
		return 0;

	return table.at(z).at<double>(x, y);
}

int trilinearInterpolation(Vec4d point, vector<Mat> table)
{
	vector<Vec4d> eight = getpoint(point);
	double interpolationX[4]{ 0 }, interpolationY[2]{ 0 };
	double x = point[0];
	double y = point[1];
	double z = point[2];
	int fx = floor(x), fy = floor(y), fz = floor(z);
	int cx = ceil(x), cy = ceil(y), cz = ceil(z);

	double leftdiff = cx - x, rightdiff = x - fx;
	if (fx == cx)
	{
		interpolationX[0] = getvalue(eight[6], table);//0 1 a2
		interpolationX[1] = getvalue(eight[4], table);//2 3 a3
		interpolationX[2] = getvalue(eight[2], table);//4 5 a0
		interpolationX[3] = getvalue(eight[0], table);//6 7 a1
	}
	else
	{
		interpolationX[0] = getvalue(eight[6], table) * leftdiff + getvalue(eight[7], table) * rightdiff;
		interpolationX[1] = getvalue(eight[4], table) * leftdiff + getvalue(eight[5], table) * rightdiff;
		interpolationX[2] = getvalue(eight[2], table) * leftdiff + getvalue(eight[3], table) * rightdiff;
		interpolationX[3] = getvalue(eight[0], table) * leftdiff + getvalue(eight[1], table) * rightdiff;
	}

	double downdiff = cy - y, updiff = y - fy;
	if (fy == cy)
	{
		interpolationY[0] = interpolationX[1];
		interpolationY[1] = interpolationX[3];
	}
	else
	{
		interpolationY[0] = interpolationX[0] * updiff + interpolationX[1] * downdiff;// b1
		interpolationY[1] = interpolationX[2] * updiff + interpolationX[3] * downdiff;// b0
	}

	double fdiff = z - fz, bdiff = cz - z, ans = 0;
	if (fz == cz)
		ans = interpolationY[1];
	else
		ans = interpolationY[0] * fdiff + interpolationY[1] * bdiff;

	return (int)ans;
}

int main(int argc, char* argv[])
{
	ifstream input1(argv[1]);
	//input1.open(argv[1]);

	vector<Vec4d> vin;
	for (int i = 0; i < 5; i++) {
		double x, y, z;
		input1 >> x >> y >> z;
		vin.push_back(Vec4d(x, y, z, 1));
	}

	Mat d1(4, 4, CV_64F);
	for (int i = 0; i < 4; i++)
	{
		d1.at<double>(i, 0) = vin[0][i];
		d1.at<double>(i, 1) = vin[1][i];
		d1.at<double>(i, 2) = vin[2][i];
		d1.at<double>(i, 3) = vin[3][i];
	}
	//cout << d1 << endl;
	double determinant1 = abs(determinant(d1) / 6);
	//cout << determinant1 << endl;

	//get matrix
	int n = 0;
	vector<Mat> list;
	input1 >> n;
	Mat firstmatrix = Mat::eye(4, 4, CV_64F);
	list.push_back(firstmatrix);

	for (int i = 0; i < n; i++)
	{
		string transform;
		input1 >> transform;
		transfomation(list, transform, input1);
	}

	string outputPath = argv[2];
	ofstream output1(outputPath);

	for (int i = 1; i < list.size(); i++)
	{
		list[0] = list[i] * list[0];
		//cout << list[0] << endl;
	}
	for (int i = 0; i < list[0].rows; i++)
	{
		for (int j = 0; j < list[0].cols; j++)
		{
			output1 << fixed << setprecision(2) << list[0].at<double>(i, j);
			if (j != list[0].cols - 1)output1 << " ";
		}
		output1 << endl;
	}

	vector<Mat> vectorMat;
	vector<Vec4d> vectorVec;
	for (int i = 0; i < 4; i++)
	{
		vectorMat.push_back(list[0] * vin[i]);
		//cout << list[0] * vin[i] << endl;;
		vectorVec.push_back(vectorMat[i]);
		//cout << vectorVec[i] << endl;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			output1 << fixed << setprecision(2) << vectorVec[i][j];
			if (j < 3)output1 << " ";
		}
		output1 << endl;
	}
	//input1.close();

	//計算四面體體積
	Mat d2(4, 4, CV_64F);
	for (int i = 0; i < 4; i++)
	{
		d2.at<double>(i, 0) = vectorVec[0][i];
		d2.at<double>(i, 1) = vectorVec[1][i];
		d2.at<double>(i, 2) = vectorVec[2][i];
		d2.at<double>(i, 3) = vectorVec[3][i];
	}
	//cout << d2 << endl;
	double determinant2 = abs(cv::determinant(d2) / 6);
	//cout << determinant2 << endl;

	double r = determinant2 / determinant1;//體積為正
	//cout << determinant1 << " " << determinant2 << " " << r;
	double detT = cv::determinant(list[0]);

	output1 << fixed << setprecision(2) << r << " " << fixed << setprecision(2) << detT << endl;
	r = round(r * 100);
	detT = round(detT * 100);

	if (r == detT && r != 0 && detT != 0)
	{
		output1 << "r==det(T)\n";
	}
	else if (r == -detT && r != 0 && detT != 0)
	{
		output1 << "r==-det(T)\n";
	}
	else if (r == detT && r == 0)
	{
		output1 << "zeros\n";
	}
	else
	{
		output1 << "others\n";
	}

	if (detT != 0)
	{
		//get invert
		Mat inverse;
		cv::invert(list[0], inverse, DECOMP_LU);
		//cout << vin[4];
		inverse = inverse * vin[4];
		Vec4d inv = inverse;
		//cout << fixed << setprecision(2) << utov << endl;
		for (int i = 0; i < inv.rows; i++)
		{
			output1 << fixed << setprecision(2) << inv[i];
			if (i != inv.rows - 1)output1 << " ";
		}
		output1 << endl;
	}
	else
	{
		output1 << "NaN\n";
	}



	ifstream input2;
	input2.open(argv[3]);

	int l = 0, w = 0, h = 0;
	vector<Mat> data;
	input2 >> l >> w >> h;
	//data[z][x][y]
	for (int z = 0; z < h; z++)
	{
		Mat temp(l, w, CV_64F);
		for (int x = 0; x < l; x++)
			for (int y = 0; y < w; y++)
				input2 >> temp.at<double>(x, y);
		data.push_back(temp);
	}

	//找反矩陣
	int n2 = 0;
	vector<Mat> list2;
	input2 >> n2;
	//cout << n2 << endl;
	Mat firstmatrix2 = Mat::eye(4, 4, CV_64F);
	list2.push_back(firstmatrix2);

	for (int i = 0; i < n2; i++)
	{
		string transform;
		input2 >> transform;
		//cout << transform << endl;
		transfomation(list2, transform, input2);
	}

	for (int i = 1; i < list2.size(); i++)
	{
		list2[0] = list2[i] * list2[0];
		//cout << list2[i] << endl;
	}
	Mat inverse2;
	cv::invert(list2[0], inverse2, DECOMP_LU);

	//找位置
	vector<Mat> transformData;

	for (int z = 0; z < h; z++)
	{
		Mat temp = Mat::zeros(l, w, CV_64F);
		for (int x = 0; x < l; x++)
			for (int y = 0; y < w; y++)
			{
				Vec4d locate;
				locate[0] = x, locate[1] = y, locate[2] = z, locate[3] = 1;
				Mat inv = inverse2 * locate;
				locate = inv;


				temp.at<double>(x, y) = trilinearInterpolation(locate, data);
			}
		transformData.push_back(temp);
	}

	string output2Path = argv[4];
	ofstream output2(output2Path);

	for (int z = 0; z < h; z++)
	{
		for (int x = 0; x < l; x++)
		{
			for (int y = 0; y < w; y++)
			{
				output2 << transformData.at(z).at<double>(x, y);
				if (y < w - 1)
					output2 << " ";
			}
			output2 << endl;
		}
	}

	//for (int z = 0; z < h; z++)
	//{
	//	for (int x = 0; x < l; x++)
	//	{
	//		for (int y = 0; y < w; y++)
	//		{
	//			cout << transformData.at(z).at<double>(x, y);
	//			if (y < w - 1)
	//				cout << " ";
	//		}
	//		cout << endl;
	//	}
	//}
}