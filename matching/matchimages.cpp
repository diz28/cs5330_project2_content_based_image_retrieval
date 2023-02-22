
/*
 * Di Zhang
 * Feb 10, 2023
 * CS5330 - Computer Vision
 */

#include <opencv2/opencv.hpp>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <algorithm>
#include <vector>

#include <map>

#include "../util/csv_util.h"
#include "match_functions.h"

// helper function to sort all matric values
bool sortByValue(const std::pair<char*, float> &a,
                 const std::pair<char*, float> &b) {
    return a.second < b.second;
}

// main method to take input from user open target images and call the corrsponding functions to match the images
// to the most similar images
int main(int argc, char *argv[]) {

    // check for sufficient arguments
    if (argc != 6)
    {
        printf("usage: %s<match type> <directiory name> <image name> <number of output images>\n", argv[0]);
        exit(-1);
    }

    // take the first command line input to be the name of the target image
    char *image_name = argv[1];

    // take the second command line input to be the type match the program does
    char *match_type = argv[2];

    // take the third command line input to be the directory
    char *dir_name = argv[3];

    // take the fourth argument as the metric system 
    char *metric = argv[4];

    // take the fouth command line input to be the number of output images
    int ntop = std::atoi(argv[5]);
    
    // make the use input into a path
    char imagepath[256] = "../";
    std::strcat(imagepath, dir_name);
    std::strcat(imagepath, "/");
    std::strcat(imagepath, image_name);

    cv::Mat image;
    std::vector<std::pair<char *, float>> imagevec;

    // read input image
    image = cv::imread(imagepath, cv::IMREAD_ANYCOLOR);

    // check if image file can be opened
    if (image.empty()) {
        std::cout << "Could not open image" << std::endl;
        return -1;
    }

    if (std::strcmp(match_type, "baseline") == 0) {
        // different matching algo
        // baseline match
        baseline_match(image, imagevec, metric);
    } else if (std::strcmp(match_type, "histogram") == 0) {
        // histogram match
        histogram_match(image, imagevec, metric);
    } else if (std::strcmp(match_type, "multihistogram") == 0) {
        // multi histogram match method
        multi_histogram_match(image, imagevec, metric);
    } else if (std::strcmp(match_type, "texture") == 0) {
        // color and texture match
        color_and_texture_match(image, imagevec, metric);
    } else if (std::strcmp(match_type, "hsv") == 0) {
        // hsv match
        hsv_match(image, imagevec, metric);
    }

    // sort the distance between target image and images in database
    sort(imagevec.begin(), imagevec.end(), sortByValue);

    int i = 0;

    // get the sort image name in a separate vector
    std::vector<char *> sorted_imagename;
    for (auto &p : imagevec) {
        sorted_imagename.push_back(p.first);
    }
    
    // show target image
    char target_dir[256] = "../";
    char target_dir_name[256];
    std::strncpy(target_dir_name, dir_name, 256);
    char target_image_name[256];
    std::strncpy(target_image_name, image_name, 256);
    std::strcat(target_dir, target_dir_name);
    std::strcat(target_dir, "/");
    std::strcat(target_dir, target_image_name);
    cv::Mat target = cv::imread(target_dir);
    cv::namedWindow("target image", cv::WINDOW_NORMAL);
    cv::imshow("target image", target);

    // display the top N images
    for (int i = 1; i < ntop + 1; i++) {
        // show TOP n image
        char top_dir[256] = "../";
        char top_dir_name[256];
        std::strncpy(top_dir_name, dir_name, 256);
        char top_image_name[256];
        std::strncpy(top_image_name, image_name, 256);
        std::strcat(top_dir, top_dir_name);
        std::strcat(top_dir, "/");
        std::strcat(top_dir, sorted_imagename[i]);
        cv::Mat top = cv::imread(top_dir);
        char window_name[256] = "top ";
        std::string s = std::to_string(i);
        const char *n = s.c_str();
        std::strcat (window_name, n);
        std::strcat (window_name, " ");        
        std::strcat (window_name, sorted_imagename[i]);        
        cv::namedWindow(window_name, cv::WINDOW_NORMAL);
        cv::imshow(window_name, top);
    }

    // quit on q/Q
    while (true) {
        int k = cv::waitKey(0); 
        if (k == 'q') {
            break;
        } else if (k == 'Q') {
            break;
        }
    }

    return 0;
}

