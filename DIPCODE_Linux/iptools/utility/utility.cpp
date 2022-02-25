#include "utility.h"
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXLEN 256
#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, struct ROI roi)
{
	// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j) + roi.value)); 
			}
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, struct ROI roi)
{
	// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				if (src.getPixel(i,j) < roi.threshold)
					tgt.setPixel(i,j,MINRGB);
				else
					tgt.setPixel(i,j,MAXRGB);
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::colorvisual(image &src, image &tgt, struct ROI roi) {
	// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
		for (int i=0; i<src.getNumberOfRows(); i++)
		{
			for (int j=0; j<src.getNumberOfColumns(); j++)
			{
				if(isWithinRoi(roi, i, j)) {
					//apply effect
					if(abs(roi.intensity - src.getPixel(i, j, 0)) < roi.threshold) {
						tgt.setPixel(i, j, 0, 255);
						tgt.setPixel(i, j, 1, 0);
						tgt.setPixel(i, j, 2, 0);
					} else {
						tgt.setPixel(i, j, 0, src.getPixel(i, j, 0));
						tgt.setPixel(i, j, 1, src.getPixel(i, j, 1));
						tgt.setPixel(i, j, 2, src.getPixel(i, j, 2));
					}
				} 
			}
		}
}

/*-----------------------------------------------------------------------**/
void utility::reg2dsmooth(image &src, image &tgt, struct ROI roi) {
	// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
		for (int i=0; i<src.getNumberOfRows(); i++)
		{
			for (int j=0; j<src.getNumberOfColumns(); j++)
			{
				//stmt 1 checks were inside roi, stmt2 checks mask is contained inside roi
				if(isWithinRoi(roi, i, j) && isWithinRoi(roi, (i + roi.ws - 1), (j + roi.ws - 1))) {
					//apply effect
					//sum up the pixels within the mask
					int sum = 0;
					for(int m = i; m < i + roi.ws; m++) {
						for(int n = j; n < j + roi.ws; n++) {
							sum = sum + src.getPixel(m, n);
						}
					}
					//target pixel 
					tgt.setPixel(i + (roi.ws - 1) / 2, j + (roi.ws - 1) / 2, sum /(roi.ws*roi.ws));
				} 
			}
		}
}

/*-----------------------------------------------------------------------**/
void utility::colorbright(image &src, image &tgt, struct ROI roi) {
	// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
		for (int i=0; i<src.getNumberOfRows(); i++)
		{
			for (int j=0; j<src.getNumberOfColumns(); j++)
			{
				if(isWithinRoi(roi, i, j)) {
					//apply effect
					//set the red
					tgt.setPixel(i, j, 0, checkValue(round(src.getPixel(i, j, 0) * roi.dr)));
					//set the green
					tgt.setPixel(i, j, 1, checkValue(round(src.getPixel(i, j, 1) * roi.dg)));
					//set the blue
					tgt.setPixel(i, j, 2, checkValue(round(src.getPixel(i, j, 2) * roi.db)));
				} 
			}
		}
}

/*-----------------------------------------------------------------------**/
void utility::histostretch(image &src, image &tgt, struct ROI roi) {
// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	int min = findMin(src, roi);
	int max = findMax(src, roi);
	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				int tmp = abs((src.getPixel(i, j) - min) * ((roi.B - roi.A) / (max - min)) + roi.A);
			  	tgt.setPixel(i, j, checkValue(tmp));
			} 
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::histothres(image &src, image &tgt, struct ROI roi) {
// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	int min = findMin(src, roi);
	int max = findMax(src, roi);
	int pixels[src.getNumberOfRows()][src.getNumberOfColumns()];

	//apply thresholding
	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				if(src.getPixel(i, j) >= roi.threshold) {
						pixels[i][j] = 1;
				} else {
					pixels[i][j] = 0;
				}
			} 
		}
	}

	//apply histogram stretching
	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				if(roi.beta == pixels[i][j]) {
					int tmp = abs((src.getPixel(i, j) - min) * ((roi.B - roi.A) / (max - min)) + roi.A);
			  		tgt.setPixel(i, j, checkValue(tmp));
				}
			} 
		}
	}
}

/*-----------------------------------------------------------------------**/
float utility::findMin(image &src, struct ROI roi) {
	float min = src.getPixel(roi.x1, roi.y1);
	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if((src.getPixel(i, j) < min) && isWithinRoi(roi, i, j))
				min = src.getPixel(i, j);
		}
	}
	return min;
}

/*-----------------------------------------------------------------------**/
float utility::findMax(image &src, struct ROI roi) {
	float max = src.getPixel(roi.x1, roi.y1);
	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if((src.getPixel(i, j) > max) && isWithinRoi(roi, i, j))
				max = src.getPixel(i, j);
		}
	}
	return max;
}

bool utility::isValidRoi(struct ROI roi, image &src) {
	bool isValid = true;
	if(!src.isInbounds(roi.x1 + roi.sx1, roi.y1 + roi.sy1)) {
		return false;
	}
	return isValid;
}

/*-----------------------------------------------------------------------**/
bool utility::isWithinRoi(struct ROI roi, int i, int j) {
	if(((i >= roi.x1) && (i <= (roi.x1 + roi.sx1))) && ((j >= roi.y1) && (j <= (roi.y1 + roi.sy1)))) 
		return true;
	else
		return false;
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}
