/**===========================================================================

 File   : グラフィックス管理 [GraphicsDevice.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "GraphicsDevice.h"

 // ライブラリリンク指定
#pragma comment(lib, "d3d12.lib") // D3D12ライブラリをリンクする
#pragma comment(lib, "dxgi.lib") // DXGIとは、DirectX Graphics Infrastructureの略で、グラフィックスデバイスやスワップチェーンなどを管理するためのライブラリ

bool GraphicsDevice::Initialize() {
	// 順番が重量：デバッグレイヤーはデバイス生成前に有効化する必要がある
	EnableDebugLayer(); // デバッグレイヤーを有効化する

	if (!CreateFactory()) { return false; } // DXGIファクトリを生成する
	if (!SelectAdapter()) { return false; } // アダプタを選択する
	if (!CreateDevice()) { return false; } // D3D12デバイスを

	return true;
}

void GraphicsDevice::EnableDebugLayer() {
	// Debugビルドのときだけ、GPUの御用を検出するレイヤーを有効化する
	// これがないとDX12エラーの理由をほとんど教えてくれない
#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif
}

bool GraphicsDevice::CreateFactory() {
	UINT flags = 0;
#if defined(_DEBUG)
	// Debug時はDXGI側のデバッグ情報も有効にする
	flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	// DXGIファクトリを生成する（アダプタ列挙などの入り口）
	HRESULT hr = CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory));
	return SUCCEEDED(hr);
}

bool GraphicsDevice::SelectAdapter() {
	// 高性能GPUを優先して順に列挙し、D3D12で仕えるものを選ぶ
	ComPtr<IDXGIAdapter1> adapter;
	for (UINT i = 0;
		m_factory->EnumAdapterByGpuPreference(
		i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND;
		++i) {
		DXGI_ADAPTER_DESC1 desc {};
		adapter->GetDesc1(&desc);

		// ソフトウェア(WARP)アダプタは実GPUではないので飛ばす
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) { continue; }

		// このアダプタでデバイスを生成できるかを試す（第４引数がNullPtrで実際には作らない）
		if (SUCCEEDED(D3D12CreateDevice(
			adapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			__uuidof(ID3D12Device),
			nullptr))) {
			m_adapter = adapter; // 採用する
			return true;
		}
	}
	return false; // 仕えるGPUが見つからなかった
}

bool GraphicsDevice::CreateDevice() {
	// 高い順に試し、通った一番高いFeatureLevelでデバイスを作成
	const D3D_FEATURE_LEVEL LEVELS[] = {
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	for (D3D_FEATURE_LEVEL level : LEVELS) {
		if (SUCCEEDED(D3D12CreateDevice(
			m_adapter.Get(),
			level,
			IID_PPV_ARGS(&m_device)))) {
			return true; // 生成成功
		}
	}
	return false; // どのレベルでも作れなかった
}
