#pragma once
#include "opencv2/opencv.hpp"

using namespace std;

struct Pose {
  float x;
  float y;
  float z;
  float roll;
  float pitch;
  float yaw;
};

class Settings {
 public:
  Settings();
  void write(cv::FileStorage& fs) const;
  void read(const cv::FileNode& node);

 private:
  void validate();

 public:
  cv::Size cameraResolution;
  cv::Size displayResolution;
  float camera_fov_width;   // in radians
  float camera_fov_height;  // in radians
  float display_width;
  float display_height;
  Pose display_pose;
  cv::Point3_<float> eyePosition;
  string device;
  string image_format;
  float targetPlaneDistance;
  int window_offset_x;
  int window_offset_y;
  string algorithm;
};

static inline void read(const cv::FileNode& node, Settings& x,
                        const Settings& default_value = Settings()) {
  if (node.empty())
    x = default_value;
  else
    x.read(node);
}