
#include "server.hpp"
#include "client.hpp"
#include "TaskManager.hpp"


int main() 
{
    TaskManager* tm = new TaskManager();
    int mode = 1;
    while(1)
    {
        tm->clear();
        tm->task_manager(mode);
        std::cout << "\n" << "What do you want?\n1:Update\n2:Kill<pid>\n3:I am server\n4:I am client\n5:Close\n";
        std::string c = "";
        std::cin >> c;
        if(!isdigit(c[0]) && c.length() != 1)continue;
        switch(tm->valid_enter(c[0]))
        {
            case 1:
            {
                tm = new TaskManager();
                break;
            }
            case 2:
            {
                tm->enter_in_terminal(2);
                break;
            }
            case 3:
            {
                Server* server = new Server("192.168.3.44");
                server->func(server->connfd);
                tm->task_manager(3);
                server->kill_pid(server->connfd);
                close(server->connfd);
                break;
            }
            case 4:
            {
                Client* client = new Client();
                client->func(client->sockfd);
                //close(client->sockfd);
                break;
            }
            case 5:
            {
                printf("Bye!\n");
                return 0;
            }
            case 6:
            {

            }
            default:
            {
                tm->clear();
                std::cout << "Try again\n";
                break;
            }
        }
    }

    return 0;
}

