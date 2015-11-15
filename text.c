/*
Copyright (C) <2015>  <Sagar Zopate>
    This file is part of TextEditor.

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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "text.h"
gint ged_instances = 0;
/* create_menu-
returns an item named name with the menu. Each item is specified by name, accel, function*/
GtkWidget *create_menu(GedWindow *self, const char *name, ...) {
	GtkWidget *menu,  *item;
  	const char *item_name, *accelerator;
  	guint accel_key;
  	GdkModifierType accel_mod;
  	va_list apk; 	//stores the list of arguments of create_menu function  
	menu = gtk_menu_new();
	gtk_menu_set_accel_group(GTK_MENU(menu), curr_accel_group(self));
	va_start(apk, name); 	//the argument list is intialized
  	while(item_name = va_arg(apk, const char *)) { 	//runs till the argument list is empty
      		item = gtk_menu_item_new_with_mnemonic(item_name);
      		if(accelerator = va_arg(apk, const char*)) {	// install accelerator(keyboard functions) on each argument except self
	    		gtk_accelerator_parse(accelerator, &accel_key, &accel_mod);
	    		gtk_widget_add_accelerator(item, "activate",
				       		curr_accel_group(self),
				       		accel_key, accel_mod,
				       		GTK_ACCEL_VISIBLE);
	  	}
		g_signal_connect(item, "activate", // connects item to the CALLBACK function.
				G_CALLBACK(va_arg(apk, menu_function)),
				(void *)self);
      		gtk_menu_append(GTK_MENU(menu), item);
      		gtk_widget_show(item); // item is displayed.
    	}
	item = gtk_menu_item_new_with_mnemonic(name);
	gtk_widget_show(item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),menu);
	return item;
}

/*curr_accel_group -
since we only have one window, we only need one accelerator 
group, so we use this function to renew the accelerator for every call*/
GtkAccelGroup *curr_accel_group(GedWindow *self) {
	if(!self->accel_group)
		self->accel_group = gtk_accel_group_new();
  	return self->accel_group;
}

/*set_curr_filename -
sets the filename new for files*/
void set_curr_filename(GedWindow *self,const char *new) {
 	g_free(self->curr_filename);
  	if(new != NULL) {
      		self->curr_filename = g_malloc(strlen(new)+1);
      		strcpy(self->curr_filename,new);
      		gtk_window_set_title(GTK_WINDOW(self->window),self->curr_filename); // filename is set as title
    	}
  	else {
    		self->curr_filename=0;
	}
}

/*open_file -
Reads a file into the buffer and makes it the current file*/
void open_file(GedWindow *self,const char *filename) {
	FILE *fp;
	GtkTextBuffer *buffer;
  	char *data = 0;
  	size_t data_len = 0;
  	size_t data_offset = 0;
  	size_t read_len = 0;
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
  	if(gtk_text_buffer_get_modified(buffer) || self->curr_filename) {
      		open_file(draw_ged_window(), filename);
      		return;
    	}
  	set_curr_filename(self, filename);
  	if(!(fp = fopen(filename, "r")))
    		return; /*it still changes the current filename, so save will create
	      		a new file. this allows a new file to be created with ged on
	      		the command line*/
  	data = malloc(1); /*null value crashes gtk+, even if it's empty*/
  	while(!feof(fp)) {
    		data_len += 512;
    		data = g_realloc(data, data_len);
    		read_len = fread(data+data_offset, 1, 512, fp);
    		if(read_len != 512) {
			data_len = data_len - 512 + read_len;
			data = g_realloc(data, data_len+1); /*always add one byte so it's never
					   			completely freed.*/
      		}
    		data_offset += read_len;
  	}
  	fclose(fp);
  	data[data_len] = '\0';
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
  	gtk_text_buffer_set_text(buffer, data, data_len);
  	gtk_text_buffer_set_modified(buffer, 0);
}

