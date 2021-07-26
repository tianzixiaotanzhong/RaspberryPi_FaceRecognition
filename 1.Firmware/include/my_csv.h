/*
 * @Author: your name
 * @Date: 2021-07-25 18:09:45
 * @LastEditTime: 2021-07-26 10:57:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \FaceRecognition\include\my_csv.h
 */

#ifndef _MY_CSV_H
#define _MY_CSV_H

#include <string>
#include <fstream>
#include <vector>
#include "opencv2/core.hpp"
using namespace std;
using namespace cv;

void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');
void write_csv(const string& filename, string imgname, int lable, char separator = ';');

#endif
