/**===========================================================================

 File   : スワップチェーン [SwapChain.h]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef SWAP_CHAIN_H

#include <Windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>
#include <cstdint> // uint32_tなどの固定長整数型を使うために必要

// スワップチェーンを生成・保持するクラス
class SwapChain {
public:
	static constexpr uint32_t BACK_BUFFER_COUNT = 2; // バックバッファの数（ダブルバッファリング）

	SwapChain() {};
	~SwapChain() {};

	bool Initialize(
		IDXGIFactory6* factory, // DXGIファクトリ : DXGI 1.6のファクトリインターフェース
		ID3D12CommandQueue* queue, // コマンドキュー
		HWND hWnd, // ウィンドウハンドル
		uint32_t width, uint32_t height); // ウィンドウの幅と高さ

	void Present(); // バックバッファを前面に表示する

	// 今描画してよいバックバッファの取得
	uint32_t GetCurrentBackBufferIndex() const;

	// 指定番号のバックバッファの取得
	ID3D12Resource* GetBackBuffer(uint32_t index) const { return m_backBuffers[index].Get(); }

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>; // ComPtr<T>の別名

	ComPtr<IDXGISwapChain4> m_swapChain; // スワップチェーン
	ComPtr<ID3D12Resource> m_backBuffers[BACK_BUFFER_COUNT]; // バックバッファ
};

#endif // !SWAP_CHAIN_H
