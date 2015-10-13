#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
void Create() {
	FILE *fp1, *fp2;
	char name[200], ch;
	fp1 = fopen("untitle.txt","w");
	printf("\n\tEnter the text and press '#' to save\n\n\t");
	while(1) {
		ch = getchar();
		fputc(ch,fp1);
		if(ch == '#') {
			fclose(fp1);
			printf("\n\tEnter the new filename: ");
			scanf("%s",name);
			fp1 = fopen("untitle.txt","r");
		  	fp2 = fopen(name,"w");
		   	while(!feof(fp1)) {
			    	ch = getc(fp1);
			    	putc(ch,fp2);
		   	}
	   		fclose(fp2);
	   		break;
  		}
	}
}

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
	
}
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
	
}
void Edit() {
return;
}
