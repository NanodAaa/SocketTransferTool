
// NetTelecom.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNetTelecomApp: 
// �йش����ʵ�֣������ NetTelecom.cpp
//

class CNetTelecomApp : public CWinApp
{
public:
	CNetTelecomApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNetTelecomApp theApp;