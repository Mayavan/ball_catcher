#include <signal.h>

#include <cmath>
#include <cstdlib>

#include "debug.h"
#include "nlohmann/json.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "profiler.h"
#include "webcam.h"
#include "macro.h"

#define ITERATIONS 100

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

  Webcam webcam(camera_config, std::string(config["device"]));

  Image frame;
  w_cam = &webcam;
  // Register signal and signal handler
  signal(SIGINT, signal_callback_handler);

  cv::Mat image;
  bool first = true;

  cv::VideoWriter writer("captured.avi",CV_FOURCC('M','J','P','G'),10, cv::Size(640,360));

  int i = 0;
  static auto start = GET_TIME();
  while (true) {
    frame = webcam.frame();
    if (!std::string(camera_config["v4l2-settings"]["format"]).compare(YUV)) {
      image = cv::Mat(frame.height, frame.width, CV_8UC3, (void *)frame.data);
    } else if (!std::string(camera_config["v4l2-settings"]["format"])
                    .compare(MJPG)) {
      std::vector<unsigned char> img(frame.data,
                                     frame.data + frame.mjpg_buffer_size);
      cv::imdecode(img, cv::IMREAD_COLOR, &image);
    }

    cv::imwrite("../images/image_" + std::to_string(i) + ".jpg", image);
    cv::imshow("Video", image);
    cv::waitKey(1);
    i++;
    if(i%20==0)
      writer.write(image);
  }

  writer.release();
  auto end = GET_TIME();
  auto elapsed = IN_MS(end - start);
  LOG_INFO("FPS = " << (float)(1000 * ITERATIONS / elapsed));

  return 0;
}
