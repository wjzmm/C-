
// rascDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "rasc.h"
#include "rascDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
const UINT WM_RASEVENT = ::RegisterWindowMessageA(RASDIALEVENT);
CString HOST("10.6.8.2");
CString PATH("/include/auth_action.php");
CString REG_PATH("Software\\rasc\\");
CString UNAME("uname");
CString PASSWD("passwd");
const USHORT PORT = 16800;
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


// CrascDlg �Ի���




CrascDlg::CrascDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CrascDlg::IDD, pParent)
	, m_etUname(_T(""))
	, m_etPasswd(_T(""))
	, m_etNetn(_T(""))
	, m_ctlcheck(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrascDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NETN, m_ctlNet);
	DDX_Text(pDX, IDC_EDIT_UNAME, m_etUname);
	DDX_Text(pDX, IDC_EDIT_PASSWD, m_etPasswd);
	DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
	DDX_CBString(pDX, IDC_COMBO_NETN, m_etNetn);
	DDX_Check(pDX, IDC_CHECK_RE, m_ctlcheck);
}

BEGIN_MESSAGE_MAP(CrascDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_RASEVENT, OnRasDialEvent)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CrascDlg::OnBnClickedButtonConnect)
	ON_COMMAND(ID_MENU_ABOUT, &CrascDlg::OnMenuAbout)
	ON_COMMAND(ID_MENU_CONNECT, &CrascDlg::OnMenuConnect)
	ON_COMMAND(ID_MENU_DISCONNECT, &CrascDlg::OnMenuDisconnect)
	ON_COMMAND(ID_MENU_EXIT, &CrascDlg::OnMenuExit)
	ON_CBN_SELCHANGE(IDC_COMBO_NETN, &CrascDlg::OnCbnSelchangeComboNetn)
END_MESSAGE_MAP()


// CrascDlg ��Ϣ�������

