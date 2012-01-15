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

int main(int argc,char **argv)
{
	initializeSystem(1600, 900, "green");
	//updateRouteData(int velocity, int road_angle, int distance, int turn_angle, int x1, int y1, int x2, int y2)
	int a = updateRouteData(25, 0, 2, 0, 0, 0, 0, 0);
	//printf(" Number %d", a);
	processImageData();
	a = updateRouteData(25, 0, 2, -1, 0, 0, 0, 0);
	//printf(" Number %d", a);
	processImageData();

	a = updateRouteData(25, 0, 2, 1, 0, 0, 0, 0);
	printf(" Number %d", a);

	processImageData();

	return 0;
}
