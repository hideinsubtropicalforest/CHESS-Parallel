/*-------------------------------------------------------------------------------------------------------------------------------
NAME:                    FILE_IO.CPP

contains all of the functions for handling file input and output.
------------------------------------------------------------------------------------------------------------------------------*/

#include <malloc.h>
#include <stdio.h> 

#include <stdlib.h> 
#include <string.h>
#include "chess.h"
#include "functions.h"
#include "constants.h"
#include <iostream>

using namespace std;

//===============================================================================================================================
//creating x and y coordinate 
void create_x_coordinates(double *indata, int mr, int mc, double cellsize, double xll) {
	int i, j, r;
	for (i = 0; i<mr; i++) {
		for (j = 0; j<mc; j++) {
			r = i * mc + j;
			indata[r] = xll + (double)j*cellsize;
			//printf("\n longitude %f \n",indata[r]);

		}
	}
	//getchar();
	return;
}

void create_y_coordinates(double *indata, int mr, int mc, double cellsize, double yll) {
	int i, j, r;
	for (i = 0; i<mr; i++) {
		for (j = 0; j<mc; j++) {
			r = i * mc + j;
			indata[r] = yll + (double)(mr - i - 1)*cellsize;
		}
	}
	return;
}


//===============================================================================================================================
//        input_ascii_int() - input an ascii image into an interger array using the (row, col) coordinates maxr and maxc.
//===============================================================================================================================
void input_ascii_int(int *indata, char *filename, int mc, int mr, int arc_flag) {
	FILE *in1;
	int  r;
	int max;
	void    pause();

	max = 0;
	if ((in1 = fopen(filename, "r")) == NULL) {
		printf("cannot open file %s\n", filename);
		pause();
	}
	else {
		/* skip header */
		if (arc_flag == 0)
			for (r = 0; r < LEN_GRASSHEADER; r++)
				fscanf(in1, "%*s");
		else
			for (r = 0; r < LEN_ARCHEADER; r++)
				fscanf(in1, "%*s");

		for (r = 0; r < mr*mc; r++) {
			fscanf(in1, "%d", &indata[r]);
			//printf("\n patchID is:: %d \n",indata[r]);
			if (indata[r] > max)
				max = indata[r];
		}

		//printf("\n Max for %s is %d", filename, max);
		fclose(in1);
	}
	return;
}


//===============================================================================================================================
//        input_ascii_float() - input an ascii image into an float array using the (row, col) coordinates maxr and maxc.
//===============================================================================================================================
void input_ascii_float(float *indata, char *filename, int mc, int mr, int arc_flag)
{
	FILE *in1;
	int  r;
	float max;
	void    pause();

	max = 0;
	if ((in1 = fopen(filename, "r")) == NULL) {
		printf("cannot open file %s\n", filename);
		pause();
	}
	else {
		/* skip header */
		if (arc_flag == 0)
			for (r = 0; r < LEN_GRASSHEADER; r++)
				fscanf(in1, "%*s");
		else
			for (r = 0; r < LEN_ARCHEADER; r++)
				fscanf(in1, "%*s");

		for (r = 0; r < mr*mc; r++) {
			fscanf(in1, "%f", &indata[r]);

			indata[r] = (float)(indata[r]);
			//printf("dem is %f \n",indata[r]);
			//getchar();
			if (indata[r] > max)
				max = indata[r];
		}

		//printf("\n Max for %s is %f", filename, max);
		fclose(in1);
	}
	return;
}

