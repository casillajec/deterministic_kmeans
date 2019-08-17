#ifndef __PIXEL
	#define __PIXEL

	typedef struct {
		float r;
		float g;
		float b;
	} pixel;


	int pixel_cmp(pixel* p1, pixel* p2);

	void pixel_print(pixel* p);

	typedef struct{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} pixel_uint8;

	short pixel_uint8_cmp(pixel_uint8 p1, pixel_uint8 p2);
#endif
