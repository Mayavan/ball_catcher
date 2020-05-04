#include <signal.h>

#include <cmath>
#include <cstdlib>

#include "ObjectDetector.hpp"
#include "debug.h"
#include "nlohmann/json.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "profiler.h"
#include "webcam.h"
#include "macro.h"

Webcam *w_cam;

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
  if (signum == SIGINT) {
    w_cam->~Webcam();
    LOG_INFO("User terminated (SIGINT). Calling Webcam destructor.");
    // Terminate program
    exit(signum);
  }
}

int main(int argc, char *argv[]) {
  using json = nlohmann::json;
  json config, camera_config;

  std::ifstream config_file(CONFIG_FILE_PATH);
  if (!config_file.is_open()) {
    LOG_ERROR("Cannot open config file");
    return -1;
  }
  config_file >> config;

  std::ifstream camera_config_file(
      CAMERA_CONFIG_FILE_PATH(config["camera_config"]));
  if (!config_file.is_open()) {
    LOG_ERROR("Cannot open config file");
    return -1;
  }
  camera_config_file >> camera_config;

  Webcam m_webcam(camera_config, std::string(config["device"]));
  ObjectDetector m_detector(camera_config, BLUE_BALL);

  Image frame;
  w_cam = &m_webcam;
  // Register signal and signal handler
  signal(SIGINT, signal_callback_handler);

  cv::Mat image;

  while (true) {
    auto t1 = GET_TIME();
    frame = m_webcam.frame();
    if (!std::string(camera_config["v4l2-settings"]["format"]).compare(YUV)) {
      image = cv::Mat(frame.height, frame.width, CV_8UC3, (void *)frame.data);
    } else if (!std::string(camera_config["v4l2-settings"]["format"])
                    .compare(MJPG)) {
      std::vector<unsigned char> img(frame.data,
                                     frame.data + frame.mjpg_buffer_size);
      cv::imdecode(img, cv::IMREAD_COLOR, &image);
    }

    auto t2 = GET_TIME();
    LOG_DEBUG("Decode " << std::string(camera_config["v4l2-settings"]["format"])
                        << " frame took = " << IN_MS(t2 - t1)
                        << " milliseconds.");

    m_detector.detectObject(ball, image);

    auto t3 = GET_TIME();
    LOG_DEBUG("DetectionObject = " << IN_MS(t3 - t2) << " milliseconds.");

    long long microseconds = IN_MS(GET_TIME() - t1);
    LOG_DEBUG("FPS = " << (1000.0 / microseconds));
  }

  return 0;
}
