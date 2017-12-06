/*
	Name: Aninda Aminuzzaman
	ID: 1001018367
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

int current_dir;
int dir_visited[10];
int dir_index = 0;




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

//if the file is opened successfully, we get info
void get_info();
//type "info" on the terminal
void disp_info();

//type "stat" on the terminal
void get_stat(char x[]);

//type "cd" on the terminal, it can go into new directories, or go back to previous directories
void cd(char x[], char y[]);

//list the desired items in the current directory by typing "ls" on the terminal
void ls();

//get the volume name/label of the FAT32 system
void volume();

//given by professor to help in navigation
int LBAToOffset(int32_t sector)
{
    
    return ((sector - 2) * BPB_BytesPerSec) + (BPB_BytesPerSec * BPB_RsvdSecCnt) + (BPB_NumFATS * BPB_FATSz32 * BPB_BytesPerSec);
}


int main()
{

	char input[100];
	int is_open = 0;
	int i;


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

		//printf("words/line %d\n", token_count);

		token_count = 0;
		
		//open the file handled here
		if(strcmp("open", fin[0]) == 0)
		{
			if(is_open == 0)
			{
				if(fat_32 = fopen(fin[1], "r"))
				{
					is_open = 1;
					//printf("said open \n");
					//printf("file name %s \n", fin[1]);
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

		if(strcmp("cd", fin[0]) == 0)
		{
			if(is_open == 1)
			{
				//for(i = 0; i < 16; i++)
				//{
					//char temp[20] = dir_entry[i].DIR_Name;
				//	printf("Dir Name[%d] = %s\n", i, dir_entry[i].DIR_Name);
				//}

				//printf("fin[2] = %s", fin[2]);
				cd(fin[1], fin[2]);
				//for(i = 0; i < 16; i++)
				//{
					//char temp[20] = dir_entry[i].DIR_Name;
				//	printf("Dir Name[%d] = %s\n", i, dir_entry[i].DIR_Name);
				//}
			}

			else
			{
				printf("ERROR: File system image must be opened first\n");
			}
		}

		if(strcmp("ls", fin[0]) == 0)
		{
			if(is_open == 1)
			{
				ls();
			}

			else
			{
				printf("ERROR: File system image must be opened first\n");
			}
		}
		
		if(strcmp("volume", fin[0]) == 0)
		{
			if(is_open == 1)
			{
				volume();
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
	int i;

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

	RootDirSectors = (BPB_NumFATS * BPB_FATSz32 * BPB_BytesPerSec) + (BPB_RsvdSecCnt * BPB_BytesPerSec);

	current_dir = RootDirSectors;
	dir_visited[dir_index] = current_dir; 
	//dir_index++;

	fseek(fat_32, RootDirSectors, SEEK_SET);	
	for(i = 0; i < 16; i++)
	{
		fread(&dir_entry[i], 32, 1, fat_32);
	}
	
	
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
	int flag = 0;
	
	
	int counter = 0;
	int counter_giv = 0;
	
	for(i = 0; i < 16; i++)
	{
		//printf("temp_giv = %s \n", temp_giv);
		strcpy(temp, dir_entry[i].DIR_Name);
		
		
		strcpy(temp_giv, x);

		//take the string the user gives as argument and make it uppercase
		while(temp_giv[counter] != '\0')
		{

			if(temp_giv[counter] == 46)
			{
				//printf("1 temp_give[%d] = %c \n", counter, temp_giv[counter]);
				temp_giv[counter] -= 32;
			}

		
			else if(temp_giv[counter] >= 97 && temp_giv[counter] <= 122)
			{
				//if(temp_giv[counter] <= 122)
				//{
					//printf("2 temp_give[%d] = %c \n", counter, temp_giv[counter]);
					temp_giv[counter]-=32;
				//}
				//printf("j[%d] = %c\n", counter, j[counter]);
			}

			else if(temp_giv[counter] >= 65 && temp_giv[counter] <= 90)
			{
				//if(temp_giv[counter] <= 90)
				//{
					//printf("3 temp_give[%d] = %c \n", counter, temp_giv[counter]);
					counter++;
					continue;
				///}				
				
			}
		
			counter++;
		}

		//printf("temp_giv = %s \n", temp_giv);
		
		//reset the counter as manual string check is about to begin
		counter = 0;
		while(counter < 11)
		{
			//printf("temp = %s \n", temp);
			//printf("temp_give = %s \n", temp_giv);
			//printf("temp[%d] = %c\n", counter, temp[counter]);
			//printf("temp_giv[%d] = %c\n", counter_giv, temp_giv[counter_giv]);

			//32 == SPACE
			if(temp[counter] != 32)
			{
				if(temp[counter] == temp_giv[counter_giv])
				{
					//printf("1\n");
					counter_giv++;
					counter++;
					//victim of '.' being converted to uppercase
					if(temp_giv[counter_giv] == 14)
					{
						counter_giv++;
					}

					if(temp_giv[counter_giv] == '\0')
					{
						flag = 0;
						break;
					}
					if(counter == 10)
					{
						flag = 0;
						break;
					}
				}
				
				else
				{
					//printf("2\n");
					flag = 1;
					break;
				}
			}

			else
			{
				//printf("3\n");
				counter++;

			/*	if(temp_giv[counter_giv] == 14)
				{
					counter_giv++;
				}*/
			}
			//j = temp[counter];
		}

		//printf("flag == %d\n", flag);


		if(flag == 0)
		{
			//printf("MATCH FOUND the corresponding i is %d\n", i);
			printf("\n");
			printf("Name: \t\t %s\n", x);
			printf("Attribute: \t %d\n", dir_entry[i].DIR_Attr);
			printf("High: \t %d\n", dir_entry[i].DIR_FirstClusterHigh);
			printf("Low: \t %d\n", dir_entry[i].DIR_FirstClusterLow);
			printf("Size: \t %d\n", dir_entry[i].DIR_FileSize);
			printf("\n");
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

	if(flag == 1)
	{
		printf("ERROR: File not found\n");
	}

}


