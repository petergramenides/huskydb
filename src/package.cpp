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