/**===========================================================================

 File   : ウィンドウアプリケーション [WindowApp.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "WindowApp.h"

WindowApp::WindowApp() {
	// コンストラクタの処理
}

WindowApp::~WindowApp() {
	// 登録したウィンドウクラスを後始末する（省略してもよいが、リソースリークを避けるために行う）
	if (m_hInstance != nullptr) {
		UnregisterClassW(WINDOW_CLASS_NAME, m_hInstance);
	}
}

bool WindowApp::Initialize(uint32_t width, uint32_t height, const wchar_t* title) {
	m_width = width;
	m_height = height;

	// 自分のインスタンスハンドルを取得する（実行ファイル自身のハンドル）
	m_hInstance = GetModuleHandleW(nullptr);

	// ==============================
	// ウィンドウクラスを登録する
	// ==============================
	WNDCLASSEXW WindowClass {};
	WindowClass.cbSize = sizeof(WNDCLASSEX); // 構造体のサイズを指定する
	WindowClass.style = CS_HREDRAW | CS_VREDRAW; // ウィンドウのサイズが変わったときに再描画する
	WindowClass.lpfnWndProc = WindowProc; // ウィンドウプロシージャの関数ポインタを指定する
	WindowClass.hInstance = m_hInstance; // アプリケーションインスタンスハンドルを指定する
	WindowClass.lpszClassName = WINDOW_CLASS_NAME; // ウィンドウクラス名を指定する

	if (RegisterClassExW(&WindowClass) == 0) {
		// ウィンドウクラスの登録に失敗した場合
		return false;
	}

	// ==============================
	// 枠組みサイズを計算する
	// CreateWindowExWに渡すのは「枠込み」のサイズなので、
	// クライアント領域を width x height にしたいため枠分足す
	// ==============================
	DWORD style = WS_OVERLAPPEDWINDOW; // 標準的なウィンドウスタイル
	RECT rect =
	{0, 0, // 左上座標は0,0にする
		static_cast<LONG>(width), static_cast<LONG>(height)}; // 右下座標はwidth,heightにする
	AdjustWindowRect(&rect, style, FALSE); // 枠込みサイズに変換する

	const LONG windowWidth = rect.right - rect.left;
	const LONG windowHeight = rect.bottom - rect.top;

	// ==============================
	// ウィンドウを生成する
	// ==============================
	m_hWnd = CreateWindowExW(
		0, // 拡張スタイル
		WINDOW_CLASS_NAME, // ウィンドウクラス名
		title, // ウィンドウタイトル
		style, // ウィンドウスタイル
		CW_USEDEFAULT, CW_USEDEFAULT, // ウィンドウの左上座標
		windowWidth, windowHeight, // ウィンドウの幅と高さ
		nullptr, // 親ウィンドウのハンドル
		nullptr, // メニューハンドル
		m_hInstance, // アプリケーションインスタンスハンドル
		nullptr // 追加パラメータ
	);

	if (m_hWnd == nullptr) {
		// ウィンドウの生成に失敗した場合
		return false;
	}

	// ==============================
	// ウィンドウを表示する
	// ==============================
	ShowWindow(m_hWnd, SW_SHOW); // ウィンドウを表示する
	UpdateWindow(m_hWnd); // ウィンドウを更新する

	return true; // 初期化成功
}

bool WindowApp::ProcessMessage() {
	MSG msg {};
	
	// メッセージがあれば取り出す（無ければ素通り）
	while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false; // 終了要求
		}
		TranslateMessage(&msg); // キー入力メッセージへ変換
		DispatchMessageW(&msg); // WindowProcedureへ配送
	}

	return true; // 継続
}

LRESULT CALLBACK WindowApp::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY:
			// ウィンドウが破棄された -> メッセージループにWM_QUITを投げる
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
			// Escで閉じる
			if (wParam == VK_ESCAPE) {
				PostQuitMessage(0);
			}
			return 0;

		default:
			return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
}
