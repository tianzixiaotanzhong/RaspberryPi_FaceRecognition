#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/face.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <thread>
#include "my_csv.h"
using namespace std;
using namespace cv;
using namespace cv::face;

#define DEBUG_MODE

vector<Mat> detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip );
string cascadeName = "F:/OpenCV/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
string nestedCascadeName = "F:/OpenCV/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame, image;
    bool tryflip = false;
    CascadeClassifier cascade, nestedCascade;
    double scale = 1;
    
    if (!nestedCascade.load(samples::findFileOrKeep(nestedCascadeName)))
        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
    if (!cascade.load(samples::findFile(cascadeName)))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }
    int camera = 0;
    if(!capture.open(camera))
    {
        cout << "Capture from camera #" <<  camera << " didn't work" << endl;
        return 1;
    }
    
    vector<Mat> srcImgs;
    vector<int> labels;
    read_csv("../script/test.csv", srcImgs, labels);
    unordered_map<int, int> ump;
    for (auto x: labels) {
        ump[x]++;
    }

    if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;
        for(;;)
        {
            capture >> frame;
            if( frame.empty() )
                break;
            Mat frame1 = frame.clone();

            #ifdef DEBUG_MODE 
            cout << "-----" << frame1.size() << endl;
            #endif

            vector<Mat> faceImgs = detectAndDraw( frame1, cascade, nestedCascade, scale, tryflip );
            char c = (char)waitKey(10);
            if ( c == 27 || c == 'q' || c == 'Q' )
                break;
            if ( c >= '0' && c <= '9' && !faceImgs.empty()) {
                int label = c - '0';
                string imgname = format("../data\\face%d/s%d.jpg", label, ++ump[label]);
                std::filesystem::create_directory(format("../data/face%d", label));
                imwrite(imgname, faceImgs[0]);
                write_csv("../script/test.csv", imgname, label);
            }
            if ((c == 'r' || c == 'R') && !faceImgs.empty()) {
                Mat testSample = faceImgs[0];
                read_csv("../script/test.csv", srcImgs, labels);
                Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
                model->train(srcImgs, labels);
                int predictedLabel = model->predict(faceImgs[0]);
                string result_message = format("Predicted class = %d.", predictedLabel);
                cout << result_message << endl;
            }
        }
    }
    
    return 0;
}

vector<Mat> detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip )
{
    double t = 0;
    vector<Rect> faces, faces2;
    Rect detectArea = Rect(200, 120, 240, 240);
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    rectangle(img, detectArea, Scalar(0, 0, 255));
    Mat gray, smallImg;
    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
    equalizeHist( smallImg, smallImg );
    t = (double)getTickCount();
    cascade.detectMultiScale( smallImg(detectArea), faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(120, 120) );
    for (auto &r: faces) {
        r.x += detectArea.x;
        r.y += detectArea.y;
    }
    
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg(detectArea), faces2,
                                 1.1, 2, 0
                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                 //|CASCADE_DO_ROUGH_SEARCH
                                 |CASCADE_SCALE_IMAGE,
                                 Size(120, 120) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r)
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)getTickCount() - t;

    #ifdef DEBUG_MODE 
    printf( "detection time = %g ms\n", t*1000/getTickFrequency());
    #endif
    
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        double aspect_ratio = (double)r.width/r.height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.y + r.height*0.5)*scale);
            radius = cvRound((r.width + r.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                       Point(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                       color, 3, 8, 0);
    }
    vector<Mat> faceImgs;
    for (vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); ++r) {
        faceImgs.push_back(smallImg(*r));
    }
    imshow( "result", img );
    return faceImgs;
}