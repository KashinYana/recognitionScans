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
	std::cout << config.getSection("section1").getProperty("value1") << std::endl;
	std::cout << config.getSection("section2").getProperty("value3") << std::endl;
	Config::Section section;
	section.add("x", util::IntToStr(1));
	section.add("y", util::IntToStr(1));
	config.add("section", section);
	config.write("updated.cfg");
}
