/*Copyright(C)2018 by Daniel Parente, Peter C. Gramenides.

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
#include "package.h"

CPackage::CPackage(string _name, string _path, vector<CTable*> _tables)
{
	m_PackageName = _name;
	m_PackagePath = _path;
	m_Tables = _tables;
}

CPackage::~CPackage()
{
	for (size_t x = 0; x < m_Tables.size(); ++x)
	{
		delete m_Tables[x];
	} m_Tables.clear();
}

string CPackage::GetPath() 
{ 
	return m_PackagePath; 
}

void CPackage::SetPath(string _path)
{ 
	m_PackagePath = _path; 
}

vector<CTable*> CPackage::GetTables() 
{ 
	return m_Tables; 
}

void CPackage::SetTables(vector<CTable*> _tables)
{ 
	m_Tables = _tables; 
}

CTable* CPackage::FindTable(string _name)
{
	for (size_t x = 0; x < m_Tables.size(); ++x)
	{
		if (m_Tables[x]->GetName() == _name)
		{
			return m_Tables[x];
		}
	}

	return NULL;
}

string CPackage::GetName() 
{ 
	return m_PackageName; 
}

void CPackage::SetName(string _name) 
{ 
	m_PackageName = _name; 
}

void CPackage::AddTable(CTable* _pTable)
{ 
	m_Tables.push_back(_pTable); 
}

void CPackage::RemoveTable(string table_name)
{
	for (size_t x = 0; x < m_Tables.size(); ++x)
	{
		if (m_Tables[x]->GetName() == table_name)
		{
			m_Tables.erase(m_Tables.begin() + x);
		}
	}
}