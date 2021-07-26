/*
 * @Author: your name
 * @Date: 2021-07-25 18:09:22
 * @LastEditTime: 2021-07-26 15:38:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \FaceRecognition\source\my_csv.cpp
 */

#include "iostream"
#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "my_csv.h"


void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        // cout <<"-----"<<path<<"*****"<<classlabel<<endl;
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void write_csv(const string& filename, string imgdir, int lable, char separator) {
    std::ofstream file(filename.c_str(), ofstream::app);
    if (!file) {
        string error_message = "No valid output file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    file << format("\n%s%c%d", imgdir.c_str(), separator, lable);
    file.close();
}