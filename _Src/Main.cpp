/**===========================================================================

 File   : メイン関数 [Main.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "WindowApp.h"
#include "GraphicsDevice.h"

// ウィンドウ設定
namespace {
	constexpr uint32_t WINDOW_WIDTH = 1280;
	constexpr uint32_t WINDOW_HEIGHT = 720;
	constexpr const wchar_t* WINDOW_TITLE = L"Hyzor FrameWork";
}

// SubSystem=Windows なのでエントリはwWinMain（Unicode版）
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	WindowApp app; // ウィンドウアプリケーションのインスタンスを生成する
	GraphicsDevice graphics; // グラフィックスデバイスのインスタンスを生成する

    // 初期化
    if (!app.Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)) { return -1; }
    if (!graphics.Initialize()) { return 1; }


    // メインループ：閉じられるまで回り続ける
    while (app.ProcessMessage()) {
        // ここに後で「描画処理」を書いていく
    }

    return 0;
}
