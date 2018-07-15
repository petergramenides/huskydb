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
//#include "package.h"
#include <vector>
#include <string>

using namespace std;

class package;

class table
{
public:
	table(string _fname, string _fpath, package* _package, vector<file*> _docs)
	{
		folder_name = _fname;
		folder_path = _fpath;
		package = _package;
		docs = _docs;
	}
	friend class huskydb;
	friend class package;
	friend class file;
	~table()
	{
		for (size_t x = 0; x < docs.size(); ++x)
		{
			delete docs[x];
		} docs.clear();
	}
	string get_name() { return folder_name; }
	void set_name(string _name) { folder_name = _name; }
	string get_path() { return folder_path; }
	void set_path(string _path) { folder_path = _path; }
	package* get_package() { return package; }
	void set_package(package* _package) { package = _package; }
	vector<file*> get_file() { return docs; }
	void set_files(vector<file*> _docs) { docs = _docs; }
	void add_file(file* file) { docs.push_back(file); }
	vector<file*> find_file(size_t priority)
	{
		vector<file*> matches;
		for (size_t x = 0; x < docs.size(); ++x)
		{
			if (docs[x]->get_priority() == priority)
			{
				matches.push_back(docs[x]);
			} return matches;
		}
	}
	void remove_file(string file_name)
	{
		for (size_t x = 0; x < docs.size(); ++x)
		{
			if (docs[x]->file_name == file_name)
			{
				docs.erase(docs.begin() + x);
			}
		}
	}
	file* find_file(string name_)
	{
		for (size_t x = 0; x < docs.size(); ++x)
		{
			if (docs[x]->get_name() == name_)
			{
				return docs[x];
			}
		} return nullptr;
	}

private:
	string folder_name;
	string folder_path;
	package* package;
	vector<file*> docs;

};

#endif