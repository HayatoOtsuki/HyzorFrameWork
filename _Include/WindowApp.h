/**===========================================================================

 File   : ウィンドウアプリケーション [WindowApp.h]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef WINDOWAPP_H

#include <Windows.h>
#include <cstdint> // uint32_tなどの固定長整数型を使うために必要

 // ウィンドウアプリケーションの基本クラス
class WindowApp {
public:
	WindowApp() {};
	~WindowApp() {
		// 登録したウィンドウクラスを後始末する（省略してもよいが、リソースリークを避けるために行う）
		if (m_hInstance != nullptr) { UnregisterClassW(WINDOW_CLASS_NAME, m_hInstance); }
	};
	
	// ウィンドウを生成する
	bool Initialize(uint32_t width, uint32_t height, const wchar_t* title);

	// メッセージを１フレーム分処理する
	bool ProcessMessage();

	HWND GetHwnd() const { return m_hWnd; }
	HINSTANCE GetHinstance() const { return m_hInstance; }
	uint32_t GetWidth() const { return m_width; }
	uint32_t GetHeight() const { return m_height; }

private:
	// OSから呼ばれるコールバック
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msd, WPARAM wParam, LPARAM lParam);

	HWND m_hWnd = nullptr; // ウィンドウハンドル
	HINSTANCE m_hInstance = nullptr; // アプリケーションインスタンスハンドル
	uint32_t m_width = 0; // ウィンドウの幅
	uint32_t m_height = 0; // ウィンドウの高さ

	// ウィンドウクラス名
	static constexpr const wchar_t* WINDOW_CLASS_NAME = L"HyzorWindowClass";
};

#endif // !WINDOWAPP_H
