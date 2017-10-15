/*
	Name: Aninda Aminuzzaman
	ID:   1001018367
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
	//2-D array to keep track of the last 10 commands by the user
	//every command is stored in input
	//counter to keep track of the index of the command (0 <= counter <= 9)
	
	char history[10][100];
	char input[100];
	int counter = 0;

	//run forever *
	while(1)
	{
		printf("msh:[%d]:> ", counter);
		
		//checking to see whether or not fgets() failed
		if(fgets(input, 100, stdin))
		{
			if (strcmp(input, " \n") == 0){continue;}
		}

		else
		{
			printf("fgets failed\n");
		}


		//checking to see if the user wrote 'quit' or 'exit'
		if((strcmp(input, "quit\n") == 0) || (strcmp(input, "exit\n") == 0))
		{
			//if true stop the program *
			break;
		}

		//store the input in history
		strcpy(history[counter], input);

		//token is for strtok()
		//look into the part where i add to input_to_exec and see if i can add to a char *array		
		char *token;
		char *fin[11];
		
		//keep track of how many words per line
		int token_count = 1;
		
		while(1)
		{
			token = strtok(input, " \n");
			
			
			while(token != NULL)
			{
				fin[token_count-1] = token;
				token = strtok(NULL, " \n");
				token_count++;
			}

			//last item should always be NULL
			fin[token_count-1] = NULL;

			break;
			
		}
		
		//increase the counter for history
		counter++;
		
		int i;
		

		if(strcmp("history", fin[0]) == 0)
		{
			for(i = 0; i < counter; i++){printf("[%d] %s", i, history[i]);}
		}
		
		char n[2];
		strcpy(n, fin[0]);

		if(n[0] == 33)
		{
			int ind = n[1] - '0';
			strcpy(input, history[ind]);
			token = strtok(input, " \n");
			token_count = 1;
			while(token != NULL)
			{
				fin[token_count-1] = token;
				token = strtok(NULL, " \n");
				token_count++;
			}

			//last item should always be NULL
			fin[token_count-1] = NULL;
			
		}	
		
		pid_t child_pid = fork();
		int status;


		if (child_pid > 0)
		{
			if(strcmp("cd", fin[0]) == 0)
			{
		                int cd_check = chdir(fin[1]);
				if(cd_check == -1){printf("%s: No such file in directory\n", fin[1]);}
			}

			waitpid(child_pid, &status, 0);

		}

		else if(child_pid == 0)
		{
			execvp(fin[0], fin);
			if((execvp(fin[0], fin) == -1) && (strcmp(fin[0], "cd") != 0) && (strcmp(fin[0], "history") != 0))
			{	
				printf("%s: command not found.\n", fin[0]);
			}
		}
		
		waitpid(child_pid, &status, 0);	
		
		

		if(child_pid == 0)
		{
			execvp(fin[0], fin);	
			exit(EXIT_SUCCESS);
		}

		
		waitpid(child_pid, &status, 0);
		

		if (counter > 5)
		{
			int i;
			for(i = 0; i < counter; i++)
			{
				printf("\n [%d] = %s", i, history[i]);
			}
			counter = 0;
		}

		
	}

	
	return 0;

}
