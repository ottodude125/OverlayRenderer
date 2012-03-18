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

void processTrackingData(OverlayRendererControl *control)
{

}


