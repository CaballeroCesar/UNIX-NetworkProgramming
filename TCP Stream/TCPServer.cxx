//EXTRA CREDIT DONE

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;

char root[256]; //global variable used to store argv[2] to combine with path

void processClientRequest(int connSock) 
{
	int received;
	char path[1024], buffer[1024];

	// read a message from the client
	if ((received = read(connSock, path, sizeof(path))) < 0) 
	{
		perror("receive");
		exit(EXIT_FAILURE);
	}
	
	cout << "Client request: " << path << endl;
	
	if (strcmp(path,"..") == 0 && strcmp(path,".") == 0) //chek to not alow path to have .. or . 
	{
		cerr<< "Path cannot contain \"..\"" << endl;
		exit(EXIT_FAILURE);
	
	}
	
	if(strcmp(path,"GET /") ==-1 || strcmp(path,"INFO")==-1 ) //make sure "GET /" or "INFO" are entries
    {
		cerr << "Request files or directories with \"GET\" or the time with \"INFO\"" << endl;
        exit(EXIT_FAILURE);
    }


	if (strcmp(path,"INFO") == 0) // EXTRA CREDIT PART //
	{
		auto start = std::chrono::system_clock::now(); //part of c++11 to get current time and date
		auto end = std::chrono::system_clock::now();
 
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		
		strcpy(buffer,"Todays date and time : "); // store info in buffer to send to client
		strcat(buffer, std::ctime(&end_time));
		
		cout << "sent: " << buffer << std::endl;
     
        write(connSock, buffer, strlen(buffer)); //send to client
		close(connSock); 
		exit(EXIT_SUCCESS); //close and exit after succesfully sending time and date to client
	}// EXTRA CREDIT //
	
	
	// In order to work with the path given you must delete "GET" from the input because our system calls won't know what that means
	string str; //convert path to str
	char charStr[24]; //convert str back to a char array
	char get[] = "GET "; // characters to be removed from the path
	
	if (strncmp(path,"GET",3) == 0) // if GET is found in pathname remove "GET" and store the rest only
	{
		str.append(path);//convert path to str to be able to use below method of erasing part of string
		
		for(size_t i =0; i<strlen(get); ++i)
		{
			str.erase(std::remove(str.begin(), str.end(), get[i]), str.end());
			strcpy(charStr, str.c_str()); //after "GET" is erased from string convert it back to a char array
		}
		 
		//pathname must contain a "/" at the beginning so only store them into path variable if they start with "/"
		if(charStr[0] == '/')
		{
			strcpy(path, charStr);
		}
		else
		{
			strcpy(buffer, "Path must start with \"/\"");
			cerr << buffer << endl;
			write(connSock, buffer, strlen(buffer));
			exit(EXIT_FAILURE);
		}
		
	}
	//combine the root and the path to use to open directories/files
	strcat(root, path);//
	strcpy(path,root);
	
	// open directory
	DIR *dirp = opendir(path);
	if (dirp == 0) 
	{
	
		if (errno == ENOENT) //if pathname doesnt exist tell the client
		{
			strcpy(buffer,"Error: ");
			strncat(buffer, &path[0], strlen(path));
			strcat(buffer, " not found");
			
			write(connSock, buffer, strlen(buffer));//send to client
		}
		
		
		else if(errno == ENOTDIR) //if not directory then it is a file
		{
						
			int fd = open(path, O_RDONLY); //open file
			
			if (fd == -1) //exit if problem opening file
			{
				perror(path);
				exit(EXIT_FAILURE);
			}

			int rd = read(fd, buffer, 1024);// read from file
			if (rd == -1) 
			{
				perror(path);
				exit(EXIT_FAILURE);
			}
			
			//send the contents of file back to client with proper heading
			char contents[] = "... content of ";
			strncat(contents, &path[0], strlen(path));
			strcat(contents, "\n");
			write(connSock, contents , strlen(contents));
			write(connSock, buffer, strlen(buffer));
		}
			
	}
	
	// read directory entries
	struct dirent *dirEntry;
	
	while ((dirEntry = readdir(dirp)) != NULL) 
	{
		if(strcmp(dirEntry->d_name,".") != 0 && strcmp(dirEntry->d_name,"..") != 0)
		{
			strcpy(buffer, dirEntry->d_name); 
			strcat(buffer, " ");
			
			if (write(connSock, buffer, strlen(buffer)) < 0) 
			{
				perror("write");
				exit(EXIT_FAILURE);
			}
			cout << "sent: " << buffer;	
		}
	}	
	
	closedir(dirp);
	cout << "done with client request\n";
	close(connSock);
	exit(EXIT_SUCCESS);
}
        
int main(int argc, char *argv[]) 
{

	if (argc != 3) //must equal 3 
	{
		cerr << "USAGE: z1944013 port pathname\n";
		exit(EXIT_FAILURE);
	}
	
	
	strcpy(root,argv[2]); //store the root from argc[2] into global variable root
	
	// Create the TCP socket 
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}	
	
	// create address structures
	struct sockaddr_in server_address;  // structure for address of server
	struct sockaddr_in client_address;  // structure for address of client
	unsigned int addrlen = sizeof(client_address);	

	// Construct the server sockaddr_in structure 
	memset(&server_address, 0, sizeof(server_address));   /* Clear struct */
	server_address.sin_family = AF_INET;                  /* Internet/IP */
	server_address.sin_addr.s_addr = INADDR_ANY;          /* Any IP address */
	server_address.sin_port = htons(atoi(argv[1]));       /* server port */

	// Bind the socket
	if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) 
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}	
	
	// listen: make socket passive and set length of queue
	if (listen(sock, 64) < 0) 
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	cout << "Server listening on port: " << argv[1] << endl;

	// Run until cancelled 
	while (true) 
	{
		int connSock=accept(sock, (struct sockaddr *) &client_address, &addrlen);
		if (connSock < 0) 
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		// fork
		if (fork()) // parent process
		{ 	    
			close(connSock);
		} 
		
		else // child process
		{ 			
			processClientRequest(connSock);
		}
	}	
	close(sock);
	return 0;
}