BOOL CrascDlg::OnInitDialog()
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

	CComboBox *c = (CComboBox *)GetDlgItem(IDC_COMBO_NETN);
	for(DWORD i=0; i<dwEntries; lpRasEntryName++, i++){
		c->AddString(lpRasEntryName->szEntryName);
	}
	delete [] lpBuffer;
	c->SetFocus();
	((CButton*)GetDlgItem(IDC_CHECK_RE))->SetCheck(BST_CHECKED);
	//UpdateData(false);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CrascDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CrascDlg::OnPaint()
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
HCURSOR CrascDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CrascDlg::OnBnClickedButtonConnect()
{
	UpdateData(true);
m_ras = Rasdial(m_etUname, m_etPasswd, m_etNetn);
	DWORD result = m_ras.rasConnect(this->m_hWnd);
	if(result != 0)
		m_ras.rasDisConnect();

		
		/*RASDIALPARAMS RasPragma;
	RasPragma.dwSize = sizeof(RASDIALPARAMS);
	strcpy_s(RasPragma.szEntryName, m_etNetn);
	strcpy_s(RasPragma.szPhoneNumber, "");
	strcpy_s(RasPragma.szCallbackNumber, "");
	strcpy_s(RasPragma.szUserName, m_etUname);
	strcpy_s(RasPragma.szPassword, m_etPasswd);
	strcpy_s(RasPragma.szDomain, "");
	//MessageBox(path);

	HRASCONN hRasConn = NULL;
	DWORD result=RasDial(NULL, NULL, &RasPragma, 0xFFFFFFFF, this->m_hWnd, &hRasConn);
	if(result!= 0)
	{
		hRasConn = NULL;
	}*/
	if(m_ctlcheck){
		CString path;
		path = (REG_PATH + m_etNetn);
		Reg m_reg(path);
		m_reg.setReg(UNAME, m_etUname);
		m_reg.setReg(PASSWD, m_etPasswd);
	}
	UpdateData();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

LRESULT CrascDlg::OnRasDialEvent(WPARAM wp, LPARAM lp){
	RASCONNSTATE rasstate = (RASCONNSTATE)wp;
	CListBox *info = (CListBox *)GetDlgItem(IDC_LIST_INFO);
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
			if(sendPost()){
				info->AddString(_T("��ҳ��½�ɹ�"));
				Sleep(300);
				toTray( "У԰�������ӣ���������������������(/���v)");
				((CButton*)GetDlgItem(IDC_BUTTON_CONNECT))->EnableWindow(false);
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

bool CrascDlg::sendPost(){
	CString postData;
	postData += ("action=login&username=" + m_etUname + "&password=" + m_etPasswd + "&ac_id=1&user_ip=&nas_ip=&user_mac=&save_me=1&ajax=1");
	CInternetSession m_inernetSession(_T("Mozilla/5.0 (Windows NT 6.3; WOW64)"),
										0,
										INTERNET_OPEN_TYPE_PRECONFIG,
										NULL,
										NULL,
										INTERNET_FLAG_DONT_CACHE); 
	CString strHtml;
	DWORD statusCode;
	try{
		CHttpConnection * m_httpConnection = m_inernetSession.GetHttpConnection( (LPCTSTR)HOST, 0, PORT, NULL, NULL);
		CHttpFile* pFile = m_httpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,  PATH);
		CString strHeaders("Content-Type: application/x-www-form-urlencoded");
		BOOL result = pFile->SendRequest(strHeaders, (LPVOID)(LPCTSTR)postData, postData.GetLength());
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
			return true;
		}

	}
	catch (CInternetException* e){
		e->m_dwContext;
		return false;
	}
	return false;
}

void CrascDlg::toTray(CString tipInfo)  
{  
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);  
	nid.hWnd = this->m_hWnd;  
	nid.uID = IDR_MAINFRAME;  
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;  
	nid.uCallbackMessage = WM_SHOWTASK;//�Զ������Ϣ����  
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDIGNORE));  
	strcpy_s(nid.szTip,"��¹����");//��Ϣ��ʾ��Ϊ��У԰�������ӡ�  
	Shell_NotifyIcon(NIM_ADD, &nid);//�����������ͼ��  
	ShowWindow(SW_HIDE);//����������  
	nid.cbSize=sizeof(NOTIFYICONDATA);  
	nid.uFlags = NIF_INFO;  
	nid.uVersion = NOTIFYICON_VERSION;  
	nid.uTimeout = 2000;  
	nid.dwInfoFlags = NIF_INFO;  
	strcpy_s( nid.szInfoTitle, "��¹����" );//���ݱ���  
	strcpy_s( nid.szInfo, tipInfo );//��������  
	Shell_NotifyIcon( NIM_MODIFY, &nid );
}

LRESULT CrascDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)   
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
			toTray("�����Ѿ���С����������ʾ��������ͼ��");
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
void CrascDlg::OnMenuAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg about;
	about.DoModal();
}

void CrascDlg::OnMenuConnect()
{
	// TODO: �ڴ���������������
	m_ras = Rasdial(m_etUname, m_etPasswd, m_etNetn);
	DWORD result = m_ras.rasConnect(this->m_hWnd);
	if(result != 0)
		m_ras.rasDisConnect();
}

void CrascDlg::OnMenuDisconnect()
{
	m_ras.rasDisConnect();
	// TODO: �ڴ���������������
}

void CrascDlg::OnMenuExit()
{
	::Shell_NotifyIcon(NIM_DELETE, &nid);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	// TODO: �ڴ���������������
}

void CrascDlg::OnCbnSelchangeComboNetn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CString mreg;
	CComboBox *c = (CComboBox* )GetDlgItem(IDC_COMBO_NETN);
	c->GetLBText(c->GetCurSel(), mreg);
	//MessageBox(mreg);
	CString path;
	path = (REG_PATH + mreg);
	Reg m_reg(path);
	if(m_reg.getReg(UNAME, m_etUname) &&  m_reg.getReg(PASSWD, m_etPasswd)){
		//MessageBox(m_etUname);
	}else{
		m_etUname = "";
		m_etPasswd = "";
	}
	UpdateData(false);
}

