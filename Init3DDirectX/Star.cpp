////////////////////////////////////////////////////////////////////////////////////
//
//			www.interestprograms.ru - ���������, ���� � �� �������� ����
//
/////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\star.h"



CStar::CStar(void)
: m_pVBStar(NULL)
, m_Trans(100)
{
	// ��� ������ ������� ��������� ������������ �������������
	// ������� ��������������� �����
	srand((unsigned)time( NULL ));
}

CStar::~CStar(void)
{
}


// �������� � ��������� ������ ������ ���������
HRESULT CStar::InitStar(LPDIRECT3DDEVICE9 pd3dDevice)
{

	HRESULT hr = S_OK;

	if(pd3dDevice == NULL)
	{
		return E_FAIL;
	}


	// ��� �������� �������� ��������� ����������
	// ��� ��������� ��������� ������ � ������������,
	// �� ���������� �������, ���� �����
	float x = 0.0f;
	float y = 0.0f;

	float width  = 0.3f;
	float height = 0.3f;

	float width_center  = 0.05f;
	float height_center = 0.05f;


	DWORD color1 = 0xffff0000;
	DWORD color2 = 0xff0000ff;
	DWORD color3 = 0xff00ff00;
	DWORD color4 = 0xff00ffff;
	

	// ��������� ������ ������ � ������� ��������� ��������� �������� �������
	// �������� ������ ����, �.�. ����� 12 ������
	CUSTOMVERTEXSTAR vertices[] =
	{
		{-width_center+x,		-height_center+y,		0.0f, color1, },
        {-width+x,				 y,						0.0f, color1, },
        {-width_center+x,		 height_center+y,		0.0f, color1, },

		{-width_center+x,		 height_center+y,		0.0f, color2, },
        { x,					 height+y,				0.0f, color2,},
        { width_center+x,		 height_center+y,		0.0f, color2, },


		{ width_center+x,		 height_center+y,		0.0f, color3, },
		{ width+x,				 y,						0.0f, color3,},
        { width_center+x,		-height_center+y,		0.0f, color3, },

		{ width_center+x,		-height_center+y,		0.0f, color4, },
        { x,					-height+y,				0.0f, color4,},
        {-width_center+x,		-height_center+y,		0.0f, color4, },
		
	};

	// ���������������� �������� ������ ������ �������������
	// ������� � �������
	hr = pd3dDevice->CreateVertexBuffer(	12*sizeof(CUSTOMVERTEXSTAR),
											0, D3DFVF_CUSTOMVERTEXSTAR,
											D3DPOOL_DEFAULT, &m_pVBStar, NULL );

	if(hr != S_OK)
	{
		AfxMessageBox("������ - �� ������� ������� ����� ������ ���������!");
		return hr;
	}

	VOID* pVertices;
	// ��������� ����� �� ������� ����� ���������
	// � �������� ������ ������ � ��������������� �����,
	// �� ���������� �������� ����������� ������������ ����� ������
	hr = m_pVBStar->Lock( 0, sizeof(vertices), (void**)&pVertices, 0);

	if(hr == S_OK)
	{
		memcpy( pVertices, vertices, sizeof(vertices) );
		hr = m_pVBStar->Unlock();
	}

	
	return hr;
}


// ��������� ���������
HRESULT CStar::Render(LPDIRECT3DDEVICE9 pd3dDevice, BOOL active)
{
	HRESULT hr = S_OK;
	D3DXMATRIXA16 matWorld;

	// ���� ���������� �������, ���������� �������� �����,
	// ���� �� �������, ������ ���������������
	if(active == true)
		m_Trans -= 0.8f;


	// ��� ������ ���������(������� ������ CStar) ������ �����������
	// ���������� ���������� ��������
	if(m_Trans >= 0.0f && m_Trans <= 1.0f)
	{
		// ������������ ���������� ��������� �� ��� X
		FLOAT fraction = (FLOAT)(rand()%100)/100.0f;
		m_PosX = (FLOAT)(rand()%50) + fraction;

		// ������������ ���������� ��������� �� ��� X
		fraction    = (FLOAT)(rand()%100)/100.0f;
		m_PosY = (FLOAT)(rand()%50) + fraction;

		// ������������ ���������� ���������� ��������� �� ��� Z
		m_Trans = ((FLOAT)(rand()%100) + 100.0f); 
		
		// ��� ��������� ��������������
		// ���� ������ ����� ����� �� ���
		// ������� �� �� ������ ���������� �� ������
		if( m_PosX < 1.5f && m_PosX >= -1.5f)  m_PosY += 2.0f;
		if( m_PosY < 1.5f && m_PosY >= -1.5f ) m_PosX += 2.0f;
		

		// �� ���������� ��������� �������
		// ��������� � �� ������������� �����������
		if(rand()% 2 != 0) m_PosY *= -1;
		if(rand()% 2 != 0) m_PosX *= -1;

	}

	// � �������� �������� �� ��� � � Y ��������� �������� � ���� �������
	D3DXMatrixTranslation(&matWorld, m_PosX, m_PosY, m_Trans);

	// ��� ������ ����� ��� ������������ � ��������� ���� ��������� 
	//D3DXMatrixTranslation(&matWorld, 0, 0, 0);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// ��������� ����� ������ ��������� � ����� ���������� Direct3D
	hr = pd3dDevice->SetStreamSource( 0, m_pVBStar, 0, sizeof(CUSTOMVERTEXSTAR) );
	if(hr == S_OK)
	{
		// ������������� ������ ������ ���������� �� ������� ������� ���������,
		// ����� ���������� ��������� 4-� �������������
		hr = pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEXSTAR );
		if(hr == S_OK)
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 4 );
	}

	return hr;
}

// ������������ ������ ���������� ������� ������ ���������
HRESULT CStar::ReleaseStar(void)
{
	RELEASE(m_pVBStar);

	return S_OK;
}
