
// rasaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rasa.h"
#include "rasaDlg.h"
//#include "rasdial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const UINT WM_RASEVENT = ::RegisterWindowMessageA(RASDIALEVENT);
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
HRASCONN hRasConn = NULL;
const UINT PORT = 16800;
CString HOST("10.6.8.2");
//const char* HOST = "10.6.8.2";
const char* PATH = "/include/auth_action.php";
CString uname, passwd;
CString queryuname = _T("uname");
CString querypwd = _T("passwd");
CString basepath = _T("Software\\rasa\\");
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CrasaDlg �Ի���




CrasaDlg::CrasaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CrasaDlg::IDD, pParent)
	, m_uname(_T(""))
	, m_passwd(_T(""))
	, m_regpath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CrasaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UNAME, m_uname);
	DDX_Text(pDX, IDC_PASSWD, m_passwd);
	DDX_CBString(pDX, IDC_CONNECTION, m_regpath);
	DDX_Control(pDX, IDC_CHECK_RE, m_ct_re);
}

BEGIN_MESSAGE_MAP(CrasaDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SHOWTASK, OnNotifyMsg)  
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CONNECT, &CrasaDlg::OnBnClickedConnect)
	ON_REGISTERED_MESSAGE(WM_RASEVENT, OnRasDialEvent)
	ON_BN_CLICKED(IDC_DISCONNECT, &CrasaDlg::OnBnClickedDisconnect)

	ON_WM_DESTROY()
	ON_COMMAND(ID_MENU_EXIT, &CrasaDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_ABOUT, &CrasaDlg::OnMenuAbout)
	ON_CBN_SELCHANGE(IDC_CONNECTION, &CrasaDlg::OnCbnSelchangeConnection)
END_MESSAGE_MAP()


// CrasaDlg ��Ϣ�������

BOOL CrasaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	DWORD dwRet, dwSize, dwEntries;
	LPBYTE lpBuffer;

	LPRASENTRYNAME lpRasEntryName = new RASENTRYNAME;
	lpRasEntryName->dwSize = sizeof(RASENTRYNAME);
	dwSize = sizeof(RASENTRYNAME);
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwSize, &dwEntries);
	delete lpRasEntryName;

	lpBuffer = new BYTE[dwSize];
	lpRasEntryName = (LPRASENTRYNAME)lpBuffer;
	lpRasEntryName->dwSize = sizeof(RASENTRYNAME);
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwSize, &dwEntries);

	CComboBox *c = (CComboBox *)GetDlgItem(IDC_CONNECTION);
	for(DWORD i=0; i<dwEntries; lpRasEntryName++, i++){
		c->AddString(lpRasEntryName->szEntryName);
	}
	delete [] lpBuffer;
	((CButton*)GetDlgItem(IDC_CHECK_RE))->SetCheck(BST_CHECKED);
	//UpdateData(false);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
	
}

void CrasaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CrasaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CrasaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CrasaDlg::OnBnClickedConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CListBox *info = (CListBox *)GetDlgItem(IDC_LISTINFO);
	uname = m_uname;
	passwd = m_passwd;
	TCHAR lpszEntry[256];
	GetDlgItemText(IDC_CONNECTION, lpszEntry, 255);
	RASDIALPARAMS RasDialParams;
	RasDialParams.dwSize = sizeof(RASDIALPARAMS);
	_tcscpy_s(RasDialParams.szEntryName, lpszEntry);
	_tcscpy_s(RasDialParams.szPhoneNumber, _T(""));
	_tcscpy_s(RasDialParams.szCallbackNumber, _T(""));
	GetDlgItemText(IDC_UNAME, RasDialParams.szUserName, UNLEN);
	GetDlgItemText(IDC_PASSWD, RasDialParams.szPassword, PWLEN);
	_tcscpy_s(RasDialParams.szDomain, _T(""));
	
	if(m_ct_re.GetCheck())
		setReg();
	DWORD result;
	result=RasDial(NULL, NULL, &RasDialParams, 0xFFFFFFFF, this->m_hWnd, &hRasConn);
	if(result!= 0)
	{
		hRasConn = NULL;
	}
	UpdateData();
}
LRESULT CrasaDlg::OnRasDialEvent(WPARAM wp, LPARAM lp){
	RASCONNSTATE rasstate = (RASCONNSTATE)wp;
	CListBox *info = (CListBox *)GetDlgItem(IDC_LISTINFO);
	switch(rasstate){
		case RASCS_OpenPort:
			info->AddString(_T(" �򿪶˿ڡ���"));
			break;
		case RASCS_PortOpened:
			info->AddString(_T(" �˿��Ѵ�."));
			break;
		case RASCS_ConnectDevice:
			info->AddString(_T("�����豸..."));
			break;
		case RASCS_DeviceConnected:
			info->AddString(_T("�豸������."));
			break;
		case RASCS_Authenticate:
			info->AddString(_T("��֤�û�������"));
			break;
		case RASCS_Authenticated:
			info->AddString(_T("ͨ��"));
			break;
		case RASCS_Connected:
			info->AddString(_T("������"));
			//Sleep(500);
			if(sendByPost()){
				info->AddString(_T("��ҳ��½�ɹ�"));
				Sleep(300);
				toTray();
				((CButton*)GetDlgItem(IDC_CONNECT))->EnableWindow(false);
			}
			break;
		case RASCS_Disconnected:
			info->AddString(_T("�����ѶϿ�"));
			break;
		default:
			return (LRESULT)0;
	}
	return (LRESULT)0;
}

