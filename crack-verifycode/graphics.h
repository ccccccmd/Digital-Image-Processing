#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <vector>
using namespace std;

#define PROCESS_FILE_OPEN_PATH  "..\\bmp\\test.bmp"
#define PROCESS_FILE_SAVE_PATH  "..\\bmp\\"
#define PROCESS_REMOVEBLA_PATH  "..\\bmp\\remove_pixels"
#define PROCESS_TEXT_SAVE_PATH  "..\\data\\"

const int REMOVE_LONELY_PIXEL	= 1;
const int MAX_RANGE				= 250;
const int PIXELNUM				= 256;
const int FILEHEADERSIZE		= 14;
const int INFOHEADERSIZE		= 40;
const int BMPHEADERSIZE			= 54 ;
const int ERROR					= -1;
const int GAUSSIAN				= 25;
const int Filter_Num			= 8;
const int SobelX				= 4;
const int SobelY				= 5;
const int PathLength			= 200;
const int NumLength				= 10;
const int BLACK					= 0;
const int WHITE					= 255;
const int EDGE_THICK			= 3;
const bool BOOL_WHITE			= true;
const double PI					= 3.1415926;

const int DT_CHANGE_WHITE = 0; // not change white
const int DO_CHANGE_WHITE = 1; // change white

const long DT_CHANGE_WHITE_TIMES = 12;// the times of procession do not change white
const long DO_CHANGE_WHITE_TIMES = 1; // the times of procession do change white

const double gaussian[GAUSSIAN] = 
                            {2,  4,  5,  4,  2,
	                         4,  9, 12,  9,  4,
					         5, 12, 15, 12,  5,
					         4,  9, 12,  9,  4,
					         2,  4,  5,  4,  2};

const int DIREC_ARRAY_LEN = 4;
const int DIRECTIONS[4] = {-45, 0, 45, 90};

const int MASK[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

enum canny_direction
{
	CANNY_HORIZONTAL     = 0, // 90
	CANNY_VERTICAL       = 1, // 0
	CANNY_LEFT_DIAGONAL  = 2, // 45
	CANNY_RIGHT_DIAGONAL = 3  // -45
};

// file type
const int BMPTYPE = 0;
const int TXTTYPE = 1;

template <class T>
inline T iABS(T X)
{
	return X > 0 ? X : -X;
}

template <class T>
inline T imax(T X, T Y)
{
	return X > Y ? X : Y;
}

template <class T>
inline T imin(T X, T Y)
{
	return X < Y ? X : Y;
}

// get the angle through the value of tan
inline double getAngle(int gradientX, int gradientY)
{
	return 180 * atan(gradientY * 1.0 / gradientX) / PI;
}

// round the pixel value
// if value > 255, then set value = 255
// if value < 0,   then set value = 0
inline int roundPixelValue(int value)
{
	return value > 255 ? 255 : (value < 0 ? 0 : value);
}

//�����(x,y)�ڿ�Ϊw,��Ϊh��ͼ������������е�����ֵ
template <class T>
inline T INDEX(T X, T Y, T Width)
{
	return (X * Width + Y) * 3;
}

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;

typedef struct tagRGBQUAD {
	BYTE    rgbBlue;				//��ɫ������
	BYTE    rgbGreen;				//��ɫ������
	BYTE    rgbRed;					//��ɫ������
	BYTE    rgbReserved;			//����������Ϊ0
} RGBQUAD, *BITMAPQUAD;

typedef struct tagBITMAPFILEHEADER {
	WORD    bfType;					//λͼ�ļ������ͣ�����Ϊ"BM"
	DWORD   bfSize;					//λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
	WORD    bfReserved1;			//λͼ�ļ������֣�����Ϊ0
	WORD    bfReserved2;			//λͼ�ļ������֣�����Ϊ0
	DWORD   bfOffBits;				//λͼ���ݵ���ʼλ�ã��������λͼ�ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ
} FILEHEADER, *BITMAPFILEHEADER;	//�ýṹռ��14���ֽ�

typedef struct tagBITMAPINFOHEADER{
	DWORD      biSize;				//���ṹ��ռ�õ��ֽ���
	DWORD      biWidth;				//λͼ�Ŀ�ȣ�������Ϊ��λ
	DWORD      biHeight;			//λͼ�ĸ߶ȣ�������Ϊ��λ
	WORD       biPlanes;			//Ŀ���豸��ƽ�������壬����Ϊ1
	WORD       biBitCount;			//ÿ�����������λ����������1(˫ɫ)��4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
	DWORD      biCompression;		//λͼѹ�����ͣ�������0(��ѹ��)��1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
	DWORD      biSizeImage;			//λͼ�Ĵ�С�����ֽ�Ϊ��λ
	DWORD      biXPelsPerMeter;		//λͼˮƽ�ֱ��ʣ�ÿ��������
	DWORD      biYPelsPerMeter;		//λͼ��ֱ�ֱ��ʣ�ÿ��������
	DWORD      biClrUsed;			//λͼʵ��ʹ�õ���ɫ���е���ɫ��
	DWORD      biClrImportant;		//λͼ��ʾ��������Ҫ����ɫ��
} INFOHEADER, *BITMAPINFOHEADER;	//�ýṹռ��40���ֽ�

typedef struct image {
	INFOHEADER infoHeader;	//λͼ��Ϣͷ
	FILEHEADER fileHeader;	//λͼ�ļ�ͷ
	RGBQUAD	   rgbQuad;		//λͼ��ɫ��
} bmpImage, *BMPIMAGE;

typedef struct fetch {
	int w;			//ͼƬ���
	int h;			//ͼƬ�߶�
	int bitCount;	//bitCountֵ
	int lenLine;	//ͼƬ��ռ�ֽ���
	long bmpSize;	//ͼƬ���ݳ���
	BYTE * data;	//ͼ������
} infoFetch, *BMPINFO;