/*--------------------------------------------------------------*/
/* 																*/
/*					construct_ComLin						*/
/*																*/
/*	construct_ComLin->c - creates command line object		*/
/*																*/
/*	NAME														*/
/*	construct_ComLin->c - creates command line object		*/
/*																*/
/*	SYNOPSIS													*/
/*	struct	CommandLineObject	*command_line(		*/
/*								argc, argv, ComLin)		*/
/*																*/
/*	OPTIONS														*/
/*																*/
/*	DESCRIPTION													*/
/*																*/
/*	Creates a ComLin object which consists of flags		*/
/*	entered on the command line during execution of rhessys.	*/
/*	Some error checking is performed but error checking must	*/
/*	wait until the world object has been specified.				*/
/*																*/
/*	PROGRAMMER NOTES											*/
/*																*/
/*	Original code, January 15, 1996.							*/
/*	valid_option to be written still - determines if the next	*/
/*			arguement is a valid option.						*/
/*	added routing option - May 7, 1997, C.Tague					*/
/*																*/
/*																*/
/*	Sep 1997	RAF												*/
/*	Removed extended output option flag as all output is		*/
/*	now of a single format specified by output routines.		*/
/*								*/
/*	Sept 1998	C.Tague					*/
/* 	added comma separated output option			*/
/*--------------------------------------------------------------*/
#include <stdio.h>

#include <string.h>
#include "functions.h"
#include "chess.h"

struct CommandLineObject *construct_command_line(int main_argc,	char **main_argv,CommandLineObject *ComLin)
{
	/*--------------------------------------------------------------*/
	/*	Local function definition.									*/
	/*--------------------------------------------------------------*/
	int	    valid_option( char * );
	void	*alloc( size_t, char *, char * );
	
	/*--------------------------------------------------------------*/
	/*	Local variable definition.									*/
	/*--------------------------------------------------------------*/
	int		i;
	
	/*--------------------------------------------------------------*/
	/*	Initialize the options as null				*/
	/*--------------------------------------------------------------*/
	ComLin->grow_flag = 1;
	ComLin->road_flag = 1;
	ComLin->verbose_flag = 0;
	ComLin->routing_flag = 1;
	ComLin->tec_flag = 0;
	ComLin->vmort_flag = 0;
	ComLin->gw_flag = 0;
	ComLin->b = true;
	ComLin->p = NULL;
	ComLin->gg = NULL;
	ComLin->cf = NULL;
	ComLin->re = NULL;
	ComLin->pmon = NULL;
	ComLin->pday = NULL;

