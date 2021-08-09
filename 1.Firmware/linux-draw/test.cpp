// #include "DrawImage.h"
// #include "readjpg.h"
// #include "GenVedio.h"
// #include <iostream>

// using namespace std;

// int ImgW = 640;
// int ImgH = 480;
// int nB = 3;
// int ImgPitch = ROUND4(ImgW*3);
// int len = ImgPitch*ImgH;
// //unsigned char Img[len] = {0};

// int main(){
//     //CGenVedio camera;
//     CDrawImg_Linux drawer;
    
//     //if (camera.Initial()){ cout << "init" << endl; }
//     //else return 0;
    
//     unsigned char* rb = NULL;
//     rb = ReadJPEG("./wangy.jpg",ImgW,ImgH,nB);
//     while(1){
//         //if( camera.GetFrame_RGB24(rb,ImgW,ImgH,nB) ){
//             drawer.ShowImage("",rb,ImgW,ImgH,nB);
//         //}
//         //camera.SkipFrame(5);
//     }
    
//     delete [] rb;
//     rb = NULL;
//     return 0;
// }

