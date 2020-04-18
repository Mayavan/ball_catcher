#include <signal.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "Settings.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "webcam.h"
#include "debug.h"

using namespace std;

Webcam *w_cam;

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
  w_cam->~Webcam();
  if (signum == SIGINT)
    cout << "User terminated (SIGINT). Calling Webcam destructor." << endl;
  // Terminate program
  exit(signum);
}

int main(int argc, char *argv[]) {
  const cv::String keys =
      "{help h usage ? |           | print this message            }"
      "{@settings      |default.xml| input setting file            }";
  cv::CommandLineParser parser(argc, argv, keys);
  parser.about(
      "Usage: a_pillar [configuration_file -- default ./default.xml]\n"
      "Near the sample file you'll find the configuration file, which has "
      "detailed help of "
      "how to edit it. It may be any OpenCV supported file format XML/YAML.");
  if (!parser.check()) {
    parser.printErrors();
    return 0;
  }

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  //! [file_read]
  Settings s;
  const string inputSettingsFile = parser.get<string>(0);
  cv::FileStorage fs(inputSettingsFile,
                     cv::FileStorage::READ);  // Read the settings
  if (!fs.isOpened()) {
    cerr << "Could not open the configuration file: \"" << inputSettingsFile
         << "\"" << endl;
    parser.printMessage();
    return -1;
  }
  fs["Settings"] >> s;
  fs.release();  // close Settings file
  //! [file_read]

  /**
   * Configure access to camera
   */
  Webcam webcam(s.device, s.cameraResolution.width, s.cameraResolution.height,
                s.image_format);
  Image frame;
  w_cam = &webcam;
  // Register signal and signal handler
  signal(SIGINT, signal_callback_handler);

  /**
   * Begin image processing algorithm
   */
  cv::Mat image;
  bool first = true;

  int i = 0;
  auto start = std::chrono::high_resolution_clock::now();
  while (i != 1000) {
    frame = webcam.frame();
    if (s.image_format == "YUV") {
      image = cv::Mat(frame.height, frame.width, CV_8UC3, (void *)frame.data);
    } else if (s.image_format == "MJPG") {
      vector<unsigned char> img(frame.data,
                                frame.data + frame.mjpg_buffer_size);
      cv::imdecode(img, cv::IMREAD_COLOR, &image);
    }

#ifdef DEBUG
    if (first) {
      LOG("---------------------------------" << endl);
      LOG("frame.width = " << frame.width << endl);
      LOG("frame.height = " << frame.height << endl);
      LOG("---------------------------------" << endl);
    }
    first = false;
#endif
    cv::imwrite("images/image_" + std::to_string(i) + ".jpg", image);
    //cv::imshow("Video", image);
    //cv::waitKey(1);
    i++;
  }
  auto elapsed = std::chrono::high_resolution_clock::now() - start;
  long long microseconds =
      std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  cout << "FPS = " << (float)(1000 * 1000 * 1000 / microseconds) << endl;
  return 0;
}
