#pragma once

#include "star.h"


// ����� ����������� ����� ������ �������,
// ��� �������� �������� ������� � ��������� ����������� �����
// ������ � ������������ ������
class CCosmos
{
public:
	CCosmos(void);
	CCosmos(int number_stars);
	~CCosmos(void);
	
	HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice, BOOL active);
	HRESULT Init(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT ReleaseCosmos(void);
private:
	CStar* m_pArrStars; // ��������� �� ������ ���������
	int m_NumberStars;  // ���������� ����� ����������� � ������� �������
};
