#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 
#include "DrawImage.h"
#include "Pubdef.h"

void CDrawImg_Linux::ShowImage(const char* title, unsigned char *pbuff,int nImgW,int nImgH,int nBs){
	if( !m_initial ){ return; }
	int scrW = GetScreenW();
	int scrH = GetScreenH();
	int scrPW = GetScreenPW();
	int imgPW = ROUND4(nImgW*nBs);
	int drW = MIN(scrW,nImgW);
	int drH = MIN(scrH,nImgH);
	unsigned char *pScr = PtrBuff();
	
	int ImgPos = 0;
	int ScrPos = 0;
	int j=0, i=0;
	if( nBs == 3 ){
		if( m_Bit == 4 ){
			for( j=0; j<drH; ++j){
				ScrPos = j * scrPW ;
				//ImgPos = (drH - 1 - j)*imgPW;  //readjpg
				ImgPos = j * imgPW;    //摄像头
				for( i=0; i<drW; ++i ){
					pScr[ScrPos+2] = pbuff[ImgPos];
					pScr[ScrPos+1] = pbuff[ImgPos+1];
					pScr[ScrPos] = pbuff[ImgPos+2];
				
					ScrPos+=m_Bit;
					ImgPos+=3;
				}
			}
		}else if( m_Bit == 2 ){
			for( j=0; j<drH; ++j){
				ScrPos = j * scrPW ;
				ImgPos = j * imgPW;    //摄像头
				for( i=0; i<drW; ++i ){//BRG565
					unsigned char gray = 0x80;
					unsigned char r = (pbuff[ImgPos])>>3;
					unsigned char g = (pbuff[ImgPos+1])>>2;
					unsigned char b = (pbuff[ImgPos+2])>>3;
					pScr[ScrPos+1] = (r<<3) | (g>>3);//0x00;//pbuff[ImgPos+1];
					pScr[ScrPos]   = ((g&0x7)<<5) | b;// (r>>3)|(b<<3);//0xff;//pbuff[ImgPos+2];
				
					ScrPos+=m_Bit;
					ImgPos+=3;
				}
			}
		}
	}
}

