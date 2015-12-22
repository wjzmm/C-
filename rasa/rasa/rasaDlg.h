
// rasaDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define WM_SHOWTASK WM_USER + 1
#define WM_ABOUT WM_USER + 2
// CrasaDlg 对话框
class CrasaDlg : public CDialog
{
// 构造
public:
	CrasaDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RASA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	  NOTIFYICONDATA nid; 
	  CRegKey m_regkey;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam,LPARAM lparam); 
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	LRESULT OnRasDialEvent(WPARAM wp, LPARAM lp);
	// 用户名
	CString m_uname;
	// 密码
	CString m_passwd;
	// 连接名称
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
