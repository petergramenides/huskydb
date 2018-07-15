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

#include "huskydb.h"

namespace fs = std::experimental::filesystem;

// PUBLIC FUNCTIONS

// Index database in memory
void huskydb::index()
{
	using namespace std::chrono;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	vector<string> package_names = get_folders(db_path);
	for (size_t x = 0; x < package_names.size(); ++x)
	{
		vector<table*> pkg_tables;
		string path = db_path + "/" + package_names[x];
		package* pkg = new package(package_names[x], path, empty_tables);
		vector<string> tables = get_folders(path);

		for (size_t y = 0; y < tables.size(); ++y)
		{
			string table_path = path + "/" + tables[y];
			vector<string> files = get_files(table_path);

			table* tbl = new table(tables[y], table_path, pkg, empty_files);
			vector<file*> table_files;
				
			for (size_t z = 0; z < files.size(); ++z)
			{
				string file_path = table_path + "/" + files[z];
				file* fl = new file(files[z], file_path, 0, tbl);
				table_files.push_back(fl);
				_files.push_back(fl);
			}

			tbl->set_child(table_files);
			pkg_tables.push_back(tbl);
			_tables.push_back(tbl);
		}

		pkg->set_tables(pkg_tables);
		_packages.push_back(pkg);
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();
	cout << "[HuskyDB] Database indexed in " << duration << "ms." << endl;
	cout << "[HuskyDB] # of packages: " << _packages.size() << endl;
	cout << "[HuskyDB] # of tables: " << _tables.size() << endl;
	cout << "[HuskyDB] # of files: " << _files.size() << endl;
}

// Make package
bool huskydb::make_package(string package_name)
{
	return make_folder(db_path, package_name);
}

// Make table
bool huskydb::make_table(string package_name, string table_name)
{
	string table_path = db_path + "/" + package_name;
	return make_folder(table_path, table_name);
}

// Make normal file from string
bool huskydb::make_file(string package_name, string table_name, string file_name, string data)
{
	string file_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	bool response = file_exists(file_path);
	if (response)
	{
		ofstream new_file(file_path);
		new_file << data << endl;
		//index();
		cout << "[HuskyDB] Success: file " << file_name << " created in database." << endl;
		return true;
	}
	else
	{
		cout << "[HuskyDB] Error: " << file_path << " already exists." << endl;
		return false;
	}

	return false;
}

// Append data to file from string (can be used for large files)
bool huskydb::append_file(string package_name, string table_name, string file_name, vector<string> data)
{
	string file_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	ofstream file(file_path, ios_base::app | ios_base::out);

	if (!file)
	{
		cout << "[HuskyDB] Error: " << file_path << " is not a valid directory." << std::endl;
		return false;
	}
	else
	{
		for (size_t x = 0; x < data.size(); ++x)
		{
			file << data[x] << endl;
		}
		return true;
	}

	return false;
}

// Delete package if it exists including all content (tables, files, etc.)
bool huskydb::delete_package(string package_name)
{
	string path = db_path + "/" + package_name;
	if (folder_exists(path))
	{
		delete_folder(path);
		return true;
	}
	else
	{
		cout << "[HuskyDB] Error: " << path << " is not a valid directory." << std::endl;
		return false;
	}

	return false;
}

// Delete table if it exists including all content
bool huskydb::delete_table(string package_name, string table_name)
{
	string path = db_path + "/" + package_name + "/" + table_name;
	if (folder_exists(path))
	{
		delete_folder(path);
		return true;
	}
	else
	{
		cout << "[HuskyDB] Error: " << path << " is not a valid directory." << std::endl;
		return false;
	}

	return false;
}

// Delete individual file
bool huskydb::delete_file(string package_name, string table_name, string file_name)
{
	string full_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	string path = db_path + "/" + package_name + "/" + table_name;
	if (file_exists(full_path))
	{
		delete_file(path, file_name);
		return true;
	}
	else
	{
		cout << "[HuskyDB] Error: " << full_path << " is not a valid directory." << std::endl;
		return false;
	}

	return false;
}

// PRIVATE INTERNAL FUNCTIONS


// Checks if a folder already exists
bool huskydb::folder_exists(string &directory)
{
	DWORD ftyp = GetFileAttributesA(directory.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		cout << "[HuskyDB] Error: " << directory << " is not a valid directory." << std::endl;
		return false;
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}

	cout << "[HuskyDB] Error: " << directory << " is not a valid directory." << std::endl;
	return false;
}

// Checks if a file already exists
bool huskydb::file_exists(string &directory)
{
	ifstream dir(directory);
	if (!dir)
	{
		cout << "[HuskyDB] Error: " << directory << " is not a valid directory." << std::endl;
		return false;
	}

	return true;
}

// Get elements in directory based on abstract (file or folder)
void huskydb::get_elements(vector<string> &files, string &search_path, string abstract)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (abstract == "file") {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					files.push_back(fd.cFileName);
				}
			}
			else if (abstract == "folder") {
				if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					if (strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
					{
						files.push_back(fd.cFileName);
					}
				}
			}
			else {
				cout << "[HuskyDB] Error: " << abstract << " is not a supported type." << endl;
				return;
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
}

// Get folders in directory
vector<string> huskydb::get_folders(string &directory)
{
	vector<string> folders;
	string search_path = directory + "/*.*";
	get_elements(folders, search_path, "folder");
	return folders;
}

// Get files in directory with all file extensions
vector<string> huskydb::get_files(string &directory)
{
	vector<string> files;
	string search_path = directory + "/*.*";
	get_elements(files, search_path, "file");
	return files;
}

// Get files in directory with specific file extension
vector<string> huskydb::get_files(string &directory, string &extension)
{
	vector<string> files;
	string search_path = directory + "/*." + extension + "*";
	get_elements(files, search_path, "file");
	return files;
}

// Make folder in directory if it does not exist
bool huskydb::make_folder(string &directory, string &folder_name)
{
	string path = directory + "/" + folder_name;
	bool response = folder_exists(path);
	if (response)
	{
		fs::create_directory(path);
		//index();
		cout << "[HuskyDB] Success: folder " << folder_name << " created in database." << endl;
		return true;
	}
	else
	{
		cout << "[HuskyDB] Error: " << path << " already exists." << endl;
		return false;
	}

	return false;
}

// Recursively delete folder if it exists
bool huskydb::delete_folder(string &directory)
{
	std::uintmax_t n = fs::remove_all(directory);
	//index();
	cout << "[HuskyDB] Success: " << directory << " deleted in database." << endl;
	return true;
}

// Delete file if it exists
bool huskydb::delete_file(string &directory, string &file_name)
{
	string full_path = directory + "/" + file_name;
	int response = remove(full_path.c_str());
	//index();

	if (response != 0)
	{
		cout << "[HuskyDB] Error: " << full_path << " was not deleted." << endl;
		return false;
	}
	else
	{
		cout << "[HuskyDB] Success: " << file_name << " file deleted in database." << endl;
		return true;
	}

	return false;
}
