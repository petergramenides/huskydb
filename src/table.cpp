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
#include "table.h"

CTable::CTable(string _fname, string _fpath, CPackage* _package, vector<CFile*> _docs)
{
	m_FolderName = _fname;
	m_FolderPath = _fpath;
	m_pPkg = _package;
	m_Docs = _docs;
}

CTable::~CTable()
{
	for (CFile* obj : m_Docs)
		delete obj;
	m_Docs.clear();
}

string CTable::GetName() 
{ 
	return m_FolderName; 
}

void CTable::SetName(string _name)
{ 
	m_FolderName = _name; 
}

string CTable::GetPath()
{
	return m_FolderPath; 
}

void CTable::SetPath(string _path) 
{ 
	m_FolderPath = _path; 
}

CPackage* CTable::GetPackage()
{ 
	return m_pPkg; 
}

void CTable::SetPackage(CPackage* _package)
{ 
	m_pPkg = _package; 
}

vector<CFile*> CTable::GetFiles()
{ 
	return m_Docs; 
}

void CTable::SetFiles(vector<CFile*> _docs) 
{ 
	m_Docs = _docs; 
}

void CTable::AddFile(CFile* file)
{ 
	m_Docs.push_back(file);
}

vector<CFile*> CTable::FindFile(size_t priority)
{
	vector<CFile*> matches;
	for (size_t x = 0; x < m_Docs.size(); ++x)
	{
		if (m_Docs[x]->GetPriority() == priority)
		{
			matches.push_back(m_Docs[x]);
		} return matches;
	}
}

void CTable::RemoveFile(string file_name)
{
	for (size_t x = 0; x < m_Docs.size(); ++x)
	{
		if (m_Docs[x]->GetName() == file_name)
		{
			m_Docs.erase(m_Docs.begin() + x);
		}
	}
}

CFile* CTable::FindFile(string name_)
{
	for (size_t x = 0; x < m_Docs.size(); ++x)
	{
		if (m_Docs[x]->GetName() == name_)
		{
			return m_Docs[x];
		}
	} 
	
	return NULL;
}