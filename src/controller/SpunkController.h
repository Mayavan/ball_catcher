#include <memory>

#include "Serial.h"

// In millimeters
struct Position {
  float x;
  float y;
};

// In radians
struct JointState {
  float a;
  float b;
};

class SpunkController {
 public:
  SpunkController();
  void move_to_xy(Position& target_position);

 private:
  float l1, l2;
  std::unique_ptr<Serial> ser = nullptr;
  JointState target_joint_state;
  void calculate_i_kine(Position& target_position);
  void execute_motion();
};