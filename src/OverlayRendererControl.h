/*
 * OverlayRendererControl.h
 *
 *  Created on: Dec 3, 2011
 *      Author: Jonathan Katon
 *
 *	The header file contains a struct which will contain all the variables required for the
 *	OverlayRendering system. These variables control the projector along with hold values
 *	passed by MappingAndRouting and  RoadTracking System
 */

#ifndef OVERLAYRENDERERCONTROL_H_
#define OVERLAYRENDERERCONTROL_H_

#include "RouteInputAnalyzer.h"
#include "ChoicePathAlgorithm.h"
#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct
	{
		// projector and OverlayRendering setup parameters
		int width; // width of projection
		int height; // height of projection
		char color[40]; // color of projection
		int textPosition[2][2]; // (x,y) position of speed and turn distance text
		int camera_x; // width of camera
		int camera_y; // height of camera
		
		// pixel box range for guess of road location
		int x1;
		int y1;
		int x2;
		int y2;
		int x3;
		int y3;
		int x4;
		int y4;
		int type_of_arrow; // 1 left, 2 straight, 3 right
		
		// calculated pixels for arrow placement
		int triangle[3][2];
		int square[4][2];
		
		// Text to display
		char speed_text[40]; // text of speed to be displayed on window
		char distance_to_next_turn_text[40]; // text of distance to next turn to be displayed on window
		
		// mapping data set by MappingAndRouting
		int velocity; // velocity in meters/seconds
		int road_angle; // road angle in +/- 90 degrees with respect to vehicle
		int distance; // distance before next turn is made in meters
		int turn_angle; // angle of next turn in +/- 180 degrees with respect to road currently on
		
		// mapping data set by OverlayControl
		time_t time_since_mapping_update;
		int map_data_up_to_date; // 1 for yes, 0 for no
		
		// roadway boundaries data set by RoadTracking
		int boundary_array[6][2]; // matrix of 6 points indicating edge conditions
		
		// roadway boundaries data set by OverlayControl
		time_t time_since_road_update;
		int road_data_up_to_date; // 1 for yes, 0 for no
		
	} OverlayRendererControl;


// method to initialize  the overlay rendering system - projector size and color
void initializeSystem(int xdimension, int ydimension, char projection_color[], int camera_X, int camera_Y);

// method to change the color of the projection
void setProjectionColor(char projection_color[]);

// method to update the current route information
int updateRouteData(int velocity, int road_angle, int distance, int turn_angle);

// method responsible for controlling the RouteInputAnalyzer, ChoicePath, and ImageCreation
void processImageData();

OverlayRendererControl controller;


#endif /* OVERLAYRENDERERCONTROL_H_ */
