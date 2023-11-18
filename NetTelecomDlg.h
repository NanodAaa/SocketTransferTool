// NetTelecomDlg.h : 头文件
// Powered By NanodAaa
// 2023/11/18

#pragma once
#include "Global.h"
#include "afxcmn.h"

// CNetTelecomDlg 对话框
class CNetTelecomDlg : public CDialogEx
{
// 构造
public:
	CNetTelecomDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETTELECOM_DIALOG };
#endif

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
	
	CEdit m_editSend;		// SendBox
	CEdit m_editReceive;	// ReceiveBox
	CEdit m_editRCVDPort;
	CEdit m_editTGTPort;
	CEdit m_editTGTAddr;
	CEdit m_editMessage;
	
	CComboBox m_comboOrder;

	CTreeCtrl m_treeADDR;
	HTREEITEM m_treeNodeTemp;

	bool m_bMsgSendFlag;

	int ServerHandler(UINT nScoketPort);
	int ClientHandler(CString strTGTAddr, UINT nSocketPort);
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonStopAll();
};
