////////////////////////////////////////////////////////////////////////////////////
//
//			www.interestprograms.ru - программы, игры и их исходные коды
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
		// регистрируем класс с нулевой кистью, для исключения ее перерисовки и мерцания окна-рендеринга
		::LoadCursor(NULL, IDC_ARROW), NULL /*reinterpret_cast<HBRUSH>(COLOR_WINDOW+1)*/, NULL);

	return TRUE;
	
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	CreateDevice3D();
	// можно изменять количество звезд в космосе
	m_pCosmos = new CCosmos(1500);
	m_pCosmos->Init(m_pd3dDevice);
	
	return 0;
}


void CChildView::OnPaint() 
{
	// если фрейм приложения не активен (переключение на другое приложение,
	// активирование меню и др.) запрещаем принудительную 
	// перерисовку окна, остается только поддержка оспорченного изображения
	if(m_Active == false)
		CPaintDC dc(this); // device context for painting
	
	// размещать функцию рендеринга можно OnPaint
	// или в функции OnIdle приложения
	// размещая в OnPaint нет необходимости перерисовывать рабочее окно 
	// приложения во время открытия какого-либо диалогового окна
	// т.к. в этом случае изображение в окне не испортится
	Render();
	
	
	// без обработок по умолчанию события OnPaint, 
	// обработчик вызывается постоянно, не дожидаясь получения сообщения WM_PAINT
	//CWnd::OnPaint();
}

// инициализация среды Direct3D
HRESULT CChildView::InitD3D(void)
{

	HRESULT hr = S_OK;

	// параметр D3D_SDK_VERSION - предоставляет выбор необходимой версии
	// DirectX самому Direct3D
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D ==  NULL)
	{
		AfxMessageBox("Ошибка - Direct3D не создан!");
		return E_FAIL;
	}

	return hr;

}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ResetScene();

}


// настройка матриц - 
// мировой для размещения объектов рендеринга в заданых координатах данного 3D мира,
// матрицы вида для размещения камеры(взгляда на данный 3D мир) в заданной позиции,
// матрицы перспективы для проекции данного 3D мира на плоскость монитора
HRESULT CChildView::SetupMatrices()
{
	HRESULT hr = S_OK;
    D3DXMATRIXA16 matWorld;
	// создаем нулевую матрицу
	D3DXMatrixIdentity(&matWorld);

    hr = m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	if(hr != S_OK) return hr;

    // координата нахождения нашей камеры
    D3DXVECTOR3 vEyePt( 0.0f, 0.0f, -5.0f );

	// координаты объекта на который мы смотрим
    D3DXVECTOR3 vLookatObject( 0.0f, 0.0f, 0.0f );

	// определение точки верха камеры
    D3DXVECTOR3 vUpCamera( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatObject, &vUpCamera );
    hr = m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	if(hr != S_OK) return hr;

    D3DXMATRIXA16 matProj;
	FLOAT aspect = m_Aspect;
    D3DXMatrixPerspectiveFovLH( &matProj, 
								D3DX_PI/4,	// 
								aspect,		// форматное соотношение сторон(отношение ширины к высоте экрана)
								1.0f,		// расстояние до ближней плоскости отсечения
								400.0f );	// расстояние до дальней плоскости отсечения
    hr = m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	return hr;
}



// в данной функции просто создаем устройство с совместимыми
// с дисплем форматами
// дополнительные настройки устройства Direct3D производится
// при сбросе/восстановлении устройства в функции ResetDevice3D
// срабатывающее при различных событиях( изменение размера окна рендеринга, 
// изменение пользователем компьютера цветовую глубину пикселя монитора и др.) 
HRESULT CChildView::CreateDevice3D(void)
{
	HRESULT hr = S_OK;
	
	D3DPRESENT_PARAMETERS d3dpp;
	hr = GetD3DPresentParameters(&d3dpp);

	// всегда проверяем результат возврата на корректность
	if(hr != S_OK)
	{
		AfxMessageBox("Ошибка - некорректные параметры представления сцены Direct3D");
		return hr;
	}

	// 
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
														&d3dpp, &m_pd3dDevice);

	if(hr != S_OK)
	{
		AfxMessageBox("Ошибка - устройство Direct3D не создано!");
		return hr;
	}


	return hr;
}


