/*
 * ImageCreation.c
 *
 *  Created on: Dec 3, 2011
 *      Author: Jonathan Katon
 *
 *      ImageCreation is responsible for creating the final graphic which will be displayed. It does the following:
 *      	1) Produce arrow within constraints of provided pixel box and in direction determined by angle
 *
 *
 */

#include "OverlayRendererControl.h"
#include "RouteInputAnalyzer.h"
#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <wand/magick_wand.h>
#include <MagickWand.h>

// method for drawing a generic "one sized fits all" right arrow
static void genericRightArrow(OverlayRendererControl *controller)
{
	int p1x = controller->x1;
	int p1y = controller->y1;
	int p2x = controller->x2;
	int p2y = controller->y2;
	
	// Calculate the pixel locations for the three points of the arrow head
	// x0
	controller->triangle[0][0] = p2x - (p2x-p1x)/3;
	// y0
	controller->triangle[0][1] = p1y;
	// x1
	controller->triangle[1][0] = p2x;
	// y1
	controller->triangle[1][1] = p1y + (p2y-p1y)/2;
	// x2
	controller->triangle[2][0] = p2x - (p2x-p1x)/3;
	// y2
	controller->triangle[2][1] = p2y;
	
	// Calculate the four pixel locations for the bar of the arrow
	// x0
	controller->square[0][0]  = p1x - 2*(p2y-p1y)/3;
	// y0
	controller->square[0][1]  = p1y + (p2y-p1y)*.33;
	// x1
	controller->square[1][0]  = p2x - (p2x-p1x)/3;
	// y1
	controller->square[1][1]  = p1y + (p2y-p1y)*.33;
	// x2
	controller->square[2][0]  = p2x - (p2x-p1x)/3;
	// y2
	controller->square[2][1]  = p2y - (p2y-p1y)*.33;
	// x3
	controller->square[3][0]  = p1x - 2*(p2y-p1y)/3;
	// y3controller
	controller->square[3][1]  = p2y - (p2y-p1y)*.33;
}

// method for drawing a generic "one sized fits all" left arrow
static void genericLeftArrow(OverlayRendererControl *controller)
{
	int p1x = controller->x1;
	int p1y = controller->y1;
	int p2x = controller->x2;
	int p2y = controller->y2;
	
	// Calculate the pixel locations for the three points of the arrow head
	// x0
	controller->triangle[0][0] = p1x + (p2x-p1x)/3;
	// y0
	controller->triangle[0][1] = p1y;
	// x1
	controller->triangle[1][0] = p1x;
	// y1
	controller->triangle[1][1] = p1y + (p2y-p1y)/2;
	// x2
	controller->triangle[2][0] = p1x + (p2x-p1x)/3;
	// y2
	controller->triangle[2][1] = p2y;
	
	// Calculate the four pixel locations for the bar of the arrow
	// x0
	controller->square[0][0]  = p1x + (p2x-p1x)/3;
	// y0
	controller->square[0][1]  = p1y + (p2y-p1y)*.33;
	// x1
	controller->square[1][0]  = p2x;
	// y1
	controller->square[1][1]  = p1x + (p2x-p1x)/3;
	// x2
	controller->square[2][0]  = p2x;
	// y2
	controller->square[2][1]  = p2y - (p2y-p1y)*.33;
	// x3
	controller->square[3][0]  = p1x + (p2x-p1x)/3;
	// y3
	controller->square[3][1]  = p2y - (p2y-p1y)*.33;
}

// method for drawing a generic "one sized fits all" straight arrow
static void genericStraightArrow(OverlayRendererControl *controller)
{
	int p1x = controller->x1;
	int p1y = controller->y1;
	int p2x = controller->x2;
	int p2y = controller->y2;
	
	// Calculate the pixel locations for the three points of the arrow head
	// x0
	controller->triangle[0][0] = p1x;
	// y0
	controller->triangle[0][1] = p1y+(p2y-p1y)/3;
	// x1
	controller->triangle[1][0] = (p1x+p2x)/2;
	// y1
	controller->triangle[1][1] = p1y;
	// x2
	controller->triangle[2][0] = p2x;
	// y2
	controller->triangle[2][1] = p1y+(p2y-p1y)/3;
	
	// Calculate the four pixel locations for the bar of the arrow
	// x0
	controller->square[0][0]  = p1x + (p2x-p1x)*.33;
	// y0
	controller->square[0][1]  = p1y+(p2y-p1y)/3;
	// x1
	controller->square[1][0]  = p2x-(p2x-p1x)*.33;
	// y1
	controller->square[1][1]  = p1y+(p2y-p1y)/3;
	// x2
	controller->square[2][0]  = p2x-(p2x-p1x)*.33;
	// y2
	controller->square[2][1]  = p2y;
	// x3
	controller->square[3][0]  = p1x + (p2x-p1x)*.33;
	// y3
	controller->square[3][1]  = p2y;
}

