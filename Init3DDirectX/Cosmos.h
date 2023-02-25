#pragma once

#include "star.h"


// класс реализующий видео эффект космоса,
// для удобства создания космоса с различным количеством звезд
// должен в динамической памяти
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
	CStar* m_pArrStars; // указатель на массив звездочек
	int m_NumberStars;  // количество звезд участвующих в эффекте космоса
};
