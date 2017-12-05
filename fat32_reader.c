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

char BS_OEMName[8];
int16_t BPB_RootEntCnt;
char BS_VolLab[11];
int32_t BPB_RootClus;

int32_t RootDirSectors = 0;
int32_t FirstDataSector = 0;
int32_t FirstSectorofCluster = 0;
	

int16_t BPB_BytesPerSec;
int8_t BPB_SecPerClus;
int16_t BPB_RsvdSecCnt;
int8_t BPB_NumFATS;
int32_t BPB_FATSz32;




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
void disp_info();

void get_stat(char x[]);


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
					get_info();
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
				disp_info();
			}

			else
			{
				printf("ERROR: File system image must be opened first\n");
			}		
		}

		if(strcmp("stat", fin[0]) == 0)
		{
			if(is_open == 1)
			{
				get_stat(fin[1]);
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
	fseek(fat_32, 3, SEEK_SET);
	fread(&BS_OEMName, 8, 1, fat_32);
	fread(&BPB_BytesPerSec, 2, 1, fat_32);
	fread(&BPB_SecPerClus, 1, 1, fat_32);
	fread(&BPB_RsvdSecCnt, 2, 1, fat_32);
	fread(&BPB_NumFATS, 2, 1, fat_32);
	fread(&BPB_RootEntCnt, 2, 1, fat_32);

	fseek(fat_32, 36, SEEK_SET);
	fread(&BPB_FATSz32, 4, 1, fat_32);
	
	fseek(fat_32, 44, SEEK_SET);
	fread(&BPB_RootClus, 4, 1, fat_32);

	fseek(fat_32, 71, SEEK_SET);
	fread(&BS_VolLab, 11, 1, fat_32);
	
	
}

void disp_info()
{
	printf("\n");
	printf("BPB_BytesPerSec\t\t DEC: %d \t HEX: 0x%x\n", BPB_BytesPerSec, BPB_BytesPerSec);
	printf("BPB_SecPerClus\t\t DEC: %d \t HEX: 0x%x\n", BPB_SecPerClus, BPB_SecPerClus);
	printf("BPB_RsvdSecCnt\t\t DEC: %d \t HEX: 0x%x\n", BPB_RsvdSecCnt, BPB_RsvdSecCnt);
	printf("BPB_NumFATS\t\t DEC: %d \t HEX: 0x%x\n", BPB_NumFATS, BPB_NumFATS);
	printf("BPB_FATSz32\t\t DEC: %d \t HEX: 0x%x\n", BPB_FATSz32, BPB_FATSz32);
	printf("\n");

}
void get_stat(char x[])
{
	int i;
	char temp[11];
	char temp_giv[11];
	RootDirSectors = (BPB_NumFATS * BPB_FATSz32 * BPB_BytesPerSec) + (BPB_RsvdSecCnt * BPB_BytesPerSec);
	
	fseek(fat_32, RootDirSectors, SEEK_SET);	
	for(i = 0; i < 16; i++)
	{
		fread(&dir_entry[i], 32, 1, fat_32);
	}

	for(i = 0; i < 16; i++)
	{
		//char temp[20] = dir_entry[i].DIR_Name;
		printf("Dir Name[%d] = %s\n", i, dir_entry[i].DIR_Name);
	}

	printf("x = %s \n", x);
	
	int counter = 0;
	
	strcpy(temp_giv, x);

	printf("lower x = %s \n", temp_giv);
	/*while(temp_giv[counter] != '\0')
	{
		printf("jaja \n");
		if(temp_giv[counter] >= 97 || temp_giv[counter] <= 122)
		{
			temp_giv[counter]-=32;
			//printf("j[%d] = %c\n", counter, j[counter]);
		}
		
		counter++;
	}
	
	counter = 0;*/
	printf("upper x = %s \n", temp_giv);
	
	for(i = 0; i < 16; i++)
	{
		printf("temp_giv = %s \n", temp_giv);
		strcpy(temp, dir_entry[i].DIR_Name);
		counter = 0;
		int counter_giv = 0;
		int flag = 0;
		strcpy(temp_giv, x);
		while(temp_giv[counter] != '\0')
		{
			printf("jaja \n");
			if(temp_giv[counter] >= 97 || temp_giv[counter] <= 122)
			{
				temp_giv[counter]-=32;
				//printf("j[%d] = %c\n", counter, j[counter]);
			}

			if(temp_giv[counter] == 46)
			{
				temp_giv[counter] = 32;
			}
		
			counter++;
		}
		printf("temp_giv11 = %s \n", temp_giv);
		counter = 0;
		while(counter != 11)
		{
			printf("temp = %s \n", temp);
			//printf("temp_give = %s \n", temp_giv);
			printf("temp[%d] = %c\n", counter, temp[counter]);
			printf("temp_giv[%d] = %c\n", counter_giv, temp_giv[counter_giv]);

			if(temp[counter] != 32)
			{
				if(temp[counter] == temp_giv[counter_giv])
				{
					printf("1\n");
					counter_giv++;
					counter++;
				}
				
				else
				{
					printf("2\n");
					flag = 1;
					break;
				}
			}

			else
			{
				printf("3\n");
				counter++;

				if(temp_giv[counter_giv] == 14)
				{
					counter_giv++;
				}
			}
			//j = temp[counter];
		}

		if(flag == 0)
		{
			printf("MATCH FOUND the corresponding i is %d\n", i);
			break;
		}

		//break;
		
		counter = 0;
		counter_giv = 0;
		//printf("temp = %s \n", temp);
		//if(strcmp(temp, x) == 0)
		//{
		//	printf("FOUND! \n");
		//}
	}





























}
