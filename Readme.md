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
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make
```

### Build Debug version
The debug version has a second window showing the position of the crop in the image and also prints the computed values to help with debugging the algorithm.
```
mkdir build_debug
cd build_debug
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make
```

## To run the app
Follow the comments on the default.xml file to enter your input parameters.
```
# To run debug version
cd build_debug 
./bin/a_pillar ../default.xml 

# To run release version
cd build_debug 
./bin/a_pillar ../default.xml 
```





