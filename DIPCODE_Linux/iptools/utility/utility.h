#ifndef UTILITY_H
#define UTILITY_H

#define HISTMAX 256

#include "../image/image.h"
#include <sstream>
#include <math.h>
#include <stdlib.h>

struct ROI {
	int x1, y1, sx1, sy1, threshold, intensity, ws, value;
	float dr, dg, db;
	int name, beta, channel;
	float A, B;
};

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, struct ROI roi);
		static void binarize(image &src, image &tgt, struct ROI roi);
		static void scale(image &src, image &tgt, float ratio);
		static void colorvisual(image &src, image &tgt, struct ROI roi);
		static bool isWithinRoi(struct ROI roi, int x, int y);
		static bool isValidRoi(struct ROI rois, image &src);
		static void reg2dsmooth(image &src, image &tgt, struct ROI roi);
		static void colorbright(image &src, image &tgt, struct ROI roi);
		static void histostretch(image &src, image &tgt, struct ROI roi, string& outfile_hist);
		static void colorstretch(image &src, image &tgt, struct ROI roi, string& outfile_hist);
		static void hsicolorstretch(image &src, image &tgt, struct ROI roi);
		static void hsistretch(image &src, image &tgt, struct ROI roi, string& outfile_hist);
		static void althistostretch(image &src, image &tgt, struct ROI roi, string& outfile_hist);
		static void histothres(image &src, image &tgt, struct ROI, string& outfile_hist);
		static float findMin(image &src, struct ROI roi);
		static float findMax(image &src, struct ROI roi);
		static void histImage(int hbefore[256],int hafter[256],string& fname);
		static image convertHSI(image &img, struct ROI roi);
		static void convertRGB(image &img, image &tgt, struct ROI roi);
};

#endif

