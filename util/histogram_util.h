/*
 * Di Zhang
 * Feb 10, 2023
 * CS5330 - Computer Vision
 */

// header file

// calculate the histogram of images
int calc_RGB_histogram(cv::Mat &src, cv::Mat &hist);

// converting 3x3x3 histogram into 1d vector
int vector_conversion(cv::Mat &hist, std::vector<float> &flat_hist);

// calculate the intersection matrics between 2 images vectors with normalization
float intersection (std::vector<float> &ha, std::vector<float> &hb);

// split the image into left and right halves
int image_split(cv::Mat &image, cv::Mat &top_half, cv::Mat &down_half);

// calculat the hsv histogram
int calc_HSV_histogram(cv::Mat &image, cv::Mat &hsv_hist);

// calculate sum of squared difference
float squared_difference(std::vector<float> &ha, std::vector<float> &hb);

// calculate the gradiant historgram
int calc_magnitude_histogram(cv::Mat &magnitude, std::vector<float> &magnitude_vect);
