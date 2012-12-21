#include "../images/image.hpp"
#include "../config/config.hpp"
#include "../rotate/anchor_rotater.hpp"
#include "../thresholding/thresholding.hpp"
#include "../recognition/handwritten_digits/recognizer.hpp"
#include "splitter.hpp"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		std::cerr << "Provide file to process" << std::endl;
		return 1;
	}
	std::string config_file = "config_marks.cfg"; // TODO: pass config file as command-line parameter
	std::string image_file = argv[1];
	Image::Image image = Image::read(image_file);
	Config config = Config::parse(config_file);
	AnchorRotater anchorRotater(config);
	image = anchorRotater.rotate(image);
	std::cerr << "Image normalized" << std::endl;
	image = Thresholding(image).transform();
	std::cerr << image.rows << " " << image.cols << std::endl;
	std::vector<InterestingObject> pieces = Splitter(config).split(image);
	std::cerr << "Found " << pieces.size() << " interesting objects" << std::endl;
	Config parse_result_config;
	HandwrittenDigitsRecognizer recognizer;
	for (int i = 0; i < pieces.size(); i ++) {
		Config::Section section;
		std::string value = recognizer.recognize(pieces[i].image);
		std::string outputFile = "parsed/" + value + "/image_"  + util::IntToStr(i + 1) + ".jpg";
		Image::write(outputFile, pieces[i].image);		
		section.add("output_file", outputFile);
		section.add("x", util::IntToStr(pieces[i].x));
		section.add("y", util::IntToStr(pieces[i].y));
		section.add("height", util::IntToStr(pieces[i].height));
		section.add("width", util::IntToStr(pieces[i].width));	
		section.add("recognized_value", value);
		parse_result_config.add("input_field", section);
		std::cerr << "Digit " << i + 1 << "/" << pieces.size() << " proceed: " << value + " found"<< std::endl;
	}
	parse_result_config.write("parsed/result.cfg");
	return 0;
}
