// NetTelecomDlg.cpp
// Telecom base on SOCKET
// Powered By NanodAaa
// 2023/11/18

#include "NetTelecomDlg.h"
#include "stdafx.h"
#include "NetTelecom.h"
#include "NetTelecomDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool g_bAcceptThreadFlag;
bool g_bReadThreadFlag;	// All read thread enable flag
int g_nIndex;
CAsyncSocket g_socketServer;
CAsyncSocket g_socketClient;
CTime g_Time;
CList<SReadThread, SReadThread&> g_ThreadList;
HTREEITEM TCP_CLIENT;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetTelecomDlg 对话框
CNetTelecomDlg::CNetTelecomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NETTELECOM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetTelecomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SEND, m_editSend);
	DDX_Control(pDX, IDC_EDIT_RECEIVE, m_editReceive);
	DDX_Control(pDX, IDC_EDIT_RCVD_PORT, m_editRCVDPort);
	DDX_Control(pDX, IDC_EDIT_TGT_PORT, m_editTGTPort);
	DDX_Control(pDX, IDC_EDIT_TGT_ADDR, m_editTGTAddr);
	DDX_Control(pDX, IDC_COMBO_ORDER, m_comboOrder);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editMessage);
	DDX_Control(pDX, IDC_TREE_ADDR, m_treeADDR);
}

BEGIN_MESSAGE_MAP(CNetTelecomDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CNetTelecomDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_START, &CNetTelecomDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CNetTelecomDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_STOP_ALL, &CNetTelecomDlg::OnBnClickedButtonStopAll)
END_MESSAGE_MAP()


// CNetTelecomDlg 消息处理程序

BOOL CNetTelecomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_comboOrder.AddString(TEXT("0"));
	m_comboOrder.AddString(TEXT("1"));
	m_bMsgSendFlag = FALSE;
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	// Add root of tree
	TCP_CLIENT = m_treeADDR.InsertItem(TEXT("TCP_CLIENT"));
//	TCP_CLIENT1 = m_treeADDR.InsertItem(TEXT("123"), TCP_CLIENT);

#ifdef _TEST_
	m_editRCVDPort.SetWindowText(TEXT("1"));
	m_editTGTPort.SetWindowText(TEXT("2"));
	m_editTGTAddr.SetWindowText(TEXT("127.0.0.1"));
	UpdateData();
#endif

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNetTelecomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNetTelecomDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNetTelecomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// Server handler
// nScoketPort -- RCVD Port
int CNetTelecomDlg::ServerHandler(UINT nScoketPort)
{
	CString strTemp;

	if (!AfxSocketInit())
	{
		MessageBox(TEXT("AfxSocketInit Failed!"), TEXT("ERROR"));	return -1;
	}
	if (!g_socketServer.Create(nScoketPort))
	{
		strTemp.Format(TEXT("%s%d"), TEXT("SocketServer create failed! Code: "), g_socketServer.GetLastError());
		MessageBox(strTemp, TEXT("ERROR"));	return -1;
	}
	if (!g_socketServer.Listen())
	{
		strTemp.Format(TEXT("%s%d"), TEXT("SocketServer listen failed! Code: "), g_socketServer.GetLastError());
		MessageBox(strTemp, TEXT("ERROR"));	return -1;
	}

	// Create Server accept thread
	g_bReadThreadFlag = TRUE;
	g_bAcceptThreadFlag = TRUE;
	if (!AfxBeginThread(&ServerAcceptThreadCTRL, this))
	{
		MessageBox(TEXT("Server accept thread create failed!"), TEXT("ERROR"));	return -1;
	}

	return 0;
}


