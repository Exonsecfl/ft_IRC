#include <string>
#include <iostream>

void print_all_caractere(std::string src)
{
	for (size_t i(0); i <= src.size(); i++)
	{
		if (src[i] == '\r')
			std::cout << "-r";
		if (src[i] == '\n')
			std::cout << "";
		if (src[i] == '\0')
			std::cout << "\\0";
		else
			std::cout << src[i];
	}
	std::cout << std::endl;
}
