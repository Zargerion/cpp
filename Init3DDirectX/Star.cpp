////////////////////////////////////////////////////////////////////////////////////
//
//			www.interestprograms.ru - программы, игры и их исходные коды
//
/////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\star.h"



CStar::CStar(void)
: m_pVBStar(NULL)
, m_Trans(100)
{
	// при каждом запуске программы произвольная инициализация
	// цепочки псевдослучайных чисел
	srand((unsigned)time( NULL ));
}

CStar::~CStar(void)
{
}


// создание и заполение буфера вершин звездочки
HRESULT CStar::InitStar(LPDIRECT3DDEVICE9 pd3dDevice)
{

	HRESULT hr = S_OK;

	if(pd3dDevice == NULL)
	{
		return E_FAIL;
	}


	// для удобства создадим отдельные переменные
	// для настройки положения звезды в пространстве,
	// ее габаритные размеры, цвет лучей
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
	

	// заполняем массив вершин с помощью созданной структуры описания вершины
	// создадим четыре луча, т.е. всего 12 вершин
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

	// непосредственное создание буфера вершин определенного
	// размера и формата
	hr = pd3dDevice->CreateVertexBuffer(	12*sizeof(CUSTOMVERTEXSTAR),
											0, D3DFVF_CUSTOMVERTEXSTAR,
											D3DPOOL_DEFAULT, &m_pVBStar, NULL );

	if(hr != S_OK)
	{
		AfxMessageBox("Ошибка - не удалось создать буфер вершин звездочки!");
		return hr;
	}

	VOID* pVertices;
	// блокируем буфер от доступа любых обращений
	// и копируем данные вершин в заблокированный буфер,
	// по завершению операции копирования разблокируем буфер вершин
	hr = m_pVBStar->Lock( 0, sizeof(vertices), (void**)&pVertices, 0);

	if(hr == S_OK)
	{
		memcpy( pVertices, vertices, sizeof(vertices) );
		hr = m_pVBStar->Unlock();
	}

	
	return hr;
}


// рендеринг звездочки
HRESULT CStar::Render(LPDIRECT3DDEVICE9 pd3dDevice, BOOL active)
{
	HRESULT hr = S_OK;
	D3DXMATRIXA16 matWorld;

	// если приложение активно, происходит движение звезд,
	// если не активно, звезды останавливаются
	if(active == true)
		m_Trans -= 0.8f;


	// для каждой звездочки(объекта класса CStar) задаем собственную
	// случайнную траекторию движения
	if(m_Trans >= 0.0f && m_Trans <= 1.0f)
	{
		// вырабатываем координату положения по оси X
		FLOAT fraction = (FLOAT)(rand()%100)/100.0f;
		m_PosX = (FLOAT)(rand()%50) + fraction;

		// вырабатываем координату положения по оси X
		fraction    = (FLOAT)(rand()%100)/100.0f;
		m_PosY = (FLOAT)(rand()%50) + fraction;

		// вырабатываем координату начального положения по оси Z
		m_Trans = ((FLOAT)(rand()%100) + 100.0f); 
		
		// для повышения реалистичности
		// если звезда летит прямо на нас
		// сместим ее по другой координате от центра
		if( m_PosX < 1.5f && m_PosX >= -1.5f)  m_PosY += 2.0f;
		if( m_PosY < 1.5f && m_PosY >= -1.5f ) m_PosX += 2.0f;
		

		// по случайному алгоритму смещаем
		// звездочки и по отрицательным координатам
		if(rand()% 2 != 0) m_PosY *= -1;
		if(rand()% 2 != 0) m_PosX *= -1;

	}

	// в заданных позициях по оси Х и Y звездочка движется в нашу сторону
	D3DXMatrixTranslation(&matWorld, m_PosX, m_PosY, m_Trans);

	// эта строка нужна для тестирования и настройки вида звездочки 
	//D3DXMatrixTranslation(&matWorld, 0, 0, 0);
	pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// запускаем буфер вершин звездочки в поток устройства Direct3D
	hr = pd3dDevice->SetStreamSource( 0, m_pVBStar, 0, sizeof(CUSTOMVERTEXSTAR) );
	if(hr == S_OK)
	{
		// устанавливаем формат вершин примитивов из которых состоит звездочка,
		// затем происходит рисование 4-х треугольников
		hr = pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEXSTAR );
		if(hr == S_OK)
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 4 );
	}

	return hr;
}

// освобождение памяти занимаемой буфером вершин звездочки
HRESULT CStar::ReleaseStar(void)
{
	RELEASE(m_pVBStar);

	return S_OK;
}
