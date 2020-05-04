#ifndef _COMMON_MACROS_H_
#define _COMMON_MACROS_H_

// Constants
#define BT_PI (3.14159265358979323846)
#define DEG_TO_RAD(x) ((x) * (BT_PI / 180.0))
#define RAD_TO_DEG(x) ((x) * (180.0 / BT_PI))

#define CONFIG_FILE_PATH \
  std::string(PROJECT_SOURCE_DIR) + std::string("/config/capture_frame.json")

#define CAMERA_CONFIG_FILE_PATH(x) \
  std::string(PROJECT_SOURCE_DIR) + std::string(x)

#define YUV "YUV"
#define MJPG "MJPG"
#define COLOR_THRES "color-threshold"
#define BLUE_BALL "blue-ball"
#define LOW_THRES "low-threshold"
#define HIGH_THRES "high-threshold"

#endif  // _COMMON_MACROS_H_