gchar *get_buffer_text(GedWindow *self) {
  	GtkTextIter start, end;
  	GtkTextBuffer *buffer;
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
  	gtk_text_buffer_get_start_iter(buffer, &start);
  	gtk_text_buffer_get_end_iter(buffer, &end);
  	return gtk_text_buffer_get_text(buffer, &start, &end,0);
}
/*save_file -
saves the file */
void save_file(GedWindow *self,const char *filename)
{
	FILE *fp1;
  	gchar *text, *text_remaining;
	GtkTextBuffer *buf;
	fp1 = fopen(filename, "w");
  	text = get_buffer_text(self);
  	fprintf(fp1, "%s\n", text);
  	g_free(text);
  	/*we want to save before closing, so we close now*/
  	if(self->closing)
    		do_quit(self);
  	else {
      	/*it hasn't been modified since the last save and won't need saving 
	when we quit*/
      		buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
      		gtk_text_buffer_set_modified(buf,0);
      		/*and the current filename should be what we just saved to*/
      		set_curr_filename(self,filename);
    	}
}



/*initializing a new window */
GedWindow *draw_ged_window() {
 	GtkWidget *container;
	GtkWidget *menu_bar;
	GtkWidget *text_area;
  	GedWindow *self;
  	self = g_malloc(sizeof(GedWindow)); 
  	self->curr_filename = 0;      //initializing datasructure elements    
  	self->closing = 0;                  
  	self->file_selector = 0;         
 	self->accel_group = 0;              
  	self->close_dialog = 0;             
  
  	container = gtk_vbox_new(0,0);
  
  	menu_bar = gtk_menu_bar_new();
  	gtk_widget_show(menu_bar);
  	gtk_menu_bar_append(menu_bar,
		      	create_menu(self,"_File",
					  "_New", "<Control>n", &new_menuitem_clicked, //assigning keybooard controls and appending them in 												menubar
					  "_Open...", "<Control>o", &open_menuitem_clicked,
					  "_Save", "<Control>s", &save_menuitem_clicked,
					  "Save _as...", NULL, &save_as_menuitem_clicked,
					  "_Quit", "<Control>q", &quit_menuitem_clicked,
					  NULL));
  	gtk_menu_bar_append(menu_bar,
		      	create_menu(self,"_Edit",
				  	"Cu_t", "<Control>x", &cut_menuitem_clicked,
					"_Copy", "<Control>c", &copy_menuitem_clicked,
					"_Paste", "<Control>v", &paste_menuitem_clicked,
					NULL)); 
	gtk_menu_bar_append(menu_bar,
		      	create_menu(self,"_Help", 
					"_Contents", "<Control>h", &contents_menuitem_clicked,
					NULL)); 
  	self->text_view = gtk_text_view_new();
  	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(self->text_view), GTK_WRAP_WORD); // text's wrap mode is set
  	gtk_widget_show(self->text_view);
  
  	text_area = gtk_scrolled_window_new(NULL,NULL);		//scrollbar is activated
  	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(text_area),
				 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  	gtk_container_add(GTK_CONTAINER(text_area), self->text_view);
  	gtk_widget_show(text_area);
  
  	container = gtk_vbox_new(0, 0); 		//co-ordinates for menubar and text area is set
  	gtk_box_pack_start(GTK_BOX(container), menu_bar, 0, 0, 0);
  	gtk_box_pack_start(GTK_BOX(container), text_area, 1, 1, 0);
  	gtk_widget_show(container);
  
  	self->window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // toplevel window is assigned which can contain widgets
  	gtk_container_add(GTK_CONTAINER(self->window), container);
  	gtk_window_set_title(GTK_WINDOW(self->window), "Text Editor(Untitled)");
  	gtk_window_set_default_size(GTK_WINDOW(self->window), 500, 400);
  	g_signal_connect(self->window, "delete_event", G_CALLBACK(user_quit),
                   (void *)self);
  	gtk_widget_show(self->window);
  	ged_instances++;
  	return self;
}

void do_quit(GedWindow *self) {
  	gtk_widget_destroy(self->window);
  	if(!--ged_instances)
    	gtk_main_quit();
  	g_free(self);
}

