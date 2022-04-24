#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char**argv)
{
	char shell[20] = "308sh>"; //the shells command line
	char userCmd[1024]; //user command input
	char* cmdTrack; //keeps track of command name running
	int status; //status of process

	//invalid input from user
	if(argc != 1 && argc != 3)
	{
		printf("Error\n");
		exit(0);
	} else if (argc == 3) //change shell name
	  {
		if(strcmp(argv[1], "-p") != 0)
		{
			printf("Error\n");
			return -1;
		} else
		  {
			strcpy(shell, strcat(argv[2], "> "));
		  }
	}

//infinite loop accepting input from user
while (1)
{ 
	printf("%s", shell); //receiving users commands
	fgets(userCmd, 1024, stdin);
	userCmd[strlen(userCmd) - 1] = '\0';

	//checks for background process
	int tempPID = waitpid(-1, &status, WNOHANG);
	if (tempPID > 0)
	{
		printf("[%d] %s Exit %d\n", tempPID, cmdTrack, WEXITSTATUS(status));
	}

//running built-in or programmable commands 
	if(strcmp(userCmd,"exit") ==0 ) //exit out of the shell
	{ 
		exit(0);
	}
	else if(strcmp(userCmd,"pid") ==0 ) //output PID
	{ 
		printf("%d\n",getpid());
	}
	else if(strcmp(userCmd,"ppid") ==0 ) //output PPID
	{ 
		printf("%d\n",getppid());
	}
	else if(strcmp(userCmd,"pwd") ==0 ) //output current working directory
	{ 
		char dir[1024];
		getcwd(dir, sizeof(dir));
		printf("%s\n",dir);
	}
	else if(strncmp(userCmd,"cd",2) ==0 ) //change directory
	{ 
		strtok(userCmd, " ");
		char *directory;
		directory = strtok(NULL, " ");
		if(directory != NULL) // check if the user has a specific directory to change to 
		{ 
			chdir(directory);
		}
		else
		{
			chdir("/home");
		}	
	}
	else //programmable commands
	{ 
		int bckgroundProc =0; // variable to check if a process is a background process
		if(strchr(userCmd, '&')) // check if the current is a background process
		{ 
			bckgroundProc =1;
			int l;
			for(l =0;l<1024;l++)
			{
				if(userCmd[l] == '&')
				{
					userCmd[l] = '\0';
				}
			}		
		}

		char* track[10]; //tracks a command
		track[0] = strtok(userCmd, " "); //record command name
		if(bckgroundProc == 1) //record the command name if its a background process
		{ 
			cmdTrack = track[0];
		}
		char* part = strtok(NULL, " ");
		int i=1;
		int k;
		for(k=1;k<10;k++) //initialize rest of the character array
		{ 
			track[k] = (char*) NULL;
		} 
		while(part != NULL) //record the command entered
		{ 
			track[i] =part;
			part = strtok(NULL, " ");
			i++;			
		}

		int pid,pid2; //variables to keep track of the pid of the child process			
		pid = fork(); //starts child process 
		if(bckgroundProc ==  0)
		{
			if (pid ==0) //child process 
			{ 
				int processPID = getpid();
				printf("[%d] %s\n",processPID,track[0]);
				execvp(track[0],track);
				perror('\0');
				printf("Command failed or was not found\n");
                        	printf("[%d] %s Exit 255\n", processPID, track[0]);
			}
			else //parent process
			{ 
				usleep(1000);
				int outPID =waitpid(pid, &status, 0);
				printf("[%d] %s Exit %d\n", outPID, track[0], WEXITSTATUS(status));	
			}
		}
		else
		{
		
			if (pid ==0) // is child process 
			{ 
                       		pid2 =fork();
				if( pid2 == 0)
				{
					int processPID = getpid();
                                	printf("[%d] %s\n",processPID,track[0]);
                                	execvp(track[0],track);
                                	perror('\0');
                               		printf("Command failed or was not found\n");
                                	printf("[%d] %s Exit 255\n", processPID, track[0]);
				}
				else
				{
					usleep(1000);
					int backPID = waitpid(pid2, &status, 0);
                                	printf("[%d] BackProcess Exit %d\n", backPID, WEXITSTATUS(status));
                                	bckgroundProc =0;
				}
                        }
			else // is parent process
			{ 
                                usleep(200);
                       	}

		}		
	}
}
}
