#pragma once
#include "Init3DDirectX.h"


struct CUSTOMVERTEXSTAR
{
    FLOAT x, y, z; // локальная позиция вершины
    DWORD color;   // цвет вершины
};

// определяем формат вершин в соотствии с нашими потребностями
#define D3DFVF_CUSTOMVERTEXSTAR (D3DFVF_XYZ|D3DFVF_DIFFUSE)



// класс звездочки - создание, закраска, рендеринг звезды, освобождение памяти
class CStar
{
public:
	CStar(void);
	~CStar(void);
	HRESULT InitStar(LPDIRECT3DDEVICE9 pd3dDevice);
	LPDIRECT3DVERTEXBUFFER9 m_pVBStar;	// буфер вершин
	HRESULT Render(LPDIRECT3DDEVICE9 pd3dDevice, BOOL active); // рендеринг звездочки
	HRESULT ReleaseStar(void);

private:
	FLOAT m_PosX;	// позиция звездочки по оси Х
	FLOAT m_PosY;	// позиция звездочки по оси Y
	FLOAT m_Trans;	// перемещение вдоль оси Z 
	
};
