
#include "stdafx.h"
#include "rasdial.h"

Rasdial::Rasdial(RASDIALPARAMS RasPragmas){
	//memcpy(RasPragma, RasPragmas, sizeof(RasPragmas));
	m_hRasCon = NULL;
}
DWORD Rasdial::rasConnect(LPVOID lp){
	//RASDIALPARAMS RasPragma;
	return RasDial(NULL, NULL, &RasPragma, 0xFFFFFFFF, lp, &m_hRasCon);
	
}

Rasdial::~Rasdial(){
	if(m_hRasCon != NULL){
		RasHangUp(m_hRasCon);
		m_hRasCon = NULL;
		::Sleep(2000);
	}
}