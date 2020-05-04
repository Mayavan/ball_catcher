#!/usr/bin/python

# USAGE
# python ball_tracking.py --video ball_tracking_example.mp4
# python ball_tracking.py
from __future__ import print_function

import matplotlib.pyplot as plt  # Import matplotlib functionality
import sys  # Enables the passing of arguments
from common import Sketcher

# import the necessary packages
from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time

debug = True

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
    help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64,
    help="max buffer size")
args = vars(ap.parse_args())


# if a video path was not supplied, grab the reference
# to the webcam
if not args.get("video", False):
    vs = VideoStream(src=0).start()

# otherwise, grab a reference to the video file
else:
    vs = cv2.VideoCapture(args["video"])

# allow the camera or video file to warm up
time.sleep(2.0)

# keep looping
while True:
    # grab the current frame
    frame = vs.read()
    
    # handle the frame from VideoCapture or VideoStream
    frame = frame[1] if args.get("video", False) else frame

    # if we are viewing a video and we did not grab a frame,
    # then we have reached the end of the video
    if frame is None:
        print("No frame")
        break

    image_mark = frame.copy()
    sketch=Sketcher('Image',[image_mark],lambda:((255, 255, 255),255))

    # Sketch a mask
    while True:
        ch = cv2.waitKey()
        if ch == 27: # ESC - exit
            break
        if ch == ord('r'): # r - mask the image
            break
        if ch == ord(' '): # SPACE - reset the inpainting mask
            image_mark[:] = frame
            sketch.show()
 
    # define range of white color in HSV
    lower_white = np.array([0,0,255])
    upper_white = np.array([255,255,255])
 
    # Create the mask
    mask = cv2.inRange(image_mark, lower_white, upper_white)
 
    # Create the inverted mask
    mask_inv = cv2.bitwise_not(mask)
 
    # Convert to grayscale image
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
 
    # Extract the dimensions of the original image
    rows, cols, channels = frame.shape
    frame = frame[0:rows, 0:cols]
 
    # Bitwise-OR mask and original image
    colored_portion = cv2.bitwise_or(frame, frame, mask = mask)
    colored_portion = colored_portion[0:rows, 0:cols]
 
    # Bitwise-OR inverse mask and grayscale image
    gray_portion = cv2.bitwise_or(gray, gray, mask = mask_inv)
    gray_portion = np.stack((gray_portion,)*3, axis=-1)
 
    # Combine the two images
    output = colored_portion + gray_portion

    # Create a table showing input image, mask, and output
    mask = np.stack((mask,)*3, axis=-1)
    table_of_images = np.concatenate((frame, mask, output), axis=1)
 
    # Display images, used for debugging
    # cv2.imshow('Original Image', image)
    # cv2.imshow('Sketched Mask', image_mark)
    # cv2.imshow('Mask', mask)
    # cv2.imshow('Output Image', output)
    cv2.imshow('Table of Images', table_of_images)
    key = cv2.waitKey(0) # Wait for a keyboard event
    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break

# if we are not using a video file, stop the camera video stream
if not args.get("video", False):
    vs.stop()

# otherwise, release the camera
else:
    vs.release()

# close all windows
cv2.destroyAllWindows()
