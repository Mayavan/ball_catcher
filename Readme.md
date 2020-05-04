# Ball Catcher

This Application estimates the 3D position of a ball and controls the a robot to catch the ball.

## Build enviornment required:
* Ubuntu 18.04
* OpenCV 4.1.0

## Build instructions

### Build Release version
```
mkdir build_release
cd build_release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

### Build Debug version
The debug version has a second window showing the position of the crop in the image and also prints the computed values to help with debugging the algorithm.
```
mkdir build_debug
cd build_debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## To run the app
Follow the comments on the default.xml file to enter your input parameters.
```
# To run debug version
cd build_debug 
./ball_tracker ../default.xml 

# To run release version
cd build_debug 
./ball_tracker ../default.xml 
```

## Tool for color analysis

### Colorfilter

Threshold your images in any colorspace!

* BGR (RGB)
* HSV
* HLS (HSL)
* Lab (CIELAB/L\*a\*b\*)
* Luv (L\*u\*v\*)
* YCrCb (YCbCr/YCC)
* XYZ (CIEXYZ)
* Grayscale (single channel)

#### USAGE

Install into a Python virtual environment. (optional)

```sh
$ python3 -m venv venv
$ source venv/bin/activate
(venv) $ pip install git+https://github.com/alkasm/colorfilters
```

Run the module as a script on any image you want:

```sh
(venv) $ python3 -m colorfilters path/to/image.png hsv
```



