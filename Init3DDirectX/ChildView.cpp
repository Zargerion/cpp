////////////////////////////////////////////////////////////////////////////////////
//
//			www.interestprograms.ru - ���������, ���� � �� �������� ����
//
/////////////////////////////////////////////////////////////////////////////////////

// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Init3DDirectX.h"
#include "ChildView.h"
#include ".\childview.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CChildView
CChildView::CChildView()
: m_pD3D(NULL)
, m_pd3dDevice(NULL)
, m_pPoint(NULL)
, m_FullScreen(FALSE)
, m_Active(false)
, m_pCosmos(NULL)
{
	InitD3D();
}

CChildView::~CChildView()
{
	m_pCosmos->ReleaseCosmos();
	RELEASE(m_pd3dDevice);
	RELEASE(m_pD3D);
	
	delete m_pCosmos;
	m_pCosmos = NULL;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		// ������������ ����� � ������� ������, ��� ���������� �� ����������� � �������� ����-����������
		::LoadCursor(NULL, IDC_ARROW), NULL /*reinterpret_cast<HBRUSH>(COLOR_WINDOW+1)*/, NULL);

	return TRUE;
	
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	CreateDevice3D();
	// ����� �������� ���������� ����� � �������
	m_pCosmos = new CCosmos(1500);
	m_pCosmos->Init(m_pd3dDevice);
	
	return 0;
}


void CChildView::OnPaint() 
{
	// ���� ����� ���������� �� ������� (������������ �� ������ ����������,
	// ������������� ���� � ��.) ��������� �������������� 
	// ����������� ����, �������� ������ ��������� ������������ �����������
	if(m_Active == false)
		CPaintDC dc(this); // device context for painting
	
	// ��������� ������� ���������� ����� OnPaint
	// ��� � ������� OnIdle ����������
	// �������� � OnPaint ��� ������������� �������������� ������� ���� 
	// ���������� �� ����� �������� ������-���� ����������� ����
	// �.�. � ���� ������ ����������� � ���� �� ����������
	Render();
	
	
	// ��� ��������� �� ��������� ������� OnPaint, 
	// ���������� ���������� ���������, �� ��������� ��������� ��������� WM_PAINT
	//CWnd::OnPaint();
}

// ������������� ����� Direct3D
HRESULT CChildView::InitD3D(void)
{

	HRESULT hr = S_OK;

	// �������� D3D_SDK_VERSION - ������������� ����� ����������� ������
	// DirectX ������ Direct3D
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D ==  NULL)
	{
		AfxMessageBox("������ - Direct3D �� ������!");
		return E_FAIL;
	}

	return hr;

}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ResetScene();

}


// ��������� ������ - 
// ������� ��� ���������� �������� ���������� � ������� ����������� ������� 3D ����,
// ������� ���� ��� ���������� ������(������� �� ������ 3D ���) � �������� �������,
// ������� ����������� ��� �������� ������� 3D ���� �� ��������� ��������
HRESULT CChildView::SetupMatrices()
{
	HRESULT hr = S_OK;
    D3DXMATRIXA16 matWorld;
	// ������� ������� �������
	D3DXMatrixIdentity(&matWorld);

    hr = m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if(hr != S_OK) return hr;

    // ���������� ���������� ����� ������
    D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -5.0f );

	// ���������� ������� �� ������� �� �������
    D3DXVECTOR3 vLookatObject( 0.0f, 0.0f, 0.0f );

	// ����������� ����� ����� ������
    D3DXVECTOR3 vUpCamera( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatObject, &vUpCamera );
    hr = m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	if(hr != S_OK) return hr;

    D3DXMATRIXA16 matProj;
	FLOAT aspect = m_Aspect;
    D3DXMatrixPerspectiveFovLH( &matProj, 
								D3DX_PI/4,	// 
								aspect,		// ��������� ����������� ������(��������� ������ � ������ ������)
								1.0f,		// ���������� �� ������� ��������� ���������
								400.0f );	// ���������� �� ������� ��������� ���������
    hr = m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	return hr;
}



