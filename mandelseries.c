#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <ctype.h>


int main()
{
	//part 1
	int i;
	char num[3];
	char *input[11];
	input[0] = "./mandel";
	input[1] = "-x";
	input[2] = "0.286932";
	input[3] = "-y";
	input[4] = "0.014287";
	input[5] = "-s";
	input[6] = "2";
	input[7] = "-m";
	input[8] = "100";
	input[9] = "-o";
	input[10] = "mandel.bmp";
	input[11] = NULL;

	char file_name[16] = " ";
	char file_first[7] = "mandel";
	char file_num[3];
	char file_type[5] = ".bmp";

	
	for(i = 1; i <= 50; i++)
	{
		
		double scale;

		char sscale[10];

		
		
		sprintf(num, "%d", i);
		//printf("%s\n", num);
		//strcpy(file_num, num);
		strcat(file_name, file_first);
		//printf("1");
		strcat(file_name, num);
		//printf("2");
		strcat(file_name, file_type);
		//printf("3   %d   %s\n", i, file_name);
		//strcpy(input[10], file_name);
		input[10] = file_name;

		pid_t child_pid = fork();
		int status;

		if(child_pid == 0)
		{
			printf("i = %d\n" ,i);
			execvp(input[0], input);
		}

		waitpid(child_pid, &status, 0);	
		strcpy(file_name, "");

		scale = atof(input[6]);
		printf("S = %f \n", scale);

		scale = scale / 1.25;

		printf("S = %f \n", scale);
		
		//snprintf(input[6], 8, "%f", scale);
		
		//memcpy(input[6], &scale, sizeof(scale));
		//input[6] = atof
		sprintf(sscale, "%f", scale);

		input[6] = sscale;
		printf("S Scale = %s \n", input[6]);
		
	}	

	//execvp(input[0], input);

	return 0;


}
