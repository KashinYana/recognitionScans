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
	Config::Section section;
	section.add("x", util::IntToStr(1));
	section.add("y", util::IntToStr(1));
	config.add("section", section);
	config.write("updated.cfg");
}
