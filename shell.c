/*
	Name: Aninda Aminuzzaman
	ID:   1001018367
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>




int main()
{
	//2-D array to keep track of the last 15 commands by the user
	//every command from the user is stored in input
	//counter to keep track of the index of the command (0 <= counter <= 9)
	//counter_h to keep track of the history
	//max_history represents max amount of items in history 
	char history[15][100];
	char input[100];
	int counter = 0;
	int counter_h = 0;
	int max_history = 15;

	//run forever *
	while(1)
	{
		printf("msh:[%d]:> ", counter);
		
		//checking to see whether or not fgets() failed
		if(fgets(input, 100, stdin))
		{
			//this temp is for checking whether or not the input is solely consisting of whitespace
			char *temp = input;
			
			//flag to keep count of whether or not a non-whitespace character is encountered
			int flag;
			//keep on running until null terminating character is encountered
			while (*temp != '\0')
			{
				//isspace function returns true if it encounters a whitespace character
				if(isspace(*temp))
				{
					//set flag to 1 if whitespace is encountered
					flag = 1;
				}

				else
				{	
					/*  
					    set flag to 0 and instantly break out of the loop 
					    because a non-whitespace character is encountered
					*/
					flag = 0;
					break;
				}
				//increment temp to go check the next character in the input
				temp++;
			}
			
			/*
			    if flag is 1 after the loop exits, that means that
			    the input string only consists of whitespace characters
			    this means that we don't go forward in the program
			    must run loop from beginning
			*/
			if (flag == 1)
			{
				continue;
			}
		}

		else
		{
			//if fgets() failed then run the loop from beginning
			continue;
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
		//fin has the input that goes into execvp()	
		char *token;
		char *fin[11];
		
		//keep track of how many words per line
		int token_count = 1;
		
		//this loop to iterate through the input string and find words
		while(1)
		{	
			//get the first word
			token = strtok(input, " \n\t");
			
			//while token is not null, keep iterating through the input
			while(token != NULL)
			{
				//this populates the input that goes into execvp()
				fin[token_count-1] = token;
				//go find the next word
				//NULL is passed because something about strtok() maintaining a static pointer
				// to the previous string that was passed
				token = strtok(NULL, " \n\t");
				token_count++;
			}

			//last item should always be NULL
			fin[token_count-1] = NULL;

			//break out of the loop after the input is iterated through and converte into words
			break;
			
			/*
				maybe i didn't need this while loop...
			*/
		}
		
		//increase the counter for history
		counter++;
		
		//counter_h is incremented to 14 and stays there
		if (counter_h < max_history)
		{
			counter_h++;
		}
		int i;
		
		//if the user input was "history" then print everything in history
		if(strcmp("history", fin[0]) == 0)
		{
			for(i = 0; i < counter_h; i++)
			{
				printf("[%d]: %s\n", i, history[i]);
			}
		}
		
		//this char array to hold if the first character is '!'
		char n[4];
		//copy the first character
		strcpy(n, fin[0]);
		
		
		//if the first character equals 33, the ASCII value for '!'
		if(n[0] == 33)
		{
			char num[3];
			//populate with numbers
			num[0] = n[1];
			num[1] = n[2];
			num[2] = n[3];
			//convert to int	
			int m_int = atoi(num);
			
			//if the choice is less than 15(or what counter_h is) and 0 or above
			//run the function from history again
			if(m_int < counter_h && m_int >= 0)
			{
				//this is from above with the new input from history
				strcpy(input, history[m_int]);
				token = strtok(input, " \t\n");
				token_count = 1;
				while(token != NULL)
				{
					fin[token_count-1] = token;
					token = strtok(NULL, " \t\n");
					token_count++;
				}

				fin[token_count-1] = NULL;
			}
			else
			{
				printf("Command not in history!\n");
				continue;
			}

			//if the user input was "history" from the history then print everything in history
			if(strcmp("history", fin[0]) == 0)
			{
				for(i = 0; i < counter_h; i++)
				{
					printf("[%d]: %s\n", i, history[i]);
				}
			}
		}

			
		
		//make a process
		pid_t child_pid = fork();
		int status;

		//if in parent run cd
		if (child_pid > 0)
		{
			if(strcmp("cd", fin[0]) == 0)
			{
		                int cd_check = chdir(fin[1]);
				//if cd fails give output
				if(cd_check == -1)
				{
					printf("%s: No such file in directory\n", fin[1]);
				}
			}
			//wait to finish
			waitpid(child_pid, &status, 0);

		}
		
		//if in child check to see if execvp() failed and its not "cd" or "history"
		else if(child_pid == 0)
		{
			execvp(fin[0], fin);
			if((execvp(fin[0], fin) == -1) && (strcmp(fin[0], "cd") != 0) && (strcmp(fin[0], "history") != 0))
			{	
				printf("%s: command not found.\n", fin[0]);
			}
		}
		
		//wait to finish
		waitpid(child_pid, &status, 0);	
		
		
		//here run all other input
		if(child_pid == 0)
		{
			execvp(fin[0], fin);	
			exit(EXIT_SUCCESS);
		}

		//wait to finish
		waitpid(child_pid, &status, 0);
		
		
		//reset counter to 0 because we want to go back to top of history 		
		if (counter == max_history)
		{
			counter = 0;
		}
		
	}

	
	return 0;

}
