#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <unistd.h>

int main(int argc, char const *argv[], char const *env[])
{
	std::ostringstream	out;
	std::time_t t = std::time(NULL);
	out << "-------------------------------------------" << std::endl;
	out << "[" << std::put_time(std::localtime(&t), "%F %T %Z") << "] " << std::endl;
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
	std::cout << "Content-Type: text/plain;\r\n\r\n";
	std::cout << "A copy of this is located at debug.txt" << std::endl;
	std::cout << out.str();
	std::ofstream	file("debug.txt", std::ios_base::app);
	file << out.str();
	file.close();
	return 0;
}
