// https://github.com/t-pot/tpot_bmp/

// How to use 
/* 
	int width = 1920;
	int height = 1080;
	tpot::bmp* bmp = new  tpot::bmp(width, height);

	unsigned char* p = bmp->get_image_addr();
	p[3*width*y+3*x+BGR] = xxx; // render! (0, 0) is left-bottom

	bmp->save("hoge.bmp");
	delete bmp;
*/

#pragma warning(disable : 4996)// using unsafe fopen

#pragma once
#ifndef INCLUDE_TPOT_BMP_H_
#define INCLUDE_TPOT_BMP_H_

#include <stdio.h> // for free
#include <stdlib.h> // for fopen
#include <string.h> // for memcpy
#include <stdint.h> // for uint32_t

namespace tpot 
{
	// Recommend a multiple of 4 for the width (4 bytes alignment required in each line)
	// Address calculation when width is not a multiple of 4:
	// addr(x,y) = get_image_addr() + (((3 * width * y + 3) >> 2) << 2) + 3 * x;
	// 幅は4の倍数を推奨(横方向は4byteアライメントされていないといけない)
	// 幅が4の倍数でないときのアドレス計算が上式

	class bmp
	{
	private:
		char* head_ = nullptr;
		unsigned char* data_ = nullptr;
		size_t line_size_;
		size_t size_;

		struct BITMAP_HEADER {
			// BITMAP FILEHEADER
			char bfType[2] = { 0x42, 0x4D };
			char bfSize[4];// file size = data size + 54
			char bfReserved[4] = { 0, 0, 0, 0 };
			char bfOffBits[4] = { 54, 0, 0, 0 };

			// BITMAPINFOHEADER
			char bcSize[4] = { 40, 0, 0, 0 };
			char bcWidth[4];// data width
			char bcHeight[4];// data height
			char bcPlanes[2] = { 1, 0 };
			char bcBitCount[2] = { 24, 0 };
			char biCompression[4] = { 0, 0, 0, 0 };
			char biSizeImage[4];// bytes of data
			char biXPixPerMeter[4] = { 0, 0, 0, 0 };
			char biYPixPerMeter[4] = { 0, 0, 0, 0 };
			char biClrUsed[4] = { 0, 0, 0, 0 };
			char biCirImportant[4] = { 0, 0, 0, 0 };
		};

		static void copy_uint32(char* p, int d)
		{
			p[0] = d & 0xff;
			p[1] = (d >> 8) & 0xff;
			p[2] = (d >> 16) & 0xff;
			p[3] = d >> 24;
		}

		int create(unsigned int width, unsigned int height)
		{
			line_size_ = ((3 * width + 3) >> 2) << 2;// must 4byte alignment
			size_t data_size = line_size_ * height;
			size_ = sizeof(BITMAP_HEADER) + data_size;

			head_ = (char*)malloc(size_);
			if (!head_)return -1;

			data_ = (unsigned char*)head_ + sizeof(BITMAP_HEADER);

			// fill header
			char* p = head_;
			BITMAP_HEADER h;
			copy_uint32(h.bfSize, size_);
			copy_uint32(h.bcWidth, width);
			copy_uint32(h.bcHeight, height);
			copy_uint32(h.biSizeImage, data_size);
			memcpy(p, &h, sizeof(BITMAP_HEADER));

			return 0;
		}

		void release()
		{
			if (head_) free(head_);
			head_ = nullptr;
			data_ = nullptr;
		}


	public:
		 bmp(unsigned int width, unsigned int height) { create(width, height); }
		~bmp(){ release(); }

		unsigned char* get_image_addr() { return data_; }

		int save(const char* filename) const
		{
			FILE *fp = fopen(filename, "wb");
			if (nullptr == fp) return -1;
			fwrite(head_, size_, 1, fp);
			fclose(fp);

			return 0;
		}

	public:
		// supports

		struct color {
			unsigned char b, g, r;
		};

		inline color* image(int x, int y) { return (color*)(data_ + line_size_ * y + 3 * x); }
	};
}

#endif // !INCLUDE_TPOT_BMP_H_