// � ������ ������� ������ ������� ���������� � ������������
// � ������� ���������
// �������������� ��������� ���������� Direct3D ������������
// ��� ������/�������������� ���������� � ������� ResetDevice3D
// ������������� ��� ��������� ��������( ��������� ������� ���� ����������, 
// ��������� ������������� ���������� �������� ������� ������� �������� � ��.) 
HRESULT CChildView::CreateDevice3D(void)
{
	HRESULT hr = S_OK;
	
	D3DPRESENT_PARAMETERS d3dpp;
	hr = GetD3DPresentParameters(&d3dpp);

	// ������ ��������� ��������� �������� �� ������������
	if(hr != S_OK)
	{
		AfxMessageBox("������ - ������������ ��������� ������������� ����� Direct3D");
		return hr;
	}

	// 
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
														&d3dpp, &m_pd3dDevice);

	if(hr != S_OK)
	{
		AfxMessageBox("������ - ���������� Direct3D �� �������!");
		return hr;
	}


	return hr;
}


// ����� � �������������� ���������� Direct3D ��� ��� ������.
// ��� ����� ���������, ��������, ��� ��������� �������� ������� ��������
// �� ����� ������ ����������, ��� ��� ������������� ������
// ���������� ��� ��������� �������� ���� ��� ���������� ��������������
// ��������� ������� ����������
HRESULT CChildView::ResetDevice3D(void)
{
	HRESULT hr = S_OK;
	D3DPRESENT_PARAMETERS d3dpp; 
	hr = GetD3DPresentParameters(&d3dpp);

	// ����� ������� ��������� ���� ��� ��������� Direct3D
	// ����������� ���������� �������� �� ������������ ������� 
	// ���������� ������� Direct3D
	if(hr != S_OK)
	{
		AfxMessageBox("������ - ������������ ��������� ������������� ����� Direct3D");
		return hr;
	}


	// ����� ������� �������� ���������� Direct3D
	// � �������������� ���������� � ������ ����������� ������������� 
	// 3D �����
	hr = m_pd3dDevice->Reset(&d3dpp);
	
		
	if(hr != S_OK)
	{
		//AfxMessageBox("������ - �� ������� ��������/������������ ���������� Direct3D");
		return hr;
	}
	
	// ����� �������������� ���������� ����������� ��������� �
	// ������� SetupMatrices ��� ������������� ������� ����������� �
	// D3DXMatrixPerspectiveFovLH � ����� ���������� 
	// �������������� ��������� �������� ����������,
	// ��������, ��� ������ ���������� ����� ��� �����
	// ���������� �������� ����-����������
	m_Aspect = (float)d3dpp.BackBufferWidth/d3dpp.BackBufferHeight;


	// ��������� ��� �������� ����� ������ � ����� ������,
	// �.�. � ���������� � �������
	// �� � ������ ���������� �������� ������
	// ���� ����� ����� ��������� ������
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// � ������ ������� ���� ��� �� �����, ��������� ���,
	// ����� ���� ��������� ����� ������� � 
	// ����������� ��������� "��������� �����"
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// ��������� ��������� ����������� ��� �������� ��������������
	// ����������
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	return hr;
}

// �������� ���������� ��������� ������������� �����
// ����������� ��� �������� � ������/�������������� ���������� Direct3D
HRESULT CChildView::GetD3DPresentParameters(D3DPRESENT_PARAMETERS* d3dpp)
{
	HRESULT hr = S_OK;

	// ������� ��������� �� ��������� ������
	ZeroMemory( d3dpp, sizeof(D3DPRESENT_PARAMETERS) );

	// ������������� ����� ������ ���������� (������� ��� �������������)
	d3dpp->Windowed = !m_FullScreen;
		

	D3DDISPLAYMODE display_mode;
	// ������� ��������� �� ��������� ������
	ZeroMemory( &display_mode, sizeof(D3DDISPLAYMODE) );

	// �������� ������� ����� ������ ������� � ������� �������� ��������� �������,
	// ���� ������� �������� � ���� ������ ����� �������� ������������
	// ��� ��������� ��� ���������� � ����� �������
	hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode);

	if(hr != S_OK)
	{
		AfxMessageBox("������ - ���������� �������� ����� �������!");
		return hr;
	}

	// ���� ����� ������ ���������� ������������� 
	// ������������� ��������������� ���������
	// ��� �������������� ������ �� ������ ������������� ����� ���������
	// �� ������ ���������� � �������� ������������ Direct3D9
	if(m_FullScreen == TRUE)
	{
		d3dpp->hDeviceWindow = AfxGetMainWnd()->m_hWnd;

		// ������� ������� ���������� �������  
		d3dpp->BackBufferWidth = display_mode.Width;	
		d3dpp->BackBufferHeight = display_mode.Height;
		// ������� ������� �������������� ������� ����� �������
		d3dpp->FullScreen_RefreshRateInHz = display_mode.RefreshRate;
		d3dpp->PresentationInterval =  D3DPRESENT_INTERVAL_DEFAULT;
	}

	// ���������� ������� ���������� �������
	// ��� �������� �������� �������� ��� �� ���������� �������� 
	// �������� ������
	const D3DFORMAT depth_stencil[] = 
	{
		D3DFMT_D32,
		D3DFMT_D24FS8,
		D3DFMT_D24X8,
		D3DFMT_D24S8, 
		D3DFMT_D24X4S4,
		D3DFMT_D16,
		D3DFMT_D15S1
	};

	const UINT ds_count = sizeof(depth_stencil) / sizeof(depth_stencil[0]);

	int index;
	for(index = 0; index < ds_count; index++)
	{
		// ���������� � ���������� ���������� ������ ��������� �������� 
		// ����������� � ��� ��� ����� ����������� � ����������
		if( (hr = m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			display_mode.Format, display_mode.Format, depth_stencil[index])) == S_OK)
		{
			break;
		}
	}
		

	// ������ ������� ������ ������� ������ ������ ���������� ������
	d3dpp->BackBufferFormat = display_mode.Format;

	// ����������� ���������� ���������� �������� ������ Direct3D
	d3dpp->EnableAutoDepthStencil = TRUE;
	// ��� ��������� ������ ��������� ����� ���������� ����������� ������
	d3dpp->AutoDepthStencilFormat = depth_stencil[index];

	// ���������� ������������� ������ ������ ���������� �������� (��. SDK DirectX C++)
	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;


	return hr;
}


