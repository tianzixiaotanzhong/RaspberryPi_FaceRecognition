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
		for( j=0; j<drH; ++j){
			ScrPos = j * scrPW ;
			//ImgPos = (drH - 1 - j)*imgPW;  //readjpg
			ImgPos = j * imgPW;    //摄像头
			for( i=0; i<drW; ++i ){
				pScr[ScrPos+2] = pbuff[ImgPos];
				pScr[ScrPos+1] = pbuff[ImgPos+1];
				pScr[ScrPos] = pbuff[ImgPos+2];
				
				ScrPos+=4;
				ImgPos+=3;
			}
		}
	}
}

