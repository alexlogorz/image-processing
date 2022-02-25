#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>
#include <stdlib.h>

struct ROI {
	int x1, y1, sx1, sy1, threshold, intensity, ws, value;
	float dr, dg, db;
	int name, beta;
	float A, B;
};

struct MultDimArray {
	int row[], col[];
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
		static void histostretch(image &src, image &tgt, struct ROI roi);
		static void histothres(image &src, image &tgt, struct ROI roi);
		static float findMin(image &src, struct ROI roi);
		static float findMax(image &src, struct ROI roi);
};

#endif

