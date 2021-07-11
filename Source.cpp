#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int, char**) {
	cout << "start" << endl;
	VideoCapture cap(0); // Открываем камеру или файл

	if (!cap.isOpened())// check if we succeeded
	{
		cout << "fail" << endl;
		return -1;
	}
		
	Mat edges;
	namedWindow("edges", 1);

	


	int key_light = 20;
	int key_sat = 30;
	int key_hue = 50;
	
	while (1) {

		Mat frame, img;
		cap >> frame; // get a new frame from camera
		frame.copyTo(img);

		medianBlur(frame, frame, 9);
		frame = img & frame;

		cv::Mat image_hsv;
		cv::Mat image_rgb;
		std::vector< std::vector<cv::Point> > contours;
		cv::cvtColor(frame, image_hsv, cv::COLOR_BGR2HSV); // Преобразуем в hsv
		cv::Mat tmp_img(frame.size(), CV_8U);
		Mat1b mask1, mask2;
		inRange(image_hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
		inRange(image_hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

		Mat1b mask = mask1 | mask2;
		Mat1b mask_blured;
		blur( mask, mask_blured, Size(10, 10));

		mask = mask & mask_blured;

		findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
		int max_area = 0;
		cv::Rect max_rect;
		for (uint i = 0; i<contours.size(); i++) {
			cv::Mat sticker;
			//Для каждой области определяем ограничивающий прямоугольник
			cv::Rect rect = cv::boundingRect(contours[i]);
			if (rect.area() > max_area) {
				max_area = rect.area();
				max_rect = rect;
			}
		}

		cv::rectangle(img, max_rect, cv::Scalar(0, 250, 0), 2);

		imshow("edges", img);
		if (waitKey(30) >= 0) break;
	}
	cout << "end" << endl;
	return 0;
}