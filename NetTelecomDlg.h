// NetTelecomDlg.h : ͷ�ļ�
// Powered By NanodAaa
// 2023/11/18

#pragma once
#include "Global.h"
#include "afxcmn.h"

// CNetTelecomDlg �Ի���
class CNetTelecomDlg : public CDialogEx
{
// ����
public:
	CNetTelecomDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NETTELECOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
