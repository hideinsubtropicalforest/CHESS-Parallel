#include <stdio.h>
#include "functions.h"
#include "chess.h"
#include <unistd.h>
#include "constants.h"

void construct_infile_path(struct  SimulationInformation* SimInf, struct InFilePath* InputFilePath) {


	//local file_locations
	//users can change this part for adptives changes in file system
	char* file_location = new char[120];
	getcwd(file_location, 120);


	char file_prefix[120]{};
	strcpy(file_prefix, file_location);
	strcat(file_prefix, "//");
	strcat(file_prefix, SimInf->prefix);
	strcat(file_prefix, "//");

	strcpy(InputFilePath->inImgFile, file_prefix);
	strcat(InputFilePath->inImgFile, "geo//");

	strcpy(InputFilePath->inClimPath, file_prefix);
	strcat(InputFilePath->inClimPath, "clim//");

	strcpy(InputFilePath->inDefFile, file_prefix);
	strcat(InputFilePath->inDefFile, "defs//");

	strcpy(InputFilePath->outPutPath, file_prefix);
	strcat(InputFilePath->outPutPath, "out//");

	strcpy(InputFilePath->FlowTableName, InputFilePath->inImgFile);
	strcat(InputFilePath->FlowTableName, SimInf->prefix);
	strcat(InputFilePath->FlowTableName, "_flow_table_D8.dat");

	//may not necessary
	strcpy(InputFilePath->inFlowFile, file_prefix);
	strcat(InputFilePath->inFlowFile, "geo//");

}