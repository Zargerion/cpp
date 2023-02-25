////////////////////////////////////////////////////////////////////////////////////
//
//			www.interestprograms.ru - ���������, ���� � �� �������� ����
//
/////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\cosmos.h"


// ����������� ��������,
// ��� ��������������� ������ � ������ �������������
// �������� ����������� �� ���������
// ���� ��� �� ���������, �� �� ����� ���������� ��� 
// ������������� ����������� ��������� ������ ������
CCosmos::CCosmos(void)
: m_pArrStars(NULL)
, m_NumberStars(0)
{
	AfxMessageBox("����� CCosmos - �� ������ ���������� �����!\n\
������ ������ ������ ����������� � ������������ ������.\n����������� ����������� � ������� ���������� �����.");
}


// ������� �����������
// ������������ �� ��������� ���������� �����
// ������� ������ ����� � ������������ ������
// ������ ��� ������������ ������� � ������������ ����� ����� 
CCosmos::CCosmos(int number_stars)
{
	// �������� ���������� �����
	m_NumberStars = number_stars;

	// ������� ������ ��������� ��� �������� �������������
	// �����
	m_pArrStars = new CStar[number_stars];
}

CCosmos::~CCosmos(void)
{
	// ����������� ������ �� ����� �������
	delete [] m_pArrStars;
	// �� ������ ������ ������������ ��������� � ����
	m_pArrStars = NULL;
}

// ��������� ���� ��������� � �������
HRESULT CCosmos::Render(LPDIRECT3DDEVICE9 pd3dDevice, BOOL active)
{
	for(int i = 0; i < m_NumberStars; i++)
	{
		m_pArrStars[i].Render(pd3dDevice, active);
	}

	return S_OK;
}


// ������������� ���������
HRESULT CCosmos::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for(int i = 0; i < m_NumberStars; i++)
	{
		m_pArrStars[i].InitStar(pd3dDevice);
	}

	return S_OK;
}


// ������������ ������ ���������� ������ �������
HRESULT CCosmos::ReleaseCosmos(void)
{
	HRESULT hr = S_OK;
	for(int i = 0; i < m_NumberStars; i++)
	{
		hr = m_pArrStars[i].ReleaseStar();
		if(hr != S_OK)
			return hr;
	}

	return hr;
}
