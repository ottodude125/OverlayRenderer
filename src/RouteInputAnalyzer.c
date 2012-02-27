/*
 * RouteInputAnalyzer.c
 *
 *  Created on: Dec 3, 2011
 *      Author: Jonathan Katon
 *
 *      RouteInputAnalyzers purpose is to analyze the data obtained from Mapping and Routing. There are four pieces of data obtained:
 *			1) velocity of vehicle (meters/second)
 *      	2) road angle in +/- 90 degrees with respect to vehicle
 *      	3) distance before next turn is made in meters
 *      	4) angle of next turn in +/- 180 degrees with respect to road currently on
 *
 *      This data is processed to determine a pixel box in which the arrow should be drawn
 *
 */

#include "RouteInputAnalyzer.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


// method for establishing generic right turn pixel box
static void genericRightTurnBox(OverlayRendererControl *controller)
{
	controller->x1 = controller->width / 2;
	controller->y1 = controller->height / 2 - controller->height / 10;
	controller->x2 = controller->width - 100;
	controller->y2 = controller->height / 2;

	controller->type_of_arrow = 3;
	
}

// method for establishing generic left turn pixel box
static void genericLeftTurnBox(OverlayRendererControl *controller)
{
	controller->x1 = 100;
	controller->y1 = controller->height / 2 - controller->height / 10;
	controller->x2 = controller->width / 2;
	controller->y2 = controller->height / 2;

	controller->type_of_arrow = 1;
}

// method for establishing generic straight turn pixel box
static void genericStraightRoadBox(OverlayRendererControl *controller)
{
	controller->x1 = controller->width / 2 - controller->width / 20;
	controller->y1 = 100;
	controller->x2 = controller->width / 2 + controller->width / 20;
	controller->y2 = controller->height - 50;

	controller->type_of_arrow = 2;
	
}

// method for establishing generic right turn pixel box
static void rightTurnBox(OverlayRendererControl *controller)
{
	float pi = M_PI;

	float time = controller->distance/controller->velocity + .1;

	controller->x1 = (int) (controller->width/2);
	controller->y1 = (int) (controller->height - controller->height * .2 -  ( time * controller->height/16));
	controller->x2 = (int) (controller->width - (controller->width/30 *  time));
	controller->y2 = (int) (controller->height - controller->height * .2 - ( time * (controller->height)/17)) + (15/(time + 2))*(controller->height/90);
	controller->x3 = controller->x1;
	controller->y3 = controller->y2;
	controller->x4 = controller->x2;
	controller->y4 = controller->y1;

/*	float length = sqrt(pow((controller->x2 - controller->x1),2) + pow((controller->y2 - controller->y1),2));
	float theta = atan(abs(controller->y2 - controller->y1)/(float)abs(controller->x2 - controller->x1));

	float turnrad = (450 - controller->turn_angle) * ((2*pi)/360);
	int tempx2 = (int) (controller->x1 + length * cos(turnrad - theta));
	int tempy2 = (int) (controller->y1 - length * sin(turnrad - theta));

	int tempx3 = (int) (controller->x1 - length * cos(turnrad + (pi / 2)));
	int tempy3 = (int) (controller->y1 - length * sin(turnrad + (pi / 2)));

	int tempx4 = (int) (controller->x1 + length * cos(turnrad));
	int tempy4 = (int) (controller->y1 - length * sin(turnrad));

	//printf("R  length: %f", length);
	printf("R theta: %f", theta);
	printf("   turn: %f", turnrad);
	printf("   cos: %f", cos(turnrad - theta));
	printf("   sin: %f", sin(turnrad - theta));
	printf("   x2: %d", controller->x2);
	printf("   tx2: %d", tempx2);
	printf("   y2: %d", controller->y2);
	printf("   ty2: %d", tempy2);
	printf("   x3: %d", controller->x3);
	printf("   tx3: %d", tempx3);
	printf("   y3: %d", controller->y3);
	printf("   ty3: %d", tempy3);
	printf("   x4: %d", controller->x4);
	printf("   tx4: %d", tempx4);
	printf("   y4: %d", controller->y4);
	printf("   ty4: %d\n", tempy4);

	controller->x2 = tempx2;
	controller->y2 = tempy2;
	controller->x3 = tempx3;
	controller->y3 = tempy3;
	controller->x4 = tempx4;
	controller->y4 = tempy4;*/

	controller->type_of_arrow = 3;
}