// Server accept thread control function
// pParam -- Dlg pointer
UINT ServerAcceptThreadCTRL(LPVOID pParam)
{
	CNetTelecomDlg* pDlg = (CNetTelecomDlg*)pParam;

	while (g_bAcceptThreadFlag == TRUE)
	{
		CAsyncSocket* socketTemp = new CAsyncSocket;	// ***Remeber to delete***
		SReadThread readThread;

		readThread.m_SocketTemp = socketTemp;
		if (!g_socketServer.Accept(*readThread.m_SocketTemp)) { continue; }

		// Get Client IP and PORT
		SADDRFamily ADDRFamily;
		readThread.m_SocketTemp->GetPeerName(ADDRFamily.m_strIP, ADDRFamily.m_unPORT);
		ADDRFamily.m_strADDR.Format(TEXT("%s:%u"), ADDRFamily.m_strIP, ADDRFamily.m_unPORT);
		readThread.m_ADDRFamily = ADDRFamily;
	
		readThread.m_treeNode = pDlg->m_treeADDR.InsertItem(readThread.m_ADDRFamily.m_strADDR, TCP_CLIENT);	// Add node to tree

		// Create read thread
		readThread.m_ReadThreadFlag = TRUE;
		readThread.m_pWinThread = AfxBeginThread(&ServerReadThreadCTRL, pDlg);
		if (readThread.m_pWinThread == NULL)
		{
			pDlg->m_editMessage.SetWindowText(TEXT("Create read thread failed!")); continue;
		}
		g_ThreadList.AddTail(readThread);	// Add new thread to list tail
		
		pDlg->UpdateData();
		Sleep(1);
	}

	return 0;
}


// Server read thread control function
// pParam -- Dlg pointer
UINT ServerReadThreadCTRL(LPVOID pParam)
{
	CNetTelecomDlg* pDlg = (CNetTelecomDlg*)pParam;
	pDlg->m_editMessage.SetWindowText(TEXT("No Connection acquire! Listening..."));

	CWinThread* pWinThread;
	pWinThread = AfxGetThread();	// Get the pointer of this thread
					
	// Find the pos in the list
	POSITION pos = 0;	// The position of the list
	SReadThread readThread;
	int nCount = g_ThreadList.GetCount();
	for (int i = 0; i <= nCount; i++)
	{	
		pos = g_ThreadList.FindIndex(i);
		readThread = THREAD_LIST;
		if (*readThread.m_pWinThread == *pWinThread) { break; }
	}

	pDlg->m_editMessage.SetWindowText(TEXT("Connection Success!"));
	pDlg->m_editReceive.SetWindowText(TEXT(""));

	while (THREAD_LIST.m_ReadThreadFlag == TRUE && g_bReadThreadFlag == TRUE)
	{
		char cTemp[1024] = { 0 };
		int nTemp = THREAD_LIST.m_SocketTemp->Receive(cTemp, 1024);
		if (nTemp != -1 && cTemp[0] != 0)
		{
			// Get Time
			g_Time = CTime::GetCurrentTime();
			CString strTime = g_Time.Format("%Y-%m-%d %X");
			CString strTemp(cTemp);

			// Add IP
			strTemp = strTime + TEXT("->") + THREAD_LIST.m_ADDRFamily.m_strADDR + TEXT("->") + strTemp;

			// Display msg
			pDlg->m_editReceive.SetSel(-1, -1);	// Auto scorll
			pDlg->m_editReceive.ReplaceSel(strTemp + "\r\n");	// Auto enter
			strTemp.Format(TEXT("%s%d%s"), TEXT("You have received "), strTemp.GetLength(), TEXT(" Bytes!"));
			pDlg->GetDlgItem(IDC_EDIT_MESSAGE)->SetWindowText(strTemp);
			pDlg->UpdateData();
		}
		if (strcmp(cTemp, "Server Closed!") == 0)
		{
			THREAD_LIST.m_ReadThreadFlag = FALSE;
		}

		Sleep(5);
	}

	// Delete child item
	if (pDlg->m_treeADDR.DeleteItem(THREAD_LIST.m_treeNode) == 0)
	{
//		pDlg->MessageBox(TEXT("Treectrl node delete failed!"));	// **Need to be fixed**
	}
	
	THREAD_LIST.m_SocketTemp->Close();
	delete THREAD_LIST.m_SocketTemp;
	g_ThreadList.RemoveAt(pos);	// Delete list node	

	pDlg->UpdateData();
	pDlg->m_editMessage.SetWindowText(TEXT("Server read thread Closed!")); 
	return 0;
}


