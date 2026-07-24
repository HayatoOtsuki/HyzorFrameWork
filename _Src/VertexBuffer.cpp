/**==========================================================================

 File   : 頂点バッファ [VertexBuffer.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "VertexBuffer.h"
#include <cstring>

bool VertexBuffer::Initialize(ID3D12Device* device, const void* data, size_t size, size_t stride) {
	// ====================
	// アップロードヒープ（CPUから書ける・GPUから読める）の設定
	// ====================
	D3D12_HEAP_PROPERTIES heapProps {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	// ====================
	// バッファリソースの設定
	// ====================
	D3D12_RESOURCE_DESC desc {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Width = size; // バッファは幅=バイト数
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN; // バッファはUNKNOWN
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR; // バッファは必ずこれ
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	// ====================
	// リソース生成
	// ====================
	HRESULT hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // アップロードヒープの初期状態はこれ固定
		nullptr,
		IID_PPV_ARGS(&m_resource));
	if (FAILED(hr)) { return false; }

	// ====================
	// CPUからマップしてデータをコピー
	// ====================
	void* mapped = nullptr;
	D3D12_RANGE readRange {0, 0};   // CPUでは読まない（書くだけ）
	hr = m_resource->Map(0, &readRange, &mapped);
	if (FAILED(hr)) { return false; }
	std::memcpy(mapped, data, size);
	m_resource->Unmap(0, nullptr);

	// ====================
	// 頂点バッファビューを作る
	// ====================
	m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
	m_view.SizeInBytes = static_cast<UINT>(size);
	m_view.StrideInBytes = static_cast<UINT>(stride);

	return true;
}
