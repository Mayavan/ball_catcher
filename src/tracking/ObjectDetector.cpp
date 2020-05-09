#include "ObjectDetector.hpp"

#include <opencv2/ml.hpp>
#include <opencv2/ml/ml.hpp>

#include "debug.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "profiler.h"

#define UseScalar(x) cv::Scalar(x.h, x.s, x.v)
#define Array2Struct(x) \
  { x[0], x[1], x[2] }

ObjectDetector::ObjectDetector(nlohmann::json& camera_config,
                               std::string ball_color)
    : camera_config(camera_config) {
  low_thres_1 = Array2Struct(camera_config[COLOR_THRES][ball_color][LOW_THRES]);
  high_thres_1 =
      Array2Struct(camera_config[COLOR_THRES][ball_color][HIGH_THRES]);
}

int ObjectDetector::getMaxAreaContourId(
    std::vector<std::vector<cv::Point>> contours) {
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
  LOG_DEBUG("Running on CPU:");
  // Preprocessing
  cv::Mat blurred, hsv, mask;
  std::vector<std::vector<cv::Point>> cnts;
  auto t1 = GET_TIME();

  cv::GaussianBlur(image, blurred, cv::Size(11, 11), 1);
  auto t2 = GET_TIME();
  LOG_DEBUG("Gaussian blur = " << IN_MS(t2 - t1) << " milliseconds.");

  cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);
  auto t3 = GET_TIME();
  LOG_DEBUG("Color Conversion = " << IN_MS(t3 - t2) << " milliseconds.");

  // Construct a Mask
  cv::inRange(hsv, UseScalar(low_thres_1), UseScalar(high_thres_1), mask);
  cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 5);
  cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 5);

  auto t4 = GET_TIME();
  LOG_DEBUG("inRangeMask = " << IN_MS(t4 - t3) << " milliseconds.");

  // find contours in the mask and initialize the current
  // (x, y) center of the ball
  cv::findContours(mask, cnts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::Point2f center, center_of_mask;
  float radius;

  auto t5 = GET_TIME();
  LOG_DEBUG("findContours = " << IN_MS(t5 - t4) << " milliseconds.");

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
    if (radius > 10) {  // draw the circle and centroid on the frame
      float ball_size = 80;
      float X, Y, Z;
      float fx = 509.437, fy = 509.437;

      Z = (fx * 0.08) / (2 * radius);
      X = ((center.x - 320) * Z) / (fx);
      Y = ((center.y - 180) * Z) / (fy);
      std::string text1 = "X = " + std::to_string(X);
      std::string text2 = "Y = " + std::to_string(Y);
      std::string text3 = "Z = " + std::to_string(Z);
      cv::putText(image, text1, cv::Point(25, 25), FONT_HERSHEY_DUPLEX, 0.5,
                  cv::Scalar(255, 0, 0), 1.8);
      cv::putText(image, text2, cv::Point(25, 50), FONT_HERSHEY_DUPLEX, 0.5,
                  cv::Scalar(255, 0, 0), 1.8);
      cv::putText(image, text3, cv::Point(25, 75), FONT_HERSHEY_DUPLEX, 0.5,
                  cv::Scalar(255, 0, 0), 1.9);

#ifdef DEBUG
      cv::circle(image, center_of_mask, int(radius), (0, 255, 255), 1.5);
      cv::circle(image, center, 5, (0, 0, 255), -1);
#endif
    }
  }

  auto t6 = GET_TIME();
  LOG_DEBUG("Get and draw biggest contour = " << IN_MS(t6 - t5)
                                              << " milliseconds.");
#ifdef DEBUG
  // show the frame to our screen
  cv::imshow("Mask", mask);
  cv::imshow("Frame", image);
  cv::waitKey(1);
#endif
}

/*
cv::Ptr<cv::cuda::Filter> gaussian_filter;
cv::Ptr<cv::cuda::Filter> erode_filter;
cv::Ptr<cv::cuda::Filter> dilate_filter;
void ObjectDetector::initializeGPUFilters() {
  gaussian_filter = cv::cuda::createGaussianFilter(src.type(), dst.type(),
                                                   cv::Size(11, 11), 1);
  erode_filter = cv::cuda::createMorphologyFilter(MORPH_ERODE, src.type(),
                                                  cv::Mat(), Point(-1, -1), 5);
  dilate_filter = cv::cuda::createMorphologyFilter(MORPH_DILATE, src.type(),
                                                   cv::Mat(), Point(-1, -1), 5);
}

cv::Point3f Estimate3D() { return cv::Point3f() }

int ObjectDetector::detectObjectGPU(const object_type& type, cv::cuda::GpuMat&
image) { using namespace cv; LOG_DEBUG("Running on CPU:");
  // Preprocessing
  cv::cuda::GpuMat blurred, hsv, mask;
  std::vector<std::vector<cv::Point>> cnts;
  auto t1 = GET_TIME();

  gaussian_filter->apply(image, blurred);
  auto t2 = GET_TIME();
  LOG_DEBUG("Gaussian blur = " << IN_MS(t2 - t1) << " milliseconds.");

  cv::cuda::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);
  auto t3 = GET_TIME();
  LOG_DEBUG("Color Conversion = " << IN_MS(t3 - t2) << " milliseconds.");

  // Construct a Mask
  cv::inRange(hsv, UseScalar(low_thres_1), UseScalar(high_thres_1), mask);
  erode_filter->(mask, mask);
  dilate_filter->(mask, mask);

  auto t4 = GET_TIME();
  LOG_DEBUG("inRangeMask = " << IN_MS(t4 - t3) << " milliseconds.");

  // find contours in the mask and initialize the current
  // (x, y) center of the ball
  cv::findContours(mask, cnts, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::Point2f center, center_of_mask;
  float radius;

  auto t5 = GET_TIME();
  LOG_DEBUG("findContours = " << IN_MS(t5 - t4) << " milliseconds.");

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
    if (radius > 10) {  // draw the circle and centroid on the frame
#ifdef DEBUG
      cv::circle(image, center_of_mask, int(radius), (0, 255, 255), 2);
      cv::circle(image, center, 5, (0, 0, 255), -1);
#endif
    }
  }

  auto t6 = GET_TIME();
  LOG_DEBUG("Get and draw biggest contour = " << IN_MS(t6 - t5)
                                              << " milliseconds.");
#ifdef DEBUG
  // show the frame to our screen
  cv::imshow("Mask", mask);
  cv::imshow("Frame", image);
  cv::waitKey(1);
#endif
}*/