// Client handler
// strTGTAddr  -- Target IP addr
// nSocketPort -- Target port
int CNetTelecomDlg::ClientHandler(CString strTGTAddr, UINT nSocketPort)
{
	CString strTemp;
	if (!AfxSocketInit())
	{
		MessageBox(TEXT("AfxSocketInit Failed!"), TEXT("ERROR"));	return -1;
	}
	if (!g_socketClient.Create())
	{
		strTemp.Format(TEXT("%s%d"), TEXT("SocketServer Create failed! Code: "), g_socketServer.GetLastError());
		MessageBox(strTemp, TEXT("ERROR"));	return -1;
	}
	if (!g_socketClient.Connect(strTGTAddr, nSocketPort))
	{
		/* *Need to be fixed*
		strTemp.Format(TEXT("%s%d"), TEXT("SocketServer Connect failed! Code: "), g_socketServer.GetLastError());
		MessageBox(strTemp, TEXT("ERROR"));	return -1;
		*/
	}
	
	// Create client read thread
	g_bReadThreadFlag = TRUE;
	if (!AfxBeginThread(&ClientReadThreadCTRL, this))
	{
		MessageBox(TEXT("Client read thread create failed!"), TEXT("ERROR"));	return -1;
	}
	
	return 0;
}


// Client read thread control function
// pParam -- Dlg pointer
UINT __cdecl ClientReadThreadCTRL(LPVOID pParam)
{	
	CNetTelecomDlg* pDlg = (CNetTelecomDlg*)pParam;

	while (g_bReadThreadFlag == TRUE)
	{
		char cTemp[1024] = { 0 };

		int nTemp = g_socketClient.Receive(cTemp, 1024);
		if (nTemp != -1 && cTemp[0] != 0)
		{
			// Get Time
			g_Time = CTime::GetCurrentTime();
			CString strTime = g_Time.Format("%Y-%m-%d %X -> ");
			CString strTemp(cTemp);
			strTemp = strTime + strTemp;

			// Display Msg
			pDlg->m_editReceive.SetSel(-1, -1);	// Auto scorll
			pDlg->m_editReceive.ReplaceSel(strTemp + "\r\n");	// Auto enter
			strTemp.Format(TEXT("%s %d %s"), TEXT("You have received"), strTemp.GetLength(), TEXT("Bytes!"));
			pDlg->GetDlgItem(IDC_EDIT_MESSAGE)->SetWindowText(strTemp);
			pDlg->UpdateData();
//			int nTemp = g_socketClient.Send(cTemp, sizeof(cTemp) * sizeof(char));	// Reply msg
		}
		if (strcmp(cTemp, "Server Closed!") == 0) 
		{ 
			pDlg->OnBnClickedButtonStop(); 
		}	

		Sleep(5);
	}

	pDlg->m_editMessage.SetWindowText(TEXT("Client read thread stopped!"));
	return 0;
}


// Send msg when click this button
void CNetTelecomDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	char cSend[1024];
	m_editSend.GetWindowText(strTemp);
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, strTemp, -1, cSend, sizeof(cSend) + 1, NULL, NULL);

	if (g_nIndex == INDEX_SERVER)
	{
		// Get the selection of the tree
		HTREEITEM treeNodeTemp = m_treeADDR.GetSelectedItem();	// 会一直保持选择以删除的节点的状态
		if (treeNodeTemp == NULL || treeNodeTemp == TCP_CLIENT) 
		{ 
			m_editMessage.SetWindowText(TEXT("Please select a client!"));	return;
		}

		// Find the pos in the list
		POSITION pos = 0;
		SReadThread readThread;
		int nCount = g_ThreadList.GetCount();
		for (int i = 0; i <= nCount; i++)
		{
			pos = g_ThreadList.FindIndex(i);
			readThread = THREAD_LIST;
			if (readThread.m_treeNode == treeNodeTemp) { break; }
		}

		if (cSend[0] != 0){
			int nTemp = THREAD_LIST.m_SocketTemp->Send(cSend, sizeof(cSend) * sizeof(char));
		}else{
			m_editMessage.SetWindowText(TEXT("Cannot send temp msg!"));	return; 
		}
	}
	else if (g_nIndex == INDEX_CLIENT)
	{
		if (cSend[0] != 0){
			int nTemp = g_socketClient.Send(cSend, sizeof(cSend) * sizeof(char));
		}else{
			m_editMessage.SetWindowText(TEXT("Cannot send temp msg!"));	return; 
		}
	}

	m_editSend.SetWindowText(TEXT(""));
	strTemp.Format(TEXT("%s%d%s"), TEXT("You have send "), strlen(cSend), TEXT(" bytes"));
	m_editMessage.SetWindowText(strTemp);
	return;
}


