#pragma once
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define F "\e[38;5;%dm"
#define B "\e[48;5;%dm"

int _C(int g)
{
	static const int v[7] = {0,235,239,243,247,251,255};
	return v[g];
}

struct _in
{
	unsigned char **b, *e;
	unsigned int w, h;
};

void* draw(void *input)
{
	struct _in *_input = input;
	unsigned char **buffer = _input->b;
	const unsigned int width = _input->w;
	const unsigned int height = _input->h;
	unsigned char *need_to_exit = _input->e;
	while(*need_to_exit == 0)
	{
		unsigned char *b = *buffer;
		printf("\e[1;1H\e[2J");
		for (int i = 0; i < height; i += 2)
		{
			for (int j = 0; j < width; j++)
			{
				// bit format
				// 0000   000      0
				//unused  gray  enabled
				int tile = (b[i*width+j]&1)<<1
					| (b[(i+1)*width+j]&1);
				uint8_t col1 = (b[i*width+j]&14)>>1;
				uint8_t col2 = (b[(i+1)*width+j]&14)>>1;
				/*switch (tile)
				{
					case 0:
						printf(" ");
						break;
					case 1:
						printf("▄");
						break;
					case 2:
						printf("▀");
						break;
					case 3:
						printf("█");
						break;
				}*/
				switch (tile)
				{
					case 0:
						printf(F B" ",
						_C(0), _C(0));
						break;
					case 1:
						printf(F B"▀",
						_C(0), _C(col2));
						break;
					case 2:
						printf(F B"▀",
					   	_C(col1), _C(0));
						break;
					case 3:
						printf(B F"▀",
						_C(col2), _C(col1));
						break;
				}
			}
			printf("\r\n");
		}
		usleep(16666);
	}
	return NULL;
}

/*void initDrawEngine(unsigned char **buffer, unsigned int w, unsigned int h)
{
	pthread_t t;
	struct _in i = {buffer, w, h};
	pthread_create(&t, NULL, draw, &i);
}*/

#define init(buffer, e, w, h) pthread_t t_id; struct _in i = {buffer, e, w, h}; pthread_create(&t_id, NULL, draw, &i)
