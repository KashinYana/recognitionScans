#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include "../util/util.hpp"
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
					typedef std::multimap<std::string, std::string>::iterator iterator;
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
		std::multimap<std::string, Section> sections;
		std::set<std::string> sectionNames;
		class ConfigSlice { 
			private:
				friend class Config;
				Config& config;
				std::string sectionName;
				ConfigSlice(Config& cfg, const std::string& _sectionName): config(cfg), sectionName(_sectionName) {}
			public:
				typedef std::multimap<std::string, Section>::iterator iterator;
				iterator begin() {
					return config.sections.lower_bound(sectionName);
				}
				iterator end() {
					return config.sections.upper_bound(sectionName);
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
				Section& operator[] (int index) {
					if (index < 0) {
						throw std::invalid_argument("negative array index");
					}
					iterator it = begin();
					iterator _end = end();
					for (int i = 0; i < index; i ++) {
						it ++;
						if (it == _end) {
							throw std::invalid_argument("array index out of bounds");
						}
					}
					return it->second;
				}
		};
	public:
		typedef std::multimap<std::string, Section>::iterator iterator;

		iterator begin() 
		{
			return sections.begin();
		}

		iterator end()
		{
			return sections.end();
		}

		void add(const std::string& sectionName, Section section) {
			sections.insert(std::make_pair(sectionName, section));
			sectionNames.insert(sectionName);
		}

		Config() {
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
			Section currentSection;
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
					result.sections.insert(std::make_pair(sectionName, currentSection));
					currentSection = Section();			
					sectionName = currentString.substr(1, currentString.size() - 2);
				}
				else
				{
					int firstDelim = currentString.find("=");
					if (firstDelim == std::string::npos) 
					{
						throw ParseException("Failed to parse " + fileName + ": " + currentString + " is incorrect");
					}
					std::string key = util::strip(currentString.substr(0, firstDelim));
					std::string value = util::strip(currentString.substr(firstDelim + 1, currentString.size() - firstDelim - 1));
					currentSection.add(key, value);
				}
			}
			result.sections.insert(std::make_pair(sectionName, currentSection));					
			fin.close();
			return result;
		}
		
		ConfigSlice operator [] (const std::string& value) 
		{
			return ConfigSlice(*this, value);
		}
};

#endif // __CONFIG_HPP__
