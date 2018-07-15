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

#ifndef PACKAGE_H
#define PACKAGE_H

#include "table.h"
#include <string>
#include <vector>
#include "dllExports.h"

using namespace std;

class HUSKYDB_API CPackage
{
public:
	CPackage(string _name, string _path, vector<CTable*> _tables);
	~CPackage();

	string					GetPath();
	void					SetPath(string _path);

	vector<CTable*>			GetTables();
	void					SetTables(vector<CTable*> _tables);

	CTable*					FindTable(string _name);

	string					GetName();
	void					SetName(string _name);

	void					AddTable(CTable* _pTable);
	void					RemoveTable(string table_name);

private:
	string					m_PackagePath;
	string					m_PackageName;
	vector<CTable*>			m_Tables;
};

#endif
