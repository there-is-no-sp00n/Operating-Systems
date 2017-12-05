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
					//get_info();
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

		if(strcmp("info", fin[0]) == 0)
		{
			if(is_open == 1)
			{
				get_info();
			}

			else
			{
				printf("ERROR: File system image must be opened first\n");
			}		
		}
	}

	return 0;
}

void get_info()
{
	char BS_OEMName[8];
	int16_t BPB_RootEntCnt;
	char BS_VolLab[11];
	int32_t BPB_RootClus;
	

	int16_t BPB_BytesPerSec;
	int8_t BPB_SecPerClus;
	int16_t BPB_RsvdSecCnt;
	int8_t BPB_NumFATS;
	int32_t BPB_FATSz32;

	fseek(fat_32, 11, SEEK_SET);
	//fread(&BS_OEMName, 8, 1, fat_32);
	fread(&BPB_BytesPerSec, 2, 1, fat_32);
	fread(&BPB_SecPerClus, 1, 1, fat_32);
	fread(&BPB_RsvdSecCnt, 2, 1, fat_32);
	fread(&BPB_NumFATS, 2, 1, fat_32);

	fseek(fat_32, 36, SEEK_SET);
	fread(&BPB_FATSz32, 4, 1, fat_32);
	
	printf("\n");
	printf("BPB_BytesPerSec\t\t DEC: %d \t HEX: 0x%x\n", BPB_BytesPerSec, BPB_BytesPerSec);
	printf("BPB_SecPerClus\t\t DEC: %d \t HEX: 0x%x\n", BPB_SecPerClus, BPB_SecPerClus);
	printf("BPB_RsvdSecCnt\t\t DEC: %d \t HEX: 0x%x\n", BPB_RsvdSecCnt, BPB_RsvdSecCnt);
	printf("BPB_NumFATS\t\t DEC: %d \t HEX: 0x%x\n", BPB_NumFATS, BPB_NumFATS);
	printf("BPB_FATSz32\t\t DEC: %d \t HEX: 0x%x\n", BPB_FATSz32, BPB_FATSz32);
	printf("\n");

}
