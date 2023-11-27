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
class TaskManager
{
private:
struct ProcessInfo {
    std::string pid;
    std::string user;
    double cpuPercent;
    double memoryPercent;
    std::string command;
};
public:
    std::vector<ProcessInfo> processList;
    TaskManager(){this->processList = GetAllProcesses();}
private:
    std::vector<ProcessInfo> GetAllProcesses() 
    {
        std::vector<ProcessInfo> processList;
        DIR* directory = opendir("/proc");
        dirent* entry;
        
        while ((entry = readdir(directory)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                std::string pid = entry->d_name;
                if (pid.find_first_not_of("0123456789") == std::string::npos) 
                {
                    std::string statusFilePath = "/proc/" + pid + "/status";
                    std::ifstream statusFile(statusFilePath);
                    std::string line;
                    ProcessInfo processInfo;
                    
                    processInfo.pid = pid;
                    
                    while (std::getline(statusFile, line)) 
                    {
                        std::istringstream iss(line);
                        std::string key, value;
                        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
                            if (key == "Uid") 
                            {
                                std::istringstream uidIss(value);
                                std::string uidd;
                                uidIss >> uidd;
                                uid_t uid = stoi(uidd);
                                struct passwd *pw = getpwuid(uid);
                                processInfo.user = uid;
                                if (pw != NULL) {
                                    processInfo.user = pw->pw_name;
                                }
                            }
                            else if (key == "Name") {
                                processInfo.command = value;
                            }
                        }
                    }
                    statusFile.close();
                    
                    std::string statFilePath = "/proc/" + pid + "/stat";
                    std::ifstream statFile(statFilePath);
                    std::string statLine;
                    std::getline(statFile, statLine);
                    std::istringstream statIss(statLine);
                    std::string token;
                    int fieldNumber = 1;
                    while (std::getline(statIss, token, ' ')) {
                        if (fieldNumber == 14) {
                            processInfo.cpuPercent = std::stof(token);
                        }
                        else if (fieldNumber == 23) {
                            processInfo.memoryPercent = std::stof(token);
                        }
                        fieldNumber++;
                    }
                    statFile.close();
                    
                    processList.push_back(processInfo);
                }
            }
        }
        
        closedir(directory);
        return processList;
    }
public:
    std::string formatMemory(double memory) 
    {
    while (memory >= 2) {
        memory /= 1024;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << memory;
    return ss.str();
}

void clear() {
    std::cout << "\x1B[2J\x1B[H";
}


int output(const char* name)
{
    std::ifstream file(name); // Открытие файла для чтения

                if (file) {
                    std::cout << file.rdbuf(); // Вывод содержимого файла в терминал
                    file.close(); // Закрытие файла
                } else {
                    std::cerr << "Не удалось открыть файл!" << std::endl;
                    return 1;
                }
                return 0;
}
    int enter_in_terminal(int mode)
    {
        switch(mode)
        {
            case 1:
            {
                output("processes.txt");
                break;
            }
            case 2:
            {
                std::cout << "Enter PID: ";
                std::string pid;
                std::cin >> pid;
                std::string command = "kill " + pid;
                system(command.c_str());
                break;
            }
            case 3:
            {
                output("parsed_processes.txt");
                break;
            }
        }
        return 0;
    }

    int task_manager(int mode)
    {
        if (mode == 1)
        {
            std::ofstream outputFile("processes.txt");
            if (outputFile.is_open()) {
                outputFile << std::right << std::setw(10) << "PID" << std::setw(10) << "User" << std::setw(15) << "CPU %" << std::setw(15) << "Memory %" << std::setw(13) <<"Command" << std::endl;

                for (const auto& process : processList) {
                    outputFile << std::right << std::setw(10) << process.pid << std::setw(10) << process.user
                            << std::setw(15) << process.cpuPercent / 100 << std::setw(15) << formatMemory(process.memoryPercent)
                            << process.command << std::endl;
                }

                outputFile.close();
                std::cout << "Process information saved to processes.txt" << std::endl;
            } else {
                std::cerr << "Unable to open output file" << std::endl;
            }
        }
            enter_in_terminal(mode);
            return 0;
    }
    int valid_enter(char a)
    {
    return (int)a - 48;
    }
};