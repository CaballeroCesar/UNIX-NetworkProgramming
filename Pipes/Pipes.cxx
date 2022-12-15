#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
	pid_t pid;
	int rs;
    char command[256];
     
    // do while loop to keep executing commands
    do
    {
		//get user input first time for what command to run
		cout << "Enter command: ";
		cin.getline(command,256);
		
		if(strcmp(command,"exit") == 0) //if command entered is "exit" program ends
		{
			exit(EXIT_SUCCESS);
		}
		
		else //if command is anything but "exit" execute commands
		{
			pid = fork(); //forking to create 2 processes, storing id to know where we are
			
			if(pid == -1) //error if program cant fork
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			
			else if( pid == 0) //child process
			{
				char *argv[] = {command, argv[1], argv[2], argv[3], argv[4], NULL}; //max # of args
				argv[0] = strtok(command," ");//get first word
				int i=0;//iterator for rest of args
				
				//loop if there are more arguments to be inserted to argv
				while(argv[i] != NULL)
				{
					argv[++i]=strtok(NULL," "); //store rest of arguments in argv
				}	
				rs = execvp(argv[0],argv); //perform given comands with execvp within program
					
				if (rs == -1) //error if execvp doesn't receive actual commands
				{
					cerr << command <<": No such file or directory" << endl;
					exit(EXIT_FAILURE);
				}
			}
			
			else // parent process
			{
				wait(nullptr); //wait for child process to execute first
			}
		}
	
	}while(true); //keep looping program 

    return 0;
}
