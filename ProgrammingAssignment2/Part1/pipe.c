#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
	int fd1[2]; //first pipe
	int fd2[2]; //second pipe
	pipe(fd1); //initialize first pipe
	pipe(fd2); //initialize second pipe
	char input_str[100]; //character buffer

	//get string from user
	printf("Input string: ");
	scanf("%[^\n]", input_str);

	pid_t p;

	printf("Inputted string : %s", input_str);
	printf("\n");

	p = fork(); //create child process

	//error handling for if fork fails
	if (p < 0)
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}

	//parent process
	else if (p != 0)
	{
		char flipped_str[100];

		close(fd1[0]); //close reading end of first pipe

		write(fd1[1], input_str, strlen(input_str) + 1); //write string to first pipe
		close(fd1[1]); //close writing end of first pipe

		wait(NULL); //wait for child process to return flipped string

		close(fd2[1]); //close writing end of second pipe

		read(fd2[0], flipped_str, 100); //read flipped string from child process
		printf("Flipped string: %s", flipped_str);
		printf("\n");
		close(fd2[0]); //close reading end of second pipe
	}
	
	//child process
	else
	{
		close(fd1[1]); //close writing end of first pipe
		
		char flipped_str[100];
		read(fd1[0], flipped_str, 100); //read input string from parent process

		//produce flipped string
		for(int i = 0; i < strlen(flipped_str); i++)
		{
			//uppercase to lowercase
			if(flipped_str[i] >= 'A' && flipped_str[i] <= 'Z')
			{
				flipped_str[i] = flipped_str[i] + 32;
			}
			//lowercase to uppercase
			else if(flipped_str[i] >= 'a' && flipped_str[i] <= 'z')
			{
				flipped_str[i] = flipped_str[i] - 32;
			}
		}

		flipped_str[strlen(flipped_str)] = '\0'; //append string-terminator

		close(fd1[0]); //close reading end of first pipe
		close(fd2[0]); //close reading end of second pipe

		write(fd2[1], flipped_str, strlen(flipped_str) + 1); //write flipped string to second pipe
		close(fd2[1]); //close writing end of second pipe
	
		exit(0); //terminate child process
	}
}
