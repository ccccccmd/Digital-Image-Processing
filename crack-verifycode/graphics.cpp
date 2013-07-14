#include "graphics.h"
#include "canny.h"
#include "io.h"
#include "common.h"
#include "repixels.h"

//��bmp�ļ�
void Open(int dt_change, int do_change, char* path, BMPIMAGE image, BMPINFO info)
{
	int rgb;
	long bmpSize;

	FILE * pFile;

	unsigned char* cdata = 0;

	pFile = fopen(path, "rb");

	if(pFile == NULL)
	{
		printf("file not exist.\n");
	}
	else
	{
		//��ȡbmp�ļ�����
		fseek(pFile, 0, SEEK_END);
		bmpSize = ftell(pFile);
		rewind(pFile);

		fread(&(image->fileHeader), 1, FILEHEADERSIZE, pFile);//��ȡbmp�ļ�ͷ
		fread(&(image->infoHeader), 1, INFOHEADERSIZE, pFile);//��ȡbmp��Ϣͷ

		info->w = image->infoHeader.biWidth; //��ȡͼƬ���
		info->h = image->infoHeader.biHeight;//��ȡͼƬ�߶�
		info->bitCount = image->infoHeader.biBitCount; //��ȡbiBitCountֵ

		switch(image->infoHeader.biBitCount)
		{
		case 1:
			rgb = 2;
			break;
		case 4:
			rgb = 16;
			break;
		case 8:
			rgb = 256;
			break;
		case 24:
			rgb = 0;
			break;
		default:
			break;
		}

		fread(&(image->rgbQuad), rgb * sizeof(RGBQUAD), 1, pFile);
		if(image->infoHeader.biBitCount == 24) //���ͼƬΪ���ɫ
		{
			info->bmpSize = bmpSize - FILEHEADERSIZE - INFOHEADERSIZE;	//����bmp�ļ�ͼ�����ݳ���
			info->data = (BYTE *)malloc(sizeof(BYTE) * (bmpSize - FILEHEADERSIZE - INFOHEADERSIZE));
			fread(info->data, 1, bmpSize - FILEHEADERSIZE - INFOHEADERSIZE, pFile); //��ȡbmpͼ�����ݲ���

			repixels(dt_change, do_change, info->w, info->h, info->bmpSize, info->data);
		}

		//����ÿ�е����ݳ���
		info->lenLine = ((image->infoHeader.biBitCount * image->infoHeader.biWidth) + 31) / 8;

		fclose(pFile);

		printf("Open File Success!\n\n");
	}
}

//����Ϊbmp�ļ�(biBitCountΪ24)
void Save(char* path, BMPIMAGE image, BMPINFO info)
{
	FILE * pFile;
	long lineLength;
	long totalLength;

	int bytePerpixel = 3;
	long width = info->w;
	long height = info->h;

	unsigned char header[BMPHEADERSIZE] = {
		0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0,
		40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0};

	long fileSize = (long)(info->w) * (long)(info->h) * 3 + 54;

	header[2] = (unsigned char)(fileSize & 0x000000ff);
	header[3] = (fileSize >> 8) & 0x000000ff;
	header[4] = (fileSize >> 16) & 0x000000ff;
	header[5] = (fileSize >> 24) & 0x000000ff;

	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) & 0x000000ff;
	header[20] = (width >> 16) & 0x000000ff;
	header[21] = (width >> 24) & 0x000000ff;

	header[22] = height & 0x000000ff;
	header[23] = (height >> 8) & 0x000000ff;
	header[24] = (height >> 16) & 0x000000ff;
	header[25] = (height >> 24) & 0x000000ff;

	pFile = fopen(path, "wb");
	if(pFile == NULL)
	{
		printf("Error.\n");
	}
	else
	{
		fwrite(header, sizeof(unsigned char), 54, pFile);

		lineLength = width * bytePerpixel;	//ÿ�����ݳ��ȴ���Ϊͼ���ȳ���ÿ���ص��ֽ���

		while(lineLength % 4 != 0)			//����lineLengthʹ��Ϊ4�ı���
		{
			lineLength++;
		}

		totalLength = lineLength * height;	//�����ܳ� = ÿ�г��� * ͼ��߶�

		fwrite(info->data, sizeof(unsigned char), (size_t)totalLength, pFile);
		fclose(pFile);

		printf("Save File Success!\n\n");
	}
}

void RemovePixels()
{
	int i = 10;
	int j = 1;
	char* openFile = PROCESS_FILE_OPEN_PATH;
	vector<int> nums;

	for(i = 0; i <= DT_CHANGE_WHITE_TIMES; i++)
	{
		for(j = 1; j <= DO_CHANGE_WHITE_TIMES; j++)
		{
			char saveFile[PathLength] = PROCESS_REMOVEBLA_PATH;

			nums.clear();
			nums.push_back(i);
			nums.push_back(j);

			spellSaveFilePath2(0, nums, saveFile);

			BMPIMAGE image = (bmpImage*)malloc(sizeof(bmpImage));
			BMPINFO info = (infoFetch*)malloc(sizeof(infoFetch));

			printf("Open the bmp file...\n");
			Open(i, j, openFile, image, info);

			switch(info->bitCount)
			{
			case 24:
				printf("Save the bmp file into file %s\n", saveFile);
				Save(saveFile, image, info);
				break;
			default:
				break;
			}
		}
	}
}

int main()
{
	printf("/*****************File Begin*****************/\n");

	RemovePixels();

	printf("/*****************File End*****************/\n");

	return 0;
}
