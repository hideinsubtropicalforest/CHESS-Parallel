/*--------------------------------------------------------------*/
/* 																*/
/*						validate_option							*/
/*																*/
/*	validate_option.c - makes sure a option is OK				*/
/*																*/
/*	NAME														*/
/*	validate_option.c - makes sure a option is OK				*/
/*																*/
/*	SYNOPSIS													*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	The routine performs as follows:							*/
/*																*/
/*	June 3, 1997 C.Tague					*/
/*	- changed from multipl. to if statement			*/
/*	 to check for valid (zero) option			*/
/*																*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include <string.h>
#include "functions.h"
#include "chess.h"

int	 valid_option( char *ComLin){
	/*------------------------------------------------------*/
	/*	Local Function Declarations.						*/
	/*------------------------------------------------------*/
	
	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	int i;
	/*--------------------------------------------------------------*/
	/*	Set i to zero if it matches and valid options.				*/
	/*--------------------------------------------------------------*/
	i = 1;
	if (( strcmp(ComLin,"bgc") == 0) ||
		(strcmp(ComLin,"-v")  == 0) ||
		(strcmp(ComLin,"-e")  == 0) ||
		(strcmp(ComLin,"-b")  == 0) ||
		(strcmp(ComLin,"-h")  == 0) ||
		(strcmp(ComLin,"-p")  == 0) ||
		(strcmp(ComLin,"-g")  == 0) ||
		(strcmp(ComLin,"-c")  == 0) ||
		(strcmp(ComLin,"-o")  == 0) ||
		(strcmp(ComLin,"-w")  == 0) ||
		(strcmp(ComLin,"-r")  == 0) ||
		(strcmp(ComLin,"-t")  == 0) ||
		(strcmp(ComLin,"-s")  == 0) ||
		(strcmp(ComLin,"-z")  == 0) ||
		(strcmp(ComLin,"-sv")  == 0) ||
		(strcmp(ComLin,"-st")  == 0) ||
		(strcmp(ComLin,"-th")  == 0) ||
		(strcmp(ComLin,"-ed")  == 0) ||
		(strcmp(ComLin,"-tmp")  == 0) ||
		(strcmp(ComLin,"-gw")  == 0) ||
		(strcmp(ComLin,"-tchange")  == 0) ||
		(strcmp(ComLin,"-pre") == 0) ||
		(strcmp(ComLin,"-rddn")  == 0) ||
		(strcmp(ComLin,"-std") == 0) ||
		(strcmp(ComLin,"-dor") == 0) ||
		(strcmp(ComLin,"-csv") == 0) ||
		(strcmp(ComLin,"-vgsen") == 0) ||
		(strcmp(ComLin,"-vmort") == 0) ||
		(strcmp(ComLin,"-svalt") == 0) ||
		(strcmp(ComLin,"-snowdistb") == 0) ||
		(strcmp(ComLin,"-template") == 0))
		i = 0;
	if ( i == 0 ){
		return(1);
	}
	else{
		return(0);
	}
} /*end validate_option.c*/
