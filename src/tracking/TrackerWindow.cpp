#include "TrackerWindow.hpp"

#include "debug.h"

using namespace cv;
using namespace std;

/*Function///////////////////////////////////////////////////////////////

Name:       ShowManyImages

Purpose:

This is a function illustrating how to display more than one
image in a single window using Intel OpenCV

Parameters:

string title: Title of the window to be displayed
int    nArgs: Number of images to be displayed
Mat    img1: First Mat, which contains the first image
...
Mat    imgN: First Mat, which contains the Nth image

Language:   C++

The method used is to set the ROIs of a Single Big image and then resizing
and copying the input images on to the Single Big Image.

This function does not stretch the image...
It resizes the image without modifying the width/height ratio..

This function can be called like this:

ShowManyImages("Images", 5, img2, img2, img3, img4, img5);

This function can display upto 12 images in a single window.
It does not check whether the arguments are of type Mat or not.
The maximum window size is 700 by 660 pixels.
Does not display anything if the number of arguments is less than
one or greater than 12.

Idea was from [[BettySanchi]] of OpenCV Yahoo! Groups.

If you have trouble compiling and/or executing
this code, I would like to hear about it.

You could try posting on the OpenCV Yahoo! Groups
[url]http://groups.yahoo.com/group/OpenCV/messages/ [/url]


Parameswaran,
Chennai, India.

cegparamesh[at]gmail[dot]com

...
///////////////////////////////////////////////////////////////////////*/

void ShowManyImages(string title, int nArgs, ...) {
  int size_w, size_h;
  int i;
  int m, n;
  int x, y;

  // w - Maximum number of images in a row
  // h - Maximum number of images in a column
  int w, h;

  // scale - How much we have to resize the image
  float scale;
  int max;

  // If the number of arguments is lesser than 0 or greater than 12
  // return without displaying
  if (nArgs <= 0) {
    printf("Number of arguments too small....\n");
    return;
  } else if (nArgs > 14) {
    printf(
        "Number of arguments too large, can only handle maximally 12 images at "
        "a time ...\n");
    return;
  }
  // Determine the size of the image,
  // and the number of rows/cols
  // from number of arguments
  else if (nArgs == 1) {
    w = h = 1;
    size_w = DISPLAY_RES_W * 1;
    size_h = DISPLAY_RES_H * 1;

  } else if (nArgs == 2) {
    w = 2;
    h = 1;
    size_w = DISPLAY_RES_W * 0.5;
    size_h = DISPLAY_RES_H * 0.5;
  } else if (nArgs == 3 || nArgs == 4) {
    w = 2;
    h = 2;
    size_w = DISPLAY_RES_W * 0.5;
    size_h = DISPLAY_RES_H * 0.5;
  } else if (nArgs == 5 || nArgs == 6) {
    w = 3;
    h = 2;
    size_w = DISPLAY_RES_W * 0.33;
    size_h = DISPLAY_RES_H * 0.33;
  } else if (nArgs == 7 || nArgs == 8) {
    w = 4;
    h = 2;
    size_w = DISPLAY_RES_W * 0.25;
    size_h = DISPLAY_RES_H * 0.25;
  } else {
    w = 4;
    h = 3;
    size_w = DISPLAY_RES_W * 0.25;
    size_h = DISPLAY_RES_H * 0.25;
  }

  // Create a new 3 channel image
  Mat DispImage = Mat::zeros(Size(size_w * w, size_h * h), CV_8UC3);

  // Used to get the arguments passed
  va_list args;
  va_start(args, nArgs);

  // Loop for nArgs number of arguments
  for (i = 0, m = 0, n = 0; i < nArgs; i++, m += (size_w)) {
    // Get the Pointer to the IplImage
    Mat img = va_arg(args, Mat);

    // Check whether it is NULL or not
    // If it is NULL, release the image, and return
    if (img.empty()) {
      LOG_ERROR("Invalid arguments");
      return;
    }

    // Find the width and height of the image
    x = img.cols;
    y = img.rows;

    // Find the scaling factor to resize the image
    if (x > y) {
      scale = (float)((float)x / size_w);
    } else {
      scale = (float)((float)y / size_h);
    }

    // Used to Align the images
    if (i % w == 0 && m != 0) {
      m = 0;
      n += size_h;
    }
    // Set the image ROI to display the current image
    // Resize the input image and copy the it to the Single Big Image
    Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
    Mat temp;

    resize(img, temp, Size(ROI.width, ROI.height));

    Mat dst_roi = DispImage(ROI);
    temp.copyTo(dst_roi);
  }

  // Create a new window, and show the Single Big Image
  namedWindow(title, 1);
  imshow(title, DispImage);

  // End the number of arguments
  va_end(args);
}