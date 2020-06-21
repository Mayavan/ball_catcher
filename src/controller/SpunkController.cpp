#include "SpunkController.h"

#include <cmath>

#define PI 3.14159265

SpunkController::SpunkController() {
  std::string device = "/dev/ttyACM0";
  l1 = 300;  // Length of link 1
  l2 = 300;  // Length of link 2

  ser = std::unique_ptr<Serial>(new Serial(device));
}

void SpunkController::move_to_xy(Position& target_position) {
  calculate_i_kine(target_position);
  execute_motion();
}

void SpunkController::calculate_i_kine(Position& target_position) {
  double D = (std::pow(target_position.x, 2) + std::pow(target_position.y, 2) -
              std::pow(l1, 2) - std::pow(l2, 2)) /
             (2 * l1 * l2);

  target_joint_state.b = std::atan2(std::sqrt(1 - std::pow(D, 2)), D);

  /* <-- add slash to use second solution
  target_joint_state.b =
      std::atan2(-std::sqrt(1 - std::pow(D, 2)), D);
  //*/
  target_joint_state.a = std::atan2(target_position.y, target_position.x) -
                         std::atan2(l2 * sin(target_joint_state.b),
                                    l1 + l2 * cos(target_joint_state.b));
}

void SpunkController::execute_motion() {
  ser->send_command("G0 X" + std::to_string(target_joint_state.a * 180 / PI) +
                    " Y" + std::to_string(target_joint_state.b * 180 / PI) +
                    "\n");
}