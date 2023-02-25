////////////////////////////////////////////////////////////////////////////////////
//
//			www.interestprograms.ru - ���������, ���� � �� �������� ����
//
/////////////////////////////////////////////////////////////////////////////////////

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Init3DDirectX.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	// ��� ������������� Direct3D 3D-����������, �����������! ���� ������ ����� �� ������� ������� 
	// � ����� ������ ������� ��������� ���� ������� ������� ���������� ������� ������
	CRect rc_initchildview;
	GetClientRect(&rc_initchildview);

	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rc_initchildview, this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
	
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	

	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// ����� ������������� ������� 3D �����
	// �� �������� ���������� � ������������ ������ ����������,
	// ������ ������, � �� ����������, ��������� ����� 
	// ���������  ����������� ���� �� �������� � ������������ ����������
	if(nState == WA_INACTIVE)
	{
		m_wndView.m_Active = false;
	}
	else 
	{
		if(nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
			m_wndView.m_Active = true;

		// ����� ��������� ���������� (����� ������� ������ Alt+Tab)
		// ����������� ���������� 3D
		m_wndView.ResetScene();
	}
}

