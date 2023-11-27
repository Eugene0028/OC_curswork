#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <signal.h>
#define MAX 92160
#define PORT 9090
#define SA struct sockaddr

class Server 
{
private:
struct ProcessInfo {
    std::string pid;
    std::string user;
    double cpuPercent;
    double memoryPercent;
    std::string command;
};

void parseAndWriteToFile(const char* buffer, int words) {
    std::vector<ProcessInfo> processList;
    

    std::stringstream ss(buffer);
    std::string line;

    // Skip the first line as it contains headers
    std::getline(ss, line);

    while (std::getline(ss, line)) {
        std::istringstream iss(line);
        ProcessInfo process;

        // Split the line using whitespace delimiter
        iss >> process.pid >> process.user >> process.cpuPercent >> process.memoryPercent >> process.command;

        processList.push_back(process);
    }
    processList.pop_back();

    // Write the process information to a txt file
    std::ofstream outputFile("parsed_processes.txt");

    // Write headers
    outputFile << std::setw(10) << "PID" << std::setw(10) << "User" << std::setw(15) << "CPU %" << std::setw(15) << "Memory %" << std::setw(15)  << "Command" << std::endl;

    // Write process information
    for (const auto& process : processList) {
        outputFile << std::right << std::setw(10) << process.pid << std::setw(10) << process.user
                << std::setw(15) << (process.cpuPercent) << std::setw(15) << process.memoryPercent << std::setw(15)
                << process.command << std::endl;
    }
    

    outputFile.close();
    std::cout << "File written successfully." << std::endl;
}
  int len;
  struct sockaddr_in servaddr, cli;

public:
    int sockfd, connfd;
    Server(const char* ip)
    {
      this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
      
      if (sockfd == -1){printf("socket creation failed...\n");exit(0);}
      else {printf("Socket successfully created..\n");}

      bzero(&servaddr, sizeof(servaddr));

      servaddr.sin_family = AF_INET;
      servaddr.sin_addr.s_addr = inet_addr(ip);
      servaddr.sin_port = htons(PORT);

      if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0){printf("socket bind failed...\n");exit(0);}
      else printf("Socket successfully binded..\n");

      if ((listen(sockfd, 5)) != 0){printf("Listen failed...\n");exit(0);}
      else printf("Server listening..\n");

      len = sizeof(cli);
      connfd = accept(sockfd, (SA *)&cli, (socklen_t *)&len);

      if (connfd < 0){printf("server accept failed...\n");exit(0);}
      else printf("server accept the client...\n");
    }

void kill_pid(int connfd)
{
  int pid2;
    std::cout << "Введите PID: ";
    std::cin >> pid2;
    std::string pidString2 = std::to_string(pid2);
    int bytesSent2 = write(connfd, pidString2.c_str(), pidString2.length());
    if (bytesSent2 < 0) {
        std::cerr << "Ошибка при отправке второго PID-процесса серверу" << std::endl;
        return;
    }
    close(connfd);
    close(sockfd);
}
  void func(int connfd)
  {
    FILE* fp = fopen("received_process.txt", "w");
    if (fp == NULL) {
        perror("Unable to open file");
        return;
    }

    char* buffer = new char[92160];

    int words;

    read(connfd, &words, sizeof(int));
sleep(1);
    if (read(connfd, buffer, MAX) <= 0) {
        perror("Error reading from socket");
        return;
    }
    
    
    parseAndWriteToFile(buffer, words);

    fclose(fp);
    printf("File receivedn\n");
  }
};
