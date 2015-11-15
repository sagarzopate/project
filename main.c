
/*
Copyright (C) <2015>  <Sagar Zopate>
    This file is part of Text Editor.

    Text Editor program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/#include <stdarg.h>
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
