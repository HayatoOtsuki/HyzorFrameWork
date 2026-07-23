/**===========================================================================

 File   : コマンドコンテキスト [CommandContext.h]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef COMMAND_CONTEXT_H

#include <d3d12.h>
#include <wrl/client.h>

// コマンドキュー・アロケータ・リストをまとめて管理するクラス
class CommandContext {
public:
	CommandContext() {};
	~CommandContext() {};

	bool Initialize(ID3D12Device* device);

	ID3D12CommandQueue* GetQueue() const { return m_queue.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return m_commandList.Get(); }

	void Begin(); // 記録開始（アロケータとリストをReset）
	void End(); // 記録終了（リストをClose）
	void Execute(); // キューに送って実行

private:
	bool CreateQueue(ID3D12Device* device);
	bool CreateAllocator(ID3D12Device* device);
	bool CreateCommandList(ID3D12Device* device);

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12CommandQueue> m_queue;
	ComPtr<ID3D12CommandAllocator> m_allocator;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};

#endif // !COMMAND_CONTEXT_H
