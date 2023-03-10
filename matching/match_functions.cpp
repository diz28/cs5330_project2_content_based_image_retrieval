
/*
 * Di Zhang
 * Feb 10, 2023
 * CS5330 - Computer Vision
 */

#include <vector>
#include <opencv2/opencv.hpp>

#include "../util/csv_util.h"
#include "../util/histogram_util.h"

// baseline match function - match 2 images solely based on the center 9 x 9 pixles
int baseline_match(cv::Mat &image, std::vector<std::pair<char *, float>> &imagevec, char *metric) {
    std::vector<char *> filenames;
    std::vector<std::vector<float>> data;
    char csvFile[] = "../csv_files/baseline.csv";
    std::vector<float> target_vect;

    // loop through each pixel in the image ang only the center 9 pixels
    int nrow = image.rows / 2 - 4;
    int ncol = image.cols / 2 - 4;

    // center rows
    for (int i = nrow; i < nrow + 9; i++)
    {
        // access source row pointer
        cv::Vec3b *rowptr = image.ptr<cv::Vec3b>(i);
        // center cols
        for (int j = ncol; j < ncol + 9; j++)
        {
            // go through color channels
            for (int k = 0; k < 3; k++)
            {
                // put channel value into vector
                target_vect.push_back(rowptr[j][k]);
            }
        }
    }

    // loop through each entry in the csv file and calculate distance metric against the target value
    read_image_data_csv(csvFile, filenames, data, 0);
    // calculate the sum squared error and put them into a map
    for (int i = 0; i < data.size(); i++) {
        float distance = 0;
        
        // calculate the square difference or intersection
        if (std::strcmp(metric, "squared_difference") == 0) {
            distance = squared_difference(target_vect, data[i]);
        } else if (std::strcmp(metric, "intersection") == 0) {
            distance = intersection(target_vect, data[i]);            
        }

        imagevec.push_back(std::make_pair(filenames[i], distance));
    }
    return (0);
}

// histogram match function -  calculate the similirity of the images histogram.csv with the given target image using intersection metric
int histogram_match(cv::Mat &image, std::vector<std::pair<char *, float>> &imagevec, char *metric) {
    // initilize target hist
    cv::Mat target_hist;

    // initilize target vector
    std::vector<float> target_flat_hist;

    // calculate target histogram
    calc_RGB_histogram(image, target_hist);

    // flatten out the target histogram
    vector_conversion(target_hist, target_flat_hist);

    std::vector<char *> filenames;
    std::vector<std::vector<float>> data;
    char csv_file[] = "../csv_files/histogram.csv";

    // loop through each entry in the csv file and calculate distance metric against the target value
    read_image_data_csv(csv_file, filenames, data, 0);

    for (int i = 0; i < data.size(); i++) {
        float distance;


        // calculate the square difference or intersection
        if (std::strcmp(metric, "squared_difference") == 0) {
            distance = squared_difference(target_flat_hist, data[i]);
        } else if (std::strcmp(metric, "intersection") == 0) {
            distance = intersection(target_flat_hist, data[i]);            
        }
        // save distance data into a vector for later sorting
        imagevec.push_back(std::make_pair(filenames[i], distance));
    }

    return (0);

}

// multi histogram match function - calculate the similirity of the images (combining the data in the top_histogram.csv 
// and down_histogram.csv with the given target image using intersection metric
int multi_histogram_match(cv::Mat &image, std::vector<std::pair<char *, float>> &imagevec, char *metric) {

    // initilize two halves of the target image
    cv::Mat top_image;
    cv::Mat down_image;

    // initilize target histogram top and down
    cv::Mat top_target_hist;
    cv::Mat down_target_hist;

    // initilize target vector top and down
    std::vector<float> top_target_flat_hist;
    std::vector<float> down_target_flat_hist;

    // initilize filenames vector to store the images names
    std::vector<char *> top_filenames;
    std::vector<char *> down_filenames;

    // initilize data vector to store the 1d vector after histogram 
    std::vector<std::vector<float>> top_data;
    std::vector<std::vector<float>> down_data;

    // top and down 2 csv files
    char top_csv_file[] = "../csv_files/top_histogram.csv";
    char down_csv_file[] = "../csv_files/down_histogram.csv";

    // split the target image
    image_split(image, top_image, down_image);

    // calculate target histogram top and down
    calc_RGB_histogram(top_image, top_target_hist);
    calc_RGB_histogram(down_image, down_target_hist);

    // flatten out the target histogram top and down
    vector_conversion(top_target_hist, top_target_flat_hist);
    vector_conversion(down_target_hist, down_target_flat_hist);

    // loop through each entry in the csv file and calculate distance metric against the target value
    read_image_data_csv(top_csv_file, top_filenames, top_data, 0);
    read_image_data_csv(down_csv_file, down_filenames, down_data, 0);

    for (int i = 0; i < top_data.size(); i++) {
        // make sure the the file name in the left is the same as on the right 
        int result = std::strcmp(top_filenames[i], down_filenames[i]);
        // checking if the top and down images and of the same image
        if (result != 0) {
            std::cout << "Something went wrong, please delete the left_histogram.csv and right_histogram,csv and re-build them." << std::endl;
            return (-1);
        }

        // initilize variabels
        float top_distance;
        float down_distance;
        float distance;

        // calculate the square difference or intersection
        if (std::strcmp(metric, "squared_difference") == 0) {
            top_distance = squared_difference(top_target_flat_hist, top_data[i]);
            down_distance = squared_difference(down_target_flat_hist, down_data[i]);
        } else if (std::strcmp(metric, "intersection") == 0) {
            top_distance = intersection(top_target_flat_hist, top_data[i]);
            down_distance = intersection(down_target_flat_hist, down_data[i]);            
        }

        distance = (top_distance * 0.5) + (down_distance * 0.5);
        imagevec.push_back(std::make_pair(top_filenames[i], distance));
    }
    return (0);

}

