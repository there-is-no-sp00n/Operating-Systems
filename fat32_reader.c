/*
	Aninda Aminuzzaman
	1001018367
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>


FILE *fat_32;

struct __attribute__((__packed__)) DirectoryEntry
{
	char DIR_Name[11];
	uint8_t DIR_Attr;
	uint8_t Unused1[8];
	uint16_t DIR_FirstClusterHigh;
	uint8_t Unused2[4];
	uint16_t DIR_FirstClusterLow;
	uint32_t DIR_FileSize;
};

struct DirectoryEntry dir_entry[16];

void get_info();

int main()
{

	char input[100];
	int is_open = 0;


	while(1)
	{
		printf("msh:> ");
		
		if(fgets(input, 100, stdin))
		{
			char *temp = input;
			
			int flag;
			
			while(*temp != '\0')
			{
				if(isspace(*temp))
				{
					flag = 1;
				}

				else
				{
					flag = 0;
					break;
				}
				temp++;
			}
		
			if(flag == 1)
			{
				continue;
			}
			
			
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

		char *token;
		char *fin[11];
		
		//keep track of how many words per line
		int token_count = 0;
		
		while(1)
		{
			token = strtok(input, " \n\t");
			
			
			while(token != NULL)
			{
				fin[token_count] = token;
				token = strtok(NULL, " \n\t");
				token_count++;
			}


			break;
			
		}

		printf("words/line %d\n", token_count);

		token_count = 0;
		
		//open the file handled here
		if(strcmp("open", fin[0]) == 0)
		{
			if(is_open == 0)
			{
				if(fat_32 = fopen(fin[1], "r"))
				{
					is_open = 1;
					printf("said open \n");
					printf("file name %s \n", fin[1]);
				}

				else{printf("ERROR: File system image not found\n");}
			}

			else
			{
				printf("ERROR file already open\n");
			}
		}


		if(strcmp("close", fin[0]) == 0)
		{
			if(is_open == 1)
			{
				fclose(fat_32);
				is_open = 0;
				printf("Closed the file\n");
			}

			else
			{
				printf("ERROR: File system not open\n");
			}
		}
	}

	return 0;
}

void get_info()
{
	int16_t BPB_BytesPerSec;
	int8_t BPB_SecPerClus;
	int16_t BPB_RsvdSecCnt;
	int8_t BPB_NumFATS;
	int32_t BPB_FATSz32;

	
















}