	/*-------------------------------------------------*/
	/*	Loop through each arguement in the command line.*/
	/*-------------------------------------------------*/
	i = 1;
	while  ( i < main_argc){
		/*------------------------------------------*/
		/*		Check if the verbose flag is next.    */
		/*------------------------------------------*/
		if ( i< main_argc ){
			if ( strcmp(main_argv[i],"-v") == 0 ){
				/*-----------------------------------------------*/
				/*			Check if "-v" was the last agruement.   */
				/*-----------------------------------------------*/
				i++;
				if ( i == main_argc ){
					/*------------------------------------------*/
					/* assume we want verbose level 1			  */
					/*------------------------------------------*/
					ComLin->verbose_flag= 1;
				}
				else if ( valid_option(main_argv[i]) == 1 ){
					/*----------------------------------------------*/
					/*	check if the next arguement is an option.		*/
					/*----------------------------------------------*/
					ComLin->verbose_flag= 1;
				}
				else{
					/*-------------------------------------------------*/
					/*	read in the value of the verbose level.			*/
					/*-------------------------------------------------*/
					ComLin->verbose_flag = (int)atoi(main_argv[i]);
					i++;
				}/*end if*/
			}/*end if*/

			/*------------------------------------------*/
			/*Check if the variable mortality flag is next.           */
			/*------------------------------------------*/
			else if ( strcmp(main_argv[i],"-vmort") == 0 ){
				ComLin->vmort_flag = 1;
				i++;
			}
			/*------------------------------------------*/
			/*Check if the grow flag is next.           */
			/*------------------------------------------*/
			else if ( strcmp(main_argv[i],"-g") == 0 ){
				ComLin->grow_flag = 1;
				i++;
			}
		    /*-------------------------------------------------*/
			/*	groundwater flag and coeffcients	  */
			/*-------------------------------------------------*/
			else if ( strcmp(main_argv[i],"-gw") == 0 ){
				i++;
				ComLin->gw_flag = 1;
				if ((i == main_argc-1) || (valid_option(main_argv[i])==1)){
					fprintf(stderr,"FATAL ERROR: Values for gw coefficients not specified\n");
					exit(0);
				} /*end if*/
				/*-------------------------------*/
				/*Read in the loss to gw rate multiplier values		*/
				/*-------------------------------*/
				ComLin->sat_to_gw_coeff_mult = (double)atof(main_argv[i]);
				i++;
				ComLin->gw_loss_coeff_mult = (double)atof(main_argv[i]);
				i++;
			}/* end if */


			/*--------------------------------------------------------------*/
			/*		Check if the routing option file is next.				*/
			/*--------------------------------------------------------------*/
			else if ( strcmp(main_argv[i],"-r") == 0 ){
				ComLin->routing_flag = 1;

				i++;
			} /*end if*/


			/*--------------------------------------------------------------*/
			/*		Check if the tec file is next.							*/
			/*--------------------------------------------------------------*/
			else if( strcmp(main_argv[i],"-t") == 0 ){
				/*--------------------------------------------------------------*/
				/*			Check that the next arguement exists.				*/
				/*--------------------------------------------------------------*/
				ComLin->tec_flag = 1;
				i++;
				if ((i == main_argc) || (valid_option(main_argv[i])==1) ){
					fprintf(stderr,"FATAL ERROR: TEC file name not specified\n");
					exit(0);
				} /*end if*/

			} /*end if*/
			/*--------------------------------------------------------------*/
			/*		Check if the basin output flag is next.    				*/
			/*--------------------------------------------------------------*/
			else if( strcmp(main_argv[i],"-b") == 0 ){

				ComLin->b = true;
				i++;

			} /*end if*/

			/*--------------------------------------------------------------*/
			/*		Check if the patch output flag is next.  			*/
			/*--------------------------------------------------------------*/
			else if( strcmp(main_argv[i],"-p") == 0 ){
				/*--------------------------------------------------------------*/
				/*			Allocate the patch output specifier.				*/
				/*--------------------------------------------------------------*/
				ComLin->p = true;
				ComLin->pday = true;
				i++;
			} /*end if*/
			/*--------------------------------------------------------------*/
			/*		Check if the patch output flag is next.  			*/
			/*--------------------------------------------------------------*/
			else if (strcmp(main_argv[i], "-pmon") == 0) {
			/*--------------------------------------------------------------*/
			/*			Allocate the patch output specifier.				*/
			/*--------------------------------------------------------------*/
			ComLin->p = true;//both will be activate
			ComLin->pmon = true;
			i++;
			} /*end if*/
			  
			/*--------------------------------------------------------------*/
			/*	GAUGUE OUTPUT  			*/
			/*--------------------------------------------------------------*/
			else if (strcmp(main_argv[i], "-gg") == 0) {
			/*--------------------------------------------------------------*/
			/*			Allocate the patch output specifier.				*/
			/*--------------------------------------------------------------*/
			ComLin->gg = true;
			i++;
			} /*end if*/
			/*--------------------------------------------------------------*/
			/*	CHANNEL FLOW flag  			*/
			/*--------------------------------------------------------------*/
			else if (strcmp(main_argv[i], "-cf") == 0) {
			/*--------------------------------------------------------------*/
			/*			Allocate the patch output specifier.				*/
			/*--------------------------------------------------------------*/
			ComLin->cf = true;
			i++;
			} /*end if*/
			/*--------------------------------------------------------------*/
			/*	RESERVOIR flag  			*/
			/*--------------------------------------------------------------*/
			else if (strcmp(main_argv[i], "-re") == 0) {
			/*--------------------------------------------------------------*/
			/*			Allocate the patch output specifier.				*/
			/*--------------------------------------------------------------*/
			ComLin->re = true;
			i++;
			} /*end if*/
			else{
				fprintf(stderr,
					"FATAL ERROR: in construct_ComLin option #%d is invalid.\n",i);
				fprintf(stderr,"for argument %s\n", main_argv[i]);
				exit(0);
			} /*end if*/
		} /*end if*/
	} /*end while*/

	return(ComLin);
} /*end construct_ComLin*/
