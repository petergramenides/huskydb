#include "file.h"

CFile::CFile(string _fname, string _path, size_t _priority, CTable* _pParent)
{
	m_FileName = _fname;
	m_FilePath = _path;
	m_Priority = _priority;
	m_pParent = _pParent;
}

CFile::~CFile()
{

}

void CFile::SetPriority(size_t _priority)
{
	m_Priority = _priority;
}

size_t CFile::GetPriority()
{ 
	return m_Priority;
}

string CFile::GetPath()
{ 
	return m_FilePath;
}

void CFile::SetPath(string _path)
{ 
	m_FilePath = _path;
}

CTable* CFile::GetParent()
{ 
	return m_pParent;
}

void CFile::SetParent(CTable* _pParent)
{ 
	m_pParent = _pParent;
}

string CFile::GetName()
{ 
	return m_FileName;
}

void CFile::SetName(string _name)
{ 
	m_FileName = _name;
}

bool CFile::IsChild(CTable* _pParent)
{ 
	return m_pParent == _pParent;
}