/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	int roiCount;
	struct ROI rois[3];

	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		pch = strtok(str, " "); //ipf
		src.read(pch);

		pch = strtok(NULL, " "); //opf
		strcpy(outfile, pch);

		pch = strtok(NULL, " "); //roiCount
		roiCount = atoi(pch);

		if(roiCount > 3) {
			printf("at most 3 rois allowed");
			exit(1);
		}

		//get the roi configurations
		for(int i = 0; i < roiCount; i++) {
			struct ROI tmp;
			pch = strtok(NULL, " "); //x1
			tmp.x1 = atoi(pch);
			pch = strtok(NULL, " "); //y1
			tmp.y1 = atoi(pch);
			pch = strtok(NULL, " "); //sx1
			tmp.sx1 = atoi(pch);
			pch = strtok(NULL, " "); //sy1
			tmp.sy1 = atoi(pch);
			pch = strtok(NULL, " "); //func name
			
			if(strncasecmp(pch,"colorvisual", MAXLEN) == 0) {
				tmp.name = 0;
				pch = strtok(NULL, " "); //threshold
				tmp.threshold = atoi(pch);
				pch = strtok(NULL, " "); //intensity
				tmp.intensity = atoi(pch);
				
			}
			else if(strncasecmp(pch,"reg2dsmooth", MAXLEN) == 0) {
				tmp.name = 1;
				pch = strtok(NULL, " "); //ws
				tmp.ws = atoi(pch);
			}
			else if(strncasecmp(pch,"colorbright", MAXLEN) == 0) {
				tmp.name = 2;
				pch = strtok(NULL, " "); //dr
				tmp.dr = atof(pch);
				pch = strtok(NULL, " "); //dg
				tmp.dg = atof(pch);
				pch = strtok(NULL, " "); //db
				tmp.db = atof(pch);
			}
			else if(strncasecmp(pch,"add", MAXLEN) == 0) {
				tmp.name = 3;
				pch = strtok(NULL, " "); //value
				tmp.value = atoi(pch);
			}
			else if(strncasecmp(pch,"binarize", MAXLEN) == 0) {
				tmp.name = 4;
				pch = strtok(NULL, " "); //thresh
				tmp.threshold = atoi(pch);
			}
			else if(strncasecmp(pch,"histostretch", MAXLEN) == 0) {
				tmp.name = 5;
				pch = strtok(NULL, " "); //A
				tmp.A = atoi(pch);
				pch = strtok(NULL, " "); //B
				tmp.B = atoi(pch);
			}
			else if(strncasecmp(pch,"histothres", MAXLEN) == 0) {
				tmp.name = 6;
				pch = strtok(NULL, " "); //threshold
				tmp.threshold = atoi(pch);
				pch = strtok(NULL, " "); //background or foreground
				tmp.beta = strcasecmp(pch, "B") ? 0 : 1;
				pch = strtok(NULL, " "); //A
				tmp.A = atoi(pch);
				pch = strtok(NULL, " "); //B
				tmp.B = atoi(pch);
			}
			else {
				printf("function %s not found", pch);
				exit(1);
			}

			rois[i] = tmp;

			if(!utility::isValidRoi(rois[i], src)) {
				printf("Could not apply this roi: out of bounds");
				exit(1);
			}
			
		}

		tgt.copyImage(src);

		for(int k = roiCount; k > 0; k--) {
			if(rois[k - 1].name == 0) {
				utility::colorvisual(src, tgt, rois[k - 1]);
			} else if(rois[k - 1].name == 1) {
				utility::reg2dsmooth(src, tgt, rois[k - 1]);
			}
			else if(rois[k - 1].name == 2) {
				utility::colorbright(src, tgt, rois[k - 1]);
			}
			else if(rois[k - 1].name == 3) {
				utility::addGrey(src, tgt, rois[k - 1]);
			}
			else if(rois[k - 1].name == 4) {
				utility::binarize(src, tgt, rois[k - 1]);
			}
			else if(rois[k - 1].name == 5) {
				utility::histostretch(src, tgt, rois[k - 1]);
			}
			else if(rois[k - 1].name == 6) {
				utility::histothres(src, tgt, rois[k - 1]);
			}
		}

		tgt.save(outfile);
	}
	fclose(fp);
	return 0;
}

