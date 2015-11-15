/*
Copyright (C) <2015>  <Sagar Zopate>
    This file is part of Text Editor

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

*/
#include <gtk/gtk.h>
typedef struct _GedWindow {
	GtkWidget *window;
  	GtkWidget *close_dialog; 
  	GtkWidget *file_selector;
  	GtkWidget *text_view;
  	char *curr_filename; 
  	gboolean closing;
  	GtkAccelGroup *accel_group;
} GedWindow;
typedef void (*menu_function)(GtkMenuItem*,gpointer);
/*public function declarations*/
GtkWidget *create_menu(GedWindow *self,const char *name,...);
void set_curr_filename(GedWindow *self,const char *new);
void do_quit(GedWindow *self);
void open_file(GedWindow *self,const char *filename);
void save_file(GedWindow *self,const char *filename);
gchar *get_buffer_text(GedWindow *self);
GtkAccelGroup *curr_accel_group(GedWindow *self);
GedWindow *draw_ged_window();

/*callback function declarations*/
void close_dialog_answered(GtkDialog *dialog,gint id,gpointer data);
gboolean user_quit(GtkWidget *window,
		   GdkEvent *event, gpointer user_data);
void new_menuitem_clicked(GtkMenuItem *,gpointer);
void open_menuitem_clicked(GtkMenuItem *,gpointer);
void open_selected_file(GtkButton *,gpointer);
void save_menuitem_clicked(GtkMenuItem *,gpointer);
void save_as_menuitem_clicked(GtkMenuItem *,gpointer);
void save_selected_file(GtkButton *,gpointer);
void quit_menuitem_clicked(GtkMenuItem *,gpointer);
void cut_menuitem_clicked(GtkMenuItem *,gpointer);
void copy_menuitem_clicked(GtkMenuItem *,gpointer);
void paste_menuitem_clicked(GtkMenuItem *,gpointer);
void contents_menuitem_clicked(GtkMenuItem *,gpointer);

/*our actual function definitions:*/



