//---------------------------------------------------------------------------------------------------------------------------
//xu. LAND.. PARALLEL COMPUTATION.. Initial and run parallel CHESS daily Ecohydrological process and Transport of water and nutrients
//---------------------------------------------------------------------------------------------------------------------------
#include <stdio.h>

#include <math.h>
#include "chess.h"
#include "functions.h"
#include "constants.h"
#include <iostream>
#include <thread>
#include <omp.h>

//using std::thread;

void  chess_patch_daily(struct patch_object* patch,
	struct CommandLineObject* ComLin,
	struct  SimulationInformation* SimInf,
	struct	date	current_date,
	struct  daily_clim_object* daily_clim
)
{
	//init and run all thread
	int thread_num = SimInf->thread_num;
	#pragma omp parallel for num_threads(thread_num)
	for (int thread_inx = 0; thread_inx < thread_num; thread_inx++) {

		//parallel patch daily is served to be a elementary function of patch daily process
		parallel_hillslope_subunit(patch, ComLin, SimInf,current_date, daily_clim, thread_inx);
		//printf("%d %d %d\n", thread_num, omp_get_num_threads(), thread_inx);
	}//*/

	return;
}