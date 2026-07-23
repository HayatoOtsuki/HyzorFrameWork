/**===========================================================================

 File   : コマンドコンテキスト [CommandContext.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "CommandContext.h"

bool CommandContext::Initialize(ID3D12Device* device) {
	if (!CreateQueue(device)) { return false; }
	if (!CreateAllocator(device)) { return false; }
	if (!CreateCommandList(device)) { return false; }
	return true;
}

bool CommandContext::CreateQueue(ID3D12Device* device) {
	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC desc {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // 直接描画用のコマンドキュー
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0; // マルチGPUを使わないので0

	HRESULT hr = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue));
	return SUCCEEDED(hr);
}

bool CommandContext::CreateAllocator(ID3D12Device* device) {
	// アロケータ
	HRESULT hr = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, // 直接描画用のコマンドアロケータ
		IID_PPV_ARGS(&m_allocator)
	);
	return SUCCEEDED(hr);
}

bool CommandContext::CreateCommandList(ID3D12Device* device) {
	// コマンドリスト
	HRESULT hr = device->CreateCommandList(
		0, // ノードマスク（マルチGPUを使わないので0）
		D3D12_COMMAND_LIST_TYPE_DIRECT, // 直接描画用のコマンドリスト
		m_allocator.Get(),
		nullptr, // 初期化用のレンダーターゲット（今回は使用しない）
		IID_PPV_ARGS(&m_commandList)
	);

	if (FAILED(hr)) { return false; }

	// 生成直後はOpen状態なので、Closeしておく
	m_commandList->Close();
	return true;
}

void CommandContext::Begin() {
	// リストを書き込み可能状態にする
	m_allocator->Reset();
	m_commandList->Reset(m_allocator.Get(), nullptr);
}

void CommandContext::End() {
	// 記録完了、封をする（この後Executeできる）
	m_commandList->Close();
}

void CommandContext::Execute() {
	// コマンドリストをキューに積んで実行させる
	ID3D12CommandList* lists[] = {m_commandList.Get()};
	m_queue->ExecuteCommandLists(1, lists); // 引数の意味は「リストの数」と「リストの配列」
}
