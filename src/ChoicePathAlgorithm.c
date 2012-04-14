/*
 * ChoicePathAlgorithm.c
 *
 *  Created on: Jan 31, 2012
 *      Author: Jonathan Katon
 *
 *      ChoicePathAlgorithms purpose is to increase the precision of the pixel box created by
 *      RouteInputAnalyzer. This is done by analyzing the data from RoadTracking. There is one piece
 *      of data which is obtained from RoadTracking:
 *      	1) NxM matrix where N and M are the width and height of the camera input
 *
 *
 */

#include "ChoicePathAlgorithm.h"


// 1) analyze points and figure out which points matter
// 2) determine what points create the rectangle over the turn and create that rectangle
// 3) compare my rectangle to that rectangle
// 4) based on what I see make necessary adjustments

// Analyze points given and determine which ones matter
static int analyzePoints(OverlayRendererControl *controller)
{
	if(controller->turn_angle < 0)
		return 1;
	else return 3;
}

// using results from analyzePoints obtain pixel box covering turn
static int* assembleBox(OverlayRendererControl *controller, int start)
{
	int* box[2][4];
	int i, j = 0;

	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 4; j++)
		{
			box[i][j] = 0;
		}

	}

	if(start == 1)
	{
		box[0][0] = controller->boundary_array[0][3];
		box[1][0] = controller->boundary_array[1][3];
		box[0][1] = controller->boundary_array[0][4];
		box[1][1] = controller->boundary_array[1][4];
		box[0][2] = controller->boundary_array[0][1];
		box[1][2] = controller->boundary_array[1][1];
		box[0][3] = controller->boundary_array[0][2];
		box[1][3] = controller->boundary_array[1][2];
	}
	else
	{
		box[0][0] = controller->boundary_array[0][3];
		box[1][0] = controller->boundary_array[1][3];
		box[0][1] = controller->boundary_array[0][4];
		box[1][1] = controller->boundary_array[1][4];
		box[0][2] = controller->boundary_array[0][5];
		box[1][2] = controller->boundary_array[1][5];
		box[0][3] = controller->boundary_array[0][6];
		box[1][3] = controller->boundary_array[1][6];

	}
	return &(box[0][0]);
}

// compare original pixel box with that created in assembleBox and make necessary adjustments
static void adjustPixelBox(OverlayRendererControl *controller, int* turnbox[0][0])
{

}


// Process the road tracking data in turn making necessary adjustments to previously created arrow
void processTrackingData(OverlayRendererControl *control)
{
	int start = analyzePoints(control);
	int* turnBox = assembleBox(control, start);
	adjustPixelBox(control, turnBox);
}


























