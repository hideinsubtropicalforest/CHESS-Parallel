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

void  chess_channel_daily(struct patch_object* patch,
	struct CommandLineObject* ComLin,
	struct  SimulationInformation* SimInf,
	struct	date	current_date,
	struct  daily_clim_object* daily_clim

)
{
	//---------------------------------------------------------------------------------------------------------------------------
	//xu. The patch_daily process are processed in separated sub-basins by ASP(Automatic Sub-basin Partition)
	//---------------------------------------------------------------------------------------------------------------------------

	//creat thread
	/*
	//init and run all thread
	for (int layer_inx = 0; layer_inx != parallel->layer_num; layer_inx++) {
		std::thread *thd = new std::thread[parallel->thread_num];
		for (int thread_inx = 0; thread_inx != parallel->thread_num; thread_inx++) {

			//parallel patch daily is served to be a elementary function of patch daily process
			thd[thread_inx] = thread(parallel_channel_daily, patch, ComLin, current_date, parallel, layer_inx, thread_inx);
		}
		//wait till all threads terminate
		for (int thread_inx = 0; thread_inx != parallel->thread_num; thread_inx++) {
			thd[thread_inx].join();
		}
		delete[] thd;
	}
	*/
	
	//init and run all thread
	int thread_num = SimInf->thread_num;
	for (int layer_inx = 0; layer_inx != SimInf->layer_num; layer_inx++) {
		
		#pragma omp parallel for num_threads(thread_num)
		for (int thread_inx = 0; thread_inx < SimInf->thread_num; thread_inx++) {
		parallel_channel_daily(patch, ComLin, SimInf,current_date, daily_clim, layer_inx, thread_inx);
		}
	}


	return;
}//__END OF DAILY PATCH