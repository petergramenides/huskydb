#ifndef FILE_H
#define FILE_H
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

//#include "table.h"
#include <string>

using namespace std;

class table;

class file
{
public:
	file(string _fname, string _path, size_t _priority, table* _parent)
	{
		file_name = _fname;
		file_path = _path;
		priority = _priority;
		parent = _parent;
	}
	~file() {}
	friend class huskydb;
	friend class table;
	void set_priority(size_t _priority) { priority = _priority; }
	size_t get_priority() { return priority; }
	string get_path() { return file_path; }
	void set_path(string _path) { file_path = _path; }
	table* get_parent() { return parent; }
	void set_parent(table* _parent) { parent = _parent; }
	string get_name() { return file_name; }
	void set_name(string _name) { file_name = _name; }
	bool is_child(table* _parent) { return parent == _parent; }

private:
	string file_name;
	string file_path;
	size_t priority;
	table* parent;

};

#endif