/*
 * ImageCreation.c
 *
 *  Created on: Dec 3, 2011
 *      Author: Jonathan Katon
 *
 *      ImageCreation is responsible for creating the final graphic which will be displayed. It does the following:
 *      	1) Produce arrow within constraints of provided pixel box and in direction determined by angle
 *
 */

#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <wand/magick_wand.h>
#include <MagickWand.h>

int angle = 0;

// method for drawing a generic "one sized fits all" right arrow
static void genericRightArrow(OverlayRendererControl *controller)
{
	int p1x = controller->x1;
	int p1y = controller->y1;
	int p2x = controller->x3;
	int p2y = controller->y3;
	
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

	// determine angle of skew for arrow
	//angle = 90 - controller->turn_angle;
	angle = controller->turn_angle - 90;
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

	// determine angle of skew for arrow
	//angle = abs(controller->turn_angle) + 90;
	angle = controller->turn_angle + 90;
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

	angle = 0;
}

// method for drawing a generic "one sized fits all" right arrow
static void RightArrow(OverlayRendererControl *controller)
{
	int p1x = controller->x1; // top left
	int p1y = controller->y1; // top left
	int p2x = controller->x2; // bottom right
	//int p2y = controller->y2; // bottom right
	int p3x = controller->x3; // bottom left
	int p3y = controller->y3; // bottom left
	int p4x = controller->x4; // top right
	int p4y = controller->y4; // top right
/*
	// Calculate the pixel locations for the three points of the arrow head
	// tip of triangle -- point G
	float m42 = (float) (p4y - p2y) / ( p2x - p4x); // slope
	float yg = (float) (p4y + p2y) / 2; // y coordinate
	float xg = (float) (yg - p4y) / m42 + p4x; // x coordinate

	controller->triangle[0][0] = (int) xg; // x0
	controller->triangle[0][1] = (int) yg; // y0

	// bottom verticy of triangle -- point F
	float m32 = (float) (p3y - p2y) / ( p2x - p3x); // slope
	float xf = p2x - (float)(p2x - p3x) / 3; // x coordinate
	float yf = (float) m32 * (xf - p3x) + p3y; // y coordinate

	controller->triangle[1][0] = (int) xf; // x1
	controller->triangle[1][1] = (int) yf; // y1

	// top verticy of triangle -- point E
	float m14 = (float) (p1y - p4y) / ( p4x - p1x); // slope
	float xe = p4x - (float)(p4x - p1x) / 3; // x coordinate
	float ye = (float) m14 * (xe - p1x) + p1y; // y coordinate

	controller->triangle[2][0] = (int) xe; // x2
	controller->triangle[2][1] = (int) ye; // y2*/


	// Calculate the four pixel locations for the bar of the arrow
	// x0
	controller->square[0][0]  = p1x;
	// y0
	controller->square[0][1]  = p1y;
	// x1
	controller->square[1][0]  = p2x;
	// y1
	controller->square[1][1]  = p1y;
	// x2
	controller->square[2][0]  = p3x;
	// y2
	controller->square[2][1]  = p3y;
	// x3
	controller->square[3][0]  = p4x;
	// y3controller
	controller->square[3][1]  = p4y;
}

// Method which draws the Bar of the arrow
static void createRectangle(OverlayRendererControl *controller, DrawingWand *d, PixelWand *p)
{
	PushDrawingWand(d);

	//DrawRotate(d, angle);
	//DrawSkewX(d, 0);
	//DrawSkewY(d, -angle);

	// Draw out the Rectangle using points 0 and 2
	DrawRectangle(d,controller->square[0][0], controller->square[0][1], controller->square[2][0], controller->square[2][1]);

	PopDrawingWand(d);
}

// Method which draws the head of the arrow
static void createTriangle(OverlayRendererControl *controller, DrawingWand *d, PixelWand *p, MagickWand *m)
{
	PushDrawingWand(d);

	// Set the three points of arrow head
	const PointInfo points[3] =
    {
		{ controller->triangle[0][0], controller->triangle[0][1] },
		{ controller->triangle[1][0], controller->triangle[1][1] },
		{ controller->triangle[2][0], controller->triangle[2][1] }
    };

	//DrawRotate(d, angle);
	//DrawSkewX(d, 0);
	//DrawSkewY(d, -angle);

	// Draw the arrow head
	DrawPolygon(d,3,points);

	PopDrawingWand(d);

}

static void createRect(OverlayRendererControl *controller, DrawingWand *d, PixelWand *p, MagickWand *m)
{
	PushDrawingWand(d);

	// Set the three points of arrow head
	const PointInfo points[4] =
    {
		{ controller->square[0][0], controller->square[0][1] },
		{ controller->square[1][0], controller->square[1][1] },
		{ controller->square[2][0], controller->square[2][1] },
		{ controller->square[3][0], controller->square[3][1] }
    };

	//DrawRotate(d, angle);
	//DrawSkewX(d, 0);
	//DrawSkewY(d, -angle);

	// Draw the arrow head
	DrawPolygon(d,4,points);

	PopDrawingWand(d);
}

