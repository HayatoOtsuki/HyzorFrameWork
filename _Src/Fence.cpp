/**===========================================================================

 File   : フェンス [Fence.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "Fence.h"

bool Fence::Initialize(ID3D12Device* device) {
	// ===========================
	// フェンスを生成する
	// ===========================
	HRESULT hr = device->CreateFence( 0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)); // 生成されたフェンスを受け取る
	if (FAILED(hr)) { return false; }

	// ===========================
	// 待機用イベントを生成する
	// ===========================
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr); // 自動リセットイベント
	if (m_fenceEvent == nullptr) { return false; }

	return true;
}

void Fence::Signal(ID3D12CommandQueue* queue) {
	// ===========================
	// GPUにシグナルを送る
	// ===========================
	++m_fenceValue; // 目印の値を一つ進める
	// GPUがここまで到達したらフェンスにm_fenceValueを書けとキューへ命令する
	queue->Signal(m_fence.Get(), m_fenceValue);
}

void Fence::Wait(){
	// GPUが目印に達していなければ待つ
	if (m_fence->GetCompletedValue() < m_fenceValue) {
		// 目印に達したらイベントを発火する設定
		m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
		// 発火までCPUを眠らせて待つ
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}
