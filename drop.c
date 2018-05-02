#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>



#define NUM_BLOCKS 4226
#define BLOCK_SIZE 8192
#define NUM_FILES 128
#define NUM_INODES 128
#define MAX_BLOCKS_PER_FILE 32 


unsigned char data_blocks[NUM_BLOCKS][BLOCK_SIZE];
int used_blocks[NUM_BLOCKS];

struct inode
{
	time_t date;
	int valid;
	int size;
	int blocks[MAX_BLOCKS_PER_FILE];
};

struct directory_entry
{
	char * name;
	int valid;
	int inode_idx;
};

struct directory_entry dir_ptr[128];

struct inode *inode_array_ptr[NUM_INODES];

void put(char * filename);

int findFreeDirEntry();
int findFreeInode();
int findFreeBlock();
int findFreeInodeBlockEntry(int inode_index);

void init();


int df();
void put(char * filename);
void list();
void del(char * file);


char in_system[128][100];

int in_sys_count = 0;

int main()
{

	char input[100];

	while(1)
	{
		printf("msh:> ");

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

		int i;

	/*	for(i = 0; i < 3; i++)
		{
			printf("fin[%d] = %c \n", i, fin[i]);
		}

		if(fin[1] == NULL)
		{
			printf("Invalid command\n");
		}
	 */
		//printf("Free Space: %d bytes\n", df() );
	

		init();
		if(strcmp(fin[0], "put") == 0)
		{
			//printf("fin[1] == %s\n", fin[1]);
			if(fin[1] == NULL)
			{
				printf("ERROR: Filename missing!\n");
			}
			else
			{
				put(fin[1]);
			}

		}

		else if(strcmp(fin[0], "list") == 0)
		{
			list();
		}

		else if(strcmp(fin[0], "df") == 0)
		{
			printf("Free Space: %d bytes\n", df());
		}
		
		else if(strcmp(fin[0], "del") == 0)
		{
			if(fin[1] == NULL)
			{
				printf("ERROR: Filename missing!\n");
			}
			else
			{
				del(fin[1]);
			}
		}


	}


	return 0;
}


int df()
{
	int count = 0;
	int 	i = 0;

	for(i = 130; i < 4226; i++)
	{
		if (used_blocks[i] == 0)
		{
			count ++;
		}
	}

	return count * BLOCK_SIZE;
}

void put(char * filename)
{
	struct stat buff;
	

	int status = stat(filename, &buff);
	
	if(status == -1)
	{
		printf("ERROR: File not found \n");
		return;
	}

	else
	{
		int i;
		for(i = 0; i < 128; i++)
		{
			if(strcmp(in_system[i],"") == 0)
			{
				strcpy(in_system[i], filename);
				dir_ptr[i].name = (char*)malloc(strlen(filename));	
				strncpy(dir_ptr[i].name, filename, strlen(filename));
				dir_ptr[i].valid = 1;
				//return;
				break;
			}
		}

		for(i = 0; i < 10; i++)
		{
			printf("put in system[%d] = %s\n", i, in_system[i]);
		}
		
		//in_sys_count ++;
	}


	if(buff.st_size > df())
	{
		printf("ERROR: Not enough space in the file system\n");
		return;
	}
	
	int dir_idx = findFreeDirEntry();
	//printf("1 dir index = %d\n", dir_idx);
	if(dir_idx == -1)
	{
		printf("ERROR: Not enough space in the file system\n");
		return;
	}

	//dir_ptr[dir_idx].valid = 1;
	//dir_ptr[dir_idx].name = (char*)malloc(strlen(filename));
	
	//strncpy(dir_ptr[dir_idx].name, filename, strlen(filename));
	
	int inode_idx = findFreeInode();
	
	//printf("2\n");
	if(inode_idx == -1)
	{
		printf("ERROR: No free inodes\n");
		return;
	}
	
	dir_ptr[dir_idx].inode_idx = inode_idx;

	inode_array_ptr[inode_idx]->size = buff.st_size;
	inode_array_ptr[inode_idx]->date = time(NULL);
	inode_array_ptr[inode_idx]->valid = 1;

	FILE *ifp = fopen ( filename, "r" );

	int copy_size = buff.st_size;
	int offset = 0;
	
	int block_index = findFreeBlock();
	
	//printf("3\n");
	if( block_index == -1)
	{
		printf("ERROR: Can't find free block\n");
		return;
	}
	
	while(copy_size >= BLOCK_SIZE)
	{
		int block_index = findFreeBlock();
		
		if(block_index == -1)
		{
			printf("ERROR: Can't find free block\n");	
			return;
		}
		
		used_blocks[block_index] = 1;


		int inode_block_entry = findFreeInodeBlockEntry(inode_idx);

		if(inode_block_entry == -1) 
		{
			printf("ERROR: Can't find free node block \n");
			return;
		}
		inode_array_ptr[inode_idx]->blocks[inode_block_entry] = block_index;

		fseek(ifp, offset, SEEK_SET);
		
		int bytes  = fread( data_blocks[block_index], BLOCK_SIZE, 1, ifp );

     		 // If bytes == 0 and we haven't reached the end of the file then something is 
		 // wrong. If 0 is returned and we also have the EOF flag set then that is OK.
		 // It means we've reached the end of our input file.
		 if( bytes == 0 && !feof( ifp ) )
		 {
		 	printf("An error occured reading from the input file.\n");
			return;
		 };

		clearerr(ifp);

		copy_size -= BLOCK_SIZE;
		offset += BLOCK_SIZE;
	}

	if(copy_size > 0)
	{
		int block_index = findFreeBlock();
		if(block_index == -1)
		{
			printf("ERROR: Can't find free block\n");	
			return;
		}
		used_blocks[block_index] = 1;
		fseek(ifp, offset, SEEK_SET);
		int bytes  = fread( data_blocks[block_index], copy_size, 1, ifp );

	}

	fclose(ifp);
	return;

}