// multi histogram match function - calculate the similirity of the images (combining the data in the color_histogram.csv 
// and sobel_histogram.csv with the given target image using intersection metric
int color_and_texture_match(cv::Mat &image, std::vector<std::pair<char *, float>> &imagevec, char *metric) {

    // initilize the sobel of the target image
    cv::Mat magnitude;

    // initilize target histogram color and sobel
    cv::Mat color_target_hist;

    // initilize target vector color and sobel
    std::vector<float> color_target_flat_hist;
    std::vector<float> magnitude_target_vect;

    // initilize filenames vector to store the images names
    std::vector<char *> color_filenames;
    std::vector<char *> magnitude_filenames;

    // initilize data vector to store the 1d vector after histogram 
    std::vector<std::vector<float>> color_data;
    std::vector<std::vector<float>> magnitude_data;

    // color and sobel 2 csv files
    char color_csv_file[] = "../csv_files/color_histogram.csv";
    char magnitude_csv_file[] = "../csv_files/magnitude_histogram.csv";

    // calculate target histogram color and sobel
    calc_RGB_histogram(image, color_target_hist);
    calc_magnitude_histogram(image, magnitude_target_vect);
    // flatten out the target histogram color and sobel
    vector_conversion(color_target_hist, color_target_flat_hist);

    // loop through each entry in the csv file and calculate distance metric against the target value
    read_image_data_csv(color_csv_file, color_filenames, color_data, 0);
    read_image_data_csv(magnitude_csv_file, magnitude_filenames, magnitude_data, 0);

    for (int i = 0; i < color_data.size(); i++) {
        // make sure the the file name in the left is the same as on the right 
        int result = std::strcmp(color_filenames[i], magnitude_filenames[i]);
        if (result != 0) {
            std::cout << "Something went wrong, please delete the left_histogram.csv and right_histogram,csv and re-build them." << std::endl;
            return (-1);
        }
        float color_distance;
        float magnitude_distance;
        float distance;

        // calculate the square difference or intersection
        if (std::strcmp(metric, "squared_difference") == 0) {
            color_distance = squared_difference(color_target_flat_hist, color_data[i]);
            magnitude_distance = squared_difference(magnitude_target_vect, magnitude_data[i]);
        } else if (std::strcmp(metric, "intersection") == 0) {
            color_distance = intersection(color_target_flat_hist, color_data[i]);
            magnitude_distance = intersection(magnitude_target_vect, magnitude_data[i]);         
        }

        // calculate the distance - combining the 2 calculated distances with assigned weight to make the best result
        distance = (0.1 * color_distance) + (0.9 * magnitude_distance);
        imagevec.push_back(std::make_pair(color_filenames[i], distance));
    }
    return (0);
}

// use hsv histogram to compare images to match the images that are similar to a human eye
int hsv_match(cv::Mat &image, std::vector<std::pair<char *, float>> &imagevec, char *metric) {

    cv::Mat target_hsv_hist;
    cv::Mat target_rgb_hist;
    std::vector<float> target_hsv_vect;
    std::vector<float> target_rgb_vect;

    // initilize filenames vector to store the images names
    std::vector<char *> color_filenames;
    std::vector<char *> hsv_filenames;

    // initilize data vector to store the 1d vector after histogram 
    std::vector<std::vector<float>> color_data;
    std::vector<std::vector<float>> hsv_data;

    // initilize csv files
    char color_csv_file[] = "../csv_files/histogram.csv";
    char hsv_csv_file[] = "../csv_files/hsv.csv";

    // calculate target hsv
    calc_RGB_histogram(image, target_rgb_hist);
    calc_HSV_histogram(image, target_hsv_hist);

    // histogram conversion
    vector_conversion(target_rgb_hist, target_rgb_vect);
    vector_conversion(target_hsv_hist, target_hsv_vect);

    // loop through each entry in the csv file and calculate distance metric against the target value
    read_image_data_csv(color_csv_file, color_filenames, color_data, 0);
    read_image_data_csv(hsv_csv_file, hsv_filenames, hsv_data, 0);

    for (int i = 0; i < color_data.size(); i++) {
        float distance;

        distance = 0.8 * (intersection(target_rgb_vect, color_data[i])) + 0.2 * (intersection(target_hsv_vect, hsv_data[i]));
        // call intersection and calculate the intersection between target and each entry in the csv file
        imagevec.push_back(std::make_pair(color_filenames[i], distance));
    }

    return (0);
}

