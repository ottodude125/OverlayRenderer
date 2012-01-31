/*
 * navit.c
 *
 *  Created on: Dec 3, 2011
 *      Author: ubuntu3
 */

#include "RouteInputAnalyzer.h"
#include "OverlayRendererControl.h"
#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *file = "data.csv";
FILE *f;

int main(int argc,char **argv)
{
	initializeSystem(1440, 900, "green", 400, 300);
	
	f = fopen(file, "r");
	
	if(f)
	{
		int values[4];
		size_t i;
		char line[255], *ptr;
		
		while (fgets(line, sizeof line, f) != NULL)
		{
			for(i = 0, ptr = line; i < 4; i++, ptr++)
			{
				values[i] = (int)strtol(ptr, &ptr, 10);
			}
			
			int a = updateRouteData(values[0], values[1], values[2], values[3]); // left
			processImageData();
			printf(" Number %d\n", a);
			//printf(" Number %d", values[0]);
			//printf(" Number %d", values[1]);
			//printf(" Number %d", values[2]);
			//printf(" Number %d\n", values[3]);
			
			
		}
		
	}
	else
		printf("you suck balls!!!");
	
	//updateRouteData(int velocity, int road_angle, int distance, int turn_angle)
	
	/*	int a = updateRouteData(25, 0, 200, -1); // left
	 processImageData();
	 
	 a = updateRouteData(25, 0, 2000, 1); // straight
	 processImageData();
	 
	 a = updateRouteData(25, 0, 200, 1); // right
	 printf(" Number %d", a);
	 processImageData();*/
	
	fclose(f);
	
	return 0;
}