//===============================================================================================================================
//        input_header() - input information (row, col) from [root].header
//===============================================================================================================================
void input_header(char *fndem, int arc_flag, struct  InputGridData *InputGridData) {
	FILE  *in1;
	char tmp[100];
	int i;
	void    header_help(int, int, char*);

	if ((in1 = fopen(fndem, "r")) == NULL) {
		fprintf(stderr, "cannot open file %s\n\n", fndem);
		header_help(InputGridData->rows, InputGridData->cols, fndem);
	}
	else {
		if (arc_flag == 0) {
			for (i = 0; i< 9; i++)
				fscanf(in1, "%s", tmp);
			fscanf(in1, "%d %*s %d", &InputGridData->rows, &InputGridData->cols);
		}
		else {
			fscanf(in1, "%*s %d %*s %d ", &InputGridData->cols, &InputGridData->rows);
			fscanf(in1, "%*s %lf %*s %lf", &InputGridData->xll, &InputGridData->yll);
			fscanf(in1, "%*s %lf %*s %lf", &InputGridData->cellsize, &InputGridData->no_data_value);

		}
		fclose(in1);
	}
	return;
}


//===============================================================================================================================
//        header_help() - inform user they need a [root].header file
//===============================================================================================================================

void header_help(int maxr, int maxc, char *fnhdr) {
	FILE  *in1;

	fprintf(stderr, "\n\n I cannot open the .hdr file to retreive the row and\n");
	fprintf(stderr, " column coordinates. Please enter them now, they will\n");
	fprintf(stderr, " be stored in a new header file called %s.\n\n", fnhdr);
	fprintf(stderr, " Coordinates are (row col) : ");
	scanf("%d %d", &maxr, &maxc);

	if ((in1 = fopen(fnhdr, "w")) == NULL) {
		fprintf(stderr, "I still cannot open file %s, exiting program...\n", fnhdr);
		exit(1);
	}
	else
	{
		fprintf(in1, "%6d %6d", maxr, maxc);
		fclose(in1);
	}

	return;
}


