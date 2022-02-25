#include "utility.h"
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXLEN 256
#define MAXRGB 255
#define MINRGB 0

#define WHITE 255
#define BLACK 0

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
void utility::histostretch(image &src, image &tgt, struct ROI roi, string& outfile_hist) {
// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	int min = findMin(src, roi);
	int max = findMax(src, roi);
	int hist_before[256] = {0};
	int hist_after[256] = {0};

	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				int tmp = abs((src.getPixel(i, j) - min) * ((roi.B - roi.A) / (max - min)) + roi.A);
			  	tgt.setPixel(i, j, checkValue(tmp));

			  	// Create histograms
			  	hist_before[src.getPixel(i, j)]++;
			  	hist_after[checkValue(tmp)]++;
			} 
		}
	}

	// Create historgram image
	histImage(hist_before, hist_after, outfile_hist);
}

void utility::colorstretch(image &src, image &tgt, struct ROI roi, string& outfile_hist) {

	int min = findMin(src, roi);
	int max = findMax(src, roi);
	int hist_before[256] = {0};
	int hist_after[256] = {0};

	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				int tmp = abs((src.getPixel(i, j, roi.channel) - min) * ((roi.B - roi.A) / (max - min)) + roi.A);
			  	tgt.setPixel(i, j, roi.channel, checkValue(tmp));

			  	// Create histograms
			  	hist_before[src.getPixel(i, j, roi.channel)]++;
			  	hist_after[checkValue(tmp)]++;
			} 
		}
	}

	// Create historgram image
	histImage(hist_before, hist_after, outfile_hist);
}

void utility::hsicolorstretch(image &src, image &tgt, struct ROI roi) {

	float min = findMin(src, roi);
	float max = findMax(src, roi);
	// int hist_before[256] = {0};
	// int hist_after[256] = {0};

	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				float tmp = abs((src.getPixel(i, j, roi.channel) - min) * ((roi.B - roi.A) / (max - min)) + roi.A);
			  	tgt.setPixel(i, j, roi.channel, tmp);

			  	// Create histograms
			  	// hist_before[src.getPixel(i, j, roi.channel)]++;
			  	// hist_after[checkValue(tmp)]++;
			} 
		}
	}

	// Create historgram image
	// histImage(hist_before, hist_after, outfile_hist);
}

image utility::convertHSI(image &img, struct ROI roi) 
{
	image hsi;
	hsi.copyImage(img);

	// cout << "In convertHSI" << endl;

	// Vars
	double r, g, b;
	double R, G, B;
	double h, s, I;
	double denom, min;

	for(int i = 0; i < img.getNumberOfRows(); i++){
		for(int j = 0; j < img.getNumberOfColumns(); j++){
			if(isWithinRoi(roi, i, j)) {
				R = img.getPixel(i, j, RED);
				G = img.getPixel(i, j, GREEN);
				B = img.getPixel(i, j, BLUE);

				denom = R + G + B;
				r = R / denom;
				g = G / denom;
				b = B / denom;

				min = r;
				if(g < min){
					min = g;
				}
				if(b < min){
					min = b;
				}

				if(r == g && r == b){
					h = 0.0;
					s = 0.0;
				}

				else {
					h = acos((0.5 * ((r-g) + (r-b))) / sqrt(( (r-g)*(r-g) + (r-b)*(g-b) )));
					if(b > g){
						h = 2*M_PI - h;
					}
					h = h * (180.0 / M_PI);

					if((r+g+b)/3.0 == 0){
						s = 0;
					}
					else{
						s = 1.0 - 3.0*min;
						s *= 100;
					}
				}
				

				I = (R + G + B) / (3.0 * 255.0);
				I *= 255;

				hsi.setPixel(i, j, 0, h);
				hsi.setPixel(i, j, 1, s);
				hsi.setPixel(i, j, 2, I);
				// cout << "In convertHSI roi" << endl;
				// cout << j << endl;
				// cout << I << endl;
			}
		}
		// cout << i << endl;
	}
	// cout << "Done convertHSI" << endl;
	return hsi;
}

