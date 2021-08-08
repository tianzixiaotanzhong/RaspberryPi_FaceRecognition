#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/face.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <unordered_map>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include "DrawImage.h"
#include "readjpg.h"
#include "GenVedio.h"
#include "my_csv.h"
#include "ring_queue.h"
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

ring_queue<Mat> img_rq(10);
ring_queue<Rect> rec_area_rq(10);

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
    int predictedLabel;
    Rect detectArea;
    unordered_map<int, int> ump;
    CascadeClassifier cascade;
    CascadeClassifier nestedCascade;
}dtf_dataType;


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
        dtf_data.ump[x]++;
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
        if (img_rq.isEmpty()) {
            continue;
        }
        Mat img = img_rq.getBack();
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY );
        
        equalizeHist(gray, gray );
        dtf_data.cascade.detectMultiScale(gray(dtf_data.detectArea), faces,
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
            flip(gray, gray, 1);
            dtf_data.cascade.detectMultiScale(gray(dtf_data.detectArea), face2,
                                    1.1, 2, 0
                                    //|CASCADE_FIND_BIGGEST_OBJECT
                                    //|CASCADE_DO_ROUGH_SEARCH
                                    |CASCADE_SCALE_IMAGE,
                                    Size(120, 120) );
            for( vector<Rect>::const_iterator r = face2.begin(); r != face2.end(); ++r)
            {
                faces.push_back(Rect(gray.cols - r->x - r->width, r->y, r->width, r->height));
            }
        }

        if (!faces.empty()) {
            rec_area_rq.push(faces[0]);
        }

        #ifdef DEBUG_MODE 
        t = (double)getTickCount() - t;
        printf( "detection time = %lf ms\n", t*1000/getTickFrequency());
        #endif 
        // usleep(1000);
    }
    
}

void *recognition_entry (void *arg) {
    while (1) {
        // cout << "recognition_entry" << endl;
        if (rec_area_rq.isEmpty()) {
            dtf_data.predictedLabel = 0;
            continue;
        }

        vector<Mat> imgs;
        vector<int> labels;
        Mat gray;
        read_csv("../script/test.csv", imgs, labels);
        if (imgs.empty()) {
            cout << "Imgs is empty!" << endl;
            continue;
        }
        Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
        model->train(imgs, labels);

        cvtColor(img_rq.getBack(), gray, COLOR_BGR2GRAY);
        dtf_data.predictedLabel = model->predict(gray(rec_area_rq.getBack()));

        // usleep(1000);
        //cout << result_message << endl;
    }
}

Mat col_img;
Rect col_area;

void *drawFace_entry (void *arg) {
    double t = 0;
    while (1) {
        // cout << "drawFace_entry" << endl;
        t = (double)getTickCount();
        
        if (img_rq.isEmpty()) {
            continue;
        }
        Mat img = img_rq.getBack().clone();
        
        int label = dtf_data.predictedLabel;
        
        
        
        //标记脸部区域
        if (!rec_area_rq.isEmpty()) {
            Rect r = rec_area_rq.getBack();
            col_img = img.clone();
            col_area = r;
            Mat smallImgROI;
            vector<Rect> nestedObjects;
            Point center;
            Scalar color = colors[0];
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
        string result_message = format("Predicted class = %2d.", label);
        putText(img, result_message, Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, colors[6], 2);
        draw_Screen(img);

        img_rq.pop();
        rec_area_rq.pop();
        
        
        // usleep(1000);
        #ifdef DEBUG_MODE 
        t = (double)getTickCount() - t;
        printf( "draw face time = %lf ms\n", t*1000/getTickFrequency());
        #endif
    }
}

void *collectFace_entry (void *arg) {
    string imgname;
    Mat gray;
    while (1) {
        cout << "Please enter your number:";
        int label;
        cin >> label;
        if (!col_img.empty() && !col_area.empty()) {
            cvtColor(col_img, gray, COLOR_BGR2GRAY);
            imgname = format("../data/face%d/s%d.jpg", label, ++dtf_data.ump[label]);
            mkdir(format("../data/face%d", label).c_str(), S_IRWXU);
            imwrite(imgname, gray(col_area));
            write_csv("../script/test.csv", imgname, label);
        }
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
            if (img_rq.size() >= 10) {
                continue;
                //img_rq.pop_front();
            }
            flip(frame, frame, 1);
            img_rq.push(frame.clone());
            
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