// сброс и восстановление устройства Direct3D при его потере.
// это может случиться, например, при изменении цветовой глубины монитора
// во время работы приложения, или для принудительно сброса
// устройства при изменении размеров окна для сохранения геометрических
// пропорций объекта рендеринга
HRESULT CChildView::ResetDevice3D(void)
{
	HRESULT hr = S_OK;
	D3DPRESENT_PARAMETERS d3dpp; 
	hr = GetD3DPresentParameters(&d3dpp);

	// после каждого решающего шага для настройки Direct3D
	// обязательно производим проверку на корректность возврат 
	// выполнения функций Direct3D
	if(hr != S_OK)
	{
		AfxMessageBox("Ошибка - некорректные параметры представления сцены Direct3D");
		return hr;
	}


	// сброс текущих настроек устройства Direct3D
	// и восстановление устройства с новыми параметрами представления 
	// 3D сцены
	hr = m_pd3dDevice->Reset(&d3dpp);
	
		
	if(hr != S_OK)
	{
		//AfxMessageBox("Ошибка - не удалось сбросить/восстановить устройство Direct3D");
		return hr;
	}
	
	// после восстановления устройства обязательно передадим в
	// функцию SetupMatrices для корректировки матрицы перспективы в
	// D3DXMatrixPerspectiveFovLH с целью сохранения 
	// геометрических пропорций объектов рендеринга,
	// например, куб должен оставаться кубом при любых
	// изменениях размеров окна-рендеринга
	m_Aspect = (float)d3dpp.BackBufferWidth/d3dpp.BackBufferHeight;


	// определим что примитив будем видеть с обеих сторон,
	// т.е. с внутренней и внешней
	// но в случае рендеринга объемной фигуры
	// этот режим мягко выражаясь вреден
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// в данном примере свет нам не нужен, отключаем его,
	// иначе наши примитивы будут черными и 
	// понадобится создавать "источники света"
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// разрешаем глубинную буферизацию для усиления реалистичности
	// рендеринга
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	return hr;
}

// получаем корректные параметры представления сцены
// необходимые для создания и сброса/восстановления устройства Direct3D
HRESULT CChildView::GetD3DPresentParameters(D3DPRESENT_PARAMETERS* d3dpp)
{
	HRESULT hr = S_OK;

	// очищаем структуру от случайных данных
	ZeroMemory( d3dpp, sizeof(D3DPRESENT_PARAMETERS) );

	// устанавливаем режим работы приложения (оконный или полноэкранный)
	d3dpp->Windowed = !m_FullScreen;
		

	D3DDISPLAYMODE display_mode;
	// очищаем структуру от случайных данных
	ZeroMemory( &display_mode, sizeof(D3DDISPLAYMODE) );

	// получаем текущий режим работы дисплея в котором работает первичный адаптер,
	// если дисплей работает в этом режиме можно свободно использовать
	// эти настройки для устройства в любых режимах
	hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &display_mode);

	if(hr != S_OK)
	{
		AfxMessageBox("Ошибка - невозможно получить режим дисплея!");
		return hr;
	}

	// если режим работы приложения полноэкранный 
	// устанавливаем соответствующие настройки
	// для полноэкранного режима мы вольны устанавливать любые настройки
	// по своему усмотрению в пределах разрешенного Direct3D9
	if(m_FullScreen == TRUE)
	{
		d3dpp->hDeviceWindow = AfxGetMainWnd()->m_hWnd;

		// оставим текущее разрешение дисплея  
		d3dpp->BackBufferWidth = display_mode.Width;	
		d3dpp->BackBufferHeight = display_mode.Height;
		// оставим частоту восстановления дисплея также текущую
		d3dpp->FullScreen_RefreshRateInHz = display_mode.RefreshRate;
		d3dpp->PresentationInterval =  D3DPRESENT_INTERVAL_DEFAULT;
	}

	// допустимые форматы глубинного шаблона
	// для удобства перебора форматов для их временного хранения 
	// применим массив
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
		// перебираем и определяем допустимый формат глубинных шаблонов 
		// совместимый с тем что будет представлен в рендеринге
		if( (hr = m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			display_mode.Format, display_mode.Format, depth_stencil[index])) == S_OK)
		{
			break;
		}
	}
		

	// формат заднего буфера сделаем равным фрмату первичного буфера
	d3dpp->BackBufferFormat = display_mode.Format;

	// предоставим управление глубинными буферами самому Direct3D
	d3dpp->EnableAutoDepthStencil = TRUE;
	// для глубинных буфера установим ранее полученный совместимый формат
	d3dpp->AutoDepthStencilFormat = depth_stencil[index];

	// используем рекомендуемый эффект обмена глубинными буферами (см. SDK DirectX C++)
	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;


	return hr;
}


