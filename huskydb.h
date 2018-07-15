#ifndef HUSKYDB_H
#define HUSKYDB_H
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

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <stdlib.h>
#include <fstream>
#include <utility>
#include <stdio.h>
#include <direct.h>
#include <ios>
#include <chrono>
#include <experimental/filesystem>

#include "package.h"
#include "table.h"
#include "file.h"
using namespace std;

class huskydb
{
public:

	// Initialize NoSQL database
	huskydb(string input_path)
	{
		if (folder_exists(input_path))
		{
			cout << "[HuskyDB] Starting HuskyDB - NoSQL database." << endl;
			db_path = input_path;
			index("init");
		}
		else {
			cout << "[HuskyDB] Directory " << input_path << " is not valid." << endl;
			cout << "[HuskyDB] Shutting down database." << endl;
			Sleep(5000);
			exit(EXIT_FAILURE);
		}
	}

	friend class package;
	friend class table;
	friend class file;

	// Deconstruct NoSQL database
	~huskydb() 
	{
		//TODO
	}

	// Index database in memory
	void index(string cmd);

	// Make package
	bool make_package(string package_name);

	// Make table
	bool make_table(string package_name, string table_name);

	// Make normal file from string
	bool make_file(string package_name, string table_name, string file_name, string data);

	// Append data to file from string (can be used for large files)
	bool append_file(string package_name, string table_name, string file_name, vector<string> data);
	
	// Delete package if it exists including all content (tables, files, etc.)
	bool delete_package(string package_name);

	// Delete table if it exists including all content
	bool delete_table(string package_name, string table_name);

	// Delete individual file
	bool delete_file(string package_name, string table_name, string file_name);

	// WARNING: Dumping all data is not recommended
	// Uncomment to allow this functionality

	// Return all package names
	//vector<string> dump_packages() { return _packages; }

	// Return all table names
	//vector<string> dump_tables() { return _tables; }

	// Return all file names
	//vector<string> dump_files() { return _files; }

	// Return packages_tables data
	//unordered_map<string, vector<string>> dump_packages_tables() { return packages_tables;  }

	// Return tables_files data
	//unordered_map<string, unordered_map<string, char> > dump_tables_files() { return tables_files; }

private:

	// VARIABLE DECLARATIONS

	// Location of top level of the database
	string db_path;

	// Used as null output
	vector<table*> empty_tables;
	vector<file*> empty_files;

	// All data
	vector<package*> _packages;
	vector<table*> _tables;
	vector<file*> _files;

	// INTERNAL FUNCTION DECLARATIONS

	// Check if folder exists
	bool folder_exists(string &directory);

	// Check if file exists
	bool file_exists(string &directory);

	// Get elements in directory based on abstract (file or folder)
	void get_elements(vector<string> &files, string &search_path, string abstract);

	// Get folders in directory
	vector<string> get_folders(string &directory);

	// Get files in directory with all file extensions
	vector<string> get_files(string &directory);

	// Get files in directory with specific file extension
	vector<string> get_files(string &directory, string &extension);

	// Make folder in directory if it does not exist
	bool make_folder(string &directory, string &folder_name);

	// Recursively delete folder if it exists
	bool delete_folder(string &directory);

	// Delete file if it exists
	bool delete_file(string &directory, string &file_name);
};

#endif