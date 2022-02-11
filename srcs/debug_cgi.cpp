#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <unistd.h>

int main(int argc, char const *argv[], char const *env[])
{
    std::ofstream out("debug.txt", std::ios_base::app);
    std::time_t t = std::time(nullptr);

    out << "-------------------------------------------" << std::endl;
    out<< "[" << std::put_time(std::localtime(&t), "%F %T %Z") << "] " << std::endl;
    out << "-------------------------------------------" << std::endl;
    int i = 0;
    while (i < argc) {
        out << "argv:" << i << "|" << argv[i] << "|" << std::endl;
        i++;
    }
    int j = 0;
    while (env[j]) {
        out << "env:" << j << "|" << env[j] << "|" << std::endl;
        j++;
    }
    out << "stdin:" << std::endl;
    while (1) {
        char buffer[1000] = {0};
        int ret = read(STDIN_FILENO, buffer, 1000);
        out << buffer;
        if (ret <= 0) {
            break ;
        }
    }
    out << "-------------------------------------------" << std::endl;
    out.close();
    return 0;
}
