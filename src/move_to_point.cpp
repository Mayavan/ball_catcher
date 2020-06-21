#include "SpunkController.h"

int main() {
  Position pos;
  pos.x = 20;
  pos.y = 30;
  SpunkController controller;

  controller.move_to_xy(pos);
}