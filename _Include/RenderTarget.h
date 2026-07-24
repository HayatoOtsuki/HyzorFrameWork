/**===========================================================================

 File   : レンダーターゲット [RenderTarget.h]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef RENDER_TARGET_H

#include <d3d12.h>
#include <wrl/client.h>
#include <cstdint> // uint32_tなどの固定長整数型を使うために必要

class SwapChain; // 前方宣言

class RenderTarget {
public:
	RenderTarget() {};
	~RenderTarget() {};

	bool Initialize(ID3D12Device* device, SwapChain* swapChain);

	// 指定番号のバックバッファのRTV
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandle(uint32_t index) const;

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>; // ComPtr<T>の別名

	ComPtr<ID3D12DescriptorHeap> m_rtvHeap; // レンダーターゲットビューのヒープ
	uint32_t m_rtvDescriptorSize = 0; // ディスクリプタ１個のバイトサイズ
};

#endif // !RENDER_TARGET_H
