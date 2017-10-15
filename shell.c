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
	//2-D array to keep track of the last 10 commands by the user
	//every command is stored in input
	//counter to keep track of the index of the command (0 <= counter <= 9)
	//words for how many words/tokens in the command
	char history[10][100];
	char input[100];
	int counter = 0;
	int words = 0;

	//run forever *
	while(1)
	{
		printf("msh:[%d]:>", counter);
		
		//checking to see whether or not fgets() failed
		if(fgets(input, 100, stdin))
		{
			printf("WORKED! input is %s \n", input);
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
		//input_to_exec keeps store of the words in every command
		//look into the part where i add to input_to_exec and see if i can add to a char *array		
		char *token;
		char input_to_exec[5][100];
		char *fin[11];
		
		
		int token_count = 1;
		
		while(1)
		{
			token = strtok(input, " \n");
			
			
			while(token != NULL)
			{
				fin[token_count-1] = token;
				printf("pos = %d \n", token_count -1);
				printf("= %s\n", token);
				strcpy(input_to_exec[token_count - 1], token);
				printf("== %s\n", token);
				token = strtok(NULL, " \n");
				token_count++;
			}


			fin[token_count-1] = NULL;

			break;
			
		}

		counter++;
		
		int i;
		
		
		for(i = 0; i < token_count-1; i++)
		{
			if(input_to_exec[i] != NULL){words++;}

			else{continue;}
		}	
		printf("#######		# of words in command = %d	########", words);

		words = 0;
		for(i = 0; i < token_count; i++)
		{
			printf("\n%s\n", input_to_exec[i]);
			printf("fin[%d] = %s \n",i, fin[i]);
		}

		
		
		pid_t child_pid = fork();
		int status;
		
		if(child_pid == 0)
		{
			//execl("/bin/ls",input_to_exec[0], NULL);
			execvp(fin[0], fin);	
			exit(EXIT_SUCCESS);
		}
		waitpid(child_pid, &status, 0);
		
		
		
		for(i = 0; i < token_count-1; i++)
		{
			printf("jjkk\n%s\n", input_to_exec[i]);
			strcpy(input_to_exec[i], "\0");
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

		

	}

	
	return 0;

}
