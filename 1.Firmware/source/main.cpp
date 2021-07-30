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
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include "DrawImage.h"
#include "readjpg.h"
#include "GenVedio.h"
#include "my_csv.h"
//#include <unistd.h>
using namespace std;
using namespace cv;
using namespace cv::face;

//#define DEBUG_MODE

int ImgW = 640;
int ImgH = 480;
int nB = 3;
int ImgPitch = ROUND4(ImgW*3);
int len = ImgPitch*ImgH;

CDrawImg_Linux drawer;

void draw_Screen (Mat input) {
    cv::cvtColor(input, input, cv::COLOR_RGB2BGR);
    drawer.ShowImage("", input.data, ImgW, ImgH, nB);
}

string cascadeName = "/home/pi/opencv-project/opencv/data/haarcascades/haarcascade_frontalface_alt.xml";
string nestedCascadeName = "/home/pi/opencv-project/opencv/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

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

typedef struct {
    double scale;
    bool tryflip;
    Mat img;
    Mat smallImg;
    Rect detectArea;
    vector<Rect> faces;
    CascadeClassifier cascade;
    CascadeClassifier nestedCascade;
    unordered_map<int, int> ump;
}dtf_structure;

dtf_structure dtf;
pthread_mutex_t my_mutex;
pthread_cond_t my_convar;

int dtf_init (void) {
    dtf.detectArea = Rect(200, 120, 240, 240);
    dtf.tryflip = false;
    dtf.scale = 1;

    if (!dtf.nestedCascade.load(samples::findFileOrKeep(nestedCascadeName)))
        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
    if (!dtf.cascade.load(samples::findFile(cascadeName)))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }

    vector<Mat> imgs;
    vector<int> labels;
    read_csv("../script/test.csv", imgs, labels);
    if (imgs.empty()) {
        cout << "Imgs is empty!" << endl;
        return 0;
    }
    for (auto x: labels) {
        dtf.ump[x]++;
    }
}


void *detectFace_entry (void *arg) {
    while (1)
    {
        cout << "detect face thread!" << endl;
        double t = 0;
        vector<Rect> faces;
        
        pthread_mutex_lock (&my_mutex);
        rectangle(dtf.img, dtf.detectArea, Scalar(0, 0, 255));
        Mat gray;
        cvtColor( dtf.img, gray, COLOR_BGR2GRAY );
        double fx = 1 / dtf.scale;
        resize( gray, dtf.smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
        equalizeHist( dtf.smallImg, dtf.smallImg );
        t = (double)getTickCount();
        dtf.cascade.detectMultiScale( dtf.smallImg(dtf.detectArea), dtf.faces,
            1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            |CASCADE_SCALE_IMAGE,
            Size(120, 120) );
        for (auto &r: dtf.faces) {
            r.x += dtf.detectArea.x;
            r.y += dtf.detectArea.y;
        }
        
        if( dtf.tryflip )
        {
            flip(dtf.smallImg, dtf.smallImg, 1);
            dtf.cascade.detectMultiScale( dtf.smallImg(dtf.detectArea), faces,
                                    1.1, 2, 0
                                    //|CASCADE_FIND_BIGGEST_OBJECT
                                    //|CASCADE_DO_ROUGH_SEARCH
                                    |CASCADE_SCALE_IMAGE,
                                    Size(120, 120) );
            for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); ++r)
            {
                faces.push_back(Rect(dtf.smallImg.cols - r->x - r->width, r->y, r->width, r->height));
            }
        }
        pthread_mutex_unlock (&my_mutex);
        t = (double)getTickCount() - t;

        #ifdef DEBUG_MODE 
        printf( "detection time = %g ms\n", t*1000/getTickFrequency());
        #endif 
    }
    
}

void *drawFace_entry (void *arg) {
    while (1) {
        cout << "draw face thread!" << endl;
        pthread_mutex_lock (&my_mutex);
        for ( size_t i = 0; i < dtf.faces.size(); i++ )
        {
            Rect r = dtf.faces[i];
            Mat smallImgROI;
            vector<Rect> nestedObjects;
            Point center;
            Scalar color = colors[i%8];
            int radius;
            double aspect_ratio = (double)r.width/r.height;
            if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
            {
                center.x = cvRound((r.x + r.width*0.5)*dtf.scale);
                center.y = cvRound((r.y + r.height*0.5)*dtf.scale);
                radius = cvRound((r.width + r.height)*0.25*dtf.scale);
                circle( dtf.img, center, radius, color, 3, 8, 0 );
            }
            else
                rectangle( dtf.img, Point(cvRound(r.x*dtf.scale), cvRound(r.y*dtf.scale)),
                        Point(cvRound((r.x + r.width-1)*dtf.scale), cvRound((r.y + r.height-1)*dtf.scale)),
                        color, 3, 8, 0);
        }
        draw_Screen(dtf.img);
        pthread_mutex_unlock (&my_mutex);
    }
}

void *collectFace_entry (void *arg) {
    while (1) {
        cout<<"snapshot"<<endl <<"Please enter your number:";
        pthread_mutex_lock (&my_mutex);
        int label;
        cin >> label;
        cout << "enter" << endl;
        string imgname = format("../data/face%d/s%d.jpg", label, ++dtf.ump[label]);
        mkdir(format("../data/face%d", label).c_str(), S_IRWXU);
        cout << imwrite(imgname, dtf.smallImg(dtf.faces[0]));
        write_csv("../script/test.csv", imgname, label);
        pthread_mutex_unlock (&my_mutex);
    }
}

void *recognition_entry (void *arg) {
    while (1) {
        pthread_mutex_lock (&my_mutex);
        cout << "recognition face thread!" << endl;
        if (dtf.faces.empty()) {
            pthread_exit((void*) 0);
        }
        Mat testSample = dtf.smallImg(dtf.faces[0]);
        vector<Mat> imgs;
        vector<int> labels;
        read_csv("../script/test.csv", imgs, labels);
        if (imgs.empty()) {
            cout << "Imgs is empty!" << endl;
            pthread_exit((void*) 0);
        }
        Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
        model->train(imgs, labels);
        int predictedLabel = model->predict(dtf.img(dtf.faces[0]));
        string result_message = format("Predicted class = %d.", predictedLabel);
        pthread_mutex_unlock (&my_mutex);
        cout << result_message << endl;
    }
}

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame, image;
    
    int camera = -1;
    if(!capture.open(camera))
    {
        cout << "Capture from camera #" <<  camera << " didn't work" << endl;
        return 1;
    }
    
    dtf_init();
    pthread_mutex_init(&my_mutex, NULL);
    pthread_cond_init(&my_convar,NULL)

    if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;
        capture >> frame;
        dtf.img = frame.clone();
        pthread_t dt_thd, df_thd, cf_thd, rg_thd;
        int dt_tid, df_tid, cf_tid, rg_tid;
        pthread_create(&dt_thd, NULL, detectFace_entry, (void*) 0);
        pthread_create(&df_thd, NULL, drawFace_entry, (void*) 0);
        pthread_create(&cf_thd, NULL, collectFace_entry, (void*) 0);
        pthread_create(&rg_thd, NULL, recognition_entry, (void*) 0);
        for(;;)
        {
            capture >> frame;
            if( frame.empty() )
                break;
            dtf.img = frame.clone();

            #ifdef DEBUG_MODE 
            cout << "-----" << frame1.size() << endl;
            #endif
            
        }
        void *status;
        pthread_join(dt_thd, &status);
        pthread_join(df_thd, &status);
        pthread_join(cf_thd, &status);
        pthread_join(rg_thd, &status);
    }
    
    return 0;
}

