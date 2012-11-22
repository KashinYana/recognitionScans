#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include "parse_exception.hpp"
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <stdexcept>

class Config {
	public:
		class Section {
			private:
				friend class Config;
				std::multimap<std::string, std::string> properties;
				std::set<std::string> propertyNames;
				class SectionSlice 
				{				
					friend class Section;
					friend class Config;
					Section& section;
					std::string valueName;
					SectionSlice(Section& _section, const std::string& _valueName): section(_section), valueName(_valueName) 
					{
					}
					public:
					typedef std::multimap<std::string, std::string> iterator;
					std::multimap<std::string, std::string>::iterator begin() {
						return section.properties.lower_bound(valueName);
					}
					std::multimap<std::string, std::string>::iterator end() {
						return section.properties.upper_bound(valueName);
					}
					unsigned int size() { 
						iterator it = begin();
						iterator to = end();
						unsigned int result = 0;
						while (it != to) {
							result ++;
							it ++;
						}
						return result;
					}
				};
			public:
				void add(const std::string& key, const std::string& value)
				{
					properties.insert(make_pair(key, value));
					propertyNames.insert(key);
				}
				typedef std::multimap<std::string, std::string>::iterator iterator;
				iterator begin() 
				{
					return properties.begin();
				}
				iterator end()
				{
					return properties.end();
				}
				SectionSlice operator [] (const std::string& valueName) {
					return SectionSlice(*this, valueName);
				}
		};
	private:
		std::map<std::string, Section> sections;
		std::set<std::string> sectionNames;
	public:
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
		typedef std::map<std::string, Section>::iterator iterator;

		iterator begin() 
		{
			return sections.begin();
		}

		iterator end()
		{
			return sections.end();
		}

		Config() {
		}

		void add(const std::string& sectionName, const std::string& key, const std::string& value)
		{
			sections[sectionName].add(key, value);
			sectionNames.insert(sectionName);
		}

		static Config parse(const std::string& fileName)
		{
			Config result;			
			std::ifstream fin(fileName.c_str(), std::ifstream::in);
			if (fin.fail()) 
			{
				throw std::invalid_argument("Failed to parse config: failed to read file " + fileName + ".");
			}
			std::string sectionName = "";
			while (fin) 
			{
				std::string currentString;
				std::getline(fin, currentString);
				if (currentString.size() == 0 || currentString[0] == '#') 
				{
					continue; // skip comment
				} 
				else if (currentString.size() >= 2 && currentString[0] == '[' && currentString[currentString.size() - 1] == ']')  
				{
					sectionName = currentString.substr(1, currentString.size() - 2);
				}
				else
				{
					int firstDelim = currentString.find("=");
					if (firstDelim == std::string::npos) 
					{
						throw ParseException("Failed to parse " + fileName + ": " + currentString + " is incorrect");
					}
					std::string key = strip(currentString.substr(0, firstDelim));
					std::string value = strip(currentString.substr(firstDelim + 1, currentString.size() - firstDelim - 1));
					result.add(sectionName, key, value);
				}
			}
			fin.close();
			return result;
		}
		
		Section& operator [] (const std::string& value)
		{
			return sections[value];
		}
};

#endif // __CONFIG_HPP__
