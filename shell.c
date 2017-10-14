#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>




//struct line
//{
//	char
//};

int main()
{
	char history[10][100];
	char input[100];
	int counter = 0;
	int words = 0;
	while(1)
	{
		printf("msh:[%d]:>", counter);
		//printf("here 0");
		if(fgets(input, 100, stdin))
		{
			printf("WORKED!\n");
		}

		else
		{
			printf("fgets failed\n");
		}

		if((strcmp(input, "quit\n") == 0) || (strcmp(input, "exit\n") == 0))
		{
			printf("need to exit now\n");
			break;
		}

		printf("here 00\n");
		strcpy(history[counter], input);
		printf("here 000\n");
		
		printf("here 1\n");
		char *token;
		char input_to_exec[5][100];
		printf("here 2\n");
		//const char ch[2] = " ";
		int token_count = 1;
		while(1)
		{
			printf("in second while 1\n");
			//int pos = 0;
			//int piv = 0;
			token = strtok(input, " ");

			
			
			while(token != NULL)
			{
				printf("in token != NULL\n");
				//*input_to_exec++ = token;
				printf("pos = %d \n", token_count -1);
				printf("= %s\n", token);
				strcpy(input_to_exec[token_count - 1], token);
				printf("== %s\n", token);
				token = strtok(NULL, " ");
				//token = strtok(input, " ");
				//pos++;
				token_count ++;
			}

			

			break;
			
		}

		counter++;
		char *fin;
		int i;
		
		//sizeof(input_to_exec);
		
		for(i = 0; i < token_count-1; i++)
		{
			if(input_to_exec[i] != NULL){words++;}

			else{continue;}
		}	
		printf("#######		# of words in command = %d	########", words);

		words = 0;
		for(i = 0; i < token_count-1; i++)
		{
			printf("\n%s\n", input_to_exec[i]);
		}
		
		pid_t child_pid = fork();
		int status;

		if(child_pid == 0)
		{
			//execl("/bin/ls",input_to_exec[0], NULL);
			//execvp(input_to_exec[0], input_to_exec);	
			//exit(EXIT_SUCCESS);
		}
		waitpid(child_pid, &status, 0);
		
		
		
		for(i = 0; i < token_count-1; i++)
		{
			printf("\n%s\n", input_to_exec[i]);
			strcpy(input_to_exec[i], "\0");// = "\0";
		}

		if (counter > 5)
		{
			int i;
			for(i = 0; i < counter; i++)
			{
				printf("\n [%d] = %s", i, history[i]);
			}
			counter = words = 0;
		}

		
		//printf("\n %s", input);

	}

	
	return 0;

}
