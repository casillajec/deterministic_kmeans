#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(){
	
	int width, height, n;
	unsigned char* data = stbi_load("curses_640x300.png", &width, &height, &n, 0);
	printf("width: %d, height: %d, n: %d\n", width, height, n);
	stbi_write_png("test_write.png", width, height, n, data, 0);
	stbi_image_free(data);
	return 1;
}
