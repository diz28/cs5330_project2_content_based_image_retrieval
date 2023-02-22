
/*
 * Di Zhang
 * Feb 10, 2023
 * CS5330 - Computer Vision
 */

#include <opencv2/opencv.hpp>

#include "../util/csv_util.h" 
#include "../util/histogram_util.h"

// write feature vectors based on solely the center 9 x 9 pixles of images to baseline.csv file
int build_baseline (cv::Mat &src, char *image_filename) {

    // initilize a csv file
    char csvFile[] = "../csv_files/baseline.csv";

    //initilize a vector
    std::vector<float> vect;

    // loop through each pixel in the image ang only the center 9 pixels
    int nrow = src.rows/2 - 4;
    int ncol = src.cols/2 - 4;

    // center rows
    for (int i = nrow; i < nrow + 9; i++) {
        // access source row pointer
        cv::Vec3b *rowptr = src.ptr<cv::Vec3b>(i);

        // center cols
        for (int j = ncol; j < ncol + 9; j++) {

            // go through color channels
            for (int k = 0; k < 3; k++) {
                
                // put channel value into vector
                vect.push_back(rowptr[j][k]);
            }
        }
    }
    
    // write vector values into csv file
    append_image_data_csv(csvFile, image_filename, vect, 0);
    return 0;
}

// write the feature vectors of a image to histogram.csv
// calculate the 3d histogram of the image and covert to 1d vector and 
// save the vector to a csc file
int build_histogram(cv::Mat &image, char *image_filename) {

    // initilize histogram
    cv::Mat histogram;

    // initilize histogram flatened 1d vector
    std::vector<float> flat_hist;

    // create csv filename
    char csvFile[] = "../csv_files/histogram.csv";

    // call function to construct the histogram
    calc_RGB_histogram(image, histogram);
    vector_conversion(histogram, flat_hist);
    append_image_data_csv(csvFile, image_filename, flat_hist, 0);

    return (0);
}

// write the feature vectors of a top and down images to top_histogram.csv and down_histogram.csv
// calculate the 3d histogram of the both images and covert to 1d vector and 
// save the vector to csc files
int build_multi_histogram(cv::Mat &image, char *image_filename) {

    // initilize top and down half of the image after spliting
    cv::Mat top_half;
    cv::Mat down_half;

    // initilize top and down histogram
    cv::Mat top_histogram;
    cv::Mat down_histogram;

    // initilize top and down flatened 1d vector
    std::vector<float> top_flat_hist;
    std::vector<float> down_flat_hist;

    // create top and down csv filename
    char top_csvFile[] = "../csv_files/top_histogram.csv";
    char down_csvFile[] = "../csv_files/down_histogram.csv";

    // split image into top and down halves
    image_split(image, top_half, down_half);

    // calculate histogram for top and down
    calc_RGB_histogram(top_half, top_histogram);
    calc_RGB_histogram(down_half, down_histogram);

    // convert top and down histogram to flat 1d vector
    vector_conversion(top_histogram, top_flat_hist);
    vector_conversion(down_histogram, down_flat_hist);

    // construt 2 csv files for top and down images
    append_image_data_csv(top_csvFile, image_filename, top_flat_hist, 0);
    append_image_data_csv(down_csvFile, image_filename, down_flat_hist, 0);
    return (0);
}

// write the feature vectors of a color and sobel images to color_histogram.csv and sobel_histogram.csv
// calculate the 3d histogram of the both images and covert to 1d vector and 
// save the vector to csv files
int build_texture_and_color(cv::Mat &image, char *image_filename) {

    cv::Mat magnitude;

    // initilize the histogram for both images
    cv::Mat color_histogram;

    // initilize color and magnitude flatened 1d vector
    std::vector<float> color_flat_hist;
    std::vector<float> magnitude_vect;

    // create color and magnitude csv filename
    char color_csvFile[] = "../csv_files/color_histogram.csv";
    char magnitude_csvFile[] = "../csv_files/magnitude_histogram.csv";

    // calculate histogram for both images
    calc_RGB_histogram(image, color_histogram);
    calc_magnitude_histogram(image, magnitude_vect);

    // convert color histogram to a 1d vector
    vector_conversion(color_histogram, color_flat_hist);

    // construt 2 csv files for both images
    append_image_data_csv(color_csvFile, image_filename, color_flat_hist, 0);
    append_image_data_csv(magnitude_csvFile, image_filename, magnitude_vect, 0);

    return (0);
}

// use hsv values to compare images and to match similar images
int build_hsv(cv::Mat &image, char *image_filename) {

    cv::Mat hsv_hist;

    // initilize histogram flatened 1d vector
    std::vector<float> hsv_vect;

    // initilize csv file for hsv histogram and call functions to calculate it and save it into csv file
    char hsv_csvFile[] = "../csv_files/hsv.csv";
    calc_HSV_histogram(image, hsv_hist);
    vector_conversion(hsv_hist, hsv_vect);
    append_image_data_csv(hsv_csvFile, image_filename, hsv_vect, 0);

    return (0);
}