// Method which draws the text
static void drawText(OverlayRendererControl *controller, DrawingWand *d, MagickWand *m)
{
	MagickAnnotateImage(m, d, controller->textPosition[0][0], controller->textPosition[0][1], 0, controller->speed_text);
	MagickAnnotateImage(m, d, controller->textPosition[1][0], controller->textPosition[1][1], 0, controller->distance_to_next_turn_text);
}

// Method to change Color Settings of Arrow and Text
static void changeColorSettings(OverlayRendererControl *controller, DrawingWand *d, PixelWand *p)
{
	PixelSetColor(p,controller->color); // set color of pixels in shape
	//PixelSetOpacity(p, 0); // set opacity of the fill
	DrawSetStrokeColor(d,p); // set stroke color
	DrawSetFillColor(d,p); // set the shape fill color
	DrawSetStrokeWidth(d,.0001); // set the width of the stroke
	DrawSetStrokeAntialias(d, 1); // no clue what this does =(
	//DrawSetStrokeOpacity(d,0); // set opacity of the border
	//DrawSetGravity(d,CenterGravity);
}

void drawImage(OverlayRendererControl *control)
{
	// calculate the 3 coordinates for corners of arrow head and 4 coordinates for corners of arrow bar
/*	switch(control->type_of_arrow)
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
	}*/

	// reset type of arrow to be drawn for next iteration
	control->type_of_arrow = 0;

	/*** Create the needed ImageMagick Wand's and then the Environment ***/
	MagickWand *m1 = NewMagickWand();
	DrawingWand *d1 = NewDrawingWand();
	PixelWand *p1 = NewPixelWand();

	MagickWand *m2 = NewMagickWand();
	DrawingWand *d2 = NewDrawingWand();
	PixelWand *p2 = NewPixelWand();
	MagickWandGenesis();

	
	/*** Create blank image canvas with a black background ***/
	PixelSetColor(p2, "clear"); // set color of image background
	MagickNewImage(m1,control->width,control->height,p2); // create blank image


	/*** Set color of image to users setting ***/
	changeColorSettings(control, d1, p1);


	/*** Draw Bar of Arrow ***/
	//createRectangle(control, d1, p1);
	createRect(control, d1, p1, m1);


	/*** Draw Head of Arrow ***/
	createTriangle(control, d1, p1, m1);
	//createRect(control, d1, p1, m1);

	/*** Add Bar and Head of Arrow to the Blank Canvas  ***/
	MagickDrawImage(m1,d1);


	//MagickBooleanType MagickDistortImage(MagickWand *wand, const DistortImageMethod method,const size_t number_arguments, const double *arguments,const MagickBooleanType bestfit);

	//MagickDistortImage(m_wand, Affine, 6, );
	//MagickDistortImage(m_wand, AffineDistortion, 1, MagickTrue);


	/*** Add the Text to the Canvas ***/
	drawText(control, d1, m1);

	
	/*** Save Image to file ***/
	MagickWriteImage(m1,"arrow_test.jpg");
	

	/*** Clear up Memory ***/
	p1 = DestroyPixelWand(p1);
	m1 = DestroyMagickWand(m1);
	d1 = DestroyDrawingWand(d1);
	MagickWandTerminus();

	   MagickWand *mw1 = NewMagickWand();
	   PixelWand *bgw1 = NewPixelWand();
	   DrawingWand *dw1 = NewDrawingWand();

	   MagickWand *mw2 = NewMagickWand();
	   PixelWand *bgw2 = NewPixelWand();
	   DrawingWand *dw2 = NewDrawingWand();

	   MagickWand *merged;

	   MagickWandGenesis();
	   PixelSetColor(bgw1,"red");
	   MagickNewImage(mw1, 100, 100, bgw1); // this image should have an opaque background


	//... paint on the image
	   PixelSetColor(bgw1,"white");
	   DrawSetFillColor(dw1,bgw1);
	   DrawSetFontSize(dw1,12);
	   DrawSetFont(dw1,"Times-New-Roman");
	   MagickAnnotateImage(mw1,dw1,0,15,0.0,"Hello");

	// now create another layer



	   PixelSetColor(bgw2,"none");
	   MagickNewImage(mw2, 100, 100, bgw2); // this image should have a transparent background

	// ... paint on the second image
	   PixelSetColor(bgw2,"blue");
	   DrawSetFillColor(dw2,bgw2);
	   DrawSetFontSize(dw2,12);
	   DrawSetFont(dw2,"Times-New-Roman");
	   MagickAnnotateImage(mw2,dw2,0,50,0.0,"Magick");

	// create the final image
	   MagickAddImage(mw2, mw1);
	   merged = MagickMergeImageLayers(mw2, MergeLayer);
	   MagickWriteImage(mw1, "mw1.png");
	   MagickWriteImage(mw2, "mw2.png");
	   MagickWriteImage(merged, "mwb.png");

	   merged = DestroyMagickWand(merged);
	   mw1 = DestroyMagickWand(mw1);
	   mw2 = DestroyMagickWand(mw2);
	   bgw2 = DestroyPixelWand(bgw2);
	   dw2 = DestroyDrawingWand(dw2);
	   bgw1 = DestroyPixelWand(bgw1);
	   dw1 = DestroyDrawingWand(dw1);

}









