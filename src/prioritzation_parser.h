/*Copyright(C)2018 by Daniel Parente

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

#ifndef PRIORITIZATION_PARSER_H
#define PRIORITIZATION_PARSER_H

#include <string>
#include <vector>

struct Prioritize_t
{
	std::string fileName;
	int prioritize;
};

void Prioritize(const char *pPath, std::vector<Prioritize_t>& prioritize)
{
	FILE *pFile;
	fopen_s(&pFile, pPath, "rb");

	size_t nFileSize;
	fseek(pFile, 0, SEEK_END);
	nFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	char *pBuffer = (char *)malloc(nFileSize);
	fread(pBuffer, 1, nFileSize, pFile);
	fclose(pFile);

	// ---------------------------------------- Parse ----------------------------------------
	char **ppLines = NULL;
	int *pPrioritizationNumbers = NULL;
	unsigned int nLines = 0;

	// Get number of lines.
	for (int i = 0; i < nFileSize; i++)
	{
		if (pBuffer[i] == ';')
		{
			nLines++;
		}
	}

	ppLines = (char **)malloc(sizeof(char *) * nLines);
	pPrioritizationNumbers = (int *)malloc(sizeof(int) * nLines);

	// Break lines.
	unsigned int curLine = 0;
	for (int i = 0; i < nFileSize; i++)
	{
		if (curLine == 0)
		{
			ppLines[0] = pBuffer;
			curLine++;
		}
		else
		{
			if (pBuffer[i] == ';')
			{
				for (int x = 1;; x++)
				{
					if (pBuffer[i + x] != 0xd && pBuffer[i + x] != 0xa)
					{
						if (curLine == nLines)
						{
							break;
						}

						ppLines[curLine] = pBuffer + i + x;
						curLine++;
						break;
					}
				}
			}
		}
	}

	// Null semicolons.
	for (int i = 0; i < nFileSize; i++)
	{
		if (pBuffer[i] == ';')
		{
			pBuffer[i] = 0;
		}
	}

	// Extract prioritization number.
	for (int i = 0; i < nLines; i++)
	{
		int num;

		for (int x = 0; x < strlen(ppLines[i]); x++)
		{
			if (ppLines[i][x] == '=')
			{
				num = atoi(ppLines[i] + x + 1);
				ppLines[i][x] = 0; // Null equal sign.
			}
		}

		Prioritize_t p;
		p.fileName = ppLines[i];
		p.prioritize = num;
		prioritize.push_back(p);
	}

	//Clean up.
	free(pPrioritizationNumbers);
	free(ppLines);
	free(pBuffer);
}

#endif