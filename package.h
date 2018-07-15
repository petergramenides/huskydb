#ifndef PACKAGE_H
#define PACKAGE_H
/*Copyright(C)2018 by Peter C. Gramenides

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "table.h"
#include <string>
#include <vector>

using namespace std;

class package
{
public:
	package(string _name, string _path, vector<table*> _tables)
	{
		package_name = _name;
		package_path = _path;
		tables = _tables;
	}
	~package()
	{
		for (size_t x = 0; x < tables.size(); ++x)
		{
			delete tables[x];
		} tables.clear();
	}
	friend class huskydb;
	friend class table;

	string get_path() { return package_path; }
	void set_path(string _path) { package_path = _path; }
	vector<table*> get_tables() { return tables; }
	void set_tables(vector<table*> _tables) { tables = _tables; }
	table* find_table(string _name)
	{
		for (size_t x = 0; x < tables.size(); ++x)
		{
			if (tables[x]->get_name() == _name)
			{
				return tables[x];
			}
		}

		return nullptr;
	}
	string get_name() { return package_name; }
	void set_name(string _name) { package_name = _name; }

private:
	string package_path;
	string package_name;
	vector<table*> tables;
};

#endif
