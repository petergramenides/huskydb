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

#ifndef TABLE_H
#define TABLE_H

#include "file.h"
#include <vector>
#include <string>
#include "dllExports.h"

using namespace std;

class CPackage;

class HUSKYDB_API CTable
{
public:
	CTable(string _fname, string _fpath, CPackage* _package, vector<CFile*> _docs);
	~CTable();

	string					GetName();
	void					SetName(string _name);

	string					GetPath();
	void					SetPath(string _path);

	CPackage*				GetPackage();
	void					SetPackage(CPackage* _package);

	vector<CFile*>			GetFiles();
	void					SetFiles(vector<CFile*> _docs);

	void					AddFile(CFile* file);
	void					RemoveFile(string file_name);

	vector<CFile*>			FindFile(size_t priority);
	CFile*					FindFile(string name_);

private:
	string					m_FolderName;
	string					m_FolderPath;
	CPackage*				m_pPkg;
	vector<CFile*>			m_Docs;

};

#endif