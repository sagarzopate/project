#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "text.h"
/* multiple files can be opened even by giving arguments to main*/
int main(int argc, char **argv) {
  	int i;
  	gtk_init(&argc, &argv);
  	for(i = 1; i < argc; i++) {
    		open_file(draw_ged_window(), argv[i]);
	}
  	if(argc == 1) {
    		draw_ged_window();
	}
 	gtk_main();
  	return 0;
}