// Start Connection
void CNetTelecomDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	g_nIndex = m_comboOrder.GetCurSel();	// Set order
	if (g_nIndex == INDEX_SERVER)	// Server
	{
		CString strTemp;
		m_editRCVDPort.GetWindowText(strTemp);

		if (strTemp == TEXT(""))
		{
			m_editMessage.SetWindowText(TEXT("Please input the RCVD Port!"));	return;
		}
		if (ServerHandler(CStringToUINT(strTemp)) != 0) { return; }
	}
	else if (g_nIndex == INDEX_CLIENT)	// Client
	{		
		CString strAddr;
		CString strPort;
		m_editTGTAddr.GetWindowText(strAddr);
		m_editTGTPort.GetWindowText(strPort);

		if (strAddr == TEXT("") || strPort == TEXT(""))
		{
			m_editMessage.SetWindowText(TEXT("Please input the TGT Param!"));	return;
		}
		if (ClientHandler(strAddr, CStringToUINT(strPort)) != 0	) { return; }
	}
	else
	{
		m_editMessage.SetWindowText(TEXT("Please select the order!"));	return;
	}
	
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);	// Disable button Start
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
	UpdateData();
	return;
}


// Close Connection
void CNetTelecomDlg::OnBnClickedButtonStop()
{
	if (g_nIndex == INDEX_SERVER)
	{
		HTREEITEM treeNodeTemp = m_treeADDR.GetSelectedItem();		// Get the pos in the tree
		if (treeNodeTemp == NULL || treeNodeTemp == TCP_CLIENT)
		{
			m_editMessage.SetWindowText(TEXT("Please select a client!"));	return;
		}

		// Find the pos in the list
		POSITION pos = 0;
		SReadThread readThread;
		int nCount = g_ThreadList.GetCount();
		for (int i = 0; i <= nCount; i++)
		{
			pos = g_ThreadList.FindIndex(i);
			readThread = THREAD_LIST;
			if (readThread.m_treeNode == treeNodeTemp) { break; }
		}

		char cSend[] = { "Server Closed!" };
		THREAD_LIST.m_SocketTemp->Send(cSend, sizeof(cSend) * sizeof(char));	// Reply server close msg
		THREAD_LIST.m_ReadThreadFlag = FALSE;
	}
	else if (g_nIndex == INDEX_CLIENT)
	{
		char cSend[] = { "Server Closed!" };
		g_socketClient.Send(cSend, sizeof(cSend) * sizeof(char));

		g_bReadThreadFlag = FALSE;
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);	// Switch botton stat
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

		m_editMessage.SetWindowText(TEXT(""));	// Clean text
		m_editReceive.SetWindowText(TEXT(""));
		g_socketClient.Close();	// Close socket
	}
	else {}
}


// Close all connection
void CNetTelecomDlg::OnBnClickedButtonStopAll()
{
	m_editMessage.SetWindowText(TEXT(""));	// Clean text
	m_editReceive.SetWindowText(TEXT(""));

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);	// Switch botton stat
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);

	g_bReadThreadFlag = FALSE;
	g_bAcceptThreadFlag = FALSE;
	g_socketServer.Close();
	return;
}


// Transform CString to UINT
// strTemp -- CString to transform
UINT CStringToUINT(CString strTemp)
{
	UINT nTemp;
	if (strTemp.Find(_T("0x")) == 0){
		_stscanf_s((LPCTSTR)strTemp, _T("%x"), &nTemp);
	}else{
		_stscanf_s((LPCTSTR)strTemp, _T("%d"), &nTemp); }
	return nTemp;
}
