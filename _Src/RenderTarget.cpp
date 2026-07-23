/**===========================================================================

 File   : レンダーターゲット [RenderTarget.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "RenderTarget.h"
#include "SwapChain.h"

bool RenderTarget::Initialize(ID3D12Device* device, SwapChain* swapChain) {
	// ==========================
	// レンダーターゲットビューのヒープを生成する
	// ==========================
	D3D12_DESCRIPTOR_HEAP_DESC desc {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー用のヒープ
	desc.NumDescriptors = SwapChain::BACK_BUFFER_COUNT; // バックバッファの数分のディスクリプタを確保する
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // GPUからアクセスしないのでフラグはなし
	desc.NodeMask = 0; // マルチGPUを使わないので0

	// ヒープを生成する
	HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
	if (FAILED(hr)) { return false; }

	// ディスクリプタ１個のバイトサイズを取得する
	m_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// =================
	// バックバッファのRTVを作成する
	// =================
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart(); // ヒープの先頭アドレスを取得する
	
	for (uint32_t i = 0; i < SwapChain::BACK_BUFFER_COUNT; ++i) {
		// 第２引数はnullptr = リソース本来のフォーマットで作る
		device->CreateRenderTargetView(swapChain->GetBackBuffer(i), nullptr, handle);

		// 次のディスクリプタ位置へ、サイズ分進める
		handle.ptr += m_rtvDescriptorSize; // 次のディスクリプタのアドレスに進める
	}
	return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetRtvHandle(uint32_t index) const {
	// 先頭から、index個分だけ進んだ位置のハンドルを返す
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += static_cast<SIZE_T>(m_rtvDescriptorSize) * index;
	return handle;
}
