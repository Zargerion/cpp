// ChildView.h : interface of the CChildView class
//


#pragma once
#include "star.h"
#include "cosmos.h"


struct CUSTOMVERTEX
{
    FLOAT x, y, z; // The transformed position for the vertex.
    DWORD color;        // The vertex color.
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


// CChildView window
class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	// DirectX9
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	
	HRESULT InitD3D(void);
	HRESULT SetupMatrices();
	HRESULT CreateDevice3D(void);
	HRESULT ResetDevice3D(void);
	// получить параметры представления показа сцены 3D для устройства
	HRESULT GetD3DPresentParameters(D3DPRESENT_PARAMETERS* d3dpp);
	HRESULT Render(void);
	HRESULT ResetScene(void);
	bool m_Active;
	
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pPoint;
	CCosmos* m_pCosmos;
	BOOL m_FullScreen;
	FLOAT m_Aspect;

	
};

