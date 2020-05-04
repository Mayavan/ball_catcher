/** Small C++ wrapper around V4L example code to access the webcam
 **/

#include <memory>  // unique_ptr
#include <string>

#include "nlohmann/json.hpp"

struct buffer {
  void *data;
  size_t size;
};

struct Image {
  unsigned char *data;
  size_t width;
  size_t height;
  size_t size;  // width * height * 3
  size_t mjpg_buffer_size;
};

class Webcam {
 public:
  Webcam(nlohmann::json &camera_config, const std::string &device = "/dev/video0");

  ~Webcam();

  /** Captures and returns a frame from the webcam.
   *
   * The returned object contains a field 'data' with the image data
   * format (ie, RGB24 or MJPEG), as well as 'width', 'height' and 'size' (equal
   * to width * height * 3)
   *
   * This call blocks until a frame is available or until the provided
   * timeout (in seconds).
   *
   * Throws a runtime_error if the timeout is reached.
   */
  Image frame(int timeout = 10);

 private:
  void init_mmap();

  void open_device();
  void close_device();

  void init_device();
  void uninit_device();

  void start_capturing();
  void stop_capturing();

  bool read_frame();

  void init_control();
  void set_control(int control_id, int control_value);

  std::string device;
  nlohmann::json camera_config;
  int fd;

  Image image_frame;
  struct buffer *buffers;
  unsigned int n_buffers;


  bool force_format = true;
};