gboolean user_quit(GtkWidget *window, GdkEvent *event, gpointer data) {	
  	gint result;
  	gboolean modified;
  	GtkTextBuffer *buffer;
  	GedWindow *self;
  	self = (GedWindow *)data;
  	if(self->close_dialog)
    		return 1;
  		/*see if they actually have any unsaved changes, because if they do,
    		we need to ask them.*/
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
  	modified = gtk_text_buffer_get_modified(buffer);
  	if(!modified) {
      		do_quit(self);
      		return 0;
    	}
  	//we ask them about their unsaved changes with a dialog box
  	self->close_dialog = gtk_dialog_new_with_buttons("want to Save changes to the file before"
							 " closing?",
							 GTK_WINDOW(window), 0,
							 "Close without saving", 0,
							 GTK_STOCK_CANCEL, 1,
							 GTK_STOCK_SAVE, 2,
							 NULL);
  	g_signal_connect(self->close_dialog, "response",
		   	G_CALLBACK(close_dialog_answered),
		   	data);
  	gtk_widget_show(self->close_dialog);
  	return 1;
}

void close_dialog_answered(GtkDialog *dialog, gint id, gpointer data) {
  	GedWindow *self;
  	self = (GedWindow *)data;
  	gtk_widget_destroy(self->close_dialog);
  	self->closing = 1;
  	switch(id) {
    		case 0:
      			do_quit(self);
      			break;
    		case 2:
      			save_menuitem_clicked(NULL, data); 
      			//do_quit(); handled in save_file
      			break;
    		case 1:
    		default:
     			self->closing = 0;
      			self->close_dialog = 0;
      			break;
    	}
}
/* menu item new -
opens a new file */
void new_menuitem_clicked(GtkMenuItem *item,gpointer data) {
  	GtkTextBuffer *buffer;
  	GedWindow *self;
  	self = (GedWindow *)data;
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
  	draw_ged_window();
}
/* menue item open-
 ask which file to open*/
void open_menuitem_clicked(GtkMenuItem *item,gpointer data) {
  	GedWindow *self;
  	self = (GedWindow *)data;
  	self->file_selector = gtk_file_selection_new("Select file to open...");
  	g_signal_connect(GTK_FILE_SELECTION(self->file_selector)->ok_button,/* GTK_FILE_SELECTION - opens a file selection dialog box. By 											default it will contain a GtkTreeView of the application's 
										current working directory, and a file listing. 
										Operation buttons that allow the user to create 
										a directory, delete files and rename files, are 
										also present. */
		   	"clicked", G_CALLBACK(open_selected_file), data);
  	g_signal_connect_swapped(GTK_FILE_SELECTION(self->file_selector)->
				   cancel_button, "clicked",
				   G_CALLBACK(gtk_widget_destroy), self->file_selector);
  	gtk_widget_show(self->file_selector);
}
/* open_selected_file -
 select file to open button */
void open_selected_file(GtkButton *button, gpointer data) {
	const gchar *m;
	GedWindow *self;
	self = (GedWindow *)data;
	m = gtk_file_selection_get_filename(GTK_FILE_SELECTION(self->file_selector));
	open_file(self, m);
	gtk_widget_destroy(self->file_selector);
	self->file_selector = 0;
}
/* save_menuitem_clicked-
 save item from menu bar*/
void save_menuitem_clicked(GtkMenuItem *item, gpointer data) {
  	GedWindow *self;
  	self = (GedWindow *)data;
  	if(!self->curr_filename) {
    		save_as_menuitem_clicked(NULL, data);
	}
  	else {
    		save_file(self, self->curr_filename);
	}
}
/* saves a file with new name */
void save_as_menuitem_clicked(GtkMenuItem *item, gpointer data) {
  	GedWindow *self;
  	self = (GedWindow *)data;
  	self->file_selector = gtk_file_selection_new("Select the location to save...");
  	g_signal_connect(GTK_FILE_SELECTION(self->file_selector)->ok_button,
		   	"clicked", G_CALLBACK(save_selected_file), data);
  	g_signal_connect_swapped(GTK_FILE_SELECTION(self->file_selector)->
				cancel_button, "clicked",
				G_CALLBACK(gtk_widget_destroy), self->file_selector);
  	gtk_widget_show(self->file_selector);
}
/* saves the selected file in specified directory(called by save_as_menuitem_clicked) */
void save_selected_file(GtkButton *button, gpointer data) {
	const gchar *m;
	GedWindow *self;
	self = (GedWindow *)data;
	m = gtk_file_selection_get_filename(GTK_FILE_SELECTION(self->file_selector));
	save_file(self, m);
	gtk_widget_destroy(self->file_selector);
	self->file_selector = 0;
}
/* quit_menuitem_clicked -
 quit menu option which closes the programme*/
