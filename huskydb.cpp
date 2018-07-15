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

			tbl->set_files(table_files);
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

package* huskydb::query_package(string package_name)
{
	for (size_t x = 0; x < _packages.size(); ++x)
	{
		if (_packages[x]->get_name() == package_name)
		{
			return _packages[x];
		}
	}

	return nullptr;
}

table* huskydb::query_table(string table_name)
{
	for (size_t x = 0; x < _tables.size(); ++x)
	{
		if (_tables[x]->get_name() == table_name)
		{
			return _tables[x];
		}
	}

return nullptr;
}

table* huskydb::query_table(string table_name, string package_name)
{
	package* pkg = query_package(package_name);
	table* table = pkg->find_table(table_name);
	return table;
}

// Make package
bool huskydb::make_package(string package_name)
{
	string package_path = db_path;
	if (!folder_possible(package_path))
	{
		return false;
	}
	else {
		make_folder(db_path, package_name);
		package* pkg = new package(package_name, db_path, empty_tables);
		_packages.push_back(pkg);
		return true;
	} return false;
}

// Make table
bool huskydb::make_table(string package_name, string table_name)
{
	string table_path = db_path + "/" + package_name;
	if (!folder_possible(table_path))
	{
		return false;
	}
	else {
		make_folder(table_path, table_name);
		package* pkg = query_package(package_name);
		table* tbl = new table(table_name, table_path, pkg, empty_files);
		pkg->add_table(tbl);
		_tables.push_back(tbl);
		return true;
	} return false;
}

// Make normal file from string
bool huskydb::make_file(string package_name, string table_name, string file_name, string data, size_t priority)
{
	string file_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	bool response = file_possible(file_path);
	if (response)
	{
		ofstream new_file(file_path);
		new_file << data << endl;
		package* pkg = query_package(package_name);
		table* tbl = pkg->find_table(table_name);
		file* fl = new file(file_name, file_path, priority, tbl);
		tbl->add_file(fl);
		_files.push_back(fl);
		//index();
		return true;
	}
	else {
		return false;
	} return false;
}

// Append data to file from string (can be used for large files)
bool huskydb::append_file(string package_name, string table_name, string file_name, vector<string> data)
{
	string file_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	ofstream file(file_path, ios_base::app | ios_base::out);

	if (!file)
	{
		return false;
	}
	else
	{
		for (size_t x = 0; x < data.size(); ++x)
		{
			file << data[x] << endl;
		}
		return true;
	} return false;
}

// Delete package if it exists including all content (tables, files, etc.)
bool huskydb::delete_package(string package_name)
{
	string path = db_path + "/" + package_name;
	if (folder_possible(path))
	{
		package* pkg = query_package(package_name);
		vector<table*> tables = pkg->get_tables();
		if (tables.size() > 0)
		{
			for (size_t x = 0; x < tables.size(); ++x)
			{
				for (size_t y = 0; y < _tables.size(); ++y)
				{
					if (tables[x]->get_name() == _tables[y]->get_name())
					{
						_tables.erase(_tables.begin() + y);
					}
				}

			} tables.clear();
		}

		for (size_t x = 0; x < _packages.size(); ++x)
		{
			if (_packages[x]->get_name() == package_name)
			{
				_packages.erase(_packages.begin() + x);
			}
		}
		delete_folder(path);
		delete pkg;
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

// Delete table if it exists including all content
bool huskydb::delete_table(string package_name, string table_name)
{
	string path = db_path + "/" + package_name + "/" + table_name;
	if (folder_possible(path))
	{
		package* pkg = query_package(package_name);
		cout << table_name << endl;
		pkg->remove_table(table_name);

		for (size_t x = 0; x < _tables.size(); ++x)
		{
			if (_tables[x]->get_name() == table_name)
			{
				_tables.erase(_tables.begin() + x);
			}
		}
		delete_folder(path);
		table* tbl = query_table(table_name, package_name);
		delete tbl;
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

// Delete individual file
bool huskydb::delete_file(string package_name, string table_name, string file_name)
{
	cout << file_name << endl;
	string full_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	string path = db_path + "/" + package_name + "/" + table_name;
	if (file_possible(full_path))
	{
		table* tbl = query_table(package_name, table_name);
		file* file = tbl->find_file(file_name);
		for (size_t x = 0; x < _files.size(); ++x)
		{
			if (_files[x]->get_name() == file_name)
			{
				_files.erase(_files.begin() + x);
			}
		}
		delete_file(path, file_name);
		tbl->remove_file(file_name);
		delete file;
		return true;
	}
	else
	{
		return false;
	} return false;
}

// PRIVATE INTERNAL FUNCTIONS


// Checks if a folder already exists
bool huskydb::folder_possible(string &directory)
{
	DWORD ftyp = GetFileAttributesA(directory.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	} return false;
}

// Checks if a file already exists
bool huskydb::file_possible(string &directory)
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
	bool response = folder_possible(path);
	if (!response)
	{
		fs::create_directory(path);
		//index();
		cout << "[HuskyDB] Success: folder " << folder_name << " created in database." << endl;
		return true;
	}
	else
	{
		cout << "[HuskyDB] Warning: " << path << " already exists." << endl;
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
