### Di Zhang
### Feb 10, 2023
### CS5330 - Computer Vision

# CS5330 Project2 Content Based Image Retrieval

## Project Description

This is a CBIR program that can find the most similiar image to the target image with 4 different matching methods. Also find find images that
have the same or similar object as in the target image i.e. banana.

## How to install and run

### Have the following 4 programs ready on your **Linux** machine:

- opencv 4.7.0
- g++
- vscode
- make

### How to run in terminal

1. Create the csv database with the giving images - go in the `csv_build` directory and in the terminal type `make`, an excutable `csv_build` would appear.
2. In the termianl type in `./csv_build` followed by your desired image directory. i.e. `./csv_build ../olympus`.
3. Match the target image with image data in the a csv database - go in the `matching` drectory and in the termianl type `make`, an excutable `matchimages` would appear.
4. In the terminal type in `./matchimages` followed by 5 arguments - target image name, matching type, dirctory name, distance metric, and number of output images. i.e. `./matchimages pic.0164.jpg histogram olympus intersection 3`. The top 3 matched images with the target image would show up. 

### Directory Structure
<pre>
cs5330_project2_contenbased_image_retrieval
├── banana
├── csv_build   ├── build.cpp
│   ├── build.h
│   ├── csv_build
│   ├── csv_build.cpp
│   └── Makefile
├── csv_files
│   └── banana_moment.csv
├── matching
│   ├── Makefile
│   ├── match_functions.cpp
│   ├── match_functions.h
│   ├── matchimages
│   └── matchimages.cpp
├── olympus
├── ReadMe
├── test
└── util
    ├── csv_util.cpp
    ├── csv_util.h
    ├── histogram_util.cpp
    └── histogram_util.h
</pre>

## Program features (matching mechanisms)

### Basic requirments

#### Matching type
- `baseline` - matchs 2 images solely based on the center 9x9 pixels.
- `histogram` - matches 2 images using histogram and intersection metric to evaluate the similarity between 2 images.  
- `multihistogram` - split target image into top and down halves, then match half images using histogram and intersection metric to evaluate the similarity between 2 images with 0.5 on each of the half image. 
- `texture` - apply sobel filter to the image, then match half images using histogram and intersection metric to evaluate the similarity between 2 images with 0.5 on the orginal image and the sobel image. 
- `hsv` - use the combination of hsv and rgb histograms to match the images that are most similar from the perspective of a human. 

#### Distance metric

- `intersection` - after normalization take the smaller number of out of the 2 the corresponding bins from the target image and matching image, and then add them up to be the distance between the the 2 images.
- `squared_difference` - take the squared difference of the numbers in each bin of the two comparing images to be the distance between the 2 images.
   
## Demo Video
https://youtu.be/lYkT6M6kPh8
