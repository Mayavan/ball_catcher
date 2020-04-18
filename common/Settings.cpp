#include "Settings.hpp"

Settings::Settings() {}

void Settings::write(
    cv::FileStorage& fs) const  // Write serialization for this class
{
  fs << "{"
     << "cameraResolution_width" << cameraResolution.width
     << "cameraResolution_height" << cameraResolution.height
     << "camera_fov_width" << camera_fov_width << "camera_fov_height"
     << camera_fov_height << "displayResolution_width"
     << displayResolution.width << "displayResolution_height"
     << displayResolution.height << "display_width" << display_width
     << "display_height" << display_height << "display_x" << display_pose.x
     << "display_y" << display_pose.y << "display_z" << display_pose.z

     << "display_roll" << display_pose.roll << "display_pitch"
     << display_pose.pitch << "display_yaw" << display_pose.yaw

     << "eyePosition_X" << eyePosition.x << "eyePosition_Y" << eyePosition.y
     << "eyePosition_Z" << eyePosition.z << "device" << device << "image_format"
     << image_format << "targetPlaneDistance" << targetPlaneDistance
     << "window_offset_x" << window_offset_x << "window_offset_y"
     << window_offset_y << "algorithm" << algorithm << "}";
}

void Settings::read(
    const cv::FileNode& node)  // Read serialization for this class
{
  node["cameraResolution_width"] >> cameraResolution.width;
  node["cameraResolution_height"] >> cameraResolution.height;
  node["camera_fov_width"] >> camera_fov_width;
  node["camera_fov_height"] >> camera_fov_height;
  node["displayResolution_width"] >> displayResolution.width;
  node["displayResolution_height"] >> displayResolution.height;
  node["display_width"] >> display_width;
  node["display_height"] >> display_height;
  node["display_x"] >> display_pose.x;
  node["display_y"] >> display_pose.y;
  node["display_z"] >> display_pose.z;
  node["display_roll"] >> display_pose.roll;
  node["display_pitch"] >> display_pose.pitch;
  node["display_yaw"] >> display_pose.yaw;

  node["eyePosition_X"] >> eyePosition.x;
  node["eyePosition_Y"] >> eyePosition.y;
  node["eyePosition_Z"] >> eyePosition.z;
  node["device"] >> device;
  node["image_format"] >> image_format;
  node["targetPlaneDistance"] >> targetPlaneDistance;
  node["window_offset_x"] >> window_offset_x;
  node["window_offset_y"] >> window_offset_y;
  node["algorithm"] >> algorithm;
  validate();
}

void Settings::validate() {
  bool goodInput = true;
  if (displayResolution.width <= 0 || displayResolution.height <= 0) {
    cerr << "Invalid display resolution: " << displayResolution.width << " "
         << displayResolution.height << endl;
    goodInput = false;
  }
  if (cameraResolution.width <= 0 || cameraResolution.height <= 0) {
    cerr << "Invalid camera resolution: " << cameraResolution.width << " "
         << cameraResolution.height << endl;
    goodInput = false;
  }
  if (display_width <= 0 || display_height <= 0) {
    cerr << "Invalid camera size: " << display_width << " " << display_height
         << endl;
    goodInput = false;
  }
  if (algorithm != "ViewCropper" && algorithm != "VerticesWarp") {
    cerr << "Algorithm does not exist" << endl;
    goodInput = false;
  }
  if (!goodInput) exit(EXIT_FAILURE);
}
