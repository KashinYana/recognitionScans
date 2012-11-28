#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <stdexcept>
#include <string>
#include <ctype.h>
#include <algorithm>

namespace util {

static std::string strip(const std::string& value) {
	int firstMeaningLetter = 0;
	int lastMeaningLetter = value.size() - 1;
	while (firstMeaningLetter < value.size() && isspace(value[firstMeaningLetter])) 
		firstMeaningLetter ++;
	while (lastMeaningLetter >= firstMeaningLetter && isspace(value[lastMeaningLetter]))
		lastMeaningLetter --;
	std::string result = "";
	for (int i = firstMeaningLetter; i <= lastMeaningLetter; i ++)
		result += value[i];
	return result;
}

int StrToInt(std::string s) {
	s = strip(s);
	if (s.size() == 0) {
		throw std::invalid_argument("Failed to parse empty string");
	}
	int value = 0;
	int pos = 0;
	int sign = 1;
	if (s[pos] == '-') {
		sign *= -1;
		pos ++;
	}
	if (pos == 0 && s[pos] == '+') {
		pos ++;
	}
	for (int i = pos; i < s.size(); i ++) {
		if (isdigit(s[i])) {
			value = 10 * value + s[i] - '0';
		} else {
			throw std::invalid_argument("Failed to parse " + s + " as integer: expected digit, but " + s[i] + " got");
		}
	}
	return value * sign;
}

std::string IntToStr(int value) {
	std::string result = "";
	std::string sign = "";
	if (value < 0) { 
		sign += '-';
		value = -value;
	}
	if (value == 0) {
		return "0";
	}
	while (value > 0) {
		result += (value % 10 + '0');
		value /= 10;
	}
	std::reverse(result.begin(), result.end());
	return sign + result;
}

std::pair<int,int> StrToPair(std::string s) {
	s = strip(s);
	if (s.size() == 0) {
		throw std::invalid_argument("Failed to parse empty string");
	}
	if (s[0] != '(') {
		throw std::invalid_argument("Failed to parse " + s + " as pair: expected (, got " + s[0]);
	}
	if (s[s.size() - 1] != ')') {
		throw std::invalid_argument("Failed to parse " + s + " as pair: expected ), got " + s[s.size() - 1]);
	}
	int firstDelimPos = s.find(',');
	int lastDelimPos = s.rfind(',');
	if (firstDelimPos == std::string::npos) {
		throw std::invalid_argument("Failed to parse " + s + " as pair: no delim");
	}
	std::string firstValue = s.substr(1, firstDelimPos - 1);
	std::string secondValue = s.substr(firstDelimPos + 1, s.size() - firstDelimPos - 2);
	return std::make_pair(StrToInt(firstValue), StrToInt(secondValue));
}

std::string PairToStr(int first, int second) {
	return "(" + IntToStr(first) + ", " + IntToStr(second) + ")";
}

std::string PairToStr(const std::pair<int,int>& p) {
	return PairToStr(p.first, p.second);
}

} //namespace util ends here

#endif 
