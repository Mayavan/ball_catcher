#include "macro.h"
#include "nlohmann/json.hpp"
#include "opencv2/highgui/highgui.hpp"
//#include <opencv2/cudafilters.hpp>

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
  //int detectObjectGPU(const object_type& type, cv::cuda::GpuMat& image);

 private:
  int getMaxAreaContourId(std::vector<std::vector<cv::Point>> contours);
  void initializeGPUFilters();
  nlohmann::json camera_config;
  hsv_value low_thres_1;
  hsv_value high_thres_1;
  hsv_value low_thres_2;
  hsv_value high_thres_2;
};