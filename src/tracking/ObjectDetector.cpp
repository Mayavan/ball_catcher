#include "ObjectDetector.hpp"
#include "debug.h"
#include "profiler.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/ml.hpp>
#include <opencv2/ml/ml.hpp>

#define UseScalar(x) cv::Scalar(x.h, x.s, x.v)
#define Array2Struct(x) {x[0], x[1], x[2]}

ObjectDetector::ObjectDetector(nlohmann::json &camera_config, std::string ball_color) : camera_config(camera_config)
{
  low_thres_1=Array2Struct(camera_config[COLOR_THRES][ball_color][LOW_THRES]);
  high_thres_1=Array2Struct(camera_config[COLOR_THRES][ball_color][HIGH_THRES]);
}

int ObjectDetector::getMaxAreaContourId(std::vector<std::vector<cv::Point>> contours) {
  double maxArea = 0;
  int maxAreaContourId = -1;
  for (int j = 0; j < contours.size(); j++) {
    double newArea = cv::contourArea(contours.at(j));
    if (newArea > maxArea) {
      maxArea = newArea;
      maxAreaContourId = j;
    }  // End if
  }    // End for
  return maxAreaContourId;
}  // End function

int ObjectDetector::detectObject(const object_type& type, cv::Mat& image) {

  using namespace cv;
  LOG_DEBUG("Debugging");
  // Preprocessing
  cv::Mat blurred, hsv, mask;
  std::vector<std::vector<cv::Point>> cnts;
  auto t1 = GET_TIME();

  cv::GaussianBlur(image, blurred, cv::Size(11, 11), 1);
  auto t2 = GET_TIME();
  std::cout << "Gaussian blur = " << IN_MS(t2-t1) << " milliseconds." << std::endl;

  cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);
  auto t3 = GET_TIME();
  std::cout << "Color Conversion = " << IN_MS(t3-t2) << " milliseconds." << std::endl;


  // Construct a Mask
  cv::inRange(hsv, UseScalar(low_thres_1), UseScalar(high_thres_1),
              mask);
  cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 5);
  cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 5);

  auto t4 = GET_TIME();
  std::cout << "inRangeMask = " << IN_MS(t4-t3) << " milliseconds." << std::endl;


  // find contours in the mask and initialize the current
  // (x, y) center of the ball
  cv::findContours(mask, cnts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::Point2f center, center_of_mask;
  float radius;


  auto t5 = GET_TIME();
  std::cout << "findContours = " << IN_MS(t5-t4) << " milliseconds." << std::endl;

  // only proceed if at least one contour was found
  if (cnts.size() > 0) {
    // find the largest contour in the mask, then use
    // it to compute the minimum enclosing circle and
    // centroid
    int c = getMaxAreaContourId(cnts);
    CvMoments M;
    cv::minEnclosingCircle(cnts[c], center_of_mask, radius);
    M = cv::moments(cnts[c]);
    center.x = int(M.m10 / M.m00);
    center.y = int(M.m01 / M.m00);

    // only proceed if the radius meets a minimum size
    if (radius > 10) {  // draw the circle and centroid on the frame,
      // then update the list of tracked points
      cv::circle(image, center_of_mask, int(radius), (0, 255, 255), 2);
      cv::circle(image, center, 5, (0, 0, 255), -1);
    }
  }

  auto t6 = GET_TIME();
  std::cout << "Get and draw biggest contour = " << IN_MS(t6-t5) << " milliseconds." << std::endl;

  // show the frame to our screen
  cv::imshow("Mask", mask);
  cv::imshow("Frame", image);
  cv::waitKey(0);
}