void utility::convertRGB(image &img, image &tgt, struct ROI roi) 
{
	// Vars
	double r, g, b;
	double R, G, B;
	double h, s, ii;
	double H, S, I;
	double temp, tmp;

	for(int i = 0; i < img.getNumberOfRows(); i++){
		for(int j = 0; j < img.getNumberOfColumns(); j++){
			if(isWithinRoi(roi, i, j)) {
				H = img.getPixel(i, j, 0);
				S = img.getPixel(i, j, 1);
				I = img.getPixel(i, j, 2);

				h = H * (M_PI / 180.0);
				s = S / 100.0;
				ii = I / 255.0;

				// if(h >= 0 && h < (2*M_PI/3.0)){
				// 	b = ii * (1.0 - s);
				// 	r = ii * (1.0 + (s*cos(h))/(cos(M_PI/(3.0-h))));
				// 	g = 3.0*ii - (b+r);
				// }

				// else if(h >= (2*M_PI/3.0) && h < (4*M_PI/3.0)){
				// 	h = h - (2.0*M_PI/3.0);
				// 	r = ii * (1.0 - s);
				// 	g = ii * (1.0 + (s*cos(h))/(cos(M_PI/(3.0-h))));
				// 	b = 3.0*ii - (g+r);
				// }

				// else if(h >= (4*M_PI/3.0) && h < (2*M_PI)){
				// // else {
				// 	h = h - (4.0*M_PI/3.0);
				// 	g = ii * (1.0 - s);
				// 	b = ii * (1.0 + (s*cos(h))/(cos(M_PI/(3.0-h))));
				// 	r = 3.0*ii - (b+g);
				// }

				if(H < 120 && H >= 0) {
				
					b = (int)I*(1.0-s);

					temp = 1.0 + ((s * cos(H * M_PI / 180.0)) / (cos((60-H) * M_PI / 180.0)));
					temp *= I;
					r = (int)temp;

					tmp = 3*I - r - b;
					g = tmp;
				}

				else if(H < 240 && H >= 120) {

					H -= 120;

					// this->setPixel(i,j,RED,(int)I*(1.0-s));

					// temp = 1.0 + ((s * cos(H * M_PI / 180.0)) / (cos((60-H) * M_PI / 180.0)));
					// temp *= it;
					// this->setPixel(i,j,GREEN,(int)temp);

					// tmp = 3*it - this->getPixel(i,j,RED) - this->getPixel(i,j,GREEN);
					// this->setPixel(i,j,BLUE,tmp);

					r = (int)I*(1.0-s);

					temp = 1.0 + ((s * cos(H * M_PI / 180.0)) / (cos((60-H) * M_PI / 180.0)));
					temp *= I;
					g = (int)temp;

					tmp = 3*I - r - g;
					b = tmp;
				}

				else if(H <= 360 && H >= 240) {

					H -= 240;

					// this->setPixel(i,j,GREEN,(int)it*(1.0-s));

					// temp = 1.0 + ((s * cos(H * M_PI / 180.0)) / (cos((60-H) * M_PI / 180.0)));
					// temp *= it;
					// this->setPixel(i,j,BLUE,(int)temp);

					// tmp = 3*it - this->getPixel(i,j,GREEN) - this->getPixel(i,j,BLUE);
					// this->setPixel(i,j,RED,tmp);

					g = (int)I*(1.0-s);

					temp = 1.0 + ((s * cos(H * M_PI / 180.0)) / (cos((60-H) * M_PI / 180.0)));
					temp *= I;
					b = (int)temp;

					tmp = 3*I - g - b;
					r = tmp;
				}



				R = int(r*255);
				G = int(g*255);
				B = int(b*255);

				// tgt.setPixel(i, j, 0, checkValue(R));
				// tgt.setPixel(i, j, 1, checkValue(G));
				// tgt.setPixel(i, j, 2, checkValue(B));
				tgt.setPixel(i, j, 0, checkValue(r));
				tgt.setPixel(i, j, 1, checkValue(g));
				tgt.setPixel(i, j, 2, checkValue(b));
				// cout << R << " " << G << " " << B << endl;
				// cout << r << " " << g << " " << b << endl;
				// cout << endl;
			}
		}
	}
}

