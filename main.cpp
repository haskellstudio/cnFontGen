#include "freetype/ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "lodepng/lodepng.h"
#include <stdlib.h>
#include <stdio.h>

#include "windows.h"
typedef struct
{
	FT_Library lib;
	FT_Face face;
}ftInfo;

int paddingSpace = 30;
int paddingHeight = 8;

int clamp(int input, int min, int max)
{
	return input > max ? max : input < min ? min : input;
}

int main(int argc, char **argv)
{
	ftInfo ft;

	int i = 0;
	int j = 0;
	int k = 0;

	int accumWidth = 0;

	int maxHei = 0;
	int biggestBottom = 0;
	int bestMiddle = 0;
	int averageTop = 0;
	int averageBottom = 0;
	int maxTop = 0;
	int minTop = 0;
	int xInc = 0;
	unsigned int* outputBuffer;
	FT_GlyphSlot g;

	if (FT_Init_FreeType(&ft.lib))
	{
		printf("Failed to initialize freetype");
	}

	if (argc == 6)
	{
		if (FT_New_Face(ft.lib, argv[1], 0, &ft.face))
		{
			printf("Problem loading fontfile %s", argv[1]);
		}
	}
	else
	{
		if (FT_New_Face(ft.lib, "arial.ttf", 0, &ft.face))
		{
			printf("Problem loading fontfile %s", argv[1]);
		}
	}




	// arg1 ÿ���ֵĳ��� 
	// arg2 ����texture�ĳ��� ��
	// arg3 ��ʼ�ַ�
	// arg4 �����ַ�


	int startChar = '1';
	int endChar = '4';
	


	int iw = 128;  // ÿ���ֵĳ��� 
	int ih = iw;
	



	int _width = 256;  // ��������ĳ���
	int _height = 256;

	if (argc == 6)
	{
		char * end = NULL;
		iw = ih =		   strtol(argv[1+1], &end, 10);
		_width = _height = strtol(argv[2 + 1], &end, 10);


		if(argv[3 + 1][1] == 'x')
			startChar = strtol(argv[3 + 1], &end, 16 );
		else 
			startChar = strtol(argv[3 + 1], &end, 10);

		if (argv[4 + 1][1] == 'x')
			endChar = strtol(argv[4 + 1], &end, 16);
		else
			endChar = strtol(argv[4 + 1], &end, 10);

		
	}

	int charNum = endChar - startChar + 1;
	int fontHeight = iw;//35;  2048 * 2048

	if (_width % iw != 0)
	{
		MessageBox(0, "_width % iw != 0", 0, 0);
		exit(-1);
	}
	
	if (_height % ih != 0)
	{
		MessageBox(0, "_width % iw != 0", 0, 0);
		exit(-1);
	}

	if ( iw != ih)
	{
		MessageBox(0, "_width % iw != 0", 0, 0);
		exit(-1);
	}

	if ((_width / iw) * (_height / ih) != charNum)
	{
		MessageBox(0, "wrong whole layout ", 0, 0);
		exit(-1);
	}



	//test
	int clam = '_';

	int * buf = 0;
	//buf = malloc(ft.face->glyph->bitmap.rows * ft.face->glyph->bitmap.width * sizeof(int));
	buf = malloc(_height * _width * sizeof(int));

	FT_Set_Pixel_Sizes(ft.face, 0, fontHeight);
	g = ft.face->glyph;

	int charPerRow = _width / iw;

	for (int l = startChar; l <= endChar; l++)
	{
		FT_Load_Char(ft.face, l, FT_LOAD_RENDER);

		int charIndex = l - startChar;
		if (charIndex == 0xe)
		{
			charIndex = l - startChar;
			__asm
			{
				mov eax, eax
			}
		}
		float aspect = 1.0;
		if (ft.face->glyph->bitmap.rows > ft.face->glyph->bitmap.width  && 
			ft.face->glyph->bitmap.rows > ih
			)
		{
			aspect = (float)ft.face->glyph->bitmap.rows / (float)ih;
		}

		if (ft.face->glyph->bitmap.rows < ft.face->glyph->bitmap.width  &&
			ft.face->glyph->bitmap.width > iw
			)
		{
			aspect = (float)ft.face->glyph->bitmap.width / (float)iw;
		}

		for (i = 0; i < ih  &&  i < ft.face->glyph->bitmap.rows; i++)
		{
			for (j = 0; j < iw && j < ft.face->glyph->bitmap.width; j++)
			{                         //a b g r
				unsigned int bytes = 0xFFff0000;
				//0xFFff0000 blue  0xFF00ff00 green  0xFF0000ff red
			
				int isOver = 0;
				int ajusti = (int)( (float)i * aspect);
				if (ajusti > ft.face->glyph->bitmap.rows)
				{
					isOver = 1;
					ajusti = ft.face->glyph->bitmap.rows;
				}
					


				int ajustj = (int)((float)j * aspect);

				if (ajustj > ft.face->glyph->bitmap.width)
				{
					ajustj = ft.face->glyph->bitmap.width;
					isOver = 1;
				}


				k = /*i*/ajusti*g->bitmap.width + /*j*/ajustj;

				int pastRow = charIndex / charPerRow;
				int pastCol = charIndex % charPerRow;
				int itowrite = pastRow*_width*ih + i*_width + j + pastCol * iw;
			 

				unsigned int brightness = (unsigned int)g->bitmap.buffer[k];
				if (isOver)
					brightness = 0;
				brightness = brightness << 8 | brightness << 16 | brightness << 24 | brightness;
				if ((unsigned int)g->bitmap.buffer[k] == 0xff)
				{
					buf[itowrite] = 0xffffffff;
				}
				else  if ((unsigned int)g->bitmap.buffer[k] == 0x0)
				{
					buf[itowrite] = 0x0;
				}
				else
				{
					buf[itowrite] = 0xFFff0000; // blue
				}

				buf[itowrite] = brightness;
			}
		}
	}
	

	lodepng_encode32_file("clam.png", (char*)buf, _width, _height);


	free(buf);
	return;










	accumWidth += paddingSpace;
	for (i = 33; i < 127; ++i)
	{
		FT_Load_Char(ft.face, i, FT_LOAD_RENDER);
		accumWidth += g->bitmap.width + paddingSpace;

		if ((g->bitmap.rows - g->bitmap_top) + g->bitmap_top > maxTop)
		{
			maxTop = (g->bitmap.rows - g->bitmap_top) + g->bitmap_top;
		}

		if (g->bitmap_top < 0)
		{
			if ((g->bitmap_top + (-g->bitmap.rows)) < minTop)
			{
				minTop = g->bitmap_top + (-g->bitmap.rows);
			}
		}
	}
	accumWidth += paddingSpace;

	bestMiddle = (maxTop + minTop) / 2;
	maxHei = (maxTop - minTop) + paddingHeight;

	outputBuffer = malloc(accumWidth*maxHei * 4);
	memset(outputBuffer, 0x00, accumWidth*maxHei * 4);
	//accumWidth = 0;

	xInc += paddingSpace;

	for (i = 33; i < 127; ++i)
	{
		FT_BBox  bbox;
		FT_Glyph gly;
		FT_Load_Char(ft.face, i, FT_LOAD_RENDER);

		printf("%c = %i \n", (char)i, g->advance.x >> 6);

		for (j = 0; j < g->bitmap.rows; ++j)
		{
			for (k = 0; k < g->bitmap.width; ++k)
			{
				unsigned int bytes = 0xFFFFFFFF;
				bytes = (unsigned int)g->bitmap.buffer[j*g->bitmap.width + k];
				bytes = (bytes << 24) | 0xFFFFFF;
				outputBuffer[(((j + (maxHei / 2)) - g->bitmap_top + bestMiddle)*accumWidth) + k + xInc] = bytes;
			}
		}



		FT_Get_Glyph(g, &gly);
		FT_Glyph_Get_CBox(gly, FT_GLYPH_BBOX_PIXELS, &bbox);



		outputBuffer[xInc - clamp((paddingSpace / 2) - 1, bbox.xMin, (paddingSpace / 2) - 1)] = 0xFFFF0000;
		outputBuffer[xInc - bbox.xMin] = 0xFF000000;
		outputBuffer[xInc + bbox.xMax] = 0xFF000000;
		outputBuffer[xInc + clamp(g->bitmap.width + (paddingSpace / 2) - 1, bbox.xMax + 1, g->bitmap.width + (paddingSpace / 2) - 1)] = 0xFFFF0000;

		printf("\t xMin: %i xMax: %i \n \t width: %i \n", bbox.xMin, bbox.xMax, g->bitmap.width);

		xInc += g->bitmap.width + paddingSpace;
	}

	lodepng_encode32_file("harbage.png", (char*)outputBuffer, accumWidth, maxHei);

	system("PAUSE");
}