#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
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