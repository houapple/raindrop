#ifndef _RD_FILE_H
#define _RD_FILE_H

#include "../rd_core/rd_types.h"
#include <stdio.h>

class CFile
{
public:
	enum eMode
	{
		eRead	= 0x1,
		eWrite	= 0x2,
		eAppend	= 0x4,
	};

	CFile(const char* filename, BYTE mode);

	void Open();

private:
	FILE* m_pFile;
};

#endif