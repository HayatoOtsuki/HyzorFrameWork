/**===========================================================================

 File   : スワップチェーン [SwapChain.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "SwapChain.h"

bool SwapChain::Initialize( IDXGIFactory6* factory, ID3D12CommandQueue* queue,
	HWND hWnd, uint32_t width, uint32_t height) {
	// ===========================
	// スワップチェーンの設定
	// ===========================
	DXGI_SWAP_CHAIN_DESC1 desc {};
	desc.Width = width; // バックバッファの幅
	desc.Height = height; // バックバッファの高さ
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // バックバッファのフォーマット
	desc.Stereo = FALSE; // 立体視は使わない
	desc.SampleDesc.Count = 1; // マルチサンプリングは使わない
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // レンダーターゲットとして使う
	desc.BufferCount = BACK_BUFFER_COUNT; // バックバッファの数（ダブルバッファリング）
	desc.Scaling = DXGI_SCALING_STRETCH; // ウィンドウサイズに合わせて伸縮する
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップモデルを使う（DirectX12では必須）
	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED; // アルファブレンドは使わない
	desc.Flags = 0;

	// ==========================
	// スワップチェーンを生成する
	// ==========================
	ComPtr<IDXGISwapChain1> swapChain1;
	HRESULT hr = factory->CreateSwapChainForHwnd(
		queue, // コマンドキューを指定する（レンダーターゲットの表示に使う）
		hWnd, // ウィンドウハンドルを指定する
		&desc, // スワップチェーンの設定を指定する
		nullptr, // フルスクリーンモードの設定は省略する（今回はウィンドウモードで使う）
		nullptr, // 制限なし（今回はウィンドウモードで使う）
		&swapChain1); // 生成されたスワップチェーンを受け取る
	if (FAILED(hr)) { return false; }

	// ==========================
	// 新しいインターフェースに変換する
	// ==========================
	swapChain1.As(&m_swapChain);
	
	// ==========================
	// バックバッファを取得する
	// ==========================
	for (uint32_t i = 0; i < BACK_BUFFER_COUNT; ++i) {
		hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));
		if (FAILED(hr)) { return false; }
	}
	return true;
}

void SwapChain::Present() {
	// バックバッファを前面に表示する
	m_swapChain->Present(1, 0); // 1: 垂直同期あり、0: フラグなし
}

uint32_t SwapChain::GetCurrentBackBufferIndex() const {
	// 今描画してよいバックバッファの番号を取得する
	return m_swapChain->GetCurrentBackBufferIndex();
}
