/* run as : ./try */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void printmenu() {
	printf("\n\t\t***** TEXT EDITOR *****\n");
	printf("\n\n\tMENU:\n\t-----\n ");
	printf("\n\t1.CREATE\n\t2.DISPLAY\n\t3.APPEND\n\t4.DELETE\n\t5.EXIT\n");
	printf("\n\tEnter your choice: \n");
}
int main() {
	int choice;
	printmenu();
	switch(choice) {
		case 1:
			Create();
	     		break;
		case 2:
			Display();
	 		break;
	 	case 3:
	 		Append();
	 		break;
	 	case 4:
			Delete();
			break;
	 	case 5:
	   		exit(0);
	}
	return 0;
}
 