void list()
{
	int i, flag = 0;
	

	//while(dir_ptr[i].name != NULL)
	//{
	//	printf("%s \n", dir_ptr[i].name);
	//	i++;
	//}
	//i = 0;
	//for (i = 0; i < 10; i ++)
	//{
	//	printf("$$ %s \n", in_system[i]);
	//}
	printf("\n\n\n");

	for (i = 0; i < 128; i ++)
	{		
		if(inode_array_ptr[i]->valid == 1)
		{
			flag = 1;
			printf("%s ", dir_ptr[i].name);
			printf("%d ", inode_array_ptr[i]->size);
			printf("%s", ctime(&inode_array_ptr[i]->date));
		}
	}
	printf("\n\n\n");


	if (flag ==0)
	{
		printf("ERROR: No file in the system! \n");
	}
	
	//for (i = 0; i < 10; i ++)
	//{
	//	printf("Inode[%d] = %d\n", i, inode_array_ptr[i]->valid);

	//	if(inode_array_ptr[i]->valid == 1)
	//	{
	//		printf("Size: %d \n", inode_array_ptr[i]->size);
	//		printf("Date: %s \n", ctime(&inode_array_ptr[i]->date));
	//		int j;
			//for(j = 0; j < 32; j++)
			//{
			//	printf("Blocks: %d \n", inode_array_ptr[i]->blocks[j]);
			//}
			
			//printf("Blocks: %d \n", inode_array_ptr[i]->blocks);
			//printf("Blocks: %d \n", inode_array_ptr[i]->blocks[12]);
			//printf("Blocks: %d \n", inode_array_ptr[i]->blocks[20]);
	//	}
	//}

	//for (i =0; i < 1000; i++)
	//{
	//	printf("Used Block[%d] = %d\n",i, used_blocks[i]);
	//}

}

void del(char * filename)
{	
	int i, flag = 0;
	for (i = 0; i < 10; i++)
	{
		//printf("in system[%d] = %s\n", i, in_system[i]);
		if (strcmp(in_system[i], filename)==0)
		{
			//printf("Found what to delete\n");
			//strcpy(dir_ptr[i].name, NULL);
			inode_array_ptr[i]->valid = 0;
			flag = 1;
			strcpy(in_system[i],"");
			break;
		}
	}

	if (flag == 0)
	{
		printf("ERROR: File not found!\n");
	}
}

int findFreeDirEntry()
{
	int i;
	int retval = -1;
	int counter = 0;

	for(i = 0; i < 128; i++)
	{
		if(inode_array_ptr[i]->valid == 1)
		{
			counter ++;
		}
	}
	
	//printf("Counter == %d\n", counter);
	if(counter == 127)
	{
		return retval;
	}
	/*for (i = 0; i < 128; i++)
	{
		printf("i == %d\n", i);
		if(dir_ptr[i].valid == 0)
		{
			//printf("ii 1\n");
			retval = i;
			dir_ptr[i].valid = 1;
			break;
		}
	}

	//printf(" ij 1\n"); */

	return counter;
}

int findFreeInode()
{
	int i;
	int retval = -1;
	for (i = 0; i < 128; i ++)
	{
		if(inode_array_ptr[i]->valid == 0)
		{
			retval = i;
			break;
		}
	}

	return retval;
}
 
int findFreeBlock()
{
	int i;
	int retval = -1;
	for (i = 130; i < 4226; i++)
	{
		if(used_blocks[i] == 0)
		{
			retval = i;
			break;
		}
	}
	return retval;
}

int findFreeInodeBlockEntry(int inode_index)
{
	int i;
	int retval = -1;
	for (i = 0; i < 32; i++)
	{
		if(inode_array_ptr[inode_index]->blocks[i] == -1)
		{
			retval = i;
			break;
		}
	}
	return retval;
}


void init()
{
	int i;
	
	//dir_ptr = (struct dir_entry*) &data_blocks[0];

	for (i = 0; i < NUM_FILES; i++)
	{
		dir_ptr[i].valid = 0;
	}

	int inode_idx = 0;

	for (i = 1; i < 130; i++)
	{
		inode_array_ptr[inode_idx++] = (struct inode*) &data_blocks[i];
	}
}














