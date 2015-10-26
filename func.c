/* text editor functions */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <graphics.h>

/* creates a file and saves the data by entering # */ 
void Create() {
	FILE *fp1, *fp2;
	char name[200], ch;
	fp1 = fopen("untitle.txt","w");
	if (fp1 == NULL) {
		perror("cannot open");
		return;
	}
	printf("\n\tEnter the text and press '#' and enter key to save\n\n\t");
	while(1) {
		ch = getchar();
		fputc(ch,fp1);
		if(ch == '#') {
			fclose(fp1);
			printf("\n\tEnter the new filename: ");
			scanf("%s",name);
			fp1 = fopen("untitle.txt","r");
		  	fp2 = fopen(name,"w");
			if (fp1 == NULL) {
				perror("cannot open");
				return;
			}		
		   	while(!feof(fp1)) {
			    	ch = getc(fp1);
			    	putc(ch,fp2);
		   	}
	   		fclose(fp2);
			fclose(fp1);
			remove("untitle.txt");
			printf("\tfile successfully created.");
	   		break;
  		}
	}
}

/* Deletes a file with the data inside it */
void Delete() {
	FILE *fp;
	char name[200];
	printf("\n\tEnter the file name: ");
	scanf("%s",name);
	fp = fopen(name,"r");
	if(fp == NULL) {
		perror("cannot open");
		return;	
	}
	fclose(fp);
	if(remove(name) == 0) {
		printf("\n\n\tFile has been deleted successfully!");
		return;
	}
	else
	printf("\n\tError!\n");
	getch();
	
}

/* Displays the data in the file given as input */
void Display() {
	char name[200];
	char ch;
	FILE *fp;
	printf("\n\tEnter the file name: ");
	scanf("%s",name);
	fp = fopen(name,"r");
	if(fp == NULL) {
		perror("cannot open");
		return;	
	}
	while(!feof(fp)) {
		ch = getc(fp);
		printf("%c",ch);
	}
	fclose(fp);
	getch();
	
}


/* Appends the data in the input file */
void Edit() {
	FILE *fp1, *fp2;
	char c, ch;
	char name[100], name1[100];
	printf("\n\tEnter the file name: ");
	scanf("%s",name);
	fp1 = fopen(name,"r");
	if(fp1 == NULL) {
		perror("cannot open");
		return;
	}
	while(!feof(fp1)) {
		c = getc(fp1);
		printf("%c",c);
	}
	fclose(fp1);
	printf("\n\tType the text and press '&' and enter key to append.\n\n\t");
	fp1 = fopen(name,"a");
	while(1) {
		c = getchar();
		fputc(c,fp1);
		if(c == '&') {
			fclose(fp1);
			printf("\n\tEnter the edited filename: ");
			scanf("%s",name1);
			rename(name, name1);
			printf("\tfile edited succesfully");
			break;
		}
		
	}	
	getch();
}

