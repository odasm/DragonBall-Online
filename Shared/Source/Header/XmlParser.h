#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <libxml\parser.h>
#include <iostream>
#include <memory>

#pragma comment(lib, "libxml2.lib")

class XmlParser
{
	static XmlParser singleton;
	XmlParser();
public:
	bool loadFile(char* filename);
	char* GetStr(char* fieldname, char* valuename);
	int GetInt(char* fieldname, char* valuename);
	void Free();
	char* GetChildStr(char* child, char* fieldname, char* valuename);
	int GetChildInt(char* child, char* fieldname, char* valuename);
	static XmlParser *get() noexcept
	{
		return &singleton;
	}
private:
	xmlDocPtr doc;
	xmlNodePtr cur, node;
	std::string filename;
};
#define sXmlParser XmlParser::get()
#endif XMLPARSER_H