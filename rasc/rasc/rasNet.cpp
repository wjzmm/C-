
#include "stdafx.h"
#include "rascNet.h"

Rasdial::Rasdial(){}
Rasdial::Rasdial(CString uname, CString passwd, CString netname): uname(uname), passwd(passwd), netname(netname){
	m_hRasCon = NULL;
	RasPragma.dwSize = sizeof(RASDIALPARAMS);
	strcpy_s(RasPragma.szEntryName, netname);
	strcpy_s(RasPragma.szPhoneNumber, "");
	strcpy_s(RasPragma.szCallbackNumber, "");
	strcpy_s(RasPragma.szUserName, uname);
	strcpy_s(RasPragma.szPassword, passwd);
	strcpy_s(RasPragma.szDomain, "");

}
DWORD Rasdial::rasConnect(LPVOID lp){
	//RASDIALPARAMS RasPragma;
	return RasDial(NULL, NULL, &RasPragma, 0xFFFFFFFF, lp, &m_hRasCon);	
}
void Rasdial::rasDisConnect(){
	if(m_hRasCon != NULL){
		RasHangUp(m_hRasCon);
		m_hRasCon = NULL;
		::Sleep(1000);
	}
}
Rasdial& Rasdial::operator=(const Rasdial &rhs){
	m_hRasCon = rhs.m_hRasCon;
	RasPragma = rhs.RasPragma;
	uname = rhs.uname;
	passwd = rhs.passwd;
	netname = rhs.netname;
	return *this;
}
Rasdial::~Rasdial(){
	rasDisConnect();
}

Reg::Reg(CString path): regpath(path){
}
Reg& Reg::operator=(const Reg &rhs){
	regpath = rhs.regpath;
	return *this;
}
bool Reg::getReg(CString queryValue, CString &re){
	TCHAR result[_MAX_PATH];
	DWORD len = _MAX_PATH;
	if(m_regkey.Open(HKEY_LOCAL_MACHINE, regpath) == ERROR_SUCCESS){
		m_regkey.QueryStringValue(queryValue, result, &len);
		re.Format("%s", result);  
		//AfxMessageBox(re);
		return true;
	}
	return false;
}
bool Reg::setReg(CString key, CString value){
	if(m_regkey.Create(HKEY_LOCAL_MACHINE, regpath) == ERROR_SUCCESS){
		m_regkey.SetStringValue(key, value);
		return true;
	}
	else
	{
		return false;
	}
}

Reg::~Reg(){
	m_regkey.Close();
}