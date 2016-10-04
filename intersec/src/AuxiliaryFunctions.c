//
//  AuxiliaryFunctions.c
//  quarks EOS
//
//  Created by Clebson Graeff on 2016-03-17.
//  Copyright © 2016 Clebson Graeff. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

#include "AuxiliaryFunctions.h"

int WriteVectorsToFileUpToIndex(const char * filename, const char * header, int vector_index, int vectors_count, ...)
{
    FILE * output = OpenFile(filename);
    
    fprintf(output, "%s", header);
    
    va_list arg_list;
    va_start(arg_list, vectors_count);
    
    gsl_vector * vectors[vectors_count];
    
    for (int i = 0; i < vectors_count; i++){
        gsl_vector * v = va_arg(arg_list, gsl_vector *);
        vectors[i] = v;
    }
    
    va_end(arg_list);
    
    if (vectors_count > 1)
        for (int i = 0; i < vectors_count - 1; i++)
            if ((vectors[i])->size != (vectors[i + 1])->size) {
                
                printf("ERROR: Vectors have different sizes.\n");
                exit(EXIT_FAILURE);
            }
    
    for (int i = 0; i < vector_index; i++) {
        for (int j = 0; j < vectors_count; j++) {
            
            double x = gsl_vector_get(vectors[j], i);
            
            fprintf(output, "%20.15E", x);
            
            if (j != vectors_count - 1)
                fprintf(output, "\t");
        }
        
        fprintf(output, "\n");
    }
    
    fclose(output);
    
    return 0;
}

int WriteVectorsToFile(const char * filename, const char * header, int vectors_count, ...)
{
    FILE * output = OpenFile(filename);
	
	fprintf(output, "%s", header);
	
	va_list arg_list;
	va_start(arg_list, vectors_count);
	
	gsl_vector * vectors[vectors_count];
	
	for (int i = 0; i < vectors_count; i++){
		gsl_vector * v = va_arg(arg_list, gsl_vector *);
		vectors[i] = v;
	}
	
	va_end(arg_list);
	
	if (vectors_count > 1)
		for (int i = 0; i < vectors_count - 1; i++)
			if ((vectors[i])->size != (vectors[i + 1])->size) {
				
				printf("ERROR: Vectors have different sizes.\n");
				exit(EXIT_FAILURE);
			}
	
	for (int i = 0; i < (vectors[0])->size; i++) {
		for (int j = 0; j < vectors_count; j++) {
			
			double x = gsl_vector_get(vectors[j], i);
			
			fprintf(output, "%20.15E", x);
			
			if (j != vectors_count - 1)
				fprintf(output, "\t");
		}
		
		fprintf(output, "\n");
	}
	
	fclose(output);
	
	return 0;
}

int WriteIndexedVectorsToFile(const char * filename, const char * header, int vectors_count, ...)
{
    FILE * output = OpenFile(filename);
	
	fprintf(output, "%s", header);
	
	va_list arg_list;
	va_start(arg_list, vectors_count);
	
	gsl_vector * vectors[vectors_count];
	
	for (int i = 0; i < vectors_count; i++) {
		vectors[i] = va_arg(arg_list, gsl_vector *);
	}
	
	va_end(arg_list);
	
	if (vectors_count > 1)
		for (int i = 0; i < vectors_count - 1; i++)
			if ((vectors[i])->size != (vectors[i + 1])->size) {
				
				printf("ERROR: Vectors have different sizes.\n");
				exit(EXIT_FAILURE);
			}
	
	for (int i = 0; i < (vectors[0])->size; i++) {
		
		fprintf(output, "%d\t", i);
		
		for (int j = 0; j < vectors_count; j++) {
			
			fprintf(output, "%20.15E", gsl_vector_get(vectors[j], i));
			
			if (j != vectors_count - 1)
				fprintf(output, "\t");
		}
		
		fprintf(output, "\n");
	}
	
	fclose(output);
	
	return 0;
}

