#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <cmath>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <pwd.h>
#include <cstring>
#include <signal.h>
#define MAX 92160
#define PORT 9090
#define SA struct sockaddr
class Client 
{
private:
  struct sockaddr_in servaddr, cli;
public:
  int sockfd, connfd;
  Client()
  {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {printf("socket creation failed...\n");return;}
    else printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.3.44");
    servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {printf("connection with the server failed...\n"); return;}
    else printf("connected to the server..\n");
    func(sockfd);
  }

  

  
  void func(int sockfd)
  {
      FILE* file = fopen("processes.txt", "r");
      if (file == NULL) {
          printf("Unable to open filen");
          return;
      }
      
      char buffer[MAX];
      int words = 0;
      char ch;

      while (fgets(buffer, MAX, file) != NULL) {
          write(sockfd, buffer, strlen(buffer));
          words++;
      }

      // Отправляем количество строк
      write(sockfd, &words, sizeof(int));

      fclose(file);
      printf("File sentn");

      sleep(2); 

      char buffer1[256];
      memset(buffer1, 0, sizeof(buffer1));
      int bytesRead1 = read(sockfd, buffer1, sizeof(buffer1) - 1);
      if (bytesRead1 < 0) {
          std::cerr << "Ошибка при чтении первого PID-процесса от клиента" << std::endl;
          return;
      }
      int pid = atoi(buffer1);
      std::cout << "Получен PID: " << pid << std::endl;
      if (pid > 0) {
        if (kill(pid, SIGKILL) < 0) {
            std::cerr << "Ошибка при уничтожении процесса" << std::endl;
        }
        else std::cout << "Процесс с PID " << pid << " успешно уничтожен" << std::endl;
    } else {
        std::cerr << "Получен некорректный PID" << std::endl;
        return;
    }
    // close(connfd);
    close(sockfd);
  }

};