void utility::hsistretch(image &src, image &tgt, struct ROI roi, string& outfile_hist) {

	// Convert to hsi
	image hsi_src, hsi_tgt, grey_lvl;
	hsi_src = convertHSI(src, roi);
	// string outfile_grey_lvl;
	// outfile_grey_lvl.append("-grey_lvl.pgm");

	// Stretch channel
	hsi_tgt.copyImage(hsi_src);
	hsicolorstretch(hsi_src, hsi_tgt, roi);

	// Save grayscale
	grey_lvl.resize(roi.sx1 - roi.x1+1, roi.sy1 - roi.y1+1);
	for(int i = 0; i < hsi_tgt.getNumberOfRows(); i++){
		for(int j = 0; j < hsi_tgt.getNumberOfColumns(); j++){
			if(isWithinRoi(roi, i, j)) {
				grey_lvl.setAll(i - roi.x1, j - roi.y1, hsi_tgt.getPixel(i, j, roi.channel));
			}
		}
	}
	grey_lvl.save(outfile_hist.c_str());

	// Convert back to rgb
	convertRGB(hsi_tgt, tgt, roi);
	// convertRGB(hsi_src, tgt, roi);
}

void utility::althistostretch(image &src, image &tgt, struct ROI roi, string& outfile_hist) {
// tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	int min = int(findMin(src, roi) * 1.05);
	int max = int(findMax(src, roi) * 0.95);
	int hist_before[256] = {0};
	int hist_after[256] = {0};
	int tmp;

	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				if(src.getPixel(i, j) < min)
					tmp = roi.A;
				else if(src.getPixel(i, j) > max)
					tmp = roi.B;
				else
					tmp = abs((src.getPixel(i, j) - min) * ((roi.B - roi.A) / (max - min)) + roi.A);
			  	tgt.setPixel(i, j, checkValue(tmp));

			  	// Create histograms
			  	hist_before[src.getPixel(i, j)]++;
			  	hist_after[checkValue(tmp)]++;
			} 
		}
	}

	// Create historgram image
	histImage(hist_before, hist_after, outfile_hist);
}

void utility::histImage(int hbefore[256],int hafter[256],string& fname) {
	//normalize hist array;
	int maxbef = 0, maxaft = 0, minbef, minaft;

	for(int i = 0; i < 256; ++i) {
		if(maxbef < hbefore[i])
			maxbef = hbefore[i];
		if(maxaft < hafter[i])
			maxaft = hafter[i];
	}

	minbef = maxbef, minaft = maxaft;
	for(int i = 0; i < 256; ++i) {
		if(minbef > hbefore[i])
			minbef = hbefore[i];
		if(minaft > hafter[i])
			minaft = hafter[i];
	}

	//variables for the two histograms before and after.
	int size = 512;
	double ratio, ratioa;
	image tgt(size,size), temp(size,size);

	
	for(int j = 0; j < HISTMAX; ++j) {

		ratio = (double)(hbefore[j] - minbef) / (double)(maxbef - minbef);
		ratioa = (double)(hafter[j] - minaft) / (double)(maxaft - minaft);
		//cout << ratio << "  ";
		for(int i = 0; i < size; ++i) {

			if(i < (size-1) - ((size-1) *ratio)) {
				//tgt.setPixel(i,j,BLACK); //for size = 256

				tgt.setAll(i,j*2,BLACK);
				tgt.setAll(i,j*2+1,BLACK);
			}
			else {
				//tgt.setPixel(i,j,WHITE); //for size = 256

				tgt.setAll(i,j*2,WHITE);
				tgt.setAll(i,j*2+1,WHITE);
			}

			if(i < (size-1) - ((size-1) *ratioa)) {
				//temp.setPixel(i,j,BLACK); //for size = 256

				temp.setAll(i,j*2,BLACK);
				temp.setAll(i,j*2+1,BLACK);
			}
			else {
				//temp.setPixel(i,j,WHITE); //for size = 256

				temp.setAll(i,j*2,WHITE);
				temp.setAll(i,j*2+1,WHITE);
			}
		}
	}

	tgt += temp;
	tgt.save(fname.c_str());
}

/*-----------------------------------------------------------------------**/
void utility::histothres(image &src, image &tgt, struct ROI roi, string& outfile_hist) {
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

	histostretch(src, tgt, roi, outfile_hist);

	for(int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if(isWithinRoi(roi, i, j)) {
				if(roi.beta != pixels[i][j]){
					tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
					// cout << roi.beta << " " << pixels[i][j] << endl;
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
			if((src.getPixel(i, j, roi.channel) < min) && isWithinRoi(roi, i, j))
				min = src.getPixel(i, j, roi.channel);
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
			if((src.getPixel(i, j, roi.channel) > max) && isWithinRoi(roi, i, j))
				max = src.getPixel(i, j, roi.channel);
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
