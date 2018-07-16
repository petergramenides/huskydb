/*Copyright(C)2018 by Peter Gramenides.

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
#include "prioritzation_parser.h"

namespace fs = std::experimental::filesystem;

// PUBLIC FUNCTIONS

// Initialize NoSQL database
CHuskyDB::CHuskyDB(string input_path)
{
	if (FolderPossible(input_path))
	{
		cout << "[HuskyDB] Starting HuskyDB - NoSQL database." << endl;
		m_DbPath = input_path;
		Index(false);
	}
	else {
		cout << "[HuskyDB] Directory " << input_path << " is not valid." << endl;
		cout << "[HuskyDB] Shutting down database." << endl;
		Sleep(5000);
		exit(EXIT_FAILURE);
	}
}

// Initialize NoSQL database (with prioritization setting)
CHuskyDB::CHuskyDB(string input_path, bool toggle_pr)
{
	if (FolderPossible(input_path))
	{
		cout << "[HuskyDB] Starting HuskyDB - NoSQL database." << endl;
		m_DbPath = input_path;
		Index(toggle_pr);
	}
	else {
		cout << "[HuskyDB] Directory " << input_path << " is not valid." << endl;
		cout << "[HuskyDB] Shutting down database." << endl;
		Sleep(5000);
		exit(EXIT_FAILURE);
	}
}

// Deconstruct NoSQL database
CHuskyDB::~CHuskyDB()
{
	// Delete all file objects from memory
	for (size_t x = 0; x < m_Files.size(); ++x)
	{
		delete m_Files[x];
	}

	// Delete all table objects from memory
	for (size_t x = 0; x < m_Tables.size(); ++x)
	{
		delete m_Tables[x];
	}

	// Delete all package objects from memory
	for (size_t x = 0; x < m_Packages.size(); ++x)
	{
		delete m_Packages[x];
	}

	// Clear pointers
	m_Files.clear();
	m_Tables.clear();
	m_Packages.clear();
}

// Index database in memory
void CHuskyDB::Index(bool toggle_pr)
{
	using namespace std::chrono;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	vector<string> package_names = GetFolders(m_DbPath);
	for (size_t x = 0; x < package_names.size(); ++x)
	{
		vector<CTable*> pkg_tables;
		string path = m_DbPath + "/" + package_names[x];
		CPackage* pkg = new CPackage(package_names[x], path, m_EmptyTables);
		vector<string> tables = GetFolders(path);

		for (size_t y = 0; y < tables.size(); ++y)
		{
			string table_path = path + "/" + tables[y];
			vector<string> files = GetFiles(table_path);

			CTable* tbl = new CTable(tables[y], table_path, pkg, m_EmptyFiles);
			vector<CFile*> table_files;

			vector<Prioritize_t> prioritizations;
			if (toggle_pr)
			{
				string priority_path = table_path + "/priority";
				WIN32_FIND_DATA FindFileData;
				HANDLE handle = FindFirstFile(priority_path.c_str(), &FindFileData);
				bool found = handle != INVALID_HANDLE_VALUE;
				if (found)
				{
					Prioritize(priority_path.c_str(), prioritizations);
				}
				else
				{
					FILE *pFile;
					fopen_s(&pFile, priority_path.c_str(), "w");
					fclose(pFile);
				}
			}
			
			for (size_t z = 0; z < files.size(); ++z)
			{
				string file_path = table_path + "/" + files[z];

				int priority_num = 0;
				if (toggle_pr)
				{
					// Get priority.
					for (int i = 0; i < prioritizations.size(); i++)
					{
						if (prioritizations[i].fileName == files[z])
						{
							priority_num = prioritizations[i].prioritize;
							break;
						}
					}
				}

				CFile* fl = new CFile(files[z], file_path, priority_num, tbl);

				table_files.push_back(fl);
				m_Files.push_back(fl);
			}

			tbl->SetFiles(table_files);
			pkg_tables.push_back(tbl);
			m_Tables.push_back(tbl);
		}

		pkg->SetTables(pkg_tables);
		m_Packages.push_back(pkg);
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();
	cout << "[HuskyDB] Database indexed in " << duration << "ms." << endl;
	cout << "[HuskyDB] # of packages: " << m_Packages.size() << endl;
	cout << "[HuskyDB] # of tables: " << m_Tables.size() << endl;
	cout << "[HuskyDB] # of files: " << m_Files.size() << endl;
}

CPackage* CHuskyDB::QueryPackage(string package_name)
{
	for (size_t x = 0; x < m_Packages.size(); ++x)
	{
		if (m_Packages[x]->GetName() == package_name)
		{
			return m_Packages[x];
		}
	}

	return NULL;
}

CTable* CHuskyDB::QueryTable(string table_name)
{
	for (size_t x = 0; x < m_Tables.size(); ++x)
	{
		if (m_Tables[x]->GetName() == table_name)
		{
			return m_Tables[x];
		}
	}

	return NULL;
}

CTable* CHuskyDB::QueryTable(string package_name, string table_name)
{
	CPackage* pkg = QueryPackage(package_name);
	CTable* table = pkg->FindTable(table_name);
	return table;
}

// Make package
bool CHuskyDB::MakePackage(string package_name)
{
	string package_path = m_DbPath;
	if (!FolderPossible(package_path))
	{
		return false;
	}
	else {
		MakeFolder(m_DbPath, package_name);
		CPackage* pkg = new CPackage(package_name, m_DbPath, m_EmptyTables);
		m_Packages.push_back(pkg);
		return true;
	} return false;
}

// Make table
bool CHuskyDB::MakeTable(string package_name, string table_name)
{
	string table_path = m_DbPath + "/" + package_name;
	if (!FolderPossible(table_path))
	{
		return false;
	}
	else {
		MakeFolder(table_path, table_name);
		CPackage* pkg = QueryPackage(package_name);
		CTable* tbl = new CTable(table_name, table_path, pkg, m_EmptyFiles);
		pkg->AddTable(tbl);
		m_Tables.push_back(tbl);
		return true;
	} return false;
}

// Make normal file from string
bool CHuskyDB::MakeFile(string package_name, string table_name, string file_name, string data, size_t priority)
{
	string file_path = m_DbPath + "/" + package_name + "/" + table_name + "/" + file_name;
	bool response = FilePossible(file_path);
	if (response)
	{
		ofstream new_file(file_path);
		new_file << data << endl;
		CPackage* pkg = QueryPackage(package_name);
		CTable* tbl = pkg->FindTable(table_name);
		CFile* fl = new CFile(file_name, file_path, priority, tbl);
		tbl->AddFile(fl);
		m_Files.push_back(fl);
		//index();
		return true;
	}
	else {
		return false;
	} return false;
}

// Append data to file from string (can be used for large files)
bool CHuskyDB::AppendFile(string package_name, string table_name, string file_name, vector<string> data)
{
	string file_path = m_DbPath + "/" + package_name + "/" + table_name + "/" + file_name;
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
bool CHuskyDB::DeletePackage(string package_name)
{
	string path = m_DbPath + "/" + package_name;
	if (FolderPossible(path))
	{
		CPackage* pkg = QueryPackage(package_name);
		vector<CTable*> tables = pkg->GetTables();
		if (tables.size() > 0)
		{
			for (size_t x = 0; x < tables.size(); ++x)
			{
				for (size_t y = 0; y < m_Tables.size(); ++y)
				{
					if (tables[x]->GetName() == m_Tables[y]->GetName())
					{
						m_Tables.erase(m_Tables.begin() + y);
					}
				}

			}
		}

		for (size_t x = 0; x < m_Packages.size(); ++x)
		{
			if (m_Packages[x]->GetName() == package_name)
			{
				m_Packages.erase(m_Packages.begin() + x);
			}
		}
		DeleteFolder(path);
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
bool CHuskyDB::DeleteTable(string package_name, string table_name)
{
	string path = m_DbPath + "/" + package_name + "/" + table_name;
	if (FolderPossible(path))
	{
		CPackage* pkg = QueryPackage(package_name);
		pkg->RemoveTable(table_name);

		for (size_t x = 0; x < m_Tables.size(); ++x)
		{
			if (m_Tables[x]->GetName() == table_name)
			{
				m_Tables.erase(m_Tables.begin() + x);
			}
		}
		DeleteFolder(path);
		CTable* tbl = QueryTable(table_name, package_name);
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
bool CHuskyDB::DeleteFile(string package_name, string table_name, string file_name)
{
	string full_path = m_DbPath + "/" + package_name + "/" + table_name + "/" + file_name;
	string path = m_DbPath + "/" + package_name + "/" + table_name;
	if (FilePossible(full_path))
	{
		CTable* tbl = QueryTable(package_name, table_name);
		CFile* file = tbl->FindFile(file_name);
		for (size_t x = 0; x < m_Files.size(); ++x)
		{
			if (m_Files[x]->GetName() == file_name)
			{
				m_Files.erase(m_Files.begin() + x);
			}
		}
		DeleteFile(path, file_name);
		tbl->RemoveFile(file_name);
		delete file;

		return true;
	}
	
	return false;
}

// PRIVATE INTERNAL FUNCTIONS


// Checks if a folder already exists
bool CHuskyDB::FolderPossible(string &directory)
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
bool CHuskyDB::FilePossible(string &directory)
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
void CHuskyDB::GetElements(vector<string> &files, string &search_path, string abstract)
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
vector<string> CHuskyDB::GetFolders(string &directory)
{
	vector<string> folders;
	string search_path = directory + "/*.*";
	GetElements(folders, search_path, "folder");
	return folders;
}

// Get files in directory with all file extensions
vector<string> CHuskyDB::GetFiles(string &directory)
{
	vector<string> files;
	string search_path = directory + "/*.*";
	GetElements(files, search_path, "file");
	return files;
}

// Get files in directory with specific file extension
vector<string> CHuskyDB::GetFiles(string &directory, string &extension)
{
	vector<string> files;
	string search_path = directory + "/*." + extension + "*";
	GetElements(files, search_path, "file");
	return files;
}

// Make folder in directory if it does not exist
bool CHuskyDB::MakeFolder(string &directory, string &folder_name)
{
	string path = directory + "/" + folder_name;
	bool response = FolderPossible(path);
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
bool CHuskyDB::DeleteFolder(string &directory)
{
	std::uintmax_t n = fs::remove_all(directory);
	//index();
	cout << "[HuskyDB] Success: " << directory << " deleted in database." << endl;
	return true;
}

// Delete file if it exists
bool CHuskyDB::DeleteFile(string &directory, string &file_name)
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