// method for establishing generic left turn pixel box
static void leftTurnBox(OverlayRendererControl *controller)
{
	float pi = M_PI;

	float time = controller->distance/controller->velocity + .1;

	controller->x1 = (int) (controller->width/30 *  time);
	controller->y1 = (int) (controller->height - controller->height * .2 -  ( time * controller->height/16));
	controller->x2 = (int) (controller->width/2);
	controller->y2 = (int) (controller->height - controller->height * .2 - ( time * (controller->height)/17)) + (15/(time + 2))*(controller->height/90);
	controller->x3 = controller->x1;
	controller->y3 = controller->y2;
	controller->x4 = controller->x2;
	controller->y4 = controller->y1;

/*	float length = sqrt(pow((controller->x2 - controller->x1),2) + pow((controller->y2 - controller->y1),2));
	float theta = atan(abs(controller->y2 - controller->y1)/(float)abs(controller->x2 - controller->x1));

	float turnrad = (90 + abs(controller->turn_angle)) * ((2*pi)/360);
	int x3 = (int) (controller->x2 + length * cos(turnrad - theta));
	int y3 = (int) (controller->y2 - length * sin(turnrad - theta));

	printf("L  length: %f", length);
	printf("   theta: %f", theta);
	printf("   turn: %f", turnrad);
	printf("   cos: %f", cos(turnrad - theta));
	printf("   sin: %f", sin(turnrad - theta));
	printf("   x1: %d", controller->x1);
	printf("   x3: %d", x3);
	printf("   y1: %d", controller->y1);
	printf("   y3: %d\n", y3);

	controller->x2 = tempx2;
	controller->y2 = tempy2;
	controller->x3 = tempx3;
	controller->y3 = tempy3;
	controller->x4 = tempx4;
	controller->y4 = tempy4;*/

	controller->type_of_arrow = 1;
}

// method for establishing  straight turn pixel box
static void straightRoadBox(OverlayRendererControl *controller)
{
	// the straight arrow is derived as a function of projector width and height
	controller->x1 = (int) (controller->width / 2 - controller->width / 30);
	controller->y1 = (int) (controller->height - controller->height/3);
	controller->x2 = (int) (controller->width / 2 + controller->width / 30);
	controller->y2 = (int) (controller->height - controller->height / 20);
	controller->x3 = controller->x1;
	controller->y3 = controller->y2;
	controller->x4 = controller->x2;
	controller->y4 = controller->y1;

	controller->type_of_arrow = 2;
}

// public method which is called whenever a new pixel box for a generic arrow is to be acquired from mapping data
void processGenericNavData(OverlayRendererControl *control)
{
	// pick which arrow to draw based on turn angle and distance to next turn
	if(control->turn_angle < 0 && control->distance < 500)
		genericLeftTurnBox(control);
	else if(control->turn_angle > 0 && control->distance < 500)
		genericRightTurnBox(control);
	else
		genericStraightRoadBox(control);
}

// public method which is called whenever a new pixel box for an arrow is to be acquired from mapping data
void processNavData(OverlayRendererControl *control)
{
	// int velocity; // velocity in meters/seconds
	// int road_angle; // road angle in +/- 90 degrees with respect to vehicle
	// int distance; // distance before next turn is made in meters
	// int turn_angle; // angle of next turn in +/- 180 degrees with respect to road currently on
	
	float time = control->distance / control->velocity;
	
	/* If time is greater than x then draw arrow on current road.
	 * If time is less than x and turn angle less than 0 draw arrow pointing left
	 * If time is less than x and turn angle greater than 0 draw arrow pointing right
	 */
	
	if(time >= 14)
	{
		straightRoadBox(control);
	}
	else if(time < 14 && control->turn_angle < 0)
	{
		leftTurnBox(control);
	}
	else if(time < 14 && control->turn_angle > 0)
	{
		rightTurnBox(control);
	}
	else
	{
		printf("There has been a system error.\n");
	}
}




