//
//  CommandlineOptions.h
//  quarks EOS
//
//  Created by Clebson Graeff on 2016-03-20.
//  Copyright © 2016 Clebson Graeff. All rights reserved.
//

#ifndef CommandlineOptions_h
#define CommandlineOptions_h

#include <stdbool.h>

typedef struct _options{
	// List options and flags that will be acessible
	// during the execution
	char * file_1;
	char * file_2;
	bool print_x_only;
} Options;

extern Options options;

int CommandlineOptionsParse(int argc, char * argv[]);
void CommandlineOptionsPrintUsage();

#endif /* CommandlineOptions.h */
