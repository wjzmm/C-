
// rasaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "rasa.h"
#include "rasaDlg.h"
//#include "rasdial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const UINT WM_RASEVENT = ::RegisterWindowMessageA(RASDIALEVENT);
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
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

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CrasaDlg 对话框




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


// CrasaDlg 消息处理程序

BOOL CrasaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
	
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CrasaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CrasaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CrasaDlg::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
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
			info->AddString(_T(" 打开端口……"));
			break;
		case RASCS_PortOpened:
			info->AddString(_T(" 端口已打开."));
			break;
		case RASCS_ConnectDevice:
			info->AddString(_T("连接设备..."));
			break;
		case RASCS_DeviceConnected:
			info->AddString(_T("设备已连接."));
			break;
		case RASCS_Authenticate:
			info->AddString(_T("验证用户及密码"));
			break;
		case RASCS_Authenticated:
			info->AddString(_T("通过"));
			break;
		case RASCS_Connected:
			info->AddString(_T("已连接"));
			//Sleep(500);
			if(sendByPost()){
				info->AddString(_T("网页登陆成功"));
				Sleep(300);
				toTray();
				((CButton*)GetDlgItem(IDC_CONNECT))->EnableWindow(false);
			}
			break;
		case RASCS_Disconnected:
			info->AddString(_T("连接已断开"));
			break;
		default:
			return (LRESULT)0;
	}
	return (LRESULT)0;
}

void CrasaDlg::OnBnClickedDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
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
	
	// TODO: 在此处添加消息处理程序代码
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
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称  
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));  
	_tcscpy_s(nid.szTip,_T("rasa"));//信息提示条为“校园网已连接”  
	Shell_NotifyIcon(NIM_ADD, &nid);//在托盘区添加图标  
	ShowWindow(SW_HIDE);//隐藏主窗口  
	nid.cbSize=sizeof(NOTIFYICONDATA);  
	nid.uFlags = NIF_INFO;  
	nid.uVersion = NOTIFYICON_VERSION;  
	nid.uTimeout = 2000;  
	nid.dwInfoFlags = NIF_INFO;  
	_tcscpy_s( nid.szInfoTitle, _T("rasa" ));//气泡标题  
	_tcscpy_s( nid.szInfo,_T("校园网已连接，现在您可以正常上网了(/▽╲)" ));//气泡内容  
	Shell_NotifyIcon( NIM_MODIFY, &nid );
}

LRESULT  CrasaDlg::OnNotifyMsg(WPARAM wparam,LPARAM lparam)  
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为     
{  
	if(wparam!=IDR_MAINFRAME)     
		return    1;     
	switch(lparam)     
	{     
	case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”     
		{       
			LPPOINT lpoint=new tagPOINT;     
			::GetCursorPos(lpoint);//得到鼠标位置     
			CMenu    menu;     
			menu.CreatePopupMenu();//声明一个弹出式菜单     
			//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已     
			//隐藏），将程序结束。     
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("关闭"));
			menu.AppendMenu(MF_STRING, WM_ABOUT, _T("关于"));
			//确定弹出式菜单的位置     
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);     
			//资源回收     
			HMENU hmenu=menu.Detach();     
			menu.DestroyMenu();     
			delete lpoint;     
		}     
		break;     
	case WM_LBUTTONDBLCLK://双击左键的处理     
		{     
			this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿     
		}     
		break;  
	}      
	return 0;  
}  

LRESULT CrasaDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)   
{  
	// TODO: Add your specialized code here and/or call the base class  
	switch(message) //判断消息类型  
	{   
	case WM_SHOWTASK:   
		//如果是用户定义的消息   
		if(lParam==WM_LBUTTONDBLCLK)  

		{   
			//鼠标双击时主窗口出现   
			if(AfxGetApp()->m_pMainWnd->IsWindowVisible()) //判断窗口当前状态  
			{  
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); //隐藏窗口  
			}  
			else  
			{  
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); //显示窗口  
			}  

		}   
		else if(lParam==WM_RBUTTONDOWN)  
		{ //鼠标右键单击弹出选单   
			CMenu menu;   
			menu.LoadMenu(IDR_MENU1); //载入事先定义的选单   
			CMenu *pMenu=menu.GetSubMenu(0);   
			CPoint pos;   
			GetCursorPos(&pos);   
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd());   
		}   
		break;   
	case WM_SYSCOMMAND:   
		//如果是系统消息   
		if(wParam==SC_MINIMIZE)  
		{   
			//接收到最小化消息时主窗口隐藏   
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);   
			return 0;   
		}   
		if(wParam==SC_CLOSE)  
		{  
			::Shell_NotifyIcon(NIM_DELETE, &nid); //关闭时删除系统托盘图标  
		}  
		break;  
	}  
	return CDialog::WindowProc(message, wParam, lParam);  
}  
void CrasaDlg::OnMenuExit()
{
	::Shell_NotifyIcon(NIM_DELETE, &nid);
	OnDestroy();
	// TODO: 在此添加命令处理程序代码
}

void CrasaDlg::OnMenuAbout()
{
	CAboutDlg about;
	about.DoModal();
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
}
