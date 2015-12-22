
// rasaDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define WM_SHOWTASK WM_USER + 1
#define WM_ABOUT WM_USER + 2
// CrasaDlg �Ի���
class CrasaDlg : public CDialog
{
// ����
public:
	CrasaDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RASA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	  NOTIFYICONDATA nid; 
	  CRegKey m_regkey;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam,LPARAM lparam); 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	LRESULT OnRasDialEvent(WPARAM wp, LPARAM lp);
	// �û���
	CString m_uname;
	// ����
	CString m_passwd;
	// ��������
	CString m_netname;
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnDestroy();
	bool sendByPost();
	bool getByReg(CString str);
	bool setReg();
	void toTray();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuExit();
	afx_msg void OnMenuAbout();

	afx_msg void OnCbnSelchangeConnection();
	CString m_regpath;
	CButton m_ct_re;
};
