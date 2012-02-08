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
#include "OverlayRendererControl.h"
#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
	/*
	 * First box created should be at top of display and be a tiny arrow.
	 * As distance/velocity decreases the box should move down the screen
	 * and the arrow should increase in size
	 */
	int time = controller->distance/controller->velocity;

	controller->x1 = (int) ((controller->width)/2);
	controller->y1 = (int) ((controller->height) - ( time * (controller->height)/16));
	controller->x2 = (int) ((controller->width) - (controller->width)/2.5 + ( time * (controller->width)/12));
	controller->y2 = (int) ((controller->height) - ( time * (controller->height)/18));

	controller->type_of_arrow = 3;
}

// method for establishing generic left turn pixel box
static void leftTurnBox(OverlayRendererControl *controller)
{
	int time = controller->distance/controller->velocity;

	controller->x1 = 100 + (time * controller->width/16);
	controller->y1 = controller->height - ( time * (controller->height)/16);
	controller->x2 = controller->width/2;
	controller->y2 = (controller->height) - ( time * (controller->height)/18);

	controller->type_of_arrow = 1;
}

// method for establishing  straight turn pixel box
static void straightRoadBox(OverlayRendererControl *controller)
{
	controller->x1 = controller->width / 2 - controller->width / 20;
	controller->y1 = 100;
	controller->x2 = controller->width / 2 + controller->width / 20;
	controller->y2 = controller->height - 50;
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
	
	if(time > 12)
	{
		straightRoadBox(control);
	}
	else if(time < 12 && control->turn_angle < 0)
	{
		leftTurnBox(control);
	}
	else if(time < 12 && control->turn_angle > 0)
	{
		rightTurnBox(control);
	}
	else
	{
		printf("There has been a system error. Please drive off the next available cliff. Thank you have a nice day!!");
	}
}