HRESULT CChildView::Render(void)
{
	HRESULT hr = S_OK;

	if(m_pd3dDevice == NULL) return E_FAIL;

	
	// очищаем все окно рендеринга и заполняем его желаемым цветом
	hr = m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	if(hr != S_OK) return hr;
	
	// начало подготовки будущей сцены
	hr = m_pd3dDevice->BeginScene();

	if(hr != S_OK) return hr;

	// необходимые действия для формирования сцены 
	hr = SetupMatrices();

	if(hr != S_OK) return hr;

	// рендеринг полета сквозь космос
	hr = m_pCosmos->Render(m_pd3dDevice, m_Active);

	if(hr != S_OK) return hr;

	// готовность сцены к показу
	hr = m_pd3dDevice->EndScene();

	if(hr != S_OK) return hr;

	// непосредственно визуальный показ сцены 
	hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	// если устройство потеряно, попытаемся его восстановить
	if(hr == D3DERR_DEVICELOST)
	{
		// как только устройство может быть восстановлено
		// сбрасываем его и восстанавливаем сцену 3D заново
		if(m_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ResetScene();
		}
	}

	return hr;
}


// примерное контекстное меню
void CChildView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// если полноэкранный режим запрещаем работу
	// контекстного меню, в полноэкранном режиме 
	// лучше применять меню средствами DirectX
	if(m_FullScreen == TRUE)
		return;

	CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
	CMenu* file_menu = menu_bar->GetSubMenu(0);    
	ASSERT(file_menu);

	file_menu->InsertMenu(0, MF_STRING|MF_BYPOSITION, 10, "элемент меню по желанию");

	file_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, 
		point.y, this);

	file_menu->DeleteMenu(10, MF_BYCOMMAND);
}


// сброс/восстановление сцены
HRESULT CChildView::ResetScene(void)
{
	HRESULT hr = S_OK;

	// освобождаем память занимаемую буферами звезд
	hr = m_pCosmos->ReleaseCosmos();
	if(hr != S_OK)
		return hr;
			
	// сбрасываем устройство
	hr = ResetDevice3D();
	if(hr != S_OK)
		return hr;

	// создаем заново звездное пространство
	hr = m_pCosmos->Init(m_pd3dDevice);

	return hr;
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	static bool full_screen = true;
	static WINDOWPLACEMENT m_WndPlacement;

	if(full_screen == true)
	{
		// запомним размеры и состояние окна до полноэкранного режима
		AfxGetMainWnd()->GetWindowPlacement(&m_WndPlacement);

		m_FullScreen = TRUE;

		// сброс сцену
		ResetScene();
	}
	else
	{
		m_FullScreen = FALSE;
		
		// сброс сцены
		ResetScene();

		// восстанавливаем прежнее состояние окна
		AfxGetMainWnd()->SetWindowPlacement(&m_WndPlacement);
	}

	full_screen = !full_screen;

	CWnd::OnLButtonDblClk(nFlags, point);
}