void CrasaDlg::OnBnClickedDisconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*if(hRasConn != NULL){
		RasHangUp(hRasConn);
		hRasConn = NULL;
	}*/
	//getByReg();
}

void CrasaDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(hRasConn != NULL){
		RasHangUp(hRasConn);
		hRasConn = NULL;
	}
	
	// TODO: �ڴ˴������Ϣ����������
}

bool CrasaDlg::sendByPost(){
	CString dt1("action=login&username=");
	CString dt2("&password=");
	CString dt3("&ac_id=1&user_ip=&nas_ip=&user_mac=&save_me=1&ajax=1");
	CString postData;
	
	

	//...
	
	DWORD statusCode;
	postData += (dt1 + uname + dt2 + passwd + dt3);
	//MessageBox(postData);
	int len =WideCharToMultiByte(CP_ACP,0,postData,-1,NULL,0,NULL,NULL);
	char *data =new char[len +1];
	WideCharToMultiByte(CP_ACP,0,postData,-1,data,len,NULL,NULL );
	//char *data = (LPSTR)(LPCTSTR)postData;
	//DWORD len = postData.GetLength();
	//strcpy_s(data, postData);
	//char *data = "action=login&username=dumpling&password=896953&ac_id=1&user_ip=&nas_ip=&user_mac=&save_me=1&ajax=1";
	CInternetSession m_inernetSession(_T("Mozilla/5.0 (Windows NT 6.3; WOW64)"),
		0,
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		INTERNET_FLAG_DONT_CACHE); 
	CString strHtml("hello");
	try{
		CHttpConnection * m_httpConnection = m_inernetSession.GetHttpConnection( (LPCTSTR)HOST, 0, PORT, NULL, NULL);
		CHttpFile* pFile = m_httpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,  _T("/include/auth_action.php"));
		CString strHeaders("Content-Type: application/x-www-form-urlencoded");
		BOOL result = pFile->SendRequest(strHeaders, (LPVOID)data, len);
		if( pFile->QueryInfoStatusCode(statusCode) && statusCode == HTTP_STATUS_OK){
			CString strLine;
			while (pFile->ReadString(strLine))
			{
				strHtml += strLine;
			}
			//MessageBox(strHtml);
			pFile->Close();
			delete pFile;
			m_httpConnection->Close();
			delete m_httpConnection;
			delete[] data;
			return true;
		}
		
	}
	catch (CInternetException* e){
		e->m_dwContext;
		return false;
	}
	delete[] data;
	return false;
}

void CrasaDlg::toTray()  
{  
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);  
	nid.hWnd = this->m_hWnd;  
	nid.uID = IDR_MAINFRAME;  
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;  
	nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ����  
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));  
	_tcscpy_s(nid.szTip,_T("rasa"));//��Ϣ��ʾ��Ϊ��У԰�������ӡ�  
	Shell_NotifyIcon(NIM_ADD, &nid);//�����������ͼ��  
	ShowWindow(SW_HIDE);//����������  
	nid.cbSize=sizeof(NOTIFYICONDATA);  
	nid.uFlags = NIF_INFO;  
	nid.uVersion = NOTIFYICON_VERSION;  
	nid.uTimeout = 2000;  
	nid.dwInfoFlags = NIF_INFO;  
	_tcscpy_s( nid.szInfoTitle, _T("rasa" ));//���ݱ���  
	_tcscpy_s( nid.szInfo,_T("У԰�������ӣ���������������������(/���v)" ));//��������  
	Shell_NotifyIcon( NIM_MODIFY, &nid );
}

