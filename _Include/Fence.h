/**===========================================================================

 File   : フェンス [Fence.h]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef FENCE_H

#include <Windows.h>
#include <d3d12.h>
#include <wrl/client.h>
#include <cstdint> // uint32_tなどの固定長整数型を使うために必要

 // GPUとCPUの同期を行うフェンスを生成・保持するクラス
class Fence {
public:
	Fence() {};
	~Fence() {
		// 作ったイベントは自分で閉じる（ComPtrでは管理されない生ハンドル）
		if (m_fenceEvent != nullptr) { CloseHandle(m_fenceEvent); }
	};

	bool Initialize(ID3D12Device* device);

	void Signal(ID3D12CommandQueue* queue); // GPUにシグナルを送る
	void Wait(); // CPUがGPUの処理完了を待つ

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>; // ComPtr<T>の別名

	ComPtr<ID3D12Fence> m_fence; // フェンス
	HANDLE m_fenceEvent = nullptr; // 待機用イベント
	uint64_t m_fenceValue = 0; // 目印のカウンタ
};

#endif // !FENCE_H
