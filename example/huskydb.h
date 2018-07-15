/*
Copyright(C)2018 by Peter C. Gramenides

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

#ifndef HUSKYDB_H
#define HUSKYDB_H

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

#include "dllExports.h"

#ifdef DeleteFile // Due to Windows.h name conflicts.
#undef DeleteFile
#endif

using namespace std;

class HUSKYDB_API CHuskyDB
{
public:

	// Initialize NoSQL database
	CHuskyDB(string input_path);

	// Initialize NoSQL database (with prioritization setting)
	CHuskyDB(string input_path, bool toggle_pr);

	// Deconstruct NoSQL database
	~CHuskyDB();


	// Index database in memory
	void					Index(bool toggle_pr);

	// Find package by name
	CPackage*				QueryPackage(string package_name);

	// Find table through table name
	CTable*					QueryTable(string table_name);

	// Find table through multiple parameters
	CTable*					QueryTable(string table_name, string package_name);

	// Make package
	bool					MakePackage(string package_name);

	// Make table
	bool					MakeTable(string package_name, string table_name);

	// Make priority file from string
	bool					MakeFile(string package_name, string table_name, string file_name, string data, size_t priority);

	// Append data to file from string (can be used for large files)
	bool					AppendFile(string package_name, string table_name, string file_name, vector<string> data);
	
	// Delete package if it exists including all content (tables, files, etc.)
	bool					DeletePackage(string package_name);

	// Delete table if it exists including all content
	bool					DeleteTable(string package_name, string table_name);

	// Delete individual file
	bool					DeleteFile(string package_name, string table_name, string file_name);

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
	string					m_DbPath;

	// Used as null output
	vector<CTable*>			m_EmptyTables;
	vector<CFile*>			m_EmptyFiles;

	// All data
	vector<CPackage*>		m_Packages;
	vector<CTable*>			m_Tables;
	vector<CFile*>			m_Files;

	// INTERNAL FUNCTION DECLARATIONS

	// Check if folder exists
	bool					FolderPossible(string &directory);

	// Check if file exists
	bool					FilePossible(string &directory);

	// Get elements in directory based on abstract (file or folder)
	void					GetElements(vector<string> &files, string &search_path, string abstract);

	// Get folders in directory
	vector<string>			GetFolders(string &directory);

	// Get files in directory with all file extensions
	vector<string>			GetFiles(string &directory);

	// Get files in directory with specific file extension
	vector<string>			GetFiles(string &directory, string &extension);

	// Make folder in directory if it does not exist
	bool					MakeFolder(string &directory, string &folder_name);

	// Recursively delete folder if it exists
	bool					DeleteFolder(string &directory);

	// Delete file if it exists
	bool					DeleteFile(string &directory, string &file_name);
};

#endif