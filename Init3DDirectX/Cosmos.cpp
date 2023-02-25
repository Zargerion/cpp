////////////////////////////////////////////////////////////////////////////////////
//
//			www.interestprograms.ru - программы, игры и их исходные коды
//
/////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\cosmos.h"


// конструктор заглушка,
// для перенаправления работы с другим конструктором
// заменяет конструктор по умолчанию
// если его не создавать, то не будет оповещения для 
// использования задуманного алгоритма работы класса
CCosmos::CCosmos(void)
: m_pArrStars(NULL)
, m_NumberStars(0)
{
	AfxMessageBox("Класс CCosmos - Не задано количество звезд!\n\
Объект класса должен создаваться в динамической памяти.\nИспользуйте конструктор с выбором количества звезд.");
}


// рабочий конструктор
// взависимости от заданного количества звезд
// создает массив звезд в динамической памяти
// удобно для тестирования изменяя в конструкторе число звезд 
CCosmos::CCosmos(int number_stars)
{
	// запомним количество звезд
	m_NumberStars = number_stars;

	// создаем массив звездочек для удобства использования
	// звезд
	m_pArrStars = new CStar[number_stars];
}

CCosmos::~CCosmos(void)
{
	// освобождаем память от всего массива
	delete [] m_pArrStars;
	// на всякий случай приравниваем указатель к нулю
	m_pArrStars = NULL;
}

// рендеринг всех звездочек в массиве
HRESULT CCosmos::Render(LPDIRECT3DDEVICE9 pd3dDevice, BOOL active)
{
	for(int i = 0; i < m_NumberStars; i++)
	{
		m_pArrStars[i].Render(pd3dDevice, active);
	}

	return S_OK;
}


// инициализация звездочек
HRESULT CCosmos::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	for(int i = 0; i < m_NumberStars; i++)
	{
		m_pArrStars[i].InitStar(pd3dDevice);
	}

	return S_OK;
}


// освобождение памяти занимаемой каждой звездой
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
