
/*
 * Di Zhang
 * Feb 10, 2023
 * CS5330 - Computer Vision
 */

// header file

// write feature vectors based on solely the center 9 x 9 pixles of images to baseline.csv file
int build_baseline(cv::Mat &src, char *image_filename);

// write the feature vectors of a image to histogram.csv
// calculate the 3d histogram of the image and covert to 1d vector and 
// save the vector to a csc file
int build_histogram(cv::Mat &image, char *image_filename);

// write the feature vectors of a top and down images to top_histogram.csv and down_histogram.csv
// calculate the 3d histogram of the both images and covert to 1d vector and 
// save the vector to csc files
int build_multi_histogram(cv::Mat &image, char *image_filename);

// write the feature vectors of a color and sobel images to color_histogram.csv and sobel_histogram.csv
// calculate the 3d histogram of the both images and covert to 1d vector and 
// save the vector to csv files
int build_texture_and_color(cv::Mat &image, char *image_filename);

// use hsv values to compare images and to match similar images
int build_hsv(cv::Mat &image, char *image_filename);
