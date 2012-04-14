/*
 * ImageCreation.c
 *
 *  Created on: Dec 3, 2011
 *      Author: Jonathan Katon
 *
 *      ImageCreation is responsible for creating the final graphic which will be displayed. It does the following:
 *      	1) Produce arrow using precalculated pixel coordinates for boundaries of arrow
 *
 */

#include "ImageCreation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <wand/magick_wand.h>
#include <MagickWand.h>

#include "cxcore.h"
#include "highgui.h"


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

// Method which draws the Bar of the arrow from previously rotate pixel points
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

	// Draw the arrow head
	DrawPolygon(d,4,points);

	PopDrawingWand(d);
}

// Method which draws the Bar of the arrow from previously rotate pixel points
static void createPolygon(OverlayRendererControl *controller, DrawingWand *d, PixelWand *p, MagickWand *m)
{
	PushDrawingWand(d);

	// Set the three points of arrow head
	const PointInfo points[6] =
    {
		{ controller->boundary_array[0][0], controller->boundary_array[0][1] },
		{ controller->boundary_array[1][0], controller->boundary_array[1][1] },
		{ controller->boundary_array[2][0], controller->boundary_array[2][1] },
		{ controller->boundary_array[3][0], controller->boundary_array[3][1] },
		{ controller->boundary_array[4][0], controller->boundary_array[4][1] },
		{ controller->boundary_array[5][0], controller->boundary_array[5][1] }
    };

	// Draw the arrow head
	DrawPolygon(d,6,points);

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

// method which initiates the ImageMagick environment and then creates the arrow
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
	//control->type_of_arrow = 0;


	/*** Create the needed ImageMagick Wand's and then the Environment ***/
	MagickWand *m1 = NewMagickWand();
	DrawingWand *d1 = NewDrawingWand();
	PixelWand *p1 = NewPixelWand();


	PixelWand *p2 = NewPixelWand();
	MagickWandGenesis();

	MagickSetFont(m1, "Courier");
	MagickSetPointsize(m1, 172);
	
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

	/*** Add Bar and Head of Arrow to the Blank Canvas  ***/
	MagickDrawImage(m1,d1);

	/*** Add the Text to the Canvas ***/
	drawText(control, d1, m1);

	/*
	char *temp = malloc(controller.height*controller.width);
	//MagickGetImagePixels(m1, 0, 0, controller.width, controller.height, "BGR", CharPixel, temp);

	printf("")
	IplImage* img = cvCreateImage(cvSize(controller.width, controller.height),8,3);

	img->imageData = temp;

	cvNamedWindow( "Projection", CV_WINDOW_NORMAL );
	cvSetWindowProperty("Projection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	cvShowImage("Projection", &img);

	 cvReleaseImage( &img );

	IplImage* img1;
	cvNamedWindow( "Projection", CV_WINDOW_NORMAL );
	cvSetWindowProperty("Projection", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	img1 = cvLoadImage( "arrow_test.jpg", CV_LOAD_IMAGE_COLOR );

	//printf("height: %d width: %d dataOrder: %d origin: %d imagesize: %d depth: %d colorModel: %s channelS: %s widthStep: %d\n", img1->height, img1->width, img1->dataOrder, img1->origin, img1->imageSize, img1->depth,img1->colorModel,img1->channelSeq,img1->widthStep);
	printf("Temp First Byte = %x Image First Byte = %x\n",temp[100],img1->imageData[100]); */

	/*** Save Image to file ***/
	MagickWriteImage(m1,"arrow_test.jpg");
	

	/*** Clear up Memory ***/
	p1 = DestroyPixelWand(p1);
	m1 = DestroyMagickWand(m1);
	d1 = DestroyDrawingWand(d1);
	MagickWandTerminus();

}

void drawRoadTrackingImage(OverlayRendererControl *control)
{

	/*** Create the needed ImageMagick Wand's and then the Environment ***/
	MagickWand *m1 = NewMagickWand();
	DrawingWand *d1 = NewDrawingWand();
	PixelWand *p1 = NewPixelWand();


	PixelWand *p2 = NewPixelWand();
	MagickWandGenesis();

	MagickSetFont(m1, "Courier");
	MagickSetPointsize(m1, 172);

	/*** Create blank image canvas with a black background ***/
	PixelSetColor(p2, "clear"); // set color of image background
	MagickNewImage(m1,control->width,control->height,p2); // create blank image


	/*** Set color of image to users setting ***/
	changeColorSettings(control, d1, p1);


	/*** Draw Bar of Arrow ***/
	//createRectangle(control, d1, p1);
	createPolygon(control, d1, p1, m1);


	/*** Add Bar and Head of Arrow to the Blank Canvas  ***/
	MagickDrawImage(m1,d1);


	/*** Save Image to file ***/
	MagickWriteImage(m1,"arrow_test.jpg");


	/*** Clear up Memory ***/
	p1 = DestroyPixelWand(p1);
	m1 = DestroyMagickWand(m1);
	d1 = DestroyDrawingWand(d1);
	MagickWandTerminus();

}









