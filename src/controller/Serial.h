#include <string>

class Serial {
 public:
  Serial(std::string& device);
  ~Serial();
  int send_command(std::string command);

 private:
  int serial_port;
  int _read();
  int _write(std::string data);
};