gsl_vector * VectorNewVectorFromDivisionElementByElement(gsl_vector * numerator, gsl_vector * denominator)
{
	if (numerator->size != denominator->size) {
		printf("The vectors must have the same size!\n");
		exit(EXIT_FAILURE);
	}
	
	gsl_vector * v = gsl_vector_alloc(numerator->size);
	
	for (int i = 0; i < numerator->size; i++){
		double value = gsl_vector_get(numerator, i) / gsl_vector_get(denominator, i);
		gsl_vector_set(v, i, value);
	}

	return v;
}

static const char * open_file_prefix_path = NULL;

FILE * OpenFile(const char filename[])
{
    /*
     * Check filename for '/' or overflow
     */
    if (NULL != strrchr(filename, '/')){
        printf("OpenFile: Filename contains a path element. This is not supported.\n");
        abort();
    }

    if (strlen(filename) > FILENAME_MAX_SIZE){
        printf("OpenFile: Filename is too long.\n");
        abort();
    }

    // If there is not prefix path,
    // just open the file
    if (open_file_prefix_path == NULL){
        FILE * file = fopen(filename, "w");

        if (NULL == file) {
            printf("Could not open %s for writting.\n", filename);
            perror("Reason");
            exit(EXIT_FAILURE);
        }
        
        return file;
    }
    
    /*
     * Recursivelly create dirs in path
     */
    char tmp[PATH_MAX_SIZE];

    int length = snprintf(tmp, sizeof(tmp),"%s", open_file_prefix_path);

    // Check for final slash in path
    // add if it isn't there
    if (tmp[length - 1] != '/'){

        // Check if bounds will be exceded
        // (the + 1 account for the /)
        if (length + 1 >= PATH_MAX_SIZE){
            printf("OpenFile: Path length is too long.\n");
            abort();
        }
        tmp[length] = '/';
        tmp[length + 1] = 0; // End of string must be null
    }

    // Create dirs:
    //  Reads the string sequentially replacing occurences of '/' with null ('\0')
    //  (this will effectivelly limit the string to characters before the '/' being replaced)
    //  checking if the dir pointed by de resulting string exists, creating if it doesn't exist,
    //  restoring the modified '/' (and moving to the next occurence of '/'. The loop will
    //  continue ultil the pointer poinst to the final null character, whose value can be
    //  obtained by deference and is zero, ending the loop.
    char *pointer = NULL;
    for(pointer = tmp; *pointer; pointer++)
        if(*pointer == '/') {
            *pointer = 0;
            struct stat st = {0};
            if (stat(tmp, &st) == -1)
                mkdir(tmp, S_IRWXU);
            *pointer = '/';
        }
    
    // Append filename to path.
    // Use tmp as it will have the final '/'
    char filepath[FILEPATH_MAX_SIZE];
    strcpy(filepath, tmp);

    unsigned long path_length = strlen(filepath);
    unsigned long filename_length = strlen(filename);
    unsigned long total_length = path_length + filename_length;

    // Check if bounds will be exceded
    if (total_length >= FILEPATH_MAX_SIZE){
        printf("OpenFile: Path + filename length is too long.\n");
        abort();
    }

    strcat(filepath, filename);

    // Finally, open the file
    FILE * file = fopen(filepath, "w");
    
    if (NULL == file) {
        printf("OpenFile: Could not open %s for writting.\n", filename);
        perror("Reason");
        exit(EXIT_FAILURE);
    }
    
    return file;
}

void SetFilePath(const char path[])
{
    if (NULL == path){
      open_file_prefix_path = NULL;
      return;
    }

    switch (path[0]){
    case '~':
        printf("SetFilePath: ~ expansion is not supported.\n");
        abort();
        break;
    case '.':
        printf("SetFilePath: . is unnecessary, .. is unsuported.\n");
    }

    open_file_prefix_path = path;
}

int GetFields(char   *line,
			  int 	  expected_num_fields,
			  double  fields[]);
			  