void cd(char x[], char y[])
{

	//printf("Dir Index = %d\n", dir_index);
	int i;
	char temp[11];
	char temp_giv[11];
	int flag = 0;
	
	if(strcmp("..", x) == 0)
	{

		if(dir_index == 0)
		{
			printf("ERROR: At root directory, cannot go back\n");
			return;
		}

		else
		{
			//printf("in .. \n");
			//printf("Dir Index2 = %d\n", dir_index);
			dir_index--;
			//printf("Dir Index3 = %d\n", dir_index);
			current_dir = dir_visited[dir_index];
			fseek(fat_32, current_dir, SEEK_SET);
            
        		for(i = 0; i < 16; i++)
               		{
               			fread(&dir_entry[i], 32, 1, fat_32);
          		}
			return;
		}
	}

	
	int counter = 0;
	int counter_giv = 0;


	/*if(y == '\0')
	{
		
					
	}

	else
	{
		
	}*/
	
	for(i = 0; i < 16; i++)
	{
		//printf("temp_giv = %s \n", temp_giv);
		strcpy(temp, dir_entry[i].DIR_Name);
		
		
		strcpy(temp_giv, x);

		//take the string the user gives as argument and make it capitalized
		while(temp_giv[counter] != '\0')
		{

			if(temp_giv[counter] == 46)
			{
				//printf("1 temp_give[%d] = %c \n", counter, temp_giv[counter]);
				temp_giv[counter] -= 32;
			}

		
			else if(temp_giv[counter] >= 97 && temp_giv[counter] <= 122)
			{
				//if(temp_giv[counter] <= 122)
				//{
					//printf("2 temp_give[%d] = %c \n", counter, temp_giv[counter]);
					temp_giv[counter]-=32;
				//}
				//printf("j[%d] = %c\n", counter, j[counter]);
			}

			else if(temp_giv[counter] >= 65 && temp_giv[counter] <= 90)
			{
				//if(temp_giv[counter] <= 90)
				//{
					//printf("3 temp_give[%d] = %c \n", counter, temp_giv[counter]);
					counter++;
					continue;
				///}				
				
			}
		
			counter++;
		}

		//printf("temp_giv = %s \n", temp_giv);
		
		//reset the counter as manual string check is about to begin
		counter = 0;
		while(counter < 11)
		{
		//	printf("temp = %s \n", temp);
		//	printf("temp_give = %s \n", temp_giv);
		//	printf("temp[%d] = %c\n", counter, temp[counter]);
		//	printf("temp_giv[%d] = %c\n", counter_giv, temp_giv[counter_giv]);

			if(temp[counter] != 32)
			{
				if(temp[counter] == temp_giv[counter_giv])
				{
		//			printf("1\n");
					counter_giv++;
					counter++;
					if(temp_giv[counter_giv] == 14)
					{
						counter_giv++;
					}

					if(temp_giv[counter_giv] == '\0')
					{
						flag = 0;
						break;
					}
					if(counter == 10)
					{
						flag = 0;
						break;
					}
				}
				
				else
				{
		//			printf("2\n");
					flag = 1;
					break;
				}
			}

			else
			{
			//	printf("3\n");
				counter++;

			/*	if(temp_giv[counter_giv] == 14)
				{
					counter_giv++;
				}*/
			}
			//j = temp[counter];
		}

		//printf("flag == %d\n", flag);

		//match found
		if(flag == 0)
		{

			//if the matching object's size is zero, its a folder
			if(dir_entry[i].DIR_FileSize == 0)
			{
				//raise the index
				dir_index++;
				//change the current directory
				current_dir = LBAToOffset(dir_entry[i].DIR_FirstClusterLow);
				//add it to the visited list
				dir_visited[dir_index] = current_dir;
					
				//go to that location
                		fseek(fat_32, current_dir, SEEK_SET);
                		
				//fill up the struct array with new entries
               			for(i = 0; i < 16; i++)
               			{
                  			fread(&dir_entry[i], 32, 1, fat_32);
                		}
						
			}
			else
			{
				printf("ERROR: Not a folder\n");
			}
	

			break;
		}

		
		counter = 0;
		counter_giv = 0;
		
	}

	if(flag == 1)
	{
		printf("ERROR: File not found\n");
	}
}


void ls()
{
	int i;
	char temp[11];
	char temp2[11];	
	
	for(i = 0; i < 16; i++)
	{
		//noticed that DIR_Attr value is either 32 or 16 for desired objects
		//however, in the other folders, there are objects with that value
		//but they are not the desired objects to be ls'd
		//another observation is that the items that have 32 or 16, but not desired
		//do no begin with capital letters
		if((dir_entry[i].DIR_Attr == 32) || (dir_entry[i].DIR_Attr == 16))
		{
			strcpy(temp2, dir_entry[i].DIR_Name);
			if((temp2[0] >= 65 && temp2[0] <= 90))
			{
				strncpy(temp,dir_entry[i].DIR_Name,11);
				printf("%s\n", temp);
			}
		}	
	}
	
}

void volume()
{
	if(strcmp("           ", BS_VolLab) == 0)
	{
		printf("ERROR: Volume name not found\n");
	}
	
	else
	{
		printf("VOLUME: %s\n", BS_VolLab);
	}
}








