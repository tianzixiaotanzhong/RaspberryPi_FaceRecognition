#ifndef DRAW_IMG_H
#define DRAW_IMG_H
#include <stdio.h>

class CDrawImg{
public:
	CDrawImg(){
	};
	~CDrawImg(){
	};
	
	virtual void ShowImage(const char * title,unsigned char *pbuff,int nImgW,int nImgH,int nBs) = 0; 
private:
	
};

#include<stdio.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "Pubdef.h"

class CDrawImg_Linux:public CDrawImg{
public:
	CDrawImg_Linux(){
		m_fbfd = open("/dev/fb0", O_RDWR);
		if ( m_fbfd && !ioctl(m_fbfd, FBIOGET_FSCREENINFO, &m_finfo) && !ioctl(m_fbfd, FBIOGET_VSCREENINFO, &m_vinfo) ){
			m_W = m_vinfo.xres;
			m_H = m_vinfo.yres;
			m_Bit = m_vinfo.bits_per_pixel>>3;
			m_Pitch = m_finfo.line_length;
			m_Buffer_sz = m_W * m_H * m_Bit;
			
			m_fbp = (unsigned char *)mmap(0, m_Buffer_sz, PROT_READ | PROT_WRITE, MAP_SHARED,m_fbfd, 0);
			if( m_fbp ){
				memset(m_fbp,255,m_Buffer_sz);
				m_initial = true;
			}
		}
	};
	~CDrawImg_Linux(){
		if( m_initial ){
			if( m_fbp ){ munmap(m_fbp, m_Buffer_sz); };
			if( m_fbfd ){ close(m_fbfd); };
			m_initial = false;
		}
	};
	
	unsigned char * PtrBuff(){ return m_fbp;}
	int GetScreenPW(){ return m_Pitch; }
	int GetScreenW(){ return m_W; }
	int GetScreenH(){ return m_H; }
	virtual void ShowImage(const char * title,unsigned char *pbuff,int nImgW,int nImgH,int nBs) override;
private:
	bool m_initial = false;
	int m_fbfd = 0;
	struct fb_fix_screeninfo m_finfo;
    struct fb_var_screeninfo m_vinfo;
	unsigned char * m_fbp = NULL;
	int m_Bit = 0;
	int m_W = 0;
	int m_H = 0;
	int m_Pitch = 0;
	int m_Buffer_sz = 0;
};

#endif

