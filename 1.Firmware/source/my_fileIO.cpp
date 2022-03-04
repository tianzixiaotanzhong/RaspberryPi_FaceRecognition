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
#include "my_fileIO.h"
#include "student.h"


void read_csv(const string& filename, vector<Mat>& images, vector<student>& stdts, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, num, name;
    student sd;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, num, separator);
        // cout <<"-----"<<path<<"*****"<<classlabel<<endl;
        if(!path.empty() && !classlabel.empty()) {
            sd.num = atoi(num.c_str());
            strcpy(sd.name, name.c_str());
            images.push_back(imread(path, 0));
            stdts.push_back(sd);
        }
    }
}

void write_csv(const string& filename, string imgdir, student sd, char separator) {
    std::ofstream file(filename.c_str(), ofstream::app);
    if (!file) {
        string error_message = "No valid output file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    file << format("\n%s%c%d", imgdir.c_str(), separator, sd.num, separator, sd.name);
    file.close();
}