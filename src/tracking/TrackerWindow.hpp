#pragma once

#include <opencv2/opencv.hpp>

#include <stdio.h>
#include <stdarg.h>
#include <string>

#define DISPLAY_RES_W 1920
#define DISPLAY_RES_H 1080

void ShowManyImages(std::string title, int nArgs, ...);