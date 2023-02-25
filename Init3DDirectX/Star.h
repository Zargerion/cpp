#pragma once
#include "Init3DDirectX.h"


struct CUSTOMVERTEXSTAR
{
    FLOAT x, y, z; // ��������� ������� �������
    DWORD color;   // ���� �������
};

// ���������� ������ ������ � ��������� � ������ �������������
#define D3DFVF_CUSTOMVERTEXSTAR (D3DFVF_XYZ|D3DFVF_DIFFUSE)



// ����� ��������� - ��������, ��������, ��������� ������, ������������ ������
class CStar
{
public:
	CStar(void);
	~CStar(void);
	HRESULT InitStar(LPDIRECT3DDEVICE9 pd3dDevice);
	LPDIRECT3DVERTEXBUFFER9 m_pVBStar;	// ����� ������
	HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice, BOOL active); // ��������� ���������
	HRESULT ReleaseStar(void);

private:
	FLOAT m_PosX;	// ������� ��������� �� ��� �
	FLOAT m_PosY;	// ������� ��������� �� ��� Y
	FLOAT m_Trans;	// ����������� ����� ��� Z 
	
};
