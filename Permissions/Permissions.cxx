#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
	bool clear = false;
	bool noC = false;
	int fd;
	int fd2;
	char *file;
	char *message;
	struct stat buf;
	
	//if more than 1 argument is input we can proceed
	if (argc > 1) 
	{	 
		if (0 == strcmp("-c",argv[1])) //check to see if the first argument is "-c"
		{
			clear = true; //boolean value for later use
			file = argv[2]; //if "-c" is used the filename is in argv[2]
			message = argv[3]; // if "-c" is used message is in argv[3]
			fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 00000); //open the file but truncate it to get rid of anything in file
		}
		
		else //if "-c" is not used
		{
			noC = true; //boolean for later use
			file = argv[1]; //file in this case is argv[1] instead of arg[2]
			message = argv[2]; //message in this case is located in argv[2] instead of argv[3]
			fd = open(file, O_WRONLY |O_APPEND | O_CREAT, 00000); //open file and overwrite to anything in there
		}	
		
		close(fd); //close file at the end since we no longer need it
			
		if (stat(file, &buf) == -1) //check what permissions file has
		{
			cerr << file << ": Permission denied" << endl;
			exit(EXIT_FAILURE);
		}
		
		if ((buf.st_mode & 777) != 0) //if file entered has permissions it wont be allowed
		{ 
			cout << file << " is not secure.   Ignoring." << endl;
			exit(EXIT_FAILURE);
		}
		
		chmod(file, 200); //give the user write permissions
		
		if(clear == true) //re-openeing file again truncating it with message
		{
			fd2 = open(file, O_WRONLY | O_TRUNC | O_CREAT, 00000);
			if (fd2==-1) 
			{
				perror("open");//error if file does not open
				return -1;
			}
		}
		
		if(noC == true) //re-opening file but overwriting message
		{
			fd2 = open(file, O_WRONLY |O_APPEND | O_CREAT, 00000);
			if (fd2==-1) 
			{
				perror("open");//error if file does not open
				return -1;
			}
		}
		
		write(fd2, message, strlen(message));//write message into file
		write(fd2, "\n", 1); // new line after message for better formatting

		chmod(file,000); // revoke permissions from file again
		close(fd2); // close file after we are done with it
	}
		
	//if the user uses wrong inputs, these messages print out to show proper way
	else 
	{
			cerr << "Usage:  ./z1944013 [-c] out_file message_string" << endl;
			cerr << "\twhere the message_string is appended to out_file." << endl;
			cerr << "\tThe -c option clears the file before the mesage is appended" << endl;
			return 1;
	}

	return 0;
}
