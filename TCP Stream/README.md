This project implements a C++ progrm that listens in a loop on a port for incoming TCP requests from clients. For each accepted incoming request it forks a child to read and process the request. The parent process continues to listen and accept incoming TCP requests in an endless loop.

![image](https://user-images.githubusercontent.com/89548047/208328761-25cb4951-8067-4069-b54b-1858208504ae.png)
