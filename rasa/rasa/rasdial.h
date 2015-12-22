#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include <Ras.h>
#include <atlconv.h>
#pragma comment(lib,"RASAPI32.LIB")
class Rasdial{
private:
	CString uname;
	CString passwd;
	CString netname;
	HRASCONN m_hRasCon;
	RASDIALPARAMS RasPragma;
	
public:
	
	Rasdial(RASDIALPARAMS RasPragmas);
	DWORD rasConnect(LPVOID lp);
	~Rasdial();
};