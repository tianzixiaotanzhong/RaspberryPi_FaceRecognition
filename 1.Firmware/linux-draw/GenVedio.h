#ifndef GEN_VEDIO_H
#define GEN_VEDIO_H

#define ROUNDfour(x) (((x) + 3) &~(3))
extern "C" {
#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
#include "libavutil/pixfmt.h"
}

#define DEVICE_NAME "video4linux2"
//#define DEVICE_NAME "vfwcap"

class CGenVedio{
public:
	CGenVedio(){
		m_pFormatCtx = NULL;
		m_pCodecCtx  = NULL;
		m_pCodec     = NULL;
        m_pFrame     = NULL;
	    m_pFrameYUV  = NULL;
	    m_out_buffer = NULL;
	    m_packet     = NULL;
	    m_img_convert_ctx = NULL;
	    m_nVedioIdx = -1;
	    m_bInitial = false;
	    InitYUVTab();
	};
	~CGenVedio(){
		Clear();
	};
	
public:
	bool GetFrame_RGB24(unsigned char * &pBuff, int &nW,int &nH,int &nBs){
		bool bRet = false;
		pBuff = NULL;
		if(m_bInitial){
			if(av_read_frame(m_pFormatCtx, m_packet)>=0){
                if(m_packet->stream_index==m_nVedioIdx){
                	int got_picture = 0;
                    int ret = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got_picture, m_packet);
                    if(ret>0 && got_picture ){
 						nW = m_pCodecCtx->width;
 						nH = m_pCodecCtx->height;
						
						int nDibW = ROUNDfour(nW*3);
						nBs = 3;
						
                        sws_scale(m_img_convert_ctx, (const uint8_t* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameYUV->data, m_pFrameYUV->linesize);

						int sz = nDibW*nH;
					 	pBuff = new unsigned char[sz];
						unsigned char * pRGB = m_pFrameYUV->data[0];
						memcpy(pBuff,pRGB,sz);		
						bRet = true;
						//delete pBuff ;
                    }
                }
                av_free_packet(m_packet);			
			}
		}
		return bRet;
	};
	
	bool SkipFrame(int n=1){
		if(m_bInitial){
			for( int i=0; i<n; ++i ){
				if(av_read_frame(m_pFormatCtx, m_packet)>=0){
					av_free_packet(m_packet);			
				}
			}
		}
	};	
	
	bool Initial(){
		
		bool bRet = false;
		if( m_bInitial ) return true;
	    av_register_all();
    	avdevice_register_all();
    	
    	m_pFormatCtx = avformat_alloc_context();
	    AVInputFormat *ifmt=av_find_input_format(DEVICE_NAME);//
		cout << ifmt << endl;
		AVDictionary *options = NULL;
		av_dict_set(&options, "video_size", "640x480", 0);
		av_dict_set(&options, "framerate", "20", 0);
		av_dict_set(&options, "pixel_format", "rgb24", 0);
		av_dict_set(&options, "preset", "superfast", 0);
		av_dict_set(&options, "tune", "zerolatency", 0);
		av_dict_set(&options, "profile", "main", 0);	
	    if(avformat_open_input(&m_pFormatCtx,"/dev/video0",ifmt,&options)==0){
			cout << "Open Vedio Input Ok" << endl;
	        if(avformat_find_stream_info(m_pFormatCtx,NULL)>-1){
			    cout << "Find Vedio Info Ok" << endl;
       			//find device
       			m_nVedioIdx = -1;
       			int i = 0;
	   			for(i=0; i<m_pFormatCtx->nb_streams; i++){
			        if(m_pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			            m_nVedioIdx=i;
			            break;
			        }			       	
		        }
				
				if( m_nVedioIdx>-1 ){
					
    			    m_pCodecCtx=m_pFormatCtx->streams[m_nVedioIdx]->codec;
				    m_pCodec=avcodec_find_decoder(m_pCodecCtx->codec_id);
				    if(m_pCodec!=NULL)
				    {	
				        if(avcodec_open2(m_pCodecCtx, m_pCodec,NULL)>-1)
					    {
			    			m_pFrame=av_frame_alloc();
							m_pFrameYUV=av_frame_alloc();
							//  AV_PIX_FMT_RGB24  AV_PIX_FMT_YUV422P
							#define AV_PIX_TYPE AV_PIX_FMT_RGB24
							m_out_buffer=(uint8_t *)av_malloc(avpicture_get_size(AV_PIX_TYPE, m_pCodecCtx->width, m_pCodecCtx->height));
							avpicture_fill((AVPicture *)m_pFrameYUV, m_out_buffer,AV_PIX_TYPE, m_pCodecCtx->width, m_pCodecCtx->height);
							m_packet=(AVPacket *)av_malloc(sizeof(AVPacket));
						    m_img_convert_ctx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_TYPE, SWS_BICUBIC, NULL, NULL, NULL);//AV_PIX_FMT_YUV420P 
							
							if( NULL!=m_img_convert_ctx ){
								bRet = true;
								m_bInitial = true;
							}
							else{
								Clear();
							}
					    }
				    }
				}
       		}
	    }
    	
