#pragma once
#include "afxwin.h"


class Rasdial{
private:
	CString uname;
	CString passwd;
	CString netname;
	HRASCONN m_hRasCon;
	RASDIALPARAMS RasPragma;

public:

	Rasdial(CString uname, CString passwd, CString netname);
	Rasdial();
	void rasDisConnect();
	DWORD rasConnect(LPVOID lp);
	Rasdial& operator=(const Rasdial &rhs);
	~Rasdial();
};

class Reg{
private:
	CRegKey m_regkey;
	CString regpath;

public:
	Reg(CString path);
	Reg& operator=(const Reg &rhs);
	bool setReg(CString key, CString value);
	bool getReg(CString path, CString &re);
	~Reg();
};