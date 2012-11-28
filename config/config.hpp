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
					class iterator {
						friend class SectionSlice;
						private:
							std::multimap<std::string, std::string>::iterator it;						
							explicit iterator(std::multimap<std::string, std::string>::iterator _it): it(_it) {
							}
						public:
							iterator() {
							}
							iterator& operator ++ () {
								it ++;
								return *this;
							}
							iterator& operator ++ (int) {
								it ++;
								return *this;
							}
							iterator& operator -- () {
								it --;
								return *this;
							}
							iterator& operator -- (int) {
								it --;
								return *this;
							}
							bool operator == (const iterator& rhs) { return this->it == rhs.it; }
							bool operator != (const iterator& rhs) { return this->it != rhs.it; }
							std::string& operator * () { return it->second; }
					};
					iterator begin() {
						return iterator(section.properties.lower_bound(valueName));
					}
					iterator end() {
						return iterator(section.properties.upper_bound(valueName));
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
				class iterator {
					friend class ConfigSlice;
					private:
						std::multimap<std::string, Section>::iterator it;						
						explicit iterator(std::multimap<std::string, Section>::iterator _it): it(_it) {
						}
					public:
						iterator() {
						}
						iterator& operator ++ () {
							it ++;
							return *this;
						}
						iterator& operator ++ (int) {
							it ++;
							return *this;
						}
						iterator& operator -- () {
							it --;
							return *this;
						}
						iterator& operator -- (int) {
							it --;
							return *this;
						}
						bool operator == (const iterator& rhs) { return this->it == rhs.it; }
						bool operator != (const iterator& rhs) { return this->it != rhs.it; }
						Section& operator * () { return it->second; }
				};
//				typedef std::multimap<std::string, Section>::iterator iterator;
				iterator begin() {
					return iterator(config.sections.lower_bound(sectionName));						
				}
				iterator end() {
					return iterator(config.sections.upper_bound(sectionName));
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
					return *it;
				}
		};
	public:
		typedef std::multimap<std::string, Section>::iterator iterator;
		typedef ConfigSlice::iterator slice_iterator;

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
		
		void write(const std::string& fileName) {
			std::ofstream fout(fileName.c_str(), std::ofstream::out);
			bool haveToPrintEoln = false;
			for (Section section: ConfigSlice(*this, "")) {
				for (Section::iterator it = section.begin(); it != section.end(); it ++) {
					fout << it->first << "=" << it->second << std::endl;
					haveToPrintEoln = true;
				}
			}
			for (iterator it = begin(); it != end(); it ++) {
				if (it->first != "") {
					if (haveToPrintEoln) {
						fout << std::endl;
					}
					haveToPrintEoln = true;
					fout << "[" << it->first << "]" << std::endl;
					for (Section::iterator iter = it->second.begin(); iter != it->second.end(); iter ++) {
						fout << iter->first << "=" << iter->second << std::endl;
					}
				}
			}
			fout.close();
		}

};

#endif // __CONFIG_HPP__
