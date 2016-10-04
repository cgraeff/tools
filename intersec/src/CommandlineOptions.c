//
//	CommandlineOptions.c
//  quarks EOS
//
//  Created by Clebson Graeff on 2016-03-20.
//  Copyright © 2016 Clebson Graeff. All rights reserved.
//

#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#include "CommandlineOptions.h"

// Default values for options and flags that will be acessible
// during the execution (specified in order of declaration).
Options options = {NULL, NULL, false};

int CommandlineOptionsParse(int argc, char * argv[])
{
	// Short options must be declared in the next variable.
	// In short_options, each short option is declared in a string by the character that
	// will invoke the option. If the option takes an argument, a colon (:) must be
	// follow the character. When a character contained in this string is found in the
	// options, the number that corresponds to this caracter is returned by getopt()
	// (that is "a" -> 'a', ...).
	// EXAMPLE:
	// char * short_options = "a:bvuh";
	//
	// BEWARE: if an option takes many arguments, the spaces must be escaped, otherwise
	// the arguments after the first will be misinterpreted as unknown, or unclaimed.
	// This particular implementation will stop if there are any unprocessed arguments.
	
	char * short_options = "f:s:xuh";

	int opt;
	while ((opt = getopt(argc, argv, short_options)) != -1){

		// If an option have an argument, it is accessed through 'optarg'
		switch (opt){
			case 'f':
				options.file_1 = optarg;
				break;
            case 's':
                options.file_2 = optarg;
                break;
            case 'x':
            	options.print_x_only = true;
            	break;
			case 'u':
				CommandlineOptionsPrintUsage();
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				CommandlineOptionsPrintUsage();
				exit(EXIT_SUCCESS);
				break;
			case '?':
		  		if (optopt == 'p'){
          			fprintf (stderr,
							 "Option -%c requires an argument. Use -h for help.\n",
							 optopt);
				}else if (isprint (optopt)){
          			fprintf (stderr,
							 "Unknown option `-%c'.  Use -h for help.\n",
							 optopt);
				}else{
          			fprintf (stderr,
							 "Unknown option character `\\x%x'.  Use -h for help.\n",
							 optopt);
				}
				exit(EXIT_FAILURE);
				break;
			default:
				printf("Use -h to print a list of accepted options.\n");
				exit(EXIT_FAILURE);
		}
	}


	// Print any remaining command line arguments (not options)
	// and let the user know that they are invalid
	if (optind < argc){
		printf ("%s: invalid arguments -- ", argv[0]);
		while (optind < argc)
			printf ("%s ", argv[optind++]);
		putchar ('\n');
		printf("Use -h or -u to print a list of accepted options.\n");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

void CommandlineOptionsPrintUsage()
{
    printf("This code calculates the intersection of two data sets\n\n");
	printf("Usage: intersection [options]\n");
	printf("Options:\n"
		   "\t-f First set\n"
		   "\t-s Second set\n"
		   "\t-x Print x intersection only\n");
	
	return;
}