HRESULT CChildView::Render(void)
{
	HRESULT hr = S_OK;

	if(m_pd3dDevice == NULL) return E_FAIL;

	
	// ������� ��� ���� ���������� � ��������� ��� �������� ������
	hr = m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	if(hr != S_OK) return hr;
	
	// ������ ���������� ������� �����
	hr = m_pd3dDevice->BeginScene();

	if(hr != S_OK) return hr;

	// ����������� �������� ��� ������������ ����� 
	hr = SetupMatrices();

	if(hr != S_OK) return hr;

	// ��������� ������ ������ ������
	hr = m_pCosmos->Render(m_pd3dDevice, m_Active);

	if(hr != S_OK) return hr;

	// ���������� ����� � ������
	hr = m_pd3dDevice->EndScene();

	if(hr != S_OK) return hr;

	// ��������������� ���������� ����� ����� 
	hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	// ���� ���������� ��������, ���������� ��� ������������
	if(hr == D3DERR_DEVICELOST)
	{
		// ��� ������ ���������� ����� ���� �������������
		// ���������� ��� � ��������������� ����� 3D ������
		if(m_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ResetScene();
		}
	}

	return hr;
}


// ��������� ����������� ����
void CChildView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// ���� ������������� ����� ��������� ������
	// ������������ ����, � ������������� ������ 
	// ����� ��������� ���� ���������� DirectX
	if(m_FullScreen == TRUE)
		return;

	CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
	CMenu* file_menu = menu_bar->GetSubMenu(0);    
	ASSERT(file_menu);

	file_menu->InsertMenu(0, MF_STRING|MF_BYPOSITION, 10, "������� ���� �� �������");

	file_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, 
		point.y, this);

	file_menu->DeleteMenu(10, MF_BYCOMMAND);
}


// �����/�������������� �����
HRESULT CChildView::ResetScene(void)
{
	HRESULT hr = S_OK;

	// ����������� ������ ���������� �������� �����
	hr = m_pCosmos->ReleaseCosmos();
	if(hr != S_OK)
		return hr;
			
	// ���������� ����������
	hr = ResetDevice3D();
	if(hr != S_OK)
		return hr;

	// ������� ������ �������� ������������
	hr = m_pCosmos->Init(m_pd3dDevice);

	return hr;
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	static bool full_screen = true;
	static WINDOWPLACEMENT m_WndPlacement;

	if(full_screen == true)
	{
		// �������� ������� � ��������� ���� �� �������������� ������
		AfxGetMainWnd()->GetWindowPlacement(&m_WndPlacement);

		m_FullScreen = TRUE;

		// ����� �����
		ResetScene();
	}
	else
	{
		m_FullScreen = FALSE;
		
		// ����� �����
		ResetScene();

		// ��������������� ������� ��������� ����
		AfxGetMainWnd()->SetWindowPlacement(&m_WndPlacement);
	}

	full_screen = !full_screen;

	CWnd::OnLButtonDblClk(nFlags, point);
}
