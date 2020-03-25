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
	//---------------------------------------------------------------------------------------------------------------------------
	//xu. The patch_daily process are processed in separated sub-basins by ASP(Automatic Sub-basin Partition)
	//---------------------------------------------------------------------------------------------------------------------------
	/*
	//creat thread
	std::thread *thd=new std::thread [parallel->thread_num];
	//init and run all thread
	for (int thread_inx = 0; thread_inx != parallel->thread_num; thread_inx++) {

		//parallel patch daily is served to be a elementary function of patch daily process
		thd[thread_inx] = thread(parallel_patch_daily, patch, ComLin, current_date, daily_clim,parallel, thread_inx);
	}
	//wait till all threads terminate
	for (int thread_inx = 0; thread_inx != parallel->thread_num; thread_inx++) {
		thd[thread_inx].join();
	}
	delete[] thd;
	*/
	///*

	//
	int thread_num = SimInf->thread_num;
	#pragma omp parallel for num_threads(thread_num)
	for (int patch_inx = 0; patch_inx < SimInf->patch_num; patch_inx++) {
		zero_patch_object(&patch[patch_inx]);
	}


	//init and run all thread
	#pragma omp parallel for num_threads(thread_num)
	for (int thread_inx = 0; thread_inx < thread_num; thread_inx++) {

		//parallel patch daily is served to be a elementary function of patch daily process

		parallel_patch_daily(patch, ComLin, SimInf,current_date, daily_clim, thread_inx);
		//printf("%d\n", omp_get_num_threads());
	}//*/

	
	return;
}//__END OF DAILY PATCH