/*Copyright(C)2018 by Daniel Parente, Peter Gramenides.

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