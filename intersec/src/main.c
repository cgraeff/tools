//
//  main.c
//  binod
//
//  Created by Clebson Graeff on 2016-09-30.
//  Copyright © 2016 Clebson Graeff. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <gsl/gsl_vector.h>

#include "CommandlineOptions.h"
#include "AuxiliaryFunctions.h"
#include "Maps.h"

int main(int argc, char * argv[])
{
    CommandlineOptionsParse(argc, argv);

	if(options.file_1 == NULL || options.file_2 == NULL){
		printf("Give both files.\n");
		abort();
	}
		
	// Read files
	gsl_vector *f1_abscissas = NULL;
	gsl_vector *f1_ordinates = NULL;
	gsl_vector *f2_abscissas = NULL;
	gsl_vector *f2_ordinates = NULL;
	
	ReadDataFile(options.file_1, &f1_abscissas, &f1_ordinates);
	ReadDataFile(options.file_2, &f2_abscissas, &f2_ordinates);
	
	for (int i = 0; i < f2_abscissas->size; i++)
		gsl_vector_set(f2_abscissas, i, 3.0 * gsl_vector_get(f2_abscissas, i));

	TrimVectorsAtStartByValue(&f1_abscissas, &f1_ordinates, 1000);
	TrimVectorsAtStartByValue(&f2_abscissas, &f2_ordinates, 1000);
	
	// Get intersections
	double x_intersection_return;
	double y_intersection_return;
	IntersectionPointOfTwoMaps(f1_abscissas,
                               f1_ordinates,
                               f1_abscissas->size,
                               f2_abscissas,
                               f2_ordinates,
                               f2_abscissas->size,
                               &x_intersection_return,
                               &y_intersection_return);
                          
    // Free vectors
    gsl_vector_free(f1_abscissas);
    gsl_vector_free(f1_ordinates);
    gsl_vector_free(f2_abscissas);
    gsl_vector_free(f2_ordinates);
    
    // Print intersections
    if (options.print_x_only){
  		printf("%f\n", x_intersection_return);
  	}
  	else{
		printf("Intersection: %f\t%f\n",
			   x_intersection_return,
			   y_intersection_return);
	}
    	   
  	
                                
    return 0;
}