    	return bRet;
	};
private:
	void Clear(){
	 	if(m_packet){ av_free_packet(m_packet); m_packet = NULL;}
	    if(m_img_convert_ctx){
    		sws_freeContext(m_img_convert_ctx); m_img_convert_ctx = NULL;
    	}
    	if(m_out_buffer){ av_free(m_out_buffer); m_out_buffer = NULL;}
		if(m_pFrameYUV){ av_free(m_pFrameYUV); m_pFrameYUV = NULL;}
		if(m_pFrame){ av_free(m_pFrame); m_pFrame = NULL;}
		//m_pCodec     = NULL;
	    if(m_pCodecCtx ){ avcodec_close(m_pCodecCtx); m_pCodecCtx = NULL;}
	    if(m_pFormatCtx ){ avformat_close_input(&m_pFormatCtx); m_pFormatCtx = NULL;}
		m_bInitial = false;
		m_nVedioIdx = -1;		
	};
#define TABNUM (256)
	void InitYUVTab(){
		for( int i = 0; i<TABNUM; i++ ){
			for(int j =0; j<TABNUM; j++){
				YUV_R[i][j] = (int)(i + 1.4075 * (j-128));// Y V = R
				YUV_G1[i][j] = 0.3455 * (i-128) + 0.7169*(j-128); //U V= G1 //Y-G1= G2
				YUV_B[i][j] = (int)(i + 1.779 * (j-128)); // Y U = B
				
				if( YUV_R[i][j]<0 ) YUV_R[i][j] = 0;
				else if(YUV_R[i][j]>255) YUV_R[i][j] = 255;
								
				if( YUV_B[i][j]<0 ) YUV_B[i][j] = 0;
				else if(YUV_B[i][j]>255) YUV_B[i][j] = 255;
			}
		}
	}
	
	void GetRGB(unsigned char Y,unsigned char U,unsigned char V,unsigned char &R,unsigned char &G,unsigned char &B){
		R = YUV_R[Y][U];
		B = YUV_B[Y][V];
		int tG = Y-YUV_G1[V][U];
		if( tG<0 ) tG = 0;
		else if(tG>255) tG = 255;
		G = tG;		
	}

private:
	bool             m_bInitial;
    AVFormatContext *m_pFormatCtx;
    AVCodecContext  *m_pCodecCtx;
    AVCodec         *m_pCodec;
    AVFrame 		*m_pFrame;
	AVFrame         *m_pFrameYUV;
	uint8_t 		*m_out_buffer;
	AVPacket        *m_packet;
	int				 m_nVedioIdx;
	struct SwsContext *m_img_convert_ctx;
	short int YUV_R[TABNUM][TABNUM];
	short int YUV_G1[TABNUM][TABNUM];
	short int YUV_B[TABNUM][TABNUM];
};


#endif

