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
#include <unistd.h>
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
    Mat img;
    Mat smallImg;
    vector<Rect> faces;
    unordered_map<int, int> ump;
}dtf_imgType;

typedef struct {
    double scale;
    bool tryflip;
    Rect detectArea;
    CascadeClassifier cascade;
    CascadeClassifier nestedCascade;
}dtf_dataType;


dtf_imgType dtf_img;
dtf_dataType dtf_data;

pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t my_convar = PTHREAD_COND_INITIALIZER;

int dtf_init (void) {
    dtf_data.detectArea = Rect(200, 120, 240, 240);
    dtf_data.tryflip = false;
    dtf_data.scale = 1;

    if (!dtf_data.nestedCascade.load(samples::findFileOrKeep(nestedCascadeName)))
        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
    if (!dtf_data.cascade.load(samples::findFile(cascadeName)))
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
        dtf_img.ump[x]++;
    }
}


void *detectFace_entry (void *arg) {
    double t = 0;
    while (1)
    {
        // cout << "detectFace_entry" << endl;

        #ifdef DEBUG_MODE 
        t = (double)getTickCount();
        #endif
        vector<Rect> faces, face2;
        Mat img, smallImg;

        //»¥³âËø
        pthread_mutex_lock (&my_mutex);
        img = dtf_img.img.clone();
        smallImg = dtf_img.img.clone();
        pthread_mutex_unlock (&my_mutex);

        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY );
        double fx = 1 / dtf_data.scale;
        resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT );
        equalizeHist(smallImg, smallImg );
        dtf_data.cascade.detectMultiScale(smallImg(dtf_data.detectArea), faces,
            1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            |CASCADE_SCALE_IMAGE,
            Size(120, 120) );
        for (auto &r: faces) {
            r.x += dtf_data.detectArea.x;
            r.y += dtf_data.detectArea.y;
        }
        
        if( dtf_data.tryflip )
        {
            flip(smallImg, smallImg, 1);
            dtf_data.cascade.detectMultiScale(smallImg(dtf_data.detectArea), face2,
                                    1.1, 2, 0
                                    //|CASCADE_FIND_BIGGEST_OBJECT
                                    //|CASCADE_DO_ROUGH_SEARCH
                                    |CASCADE_SCALE_IMAGE,
                                    Size(120, 120) );
            for( vector<Rect>::const_iterator r = face2.begin(); r != face2.end(); ++r)
            {
                faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
            }
        }

        //»¥³âËø
        pthread_mutex_lock (&my_mutex);
        //dtf_img.img = img;
        dtf_img.smallImg = smallImg;
        dtf_img.faces = faces;
        pthread_mutex_unlock (&my_mutex);

        #ifdef DEBUG_MODE 
        t = (double)getTickCount() - t;
        printf( "detection time = %lf ms\n", t*1000/getTickFrequency());
        #endif 
        usleep(1000);
    }
    
}

void *drawFace_entry (void *arg) {
    double t = 0;
    while (1) {
        // cout << "drawFace_entry" << endl;
        t = (double)getTickCount();
        vector<Rect> faces;
        Mat img;

        //»¥³âËø
        pthread_mutex_lock (&my_mutex);
        faces = dtf_img.faces;
        img = dtf_img.img.clone();
        pthread_mutex_unlock (&my_mutex);
        
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
                center.x = cvRound((r.x + r.width*0.5)*dtf_data.scale);
                center.y = cvRound((r.y + r.height*0.5)*dtf_data.scale);
                radius = cvRound((r.width + r.height)*0.25*dtf_data.scale);
                circle( img, center, radius, color, 3, 8, 0 );
            }
            else
                rectangle( img, Point(cvRound(r.x*dtf_data.scale), cvRound(r.y*dtf_data.scale)),
                        Point(cvRound((r.x + r.width-1)*dtf_data.scale), cvRound((r.y + r.height-1)*dtf_data.scale)),
                        color, 3, 8, 0);
        }
        rectangle(img, dtf_data.detectArea, Scalar(0, 0, 255));
        draw_Screen(img);
        
        
        usleep(1000);
        #ifdef DEBUG_MODE 
        t = (double)getTickCount() - t;
        printf( "draw face time = %lf ms\n", t*1000/getTickFrequency());
        #endif
    }
}

void *collectFace_entry (void *arg) {
    string imgname;
    while (1) {
        cout<<"snapshot"<<endl <<"Please enter your number:";
        int label;
        cin >> label;
        cout << "enter" << endl;
        //»¥³âËø
        pthread_mutex_lock (&my_mutex);
        if (!dtf_img.faces.empty()) {
            imgname = format("../data/face%d/s%d.jpg", label, ++dtf_img.ump[label]);
            mkdir(format("../data/face%d", label).c_str(), S_IRWXU);
            imwrite(imgname, dtf_img.smallImg(dtf_img.faces[0]));
            write_csv("../script/test.csv", imgname, label);
        }
        pthread_mutex_unlock (&my_mutex);
    }
}

void *recognition_entry (void *arg) {
    while (1) {
        pthread_mutex_lock (&my_mutex);
        //cout << "recognition face thread!" << endl;
        if (dtf_img.faces.empty()) {
            pthread_mutex_unlock (&my_mutex);
            continue;
        }
        Mat testSample = dtf_img.smallImg(dtf_img.faces[0]);
        vector<Mat> imgs;
        vector<int> labels;
        read_csv("../script/test.csv", imgs, labels);
        if (imgs.empty()) {
            cout << "Imgs is empty!" << endl;
            pthread_mutex_unlock (&my_mutex);
            continue;
        }
        Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
        model->train(imgs, labels);
        int predictedLabel = model->predict(dtf_img.smallImg(dtf_img.faces[0]));
        string result_message = format("Predicted class = %2d.", predictedLabel);
        putText(dtf_img.img, result_message, Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, colors[6], 2);
        pthread_mutex_unlock (&my_mutex);
        sleep(1);
        //cout << result_message << endl;
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
    //pthread_mutex_init(&my_mutex, NULL);
    //pthread_cond_init(&my_convar,NULL);

    if( capture.isOpened() )
    {
        cout << "Video capturing has been started ..." << endl;
        capture >> frame;
        dtf_img.img = frame.clone();
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
            pthread_mutex_lock (&my_mutex);
            dtf_img.img = frame.clone();
            pthread_mutex_unlock (&my_mutex);

            #ifdef DEBUG_MODE 
            cout << "-----" << frame.size() << endl;
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

