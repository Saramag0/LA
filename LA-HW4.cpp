#define _CRT_SECURE_NO_WARNINGS
#include <opencv2/opencv.hpp>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

// 定義車輛的結構
class Car
{
public:
	string tentosixth(int r);
	bool colorjudge(vector<int> a);
	void setcolor(Mat frame, Point point);

	string id;
	Point center;
	Point head;
	vector<int> color;
	vector<Point> path;
	double distance = 0;
};

string Car::tentosixth(int r)
{
	string s;
	if (r / 16 > 9)
		s += (r / 16 - 10 + 'A');
	else
		s += (r / 16 + '0');
	if (r % 16 > 9)
		s += (r % 16 - 10 + 'A');
	else
		s += (r % 16 + '0');
	return s;
}

bool Car::colorjudge(vector<int> a)
{
	for (int i = 0; i < a.size(); i++)
		if (abs(a[i] - color[i]) > 4)
			return false;
	return true;
}

void Car::setcolor(Mat frame, Point point)
{
	int b = frame.at<Vec3b>(point.y, point.x)[0];
	int g = frame.at<Vec3b>(point.y, point.x)[1];
	int r = frame.at<Vec3b>(point.y, point.x)[2];
	string s = "#" + tentosixth(r) + tentosixth(g) + tentosixth(b);
	this->id = s;
	this->color.push_back(r);
	this->color.push_back(g);
	this->color.push_back(b);
}

int findCar(vector<Car> list, Car car)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].colorjudge(car.color))
			return i;
	}
	return -1;
}

float move(Point a, Point b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

vector<Mat> videoResample(const vector<Mat>& clip)
{
	vector<Mat> resampledClip;		// 用來儲存 Resampling 後的影片
	for (int i = 0; i < clip.size(); i += 2)	// 遍歷 clip，每兩幀取一幀
		resampledClip.push_back(clip[i]);
	return resampledClip;
}

int main(int argc, char* argv[])
{
	VideoCapture cap(argv[1]);	// 替換成你的影片路徑
	Mat frame;
	vector<Mat> clip;

	while (cap.read(frame) && !frame.empty())
	{ // 讀取一幀，如果失敗或影片結束則跳出循環
		clip.push_back(frame.clone());
		//imshow("frame", frame); // 顯示出該幀
		//if (cv::waitKey(20) == 'q')
		//	break; // 如果按下 'q' 則結束循環
	}

	vector<Mat> resample;
	resample = videoResample(clip);
	vector<Car> CarList;

	for (size_t i = 0; i < resample.size(); i++)
	{
		Mat inputFrame = resample[i];
		Mat thisframe;
		threshold(inputFrame, thisframe, 90, 255, THRESH_BINARY);
		cvtColor(thisframe, thisframe, COLOR_BGR2GRAY);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(thisframe, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

		for (size_t j = 0; j < contours.size(); ++j)
		{
			Car car;
			float area = contourArea(contours[j]);

			if (area > 1200)
			{
				Mat poly;
				approxPolyDP(contours[j], poly, 3, true);

				Moments moment = moments(poly);//car center
				Point center(static_cast<int>(moment.m10 / moment.m00), static_cast<int>(moment.m01 / moment.m00));
				Rect boundingBox = boundingRect(contours[j]);//car head
				Point Head(boundingBox.x + boundingBox.width / 2, boundingBox.y + boundingBox.height);
				//cout << boundingBox.x << " " << boundingBox.y << " " << boundingBox.height << " " << boundingBox.width << endl;

				car.setcolor(resample[i], center);

				car.center = center;
				car.head = Head;
				car.path.push_back(Head);

				if (findCar(CarList, car) != -1)
					CarList[findCar(CarList, car)].path.push_back(Head);
				else
					CarList.push_back(car);
				//rectangle(resample[i], boundingBox.tl(), boundingBox.br(), Scalar(0, 0, 255), 3);
				//circle(inputFrame, center, 5, Scalar(0, 255, 0), -1);
				//circle(inputFrame, Head, 5, Scalar(255, 0, 0), -1);
			}
		}
		//imshow("frame", inputFrame);
		//waitKey(300);
	}

	for (size_t i = 0; i < CarList.size(); i++)
	{
		for (size_t j = i + 1; j < CarList.size(); j++)
		{
			if (CarList[i].id > CarList[j].id)
			{
				Car t;
				t = CarList[i];
				CarList[i] = CarList[j];
				CarList[j] = t;
			}
		}
	}

	for (size_t i = 0; i < CarList.size(); i++)
	{
		for (size_t j = 0; j < CarList[i].path.size() - 1; j++)
		{
			if (CarList[i].path[j].y < 845)
				CarList[i].distance += move(CarList[i].path[j], CarList[i].path[j + 1]);
		}
		//cout << i << ". " << CarList[i].id << ": " << CarList[i].distance << endl;
	}

	ofstream output(argv[2]);
	for (size_t i = 0; i < CarList.size(); i++)
		output << CarList[i].id << " " << fixed << setprecision(2) << CarList[i].distance << endl;

	return 0;
}
