/*
 * OverlayRendererControl.c
 *
 *  Created on: Dec 3, 2011
 *      Author: Jonathan Katon
 *
 *      OverlayRendererControl is responsible for managing the OverlayRenderer. It does the following:
 *      	1) Contains a struct which holds
 *      		a) settings of the projection to be displayed
 *      			i) height * width of projection
 *      			ii) color of projection
 *      			iii) position of text to be displayed
 *      		b) four variables to hold pixel numbers representing the top left and bottom right corners of a box
 *      		c) variables to hold data from MappingAndRouting
 *      			i) velocity of vehicle
 *      			ii) angle of road
 *      			iii) distance to next turn
 *      			iv) angle of next turn
 *      		d) variables to hold data from RoadTracking
 *      		e) various variables needed to ensure smooth operation
 *      			i) time since last mapping update
 *      			ii) is data currently in use
 *      			iii) text for speed and distance to be displayed
 *      			f) array to hold pixel values for arrow to be displayed
 *      		2) Public methods to
 *      			a) initialize OverlayRenderer system
 *      			b) change color of projection
 *      			c) update mapping and routing route data
 *      			d) process data to produce image
 *      		3) Private methods to
 *      			a) set position of text in display
 *      			b) set text to display (mph and distance to turn)
 *
 */

#include "RouteInputAnalyzer.h"
#include "OverlayRendererControl.h"
#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

typedef char * string;

static OverlayRendererControl controller;
static int initialized = 0;

// static method to set pixel position of text in projection
static void setInfoTextPosition()
{
	int a = controller.width;
	int b = controller.height;
	// set position of speed -> [0][] text [0][x] and [0][y]
	controller.textPosition[0][0] = a/2 - a*.2;
	controller.textPosition[0][1] = b - b*.1;
	
	// set position of distance to next turn -> [1][] text [1][x] and [1][y]
	controller.textPosition[1][0] = a/2 - a*.2;
	controller.textPosition[1][1] = b - b*.08;
}

// static method to set the speed and distance text to be placed on the windshield
static void setTextToDisplay()
{
	// calculate mph of vehicle
	float mph = controller.velocity * 2.2369363;
	// calculate feet to next turn
	float feet = controller.distance * 3.2808399;
	
	// set text of speed
	sprintf(controller.speed_text, "%f", mph); // convert speed to string and place in array
	strcat(controller.speed_text, " mph"); // Concatenate text onto it
	
	// if feet less than 1000 display distance to next turn in feet
	if(feet < 1000 )
	{
		// set text of distance to next turn
		sprintf(controller.distance_to_next_turn_text, "%f", feet); // convert distance to string and place in array
		strcat(controller.distance_to_next_turn_text, " feet before next turn"); // Concatenate text onto it
	}
	else // otherwise display distance to next turn in miles
	{
		// calculate the miles to next turn
		float miles = feet/5280;
		
		// set text of distance to next turn
		sprintf(controller.distance_to_next_turn_text, "%f", miles); // convert distance to string and place in array
		strcat(controller.distance_to_next_turn_text, " miles before next turn"); // Concatenate text onto it
	}
}

// method to initialize  the overlay rendering system - projector size and color
void initializeSystem(int xdimension, int ydimension, string projection_color, int camerax, int cameray)
{
	controller.width = xdimension;
	controller.height = ydimension;
	strcpy(controller.color, projection_color);
	controller.camera_x = camerax;
	controller.camera_y = cameray;
	
	int i, j = 0;
	for(i = 0; i < camerax; i++)
	{
		for(j = 0; j < cameray; j++)
		{
			controller.boundary_array[i][j] = 0;
		}
	}
	
	controller.map_data_up_to_date = 0;
	controller.velocity = 0;
	controller.road_angle = 0;
	controller.distance = 0;
	controller.turn_angle = 0;
	controller.x1 = 0;
	controller.y1 = 0;
	controller.x2 = 0;
	controller.y2 = 0;
	controller.type_of_arrow = 0;
	controller.time_since_mapping_update = 0;
	
	// set text and position of speed/distance
	setInfoTextPosition();
	
	initialized = 1;
}

// method to change the color of the projection
void setProjectionColor(char projection_color[])
{
	strcpy(controller.color, projection_color);
}

// method to update the current route information returns 1 for successful, returns 0 if failed
int updateRouteData(int velocity, int road_angle, int distance, int turn_angle)
{
	if(controller.map_data_up_to_date == 0)
	{
		controller.map_data_up_to_date = 1;
		controller.velocity = velocity;
		controller.road_angle = road_angle;
		controller.distance = distance;
		controller.turn_angle = turn_angle;
		controller.time_since_mapping_update = time(NULL);
		setTextToDisplay();
		
		return 1;
	}
	else
		return 0;
}

// method to update the current route information returns 1 for successful, returns 0 if failed
int updateRoadTrackingData(int edges[1600][900])
{
	if(controller.road_data_up_to_date == 0)
	{
		controller.road_data_up_to_date = 1;
		controller.time_since_road_update = time(NULL);
		
		int i, j = 0;
		for(i = 0; i < controller.camera_x; i++)
		{
			for(j = 0; j < controller.camera_y; j++)
			{
				controller.boundary_array[i][j] = (int) edges[i][j];
			}
		}
		
		return 1;
	}
	else
		return 0;
}

// method responsible for controlling the RouteInputAnalyzer, ChoicePath, and ImageCreation
void processImageData()
{
	// initialize program if it has not been done yet
	if(initialized == 0)
	{
		char projection_color[] = "red";
		initializeSystem(800, 600,projection_color, 400, 300);
	}
	
	// TODO: Need to figure out how to create a thread here that will constantly run until the program exits
	while(controller.map_data_up_to_date)
	{
		// If new data has been received then process it
		if(controller.map_data_up_to_date)
		{
			// calculated pixels for arrow placement
			processNavData(&controller);
			
			// create arrow and draw it to screen
			drawImage(&controller);
			
			// reset control so new data can be written
			controller.map_data_up_to_date = 0;
			
			// Display Image
			IplImage* img;
			cvNamedWindow( "Projection", CV_WINDOW_NORMAL );
			cvSetWindowProperty("Projection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
			img = cvLoadImage( "arrow_test.jpg", CV_LOAD_IMAGE_UNCHANGED );
			cvShowImage("Projection", img);
			cvWaitKey(150);
		}
	}
	
}





