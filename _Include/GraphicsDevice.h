/**===========================================================================

 File   : グラフィックス管理 [GraphicsDevice.h]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef GRAPHICS_H

#include <d3d12.h>
#include <dxgi1_6.h> // DXGI 1.6を使うために必要
#include <wrl/client.h> // Microsoft::WRL::ComPtrを使うために必要

// D3d12デバイスを生成・保持するクラス
class GraphicsDevice {
public:
	GraphicsDevice() {};
	~GraphicsDevice() {};

	// デバイスを生成する
	bool Initialize();

	ID3D12Device* GetDevice() const { return m_device.Get(); }
	IDXGIFactory6* GetFactory() const { return m_factory.Get(); } // IDXGIFactory6 : DXGI 1.6のファクトリインターフェース

private:
	void EnableDebugLayer(); // デバッグレイヤーを有効化する
	bool CreateFactory(); // DXGIファクトリを生成する
	bool SelectAdapter(); // アダプタを選択する
	bool CreateDevice(); // D3D12デバイスを生成する

	// 長い型名を書くための別名
	template<class T> // TはCOMインターフェースの型
	using ComPtr = Microsoft::WRL::ComPtr<T>; // ComPtr<T>の別名

	ComPtr<IDXGIFactory6> m_factory; // アダプタ列挙やスワップチェーン生成に使う
	ComPtr<IDXGIAdapter1> m_adapter; // 選択されたGPU
	ComPtr<ID3D12Device> m_device; // D3D12デバイス
};

#endif // !GRAPHICS_H
