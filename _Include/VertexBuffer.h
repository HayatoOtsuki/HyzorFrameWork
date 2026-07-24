/**===========================================================================

 File   : 頂点バッファ [VertexBuffer.h]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef VERTEX_BUFFER_H

#include <d3d12.h>
#include <wrl/client.h>
#include <cstddef>

// 頂点バッファ（アップロードヒープ）を管理するクラス
class VertexBuffer {
public:
	// data : 頂点配列, size : 全体バイト数, stride : １頂点のバイト数
	bool Initialize(ID3D12Device* device, const void* data, size_t size, size_t stride);

	const D3D12_VERTEX_BUFFER_VIEW& GetView() const { return m_view; }

private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> m_resource;
	D3D12_VERTEX_BUFFER_VIEW m_view {};
};

#endif // !VERTEX_BUFFER_H