void drawImage(OverlayRendererControl *control)
{
	switch(control->type_of_arrow)
	{
		case 1:
			genericLeftArrow(control);
			break;
		case 2:
			genericStraightArrow(control);
			break;
		case 3:
			genericRightArrow(control);
			break;
		default:
			genericStraightArrow(control);
			break;
	}
	control->type_of_arrow = 0;
	
	MagickWand *m_wand = NULL;
	DrawingWand *d_wand = NULL;
	PixelWand *c_wand = NULL;
	
	MagickWandGenesis();
	
	m_wand = NewMagickWand();
	d_wand = NewDrawingWand();
	c_wand = NewPixelWand();
	
	PixelSetColor(c_wand, "black"); // set color #include <time.h>of pixel colors to be drawn for background
	
	MagickNewImage(m_wand,control->width,control->height,c_wand); // create the image
	
	DrawSetStrokeOpacity(d_wand,1); // set opacity of the drawing
	PixelSetColor(c_wand,control->color); // set color of pixel colors to be drawn for edge of shape
	
	
	/* Start Draw Bar of Arrow */
	PushDrawingWand(d_wand);
	
	DrawSetStrokeColor(d_wand,c_wand); // set stroke color
	DrawSetStrokeWidth(d_wand,1); // set the width of the stroke
	DrawSetStrokeAntialias(d_wand,1); // no clue what this does =(
	DrawSetFillColor(d_wand,c_wand); // set the shape fill color
	DrawRectangle(d_wand,control->square[0][0], control->square[0][1], control->square[2][0], control->square[2][1]); // Draw out the Rectangle using points 0 and 2
	
	PopDrawingWand(d_wand);
	
	/* End Draw Bar of Arrow */
	
	
	/* Start Draw Arrow Head */
	PushDrawingWand(d_wand);
	{
		const PointInfo points[3] =
	    {
			{ control->triangle[0][0], control->triangle[0][1] },
			{ control->triangle[1][0], control->triangle[1][1] },
			{ control->triangle[2][0], control->triangle[2][1] }
	    };
		
		DrawSetStrokeAntialias(d_wand, MagickTrue);
		DrawSetStrokeWidth(d_wand, 1);
		DrawSetStrokeLineCap(d_wand, RoundCap);
		DrawSetStrokeLineJoin(d_wand, RoundJoin);
		(void) DrawSetStrokeDashArray(d_wand, 0, (const double *)NULL);
		(void) PixelSetColor(c_wand, control->color);
		DrawSetStrokeColor(d_wand, c_wand);
		//DrawSetFillRule(d_wand, EvenOddRule);
		//(void) PixelSetColor(c_wand,control->color);
		DrawSetFillColor(d_wand,c_wand);
		DrawPolygon(d_wand,3,points);
	}
	
	PopDrawingWand(d_wand);
	/* End Draw Arrow Head */
	
	MagickDrawImage(m_wand,d_wand);
	
	DrawSetStrokeOpacity(d_wand,1); // set opacity of the drawing
	PixelSetColor(c_wand,control->color); // set color of pixel colors to be drawn for edge of shape
	DrawSetStrokeColor(d_wand,c_wand); // set stroke color
	DrawSetStrokeWidth(d_wand,1); // set the width of the stroke
	DrawSetStrokeAntialias(d_wand,1); // no clue what this does =(
	DrawSetFillColor(d_wand,c_wand); // set the shape fill color
	MagickAnnotateImage(m_wand, d_wand, control->textPosition[0][0], control->textPosition[0][1], 0, control->speed_text);
	MagickAnnotateImage(m_wand, d_wand, control->textPosition[1][0], control->textPosition[1][1], 0, control->distance_to_next_turn_text);
	
	
	MagickWriteImage(m_wand,"arrow_test.jpg");
	
	c_wand = DestroyPixelWand(c_wand);
	m_wand = DestroyMagickWand(m_wand);
	d_wand = DestroyDrawingWand(d_wand);
	
	MagickWandTerminus();
}







