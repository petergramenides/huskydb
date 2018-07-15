#ifndef TABLE_H
#define TABLE_H
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

#include "file.h"
#include "package.h"
#include <vector>
#include <string>

using namespace std;

class table
{
public:
	table(string _fname, string _fpath, package* _parent, vector<file*> _child)
	{
		folder_name = _fname;
		folder_path = _fpath;
		parent = _parent;
		child = _child;
	}
	friend class huskydb;
	friend class package;
	friend class file;
	~table()
	{
		for (size_t x = 0; x < child.size(); ++x)
		{
			delete child[x];
		} child.clear();
	}
	string get_name() { return folder_name; }
	void set_name(string _name) { folder_name = _name; }
	string get_path() { return folder_path; }
	void set_path(string _path) { folder_path = _path; }
	package* get_parent() { return parent; }
	void set_parent(package* _parent) { parent = _parent; }
	vector<file*> get_child() { return child; }
	void set_child(vector<file*> _child) { child = _child; }
	void add_file(file* file) { child.push_back(file); }
	vector<file*> find_file(size_t priority)
	{
		vector<file*> matches;
		for (size_t x = 0; x < child.size(); ++x)
		{
			if (child[x]->get_priority() == priority)
			{
				matches.push_back(child[x]);
			} return matches;
		}
	}
	file* find_file(string name_)
	{
		for (size_t x = 0; x < child.size(); ++x)
		{
			if (child[x]->get_name() == name_)
			{
				return child[x];
			}
		} return nullptr;
	}

private:
	string folder_name;
	string folder_path;
	package* parent;
	vector<file*> child;

};

#endif