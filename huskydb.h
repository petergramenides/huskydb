/*Copyright(C)2018 by Peter C. Gramenides

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files(the "Software"), to deal 
in the Software without restriction, including without l> imitation the rights 
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
#include <experimental/filesystem>
using namespace std;

class huskydb
{
public:

	// Initialize NoSQL database
	huskydb(string input_path)
	{
		if (folder_exists(input_path))
		{
			cout << "[HuskyDB] Starting HuskyDB - NoSQL database.";
			db_path = input_path;
			index();
		}
		else {
			cout << "[HuskyDB] Directory " << input_path << "is not valid.";
			cout << "[HuskyDB] Shutting down database.";
			exit(EXIT_FAILURE);
		}
	}

	// Deconstruct NoSQL database
	~huskydb()
	{
		db_path.clear();
		_packages.clear();
		_tables.clear();
		packages_tables.clear();
		tables_files.clear();
	}

	// Index database in memory
	void index();

	// Get names of all packages from database
	vector<string> query_packages() { index(); return _packages; }

	// Get tables from database in package
	vector<string> query_tables(string &package);

	// Get files and priorities from package and/or table
	unordered_map<string, char> query_files(string &package, string &table);

	// Make package
	char make_package(string &package_name);

	// Make table
	char make_table(string &package_name, string &table_name);

	// Make normal file from string
	char make_file(string &package_name, string &table_name, string &file_name, string &data);

	// Append data to file from string (can be used for large files)
	char append_file(string &package_name, string &table_name, string &file_name, vector<string> &data);
	
	// Delete package if it exists including all content (tables, files, etc.)
	char delete_package(string &package_name);

	// Delete table if it exists including all content
	char delete_table(string &package_name, string &table_name);

	// Delete individual file
	char delete_file(string &package_name, string &table_name, string &file_name);

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

	// List of all packages
	vector<string> _packages;

	// List of all tables
	vector<string> _tables;

	// List of all files
	vector<string> _files;

	// Data representation of tables in packages
	// Structure: <package_name, tables>
	unordered_map<string, vector<string>> packages_tables;

	// Data representation of files in tables
	// Structure: <package_name-table_name, <document_name, priority>>
	unordered_map<string, unordered_map<string, char> > tables_files;

	// INTERNAL FUNCTION DECLARATIONS

	// Check if folder exists
	char folder_exists(string &directory);

	// Check if file exists
	char file_exists(string &directory);

	// Get elements in directory based on abstract (file or folder)
	void get_elements(vector<string> &files, string &search_path, string abstract);

	// Get folders in directory
	vector<string> get_folders(string &directory);

	// Get files in directory with all file extensions
	vector<string> get_files(string &directory);

	// Get files in directory with specific file extension
	vector<string> get_files(string &directory, string &extension);

	// Make folder in directory if it does not exist
	char make_folder(string &directory, string &folder_name);

	// Recursively delete folder if it exists
	char delete_folder(string &directory);

	// Delete file if it exists
	char delete_file(string &directory, string &file_name);
};