//===============================================================================================================================
//        input_prompt() - input root filename, create full filenames
//===============================================================================================================================
void	read_geo_images(struct patch_object *patch, struct CommandLineObject *ComLin, struct  SimulationInformation* SimInf,
	struct InFilePath *InFilePath, struct  InputGridData *InputGridData) {


	//static parameters and delivered values
	int f_flag = 1, arc_flag = 1;
	char *prefix=SimInf->prefix;
	char *filename = InFilePath->inImgFile;


	// filenames for each image and file
	char  fnpatch[MAXS], fndem[MAXS], fnslope[MAXS], fnaspect[MAXS], fneast_horizon[MAXS], fnwest_horizon[MAXS];
	char  fnsoil[MAXS], fnveg[MAXS], fnroads[MAXS], fnstreamorder[MAXS], fnsthread[MAXS], fncthread[MAXS], fngauges[MAXS];
	char  fnclimate[MAXS], fnlatitude[MAXS], fnreservoir[MAXS], fnpatchorder[MAXS];

	//File pointer
	int          *ppatch;
	float        *pdem;
	float        *pslope;
	float        *paspect;
	float        *peast_horizon;
	float        *pwest_horizon;
	int          *psoil;
	int          *pveg;
	int			*pstreamorder;
	int			*psthread;
	int			*pcthread;
	int         *ppatchorder;
	int			*pgauges;
	int			*pclimate;
	int			*preservoir;
	int          *proads;
	double       *plon;
	double       *plat;//y in cordinate system 
	float		 *platitude;//geography latitude

	//local functions
	void    header_help(int, int, char*);
	void	input_ascii_int(int *, char *, int, int, int);
	void	input_ascii_float(float *, char *, int, int, int);
	void    create_x_coordinates(double *, int, int, double, double);
	void    create_y_coordinates(double *, int, int, double, double);
	void    pause();

	// copy the root filename into the specific filenames
	strcat(filename, prefix);
	strcpy(fnpatch, filename);
	strcpy(fndem, filename);
	strcpy(fnslope, filename);
	strcpy(fnaspect, filename);
	strcpy(fneast_horizon, filename);
	strcpy(fnwest_horizon, filename);
	strcpy(fnsoil, filename);
	strcpy(fnveg, filename);
	strcpy(fnroads, filename);

	//xu.
	strcpy(fnstreamorder, filename);
	strcpy(fnsthread, filename);
	strcpy(fncthread, filename);
	strcpy(fngauges, filename);
	strcpy(fnclimate, filename);
	strcpy(fnlatitude, filename);
	strcpy(fnreservoir, filename);
	strcpy(fnpatchorder, filename);

	// append '.' extensions to each filename (these should be generalized) */
	strcat(fnpatch, ".patch");
	strcat(fndem, ".dem");
	strcat(fnslope, ".slope");
	strcat(fnaspect, ".aspect");
	strcat(fneast_horizon, ".east");
	strcat(fnwest_horizon, ".west");
	strcat(fnsoil, ".soil");
	strcat(fnveg, ".veg");
	strcat(fnroads, ".road");
	strcat(fnstreamorder, ".streamorder");
	strcat(fnsthread, ".sthread");
	strcat(fncthread, ".cthread");
	strcat(fnpatchorder, ".patchorder");
	strcat(fngauges, ".gauge");
	strcat(fnclimate, ".climate");
	strcat(fnlatitude, ".latitude");
	strcat(fnreservoir, ".reservoir");
	char char_thread[10];//thread
	sprintf(char_thread, "%d", SimInf->thread_num);
	strcat(fnsthread, char_thread);
	strcat(fncthread, char_thread);

	//read and deliver values of grid attribute
	input_header(fndem, arc_flag,InputGridData);
	int rows = InputGridData->rows, cols = InputGridData->cols;
	double cellsize = InputGridData->cellsize,xll = InputGridData->xll,  yll = InputGridData->yll;
	int num_patches = SimInf->patch_num;

	// allocate and read input map images
	ppatch = new int [rows*cols];
	input_ascii_int(ppatch, fnpatch, rows, cols, arc_flag);

	plon = new double[rows*cols];
	create_x_coordinates(plon, rows, cols, cellsize, xll);

	plat = new double[rows*cols];
	create_y_coordinates(plat, rows, cols, cellsize, yll);

	pdem = new float[rows*cols];
	input_ascii_float(pdem, fndem, rows, cols, arc_flag);

	pslope = new float[rows*cols];
	input_ascii_float(pslope, fnslope, rows, cols, arc_flag);

	paspect = new float[rows*cols];
	input_ascii_float(paspect, fnaspect, rows, cols, arc_flag);

	peast_horizon = new float[rows*cols];
	input_ascii_float(peast_horizon, fneast_horizon, rows, cols, arc_flag);

	pwest_horizon = new float [rows*cols];
	input_ascii_float(pwest_horizon, fnwest_horizon, rows, cols, arc_flag);

	psoil = new int[rows*cols];
	input_ascii_int(psoil, fnsoil, rows, cols, arc_flag);

	pveg = new int[rows*cols]{};
	input_ascii_int(pveg, fnveg, rows, cols, arc_flag);

	proads = new int[rows*cols]{};
	input_ascii_int(proads, fnroads, rows, cols, arc_flag);

	platitude = new float[rows*cols]{};
	input_ascii_float(platitude, fnlatitude, rows, cols, arc_flag);


	//xu.
	//controlled by ComLin
	// -cf channel flow
	// -re reservoir
	// -gg guage output
	pstreamorder = new int[rows*cols]{};
	if (ComLin->cf == TRUE) {
		input_ascii_int(pstreamorder, fnstreamorder, rows, cols, arc_flag);
	}
	preservoir = new int[rows*cols]{};
	if (ComLin->re == TRUE) {
		input_ascii_int(preservoir, fnreservoir, rows, cols, arc_flag);
	}
	pgauges = new int[rows*cols]{};
	if (ComLin->gg == TRUE) {
		input_ascii_int(pgauges, fngauges, GAUGE_NUM, 1, arc_flag);
	}
	//xu. default extra input
	psthread = new int[rows*cols]{};
	input_ascii_int(psthread, fnsthread, rows, cols, arc_flag);
	pcthread = new int[rows*cols]{};
	input_ascii_int(pcthread, fncthread, rows, cols, arc_flag);
	ppatchorder = new int[rows*cols]{};
	input_ascii_int(ppatchorder, fnpatchorder, rows, cols, arc_flag);
	pclimate = new int[rows*cols]{};
	input_ascii_int(pclimate, fnclimate, rows, cols, arc_flag);

	printf("\n");
	//---------------------------------------------------------------------------------------------------------------------------
	//xu. 1.MATCH PATHES WITH THEIR GEO IMAGIES
	//---------------------------------------------------------------------------------------------------------------------------
	int patchorder;
	for (int imag_inx = 0; imag_inx < cols*rows; imag_inx++) {

		//allcocate address
		int patchorder = ppatchorder[imag_inx];
		//correlate address and input data 
		if (patchorder >=0 ) {
			patch[patchorder].ID = ppatch[imag_inx];
			patch[patchorder].x = plon[imag_inx];
			patch[patchorder].y = plat[imag_inx];
			patch[patchorder].z = pdem[imag_inx];
			patch[patchorder].slope = pslope[imag_inx];
			patch[patchorder].aspect = paspect[imag_inx];
			patch[patchorder].e_horizon = peast_horizon[imag_inx];
			patch[patchorder].w_horizon = pwest_horizon[imag_inx];

			//STREAM ORDER (accociated with _channel.def)
			patch[patchorder].streamorder = pstreamorder[imag_inx];

			//SUB-BASIN (THREAD)
			patch[patchorder].sthread = psthread[imag_inx];

			//CHANNEL (THREAD)
			patch[patchorder].cthread = pcthread[imag_inx];
			if (patch[patchorder].cthread > 0) {
				patch[patchorder].clayer = patch[patchorder].cthread / 1000;//e.g. 1004 means 1 layer 4th cthread
				patch[patchorder].cthread = patch[patchorder].cthread % 1000;
			}
			else
				patch[patchorder].clayer = 0;

			//CLIMATE ZONE(accociated with _climate.def)
			patch[patchorder].climatetype = pclimate[imag_inx];

			//LATITUDE
			patch[patchorder].latitude = platitude[imag_inx];

			//DOWNSTREAM RESERVOIR ID (accociated with _reservoir.def)
			patch[patchorder].downslope_reservoir_ID = preservoir[imag_inx];

			patch[patchorder].road = proads[imag_inx];
			patch[patchorder].vegtype = pveg[imag_inx];
			patch[patchorder].soiltype = psoil[imag_inx];

		}
	}

	//---------------------------------------------------------------------------------------------------------------------------
	//xu. 2.MATCH PATHES WITH GAUGE_LISTS
	//---------------------------------------------------------------------------------------------------------------------------
	if (ComLin->gg == TRUE) {
		for (int gauge_inx = 0; gauge_inx != GAUGE_NUM; gauge_inx++) {

			SimInf->gauge_list[gauge_inx] = pgauges[gauge_inx];

			for (int patch_inx = 0; patch_inx != num_patches; patch_inx++) {

				if (patch[patch_inx].ID == SimInf->gauge_list[gauge_inx]) {	//where the list was replaced with patch_inx
					SimInf->gauge_list[gauge_inx] = patch_inx;
					break;
				}
			}
		}
	}


	//free matrix memories
	delete ppatch;delete pdem ;delete pslope ;delete paspect ;delete peast_horizon ;
	delete pwest_horizon ;delete psoil ;delete pveg ;
	delete pstreamorder ;delete psthread ;delete pcthread ;delete pgauges ;delete pclimate ;
	delete platitude; delete proads; delete plon; delete plat; delete ppatchorder;

	//deliver cellsize values to SimInf
	SimInf->cell_size = InputGridData->cellsize;

	//printf("\nFinishing reading_geo_images.cpp\n");
	return;
}


//===============================================================================================================================
//        pause() - stop program until user presses enter (return)
//===============================================================================================================================
void pause()
{
	/* clear buffer first */

	while (getchar() != '\n')
		;
	printf("\nPress enter to continue...");
	getchar();
}