LRESULT  CrasaDlg::OnNotifyMsg(WPARAM wparam,LPARAM lparam)  
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ     
{  
	if(wparam!=IDR_MAINFRAME)     
		return    1;     
	switch(lparam)     
	{     
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�     
		{       
			LPPOINT lpoint=new tagPOINT;     
			::GetCursorPos(lpoint);//�õ����λ��     
			CMenu    menu;     
			menu.CreatePopupMenu();//����һ������ʽ�˵�     
			//���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ���     
			//���أ��������������     
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�ر�"));
			menu.AppendMenu(MF_STRING, WM_ABOUT, _T("����"));
			//ȷ������ʽ�˵���λ��     
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);     
			//��Դ����     
			HMENU hmenu=menu.Detach();     
			menu.DestroyMenu();     
			delete lpoint;     
		}     
		break;     
	case WM_LBUTTONDBLCLK://˫������Ĵ���     
		{     
			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�     
		}     
		break;  
	}      
	return 0;  
}  

LRESULT CrasaDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)   
{  
	// TODO: Add your specialized code here and/or call the base class  
	switch(message) //�ж���Ϣ����  
	{   
	case WM_SHOWTASK:   
		//������û��������Ϣ   
		if(lParam==WM_LBUTTONDBLCLK)  

		{   
			//���˫��ʱ�����ڳ���   
			if(AfxGetApp()->m_pMainWnd->IsWindowVisible()) //�жϴ��ڵ�ǰ״̬  
			{  
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); //���ش���  
			}  
			else  
			{  
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); //��ʾ����  
			}  

		}   
		else if(lParam==WM_RBUTTONDOWN)  
		{ //����Ҽ���������ѡ��   
			CMenu menu;   
			menu.LoadMenu(IDR_MENU1); //�������ȶ����ѡ��   
			CMenu *pMenu=menu.GetSubMenu(0);   
			CPoint pos;   
			GetCursorPos(&pos);   
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd());   
		}   
		break;   
	case WM_SYSCOMMAND:   
		//�����ϵͳ��Ϣ   
		if(wParam==SC_MINIMIZE)  
		{   
			//���յ���С����Ϣʱ����������   
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);   
			return 0;   
		}   
		if(wParam==SC_CLOSE)  
		{  
			::Shell_NotifyIcon(NIM_DELETE, &nid); //�ر�ʱɾ��ϵͳ����ͼ��  
		}  
		break;  
	}  
	return CDialog::WindowProc(message, wParam, lParam);  
}  
void CrasaDlg::OnMenuExit()
{
	::Shell_NotifyIcon(NIM_DELETE, &nid);
	OnDestroy();
	// TODO: �ڴ���������������
}

void CrasaDlg::OnMenuAbout()
{
	CAboutDlg about;
	about.DoModal();
	// TODO: �ڴ���������������
}

bool CrasaDlg::setReg()
{
	UpdateData(true);
	if(m_regkey.Create(HKEY_LOCAL_MACHINE, _T("Software\\rasa\\xyz")) != ERROR_SUCCESS)
		MessageBox(_T("error"));
	m_regkey.SetStringValue(queryuname, m_uname);
	m_regkey.SetStringValue(querypwd, m_passwd);
	return true;
}

bool CrasaDlg::getByReg(CString str)
{
	TCHAR tuname[_MAX_PATH]; 
	TCHAR tpasswd[_MAX_PATH];
	DWORD len = _MAX_PATH;
	if(m_regkey.Open(HKEY_LOCAL_MACHINE, basepath + str) == ERROR_SUCCESS){
		m_regkey.QueryStringValue(queryuname, tuname, &len);
		m_regkey.QueryStringValue(querypwd, tpasswd, &len);
		//m_regkey.QueryStringValue(querypwd, s, 30);
		uname = tuname;
		passwd = tpasswd;
		return true;
	}else{
		return false;
	}
	
}
void CrasaDlg::OnCbnSelchangeConnection()
{
	UpdateData(true);
	CString mregp;
	CComboBox *c = (CComboBox* )GetDlgItem(IDC_CONNECTION);
	c->GetLBText(c->GetCurSel(), mregp);
	if(getByReg(mregp))
	{
		m_uname = uname;
		m_passwd = passwd;
	}
	else
	{
		m_uname = "";
		m_passwd = "";	
	}
	UpdateData(false);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
