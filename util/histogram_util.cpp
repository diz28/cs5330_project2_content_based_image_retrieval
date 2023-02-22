/*
 * Di Zhang
 * Feb 10, 2023
 * CS5330 - Computer Vision
 */

#include <opencv2/opencv.hpp> 
#include "csv_util.h"

// split the image into left and right halves
int image_split(cv::Mat &image, cv::Mat &top_half, cv::Mat &down_half) {

    // Get the height of the image
    int height = image.size().height;

    // Split the image into top and bottom halves
    top_half = image(cv::Rect(0, 0, image.size().width, height/2));
    down_half = image(cv::Rect(0, height/2, image.size().width, height/2));
    return (0);
}

// calculate the histogram of images
int calc_RGB_histogram(cv::Mat &src, cv::Mat &hist) {
    
    // make this a parameter
    const int hist_size = 8;
    
    // used to figure out which bin the pixel is in
    const int divisor = 256 / hist_size;

    int i, j;

    // 3D matrix that defines the size(dimensions) of the matrix
    int dim[3] = {hist_size, hist_size, hist_size};
    
    // allocate and initilize a histogram
    // arguments for zeros function: # of dimentions, dimmention array, type
    hist = cv::Mat::zeros(3, dim, CV_32S);

    // loop through the image and assign the pixels to bins
    for (int i = 0; i < src.rows; i++) {

        // row pointer
        cv::Vec3b *sptr = src.ptr<cv::Vec3b>(i);
        for (int j = 0; j < src.cols; j++) {
            int r = sptr[j][2] / divisor; // R index
            int g = sptr[j][1] / divisor; // G index
            int b = sptr[j][0] / divisor; // B index
            
            // increment the histogram at location <r, g, b>
            hist.at<int>(r, g, b)++;

        }
    }

    return (0);
}

// calculate the histogram of sobel image
int calc_magnitude_histogram(cv::Mat &image, std::vector<float> &magnitude_vect) {
    
    cv::Mat gray;

    cv::Mat hist;

    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Apply the Sobel filter
    cv::Mat sobelx, sobely;
    cv::Sobel(gray, sobelx, CV_32F, 1, 0);
    cv::Sobel(gray, sobely, CV_32F, 0, 1);

    // Compute the magnitude and direction of the gradient
    cv::Mat magnitude, direction;
    cv::cartToPolar(sobelx, sobely, magnitude, direction, true);

    // construct a 1d histogram for grayscale image
        // make this a parameter
    const int hist_size = 256;
    
    // used to figure out which bin the pixel is in
    const int divisor = 256 / hist_size;

    int i, j, k;

    // 3D matrix that defines the size(dimensions) of the matrix
    int dim[1] = {hist_size};
    
    // allocate and initilize a histogram
    // arguments for zeros function: # of dimentions, dimmention array, type
    hist = cv::Mat::zeros(1, dim, CV_32S);

    // loop through the image and assign the pixels to bins
    for (int i = 0; i < gray.rows; i++) {

        // row pointer
        cv::Vec3b *sptr = gray.ptr<cv::Vec3b>(i);
        for (int j = 0; j < gray.cols; j++) {
            int n = sptr[j][0] / divisor; // R index
            
            // increment the histogram at location <r, g, b>
            hist.at<int>(n)++;

        }
    }

    for (int i = 0; i < hist_size; i++) {
        magnitude_vect.push_back((float)hist.at<int>(i));
    }

    return (0);
}

// converting 8x8x8 histogram into 1d vector
// go through 3 dimensions
int vector_conversion(cv::Mat &hist, std::vector<float> &flat_hist) {

    for (int i = 0; i < 512; i++) {
        flat_hist.push_back((float) hist.at<int>(i));
    }
    return (0);
}

// calculate the intersection matrics between 2 images vectors with normalization
float intersection (std::vector<float> &ha, std::vector<float> &hb) {

    // initilize intersection
    double intersection = 0.0;

    // compute the sum of the histogram bucket (not assuming it is normalized)
    double suma = 0.0;
    double sumb = 0.0;

    // rows * cols
    for (int i = 0; i < ha.size(); i++) { 
        suma += ha[i];
        sumb += hb[i];
    }

    // computer intersection and normalization
    for (int i = 0; i < ha.size(); i++) {
        double af = ha[i] / suma;
        double bf = hb[i] / sumb;
        intersection += af < bf ? af : bf;
    }

    // convert to a distance
    return (1 - intersection);
} 

// calculate sum of squared difference
float squared_difference(std::vector<float> &ha, std::vector<float> &hb) {
    float distance = 0;
    for (int i = 0; i < hb.size(); i++) {
        distance += pow((ha[i] - hb[i]), 2);
    }
    return distance;
}


// calculate the histogram of images
int calc_HSV_histogram(cv::Mat &src, cv::Mat &hsv) {

    // make this a parameter
    const int hsv_size = 8;
    
    // used to figure out which bin the pixel is in
    const int divisor = 256 / hsv_size;

    int i, j;

    // 3D matrix that defines the size(dimensions) of the matrix
    int dim[3] = {hsv_size, hsv_size, hsv_size};
    
    // allocate and initilize a histogram
    // arguments for zeros function: # of dimentions, dimmention array, type
    hsv = cv::Mat::zeros(3, dim, CV_32S);

    // loop through the image and assign the pixels to bins
    for (int i = 0; i < src.rows; i++) {

        // row pointer
        cv::Vec3b *sptr = src.ptr<cv::Vec3b>(i);
        for (int j = 0; j < src.cols; j++) {
            int hue = sptr[j][0] / divisor; // hue index
            int saturation = sptr[j][1] / divisor; // saturation index
            int value = sptr[j][2] / divisor; // value index

            hsv.at<int>(hue, saturation, value)++;
        }
    }

    return (0);
}