void quit_menuitem_clicked(GtkMenuItem *item, gpointer data) {
  	GedWindow *self;
  	self = (GedWindow *)data;
  	user_quit(self->window, NULL, data);
}
/* cut menuitem from edit menubar*/
void cut_menuitem_clicked(GtkMenuItem *item, gpointer data) {
	GtkClipboard *clip;
	GtkTextBuffer *buffer;
	gboolean editable;
	GedWindow *self;
	self = (GedWindow *)data;
	clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD); // creates clipboard object 'clip' where object to be cut is stored.
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
	editable = gtk_text_view_get_editable(GTK_TEXT_VIEW(self->text_view));
	gtk_text_buffer_cut_clipboard(buffer, clip, editable); //Copies the currently-selected text to a clipboard 'clip', then deletes said   									text if it’s editable.
}
/*copy menuitem from edit menubar*/
void copy_menuitem_clicked(GtkMenuItem *item, gpointer data) {
  	GtkClipboard *clip;
  	GtkTextBuffer *buffer;
  	gboolean editable;
  	GedWindow *self;
  	self = (GedWindow *)data;
  	clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD); // creates clipboard object 'clip' where object to be copied is stored.
  	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
  	gtk_text_buffer_copy_clipboard(buffer, clip); // Copies the currently-selected text to a clipboard 'clip'.
}
/* paste menuitem from edit menubar*/
void paste_menuitem_clicked(GtkMenuItem *item,gpointer data) {
  	GtkClipboard *clip;
  	GtkTextBuffer *buffer;
  	gboolean editable;
  	GedWindow *self;
  	self = (GedWindow *)data;
  	clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD); // creates clipboard object 'clip' where object to be pasted is stored.
 	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(self->text_view));
  	editable = gtk_text_view_get_editable(GTK_TEXT_VIEW(self->text_view)); 
  	gtk_text_buffer_paste_clipboard(buffer, clip, 0, editable); // pastes the contents of clipboard at the cursor position.
}

void contents_menuitem_clicked(GtkMenuItem *item, gpointer data) {
	GtkWidget *window;
	GtkWidget *label;
	char help[1000];
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window), "Help");
	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
	strcpy(help,"\nTo Create a New file : \n\t GOTO File menubar->New\n\t voila!!!!New file is created\n To Open the required file from file manager :\n\t GOTO File menubar->open\n\t Choose the file to open\n\t voila!!!!! and the file gets displayed\n To save file :\n\t GOTO File menubar->save/save as\n To quit :\n\t GOTO File menubar->quit\n To use cut, copy, paste :\n\t copy :\n\t\t Select the text which needs to be copied and press CNTRL+C where you need to copy\n\t\t Or GOTO Edit menubar->copy\n\t cut :\n\t\t Select the text which needs to be cut and press CNTRL+X \n\t\t Or GOTO Edit menubar->cut\n\t paste :\n\t\t Select the text which needs to be pasted and do cut or copy and then press CNTRL+V where you need to paste\n\t\t Or GOTO Edit menubar->paste\nFOR Further developement refer to \n\thttps://developer.gnome.org/gtk-tutorial/stable/");
	strcat(help,"\0");
	label = gtk_label_new(help);
	gtk_container_add(GTK_CONTAINER (window), label); 
	gtk_widget_show_all(window);
}

