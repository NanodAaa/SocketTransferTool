#pragma once
// Global.h
// Powered By NanodAaa
// 2023/11/18

#include "stdafx.h"
#include "afxwin.h"
#include "NetTelecom.h"
#include "NetTelecomDlg.h"
#include "afxdialogex.h"
#include <afxsock.h>

#define _TEST_	// Test switch
#define THREAD_LIST g_ThreadList.GetAt(pos)
#define INDEX_SERVER 0
#define INDEX_CLIENT 1

// Storge IP address and Port
typedef struct __tagADDRFamilyStruct
{
	CString m_strADDR;	// IP + PORT
	CString m_strIP;	// IP
	UINT m_unPORT;		// PORT
}SADDRFamily;

// Struct for Thread list to storge
typedef struct __tagReadThreadStruct
{
	CAsyncSocket* m_SocketTemp;		// Temp socket for accpet
	CWinThread* m_pWinThread;		// Thread pointer
	bool m_ReadThreadFlag;			// Read thread enable flag
	HTREEITEM m_treeNode;			// Storge TreeCtrl's node
	SADDRFamily m_ADDRFamily;		// Storge IP address and Port
}SReadThread;

extern bool g_bAcceptThreadFlag;	// 
extern bool g_bReadThreadFlag;
extern int g_nIndex;
extern CAsyncSocket g_socketServer;
extern CAsyncSocket g_socketClient;
extern CTime g_Time;

UINT CStringToUINT(CString strTemp);		// Transform CString to UINT
UINT ServerAcceptThreadCTRL(LPVOID pParam);	// Server accept thread control
UINT ServerReadThreadCTRL(LPVOID pParam);	// Server read thread control
UINT ClientReadThreadCTRL(LPVOID pParam);	// Client read thread control
