#include "readjpg.h"

unsigned char* ReadJPEG(const char* filename,int &nW,int &nH,int &nB){
	unsigned char* pRet = NULL;
	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);// STEP 1: StdError

	jpeg_create_decompress(&cinfo);// STEP 2: Create
	FILE* pf = fopen(filename, "rb");
	if (pf){
		jpeg_stdio_src(&cinfo, pf);// STEP 3: IO
		jpeg_read_header(&cinfo, TRUE);// STEP 4: Header
		
		nW = cinfo.image_width;
		nH = cinfo.image_height;
		nB = cinfo.num_components;
		long bytes = cinfo.image_width*cinfo.image_height*cinfo.num_components;
		pRet = new unsigned char[bytes];
		if (pRet){
			jpeg_start_decompress(&cinfo);// STEP 5: Start
			JSAMPROW row_pointer[1];
			while (cinfo.output_scanline < cinfo.output_height){//// STEP 6: ReadScan
				row_pointer[0] = &pRet[(cinfo.output_height - cinfo.output_scanline - 1)*cinfo.image_width*cinfo.num_components];
				jpeg_read_scanlines(&cinfo, row_pointer, 1);
			}
			jpeg_finish_decompress(&cinfo);// STEP 7: Finish
		}
		jpeg_destroy_decompress(&cinfo);// STEP 8: Destroy
		fclose(pf);
	}else{
		cout << filename << " open failed" << endl;
	}

	return pRet;
}