int ReadDataFile(char 		 *filename,
				 gsl_vector **return_abscissas, 
				 gsl_vector **return_ordinates)
{
	char s_filename[FILENAME_MAX_SIZE];
	sprintf(s_filename, "%s", filename);
	
	FILE * file = fopen(s_filename, "r");
	
	if (file == NULL){
		printf("ReadDataFile: Could not open file for reading.\n");
		printf("[filename = %s]\n", s_filename);
		perror("Error: ");
		abort();
	}
	
	char line[FILE_MAX_LINE_WIDTH];
	double buffer_x[BUFFER_SIZE];
	double buffer_y[BUFFER_SIZE];
	
	int number_of_points = 0;
	
	/* Read and analyse all records of the file */
	for (;;){
		
		// Read record and test for EOF or any errors
		if (fgets(line, FILE_MAX_LINE_WIDTH, file) == NULL){
			
			// Tests wheather we got to end of file.
			// If true, break the infinite loop;
			if (feof(file) != 0)
				break;
			
			printf("Error reading densities file.\n");
			abort();
		}
		
		// Check for comments and suppress them
		char * ptr;
		if ((ptr = strpbrk(line, "#")) != NULL)
			*ptr = '\0';
			
		// Get data
		int expected_num_fields = 2;
		double fields[2];
		int status = GetFields(line, expected_num_fields, fields);
		
		if (status == 0){
			if (number_of_points == BUFFER_SIZE){
				printf("Too many data points,"
					   "increase BUFFER_SIZE.\n");
				abort();
			}
			
			buffer_x[number_of_points] = fields[0];
			buffer_y[number_of_points] = fields[1];
			number_of_points++;
		}
	}
	
	*return_abscissas = gsl_vector_alloc(number_of_points);
	*return_ordinates = gsl_vector_alloc(number_of_points);
	
	for (int i = 0; i < number_of_points; i++){
		gsl_vector_set(*return_abscissas, i, buffer_x[i]);
		gsl_vector_set(*return_ordinates, i, buffer_y[i]);
	}
	
	return 0;
}

int GetFields(char   *line,
			  int 	  expected_num_fields,
			  double  fields[])
{
	char field_separators[] = " \t";
	char string[FILE_MAX_LINE_WIDTH];
	strcpy(string, line);
	
	// Try to get first field
	char * field;
	field = strtok(string, field_separators);
	
	// Test to see if the line has a field
	if (field == NULL)
		return -1;		// The line has no fields 
	
	// We got a field, save it
	int num_fields = 1;
	fields[num_fields - 1] = atof(field);
	

	while ((field = strtok(NULL, field_separators)) != NULL){
	
		num_fields++;
		
		if (num_fields > expected_num_fields){
			printf("The line\n\t%s\nhas too many fields.\n"
			"(expected %d fields)\n",
			line,
			expected_num_fields);
			
			abort();
		}
		
		fields[num_fields - 1] = atof(field);
	}
	
	return 0;
}

void TrimVectorsAtStartByValue(gsl_vector ** a_vector, 
							   gsl_vector ** another_vector,
							   double value)
{
	int trim = 0;
	
	for (int i = 0; i < (*a_vector)->size; i++){
	
		trim++;
		
		double x = gsl_vector_get(*a_vector, i);
		
		if ( x >= value)
			break;
	}
			
	gsl_vector * a_trimmed = gsl_vector_alloc((*a_vector)->size - trim);
	gsl_vector * another_trimmed = gsl_vector_alloc((*another_vector)->size - trim);
	
	if (a_trimmed->size != another_trimmed->size){
		printf("ahn?\n");
		abort();
	}
	
	for (int i = 0; i < a_trimmed->size; i++){
		double a_value = gsl_vector_get(*a_vector, i + trim);
		double another_value = gsl_vector_get(*another_vector, i + trim);
		
		gsl_vector_set(a_trimmed, i, a_value);
		gsl_vector_set(another_trimmed, i, another_value);
	}
	
	gsl_vector_free(*a_vector);
	gsl_vector_free(*another_vector);
	
	*a_vector = a_trimmed;
	*another_vector = another_trimmed;
}


















