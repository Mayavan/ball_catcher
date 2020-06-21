#include "macro.h"
#include "nlohmann/json.hpp"
#include "opencv2/highgui/highgui.hpp"

#define GRAPH_RES_W 360
#define GRAPH_RES_H 180

enum object_type { ball };

struct hsv_value {
  unsigned int h;
  unsigned int s;
  unsigned int v;
};

class ObjectDetector {
 public:
  ObjectDetector(nlohmann::json& camera_config, std::string ball_color);
  int detectObject(const object_type& type, cv::Mat& image);

 private:
  int getMaxAreaContourId(std::vector<std::vector<cv::Point>> contours);
  void initializeGPUFilters();
  nlohmann::json camera_config;
  hsv_value low_thres_1;
  hsv_value high_thres_1;
  hsv_value low_thres_2;
  hsv_value high_thres_2;

  double x_prev, y_prev, z_prev;
  u_int64_t updater;

  cv::Mat xy_graph = cv::Mat::zeros(cv::Size(GRAPH_RES_W, GRAPH_RES_H), CV_8UC3);
  cv::Mat xz_graph = cv::Mat::zeros(cv::Size(GRAPH_RES_W, GRAPH_RES_H), CV_8UC3);
};