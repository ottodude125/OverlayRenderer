/*
 * navit.c
 *
 *  Created on: Dec 3, 2011
 *      Author: ubuntu3
 */

#include "OverlayRendererControl.h"
#include "RouteInputAnalyzer.h"
#include "ChoicePathAlgorithm.h"
#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *file = "data.txt";

FILE *f;

int main(int argc,char **argv)
{
	initializeSystem(1440, 900, "red", 400, 300);

	f = fopen(file, "r");
	
	time_t sec;

	if(f)
	{
		int values[4];
		size_t i;
		char line[255], *ptr;
		int count = 0;
		//int angle = 90;
		
		while (fgets(line, sizeof line, f) != NULL)
		{
			//angle = angle + 5;
			while(time(NULL) - sec < 1)
			{
				count++;
			}
			count = 0;
			sec = time(NULL);
			//printf ("Seconds: %ld \n", sec);

			for(i = 0, ptr = line; i < 4; i++, ptr++)
			{
				values[i] = (int)strtol(ptr, &ptr, 10);
			}

			int a = updateRouteData(values[0], values[1], values[2], values[3]);

			while( a != 1)
			{
				a = updateRouteData(values[0], values[1], values[2], values[3]);
			}

			processImageData(1);

		}
		
	}
	else
		printf("cannot open file");
	
	fclose(f);
	
	return 0;
}

