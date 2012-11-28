#include "config.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		fprintf(stderr, "Provide file to parse\n");
		return 0;
	}
	Config config = Config::parse(argv[1]);
	for (Config::Section section: config["section1"]) {
		for (std::string property: section["value1"]) {
			std::cout << "section1.value1 = " << property << std::endl;
		}
	}
//	for (Config::iterator it = config.begin(); it != config.end(); it++) 
//	{
//		std::cout << "section " << it->first << std::endl;
//		for (Config::Section::iterator it2 = it->second.begin(); it2 != it->second.end(); it2 ++)
//			std::cout << "    " << it->first << "." << it2->first << "="  << it2->second << std::endl;
//	}	
}
