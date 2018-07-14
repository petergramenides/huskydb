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

#include <huskydb.h>

namespace fs = std::experimental::filesystem;

// Index database in memory
void huskydb::index()
{
	// Gather all packages
	_packages = get_folders(db_path);

	vector<string> all_tables;
	vector<string> all_files;

	// Gather all tables
	for (size_t x = 0; x < _packages.size(); ++x)
	{
		string current_package = _packages[x];
		string path = db_path + "/" + current_package;
		vector<string> tables = get_folders(path);
		packages_tables.insert(make_pair(current_package, tables));

		for (size_t y = 0; y < tables.size(); ++y)
		{
			string current_table = tables[y];
			all_tables.push_back(current_table);

			// Gather all files
			string table_path = path + "/" + current_table;
			vector<string> files = get_files(table_path);
			unordered_map<string, char> file_registry;
			for (size_t z = 0; z < files.size(); ++z)
			{
				string current_file = files[z];
				all_files.push_back(current_file);
				file_registry.insert(make_pair(current_file, 0));
			}
			
			string unique_id = current_package + "-" + current_table;
			tables_files.insert(make_pair(unique_id, file_registry));
		}
	}

	_tables = all_tables;
	_files = all_files;

}

// Get names of all packages from database
vector<string> huskydb::query_tables(string &package)
{
	index();
	vector<string> tables = packages_tables[package];
	return tables;
}

// Get tables from database in package
unordered_map<string, char> huskydb::query_files(string &package, string &table)
{
	index();
	string unique_id = package + "-" + table;
	unordered_map<string, char> files = tables_files[unique_id];
	return files;
}

// Make package
char huskydb::make_package(string &package_name)
{
	return make_folder(db_path, package_name);
}

// Make table
char huskydb::make_table(string &package_name, string &table_name)
{
	string table_path = db_path + "/" + package_name;
	return make_folder(table_path, table_name);
}

// Make normal file from string
char huskydb::make_file(string &package_name, string &table_name, string &file_name, string &data)
{
	string file_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	char response = file_exists(file_path);
	if (response)
	{
		ofstream new_file(file_path);
		new_file << data << endl;
		index();
		return 1;
	}
	else
	{
		cout << "[HuskyDB] Error: " << file_path << " already exists." << endl;
		return 0;
	}

	return 0;
}

// Append data to file from string (can be used for large files)
char huskydb::append_file(string &package_name, string &table_name, string &file_name, vector<string> &data)
{
	string file_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	ofstream file(file_path, ios_base::app | ios_base::out);

	if (!file)
	{
		cout << "[HuskyDB] Error: " << file_path << " is not a valid directory." << std::endl;
		return 0;
	}
	else
	{
		for (unsigned int x = 0; x < data.size(); ++x)
		{
			file << data[x] << endl;
		}
		return 1;
	}

	return 0;
}

// Delete package if it exists including all content (tables, files, etc.)
char huskydb::delete_package(string &package_name)
{
	string path = db_path + "/" + package_name;
	if (folder_exists(path))
	{
		delete_folder(path);
		return 1;
	}
	else 
	{
		cout << "[HuskyDB] Error: " << path << " is not a valid directory." << std::endl;
		return 0;
	}

	return 0;
}

// Delete table if it exists including all content
char huskydb::delete_table(string &package_name, string &table_name)
{
	string path = db_path + "/" + package_name + "/" + table_name;
	if (folder_exists(path))
	{
		delete_folder(path);
		return 1;
	}
	else
	{
		cout << "[HuskyDB] Error: " << path << " is not a valid directory." << std::endl;
		return 0;
	}

	return 0;
}

// Delete individual file
char huskydb::delete_file(string &package_name, string &table_name, string &file_name)
{
	string full_path = db_path + "/" + package_name + "/" + table_name + "/" + file_name;
	string path = db_path + "/" + package_name + "/" + table_name;
	if (file_exists(full_path))
	{
		delete_file(path, file_name);
		return 1;
	}
	else
	{
		cout << "[HuskyDB] Error: " << full_path << " is not a valid directory." << std::endl;
		return 0;
	}

	return 0;
}

// Checks if a folder already exists
char huskydb::folder_exists(string &directory)
{
	DWORD ftyp = GetFileAttributesA(directory.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		cout << "[HuskyDB] Error: " << directory << " is not a valid directory." << std::endl;
		return 0;
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return 1;

	cout << "[HuskyDB] Error: " << directory << " is not a valid directory." << std::endl;
	return 0;
}

// Checks if a file already exists
char huskydb::file_exists(string &directory)
{
	ifstream dir(directory);
	if (!dir)
	{
		cout << "[HuskyDB] Error: " << directory << " is not a valid directory." << std::endl;
		return 0;
	}

	return 1;
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
					files.push_back(fd.cFileName);
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
	get_elements(folders, search_path, "file");
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
char huskydb::make_folder(string &directory, string &folder_name)
{
	string path = directory + "/" + folder_name;
	char response = folder_exists(path);
	if (response)
	{
		fs::create_directory(path);
		index();
		return 1;
	}
	else
	{
		cout << "[HuskyDB] Error: " << path << " already exists." << endl;
		return 0;
	}

	return 0;
}

// Recursively delete folder if it exists
char huskydb::delete_folder(string &directory)
{
	std::uintmax_t n = fs::remove_all(directory);
	index();
	return 1;
}

// Delete file if it exists
char huskydb::delete_file(string &directory, string &file_name)
{
	string full_path = directory + "/" + file_name;
	int response = remove(full_path.c_str());
	index();
	
	if (response != 0)
	{
		cout << "[HuskyDB] Error: " << full_path << " was not deleted." << endl;
		return 0;
	}
	else
	{
		return 1;
	}

	return 0;
}
