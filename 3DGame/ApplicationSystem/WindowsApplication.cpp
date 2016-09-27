#include "WindowsApplication.h"
#include "ApplicationData.h"

/**
* @brief コンストラクタ
*/
WindowsApplication::WindowsApplication()
{
	m_hWnd = NULL;
	ZeroMemory(&m_msg, sizeof(m_msg));

	// メモリーリーク検出
#if defined(DEBUG) | defined(_DEBUG)

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif
}

/**
* @brief デストラクタ
*/
WindowsApplication::~WindowsApplication()
{
	CloseDebugWindow();
}

/**
* @brief 作成
*/
HRESULT WindowsApplication::Create(HINSTANCE hInst)
{
	// 2重起動防止
	HANDLE hMutex = CreateMutex(NULL, TRUE, AppIns->GetApplicationName());
	if (hMutex == NULL) {
		return FALSE;
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, "既に起動されています。", "error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// アプリケーション名をセット
	AppIns->SetApplicationName(APPLICATION_NAME);

	// ウィンドウクラスの登録
	if (!RegisterWindowClass(hInst, WndProc)) return E_FAIL;

	// ウィンドウの初期化
	if (!WindowInitialize(hInst)) return E_FAIL;

	// 成功
	return S_OK;
}

/**
* @brief ウィンドウの初期化（ 作成 ）
*/
BOOL WindowsApplication::WindowInitialize(HINSTANCE hInst)
{
	// フルスクリーンモードかウィンドウモードかを選択
	bool isWindowed = true;
	if (MessageBox(0, "フルスクリーンモードで起動しますか？", "確認", MB_YESNO) == IDYES) {
		isWindowed = false;
	}

	RECT rcWindow;
	rcWindow.left = 0;
	rcWindow.top = 0;
	rcWindow.right = 1280;
	rcWindow.bottom = 720;

	// クライアント領域からウィンドウ領域を計算
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME, FALSE);

	// ウィンドウを作成する（中央に）
	m_hWnd = CreateWindow(AppIns->GetApplicationName(),
		AppIns->GetApplicationName(),
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME,
		GetSystemMetrics(SM_CXSCREEN) / 2 - (rcWindow.right - rcWindow.left) / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - (rcWindow.bottom - rcWindow.top) / 2,
		rcWindow.right - rcWindow.left, 
		rcWindow.bottom - rcWindow.top, NULL, NULL, hInst, NULL);
	if (!m_hWnd){
		MessageBox(0, "CreateWindowに失敗しました。", "WindowInitializeError!", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// ウィンドウの表示状態を設定
	ShowWindow(m_hWnd, SW_SHOW);

	// ウィンドウの更新
	UpdateWindow(m_hWnd);

	// ウィンドウハンドルをセット
	AppIns->SetWindowHandle(m_hWnd);
	// ウィンドウモードをセット
	AppIns->SetWindowed(isWindowed);
	// ウィンドウサイズをセット
	AppIns->SetWindowSize(1280, 720);

	return TRUE;
}

/**
* @brief ウィンドウクラスへ登録
* @param hInst   ： インスタンス
* @param WndProc ： ウィンドウプロシージャ
*/
ATOM WindowsApplication::RegisterWindowClass(HINSTANCE hInst, WNDPROC WndProc)
{
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = AppIns->GetApplicationName();
	wndclass.hIconSm = LoadIcon(NULL, IDI_ASTERISK);
	
	return RegisterClassEx(&wndclass);
}

/**
* @brief ウィンドウプロシージャ
*/
LRESULT CALLBACK WindowsApplication::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam){
//		case VK_ESCAPE: PostMessage(hWnd, WM_CLOSE, 0, 0); return 0;
		case VK_F1:		OpenDebugWindow(); return 0;
		}
		break;

		// マウスカーソルセット
	case WM_SETCURSOR:
		if (!AppIns->GetWindowed()){
			SetCursor(NULL);
		}
		return TRUE;
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

/**
* @brief ウィンドウからのメッセージ処理
*/
BOOL WindowsApplication::ProcessEvent()
{
	if (PeekMessage(&m_msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		// ウィンドウプロシージャへメッセージを送出
		DispatchMessage(&m_msg);
	}
	if (m_msg.message != WM_QUIT){
		return FALSE;
	}
	return TRUE;
}


// デバッグウィンドウ用
static FILE* DebugFP = NULL;

/**
* @brief デバッグ用ウィンドウを開く
*/
void WindowsApplication::OpenDebugWindow()
{
#ifdef _DEBUG
	AllocConsole();
	freopen_s(&DebugFP, "CONOUT$", "w", stdout);
	freopen_s(&DebugFP, "CONIN$", "r", stdin);
#endif
}

/**
* @brief デバッグ用ウィンドウを閉じる
*/
void WindowsApplication::CloseDebugWindow()
{
#ifdef _DEBUG
	if (DebugFP)
	{
		fclose(DebugFP);
		FreeConsole();
	}
#endif
}