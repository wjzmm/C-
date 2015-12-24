
// rascDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "rascNet.h"

// CrascDlg 对话框
#define WM_SHOWTASK WM_USER + 1
class CrascDlg : public CDialog
{
// 构造
public:
	CrascDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RASC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctlNet;
	CString m_etUname;
	CString m_etPasswd;
	CListBox m_listInfo;
	afx_msg void OnBnClickedButtonConnect();
	LRESULT OnRasDialEvent(WPARAM wp, LPARAM lp);
	bool sendPost();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	Rasdial m_ras;
public:
	CString m_etNetn;
	void toTray(CString tipInfo) ;
private:
	NOTIFYICONDATA nid; 
	//Reg m_reg;
public:
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuConnect();
	afx_msg void OnMenuDisconnect();
	afx_msg void OnMenuExit();
	BOOL m_ctlcheck;
	afx_msg void OnCbnSelchangeComboNetn();
	afx_msg void OnBnClickedButtonDisconnect();
};
