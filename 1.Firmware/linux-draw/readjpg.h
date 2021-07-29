#ifndef READ_JPG_H
#define READ_JPG_H

#include <stdio.h>
#include "libjpeg/jpeglib.h"
#include <iostream>


using namespace std;

unsigned char* ReadJPEG(const char* filename,int &nW,int &nH,int &nB);



#endif

