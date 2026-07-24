/**===========================================================================

 File   : メイン関数 [Main.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/23
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "WindowApp.h"
#include "GraphicsDevice.h"
#include "CommandContext.h"
#include "SwapChain.h"
#include "RenderTarget.h"
#include "Fence.h"
#include "Shader.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "VertexBuffer.h"

#include <d3d12.h>

// ウィンドウ設定
namespace {
	constexpr uint32_t WINDOW_WIDTH = 1280;
	constexpr uint32_t WINDOW_HEIGHT = 720;
	constexpr const wchar_t* WINDOW_TITLE = L"Hyzor FrameWork";

	// 頂点の方（入力レイアウトと一致：position=先頭, color=12バイト目）
	struct Vertex {
		float position[3];
		float color[4];
	};

	// クリップ空間の三角形（時計回り=表面）
	constexpr Vertex TRIANGLE_VERTICES[] = {
		{{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 上：赤
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 右下：緑
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}} // 左下：青
	};
}

// SubSystem=Windows なのでエントリはwWinMain（Unicode版）
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
	// =========================
	// インスタンス生成
	// =========================
	WindowApp app {};
	GraphicsDevice graphics {};
	CommandContext commandContext {};
	SwapChain swapChain {};
	RenderTarget renderTarget {};
	Fence fence {};
	Shader vertexShader {};
	Shader pixelShader {};
	RootSignature rootSignature {};
	PipelineState pipelineState {};
	VertexBuffer vertexBuffer {};

	// =========================
    // 初期化
	// =========================
    if (!app.Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE)) { return -1; }
    if (!graphics.Initialize()) { return -1; }
	if (!commandContext.Initialize(graphics.GetDevice())) { return -1; }
	if (!swapChain.Initialize(graphics.GetFactory(), commandContext.GetQueue(), app.GetHwnd(), app.GetWidth(), app.GetHeight())) { return -1; }
	if (!renderTarget.Initialize(graphics.GetDevice(), &swapChain)) { return -1; }
	if (!fence.Initialize(graphics.GetDevice())) { return -1; }
	if (!vertexShader.CompileFromFile(L"_Shader/Unlit.hlsl", L"VSMain", L"vs_6_0")) { return -1; }
	if (!pixelShader.CompileFromFile(L"_Shader/Unlit.hlsl", L"PSMain", L"ps_6_0")) { return -1; }
	if (!rootSignature.Initialize(graphics.GetDevice())) { return -1; }
	if (!pipelineState.Initialize(graphics.GetDevice(), &rootSignature, vertexShader, pixelShader)) { return -1; }
	if (!vertexBuffer.Initialize(graphics.GetDevice(), TRIANGLE_VERTICES, sizeof(TRIANGLE_VERTICES), sizeof(Vertex))) { return -1; }

	// ========================
    // メインループ：閉じられるまで回り続ける
	// ========================
    while (app.ProcessMessage()) {
		// ========================
		// １．今から描くバッファの番号
		// ========================
		const uint32_t index = swapChain.GetCurrentBackBufferIndex();

		// ========================
		// ２．コマンドリストの記録開始
		// ========================
		commandContext.Begin();
		ID3D12GraphicsCommandList* list = commandContext.GetCommandList();

		// ========================
		// ３．バリア：PRESENT -> RENDER_TARGET
		// ========================
		D3D12_RESOURCE_BARRIER barrier {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION; // バリア
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; // フラグなし
		barrier.Transition.pResource = swapChain.GetBackBuffer(index); // バリアを適用するリソース
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES; // 全サブリソースに適用
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 以前の状態
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // これからの状態
		list->ResourceBarrier(1, &barrier); // バリアをコマンド

		// ========================
		// ４．RTVの設定
		// ========================
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = renderTarget.GetRtvHandle(index);
		list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		// ========================
		// ５．画面クリア
		// ========================
		const float CLEAR_COLOR[4] = {0.2f, 0.2f, 0.2f, 1.0f};
		list->ClearRenderTargetView(rtvHandle, CLEAR_COLOR, 0, nullptr);

		// ========================
		// 三角形描画
		// ========================
		{
			// --- ビューポート（描画する矩形領域）---
			D3D12_VIEWPORT viewport {};
			viewport.Width = static_cast<float>(app.GetWidth());
			viewport.Height = static_cast<float>(app.GetHeight());
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			list->RSSetViewports(1, &viewport);

			// --- シザー矩形（この矩形外は描かない）---
			D3D12_RECT scissor {};
			scissor.right = static_cast<LONG>(app.GetWidth());
			scissor.bottom = static_cast<LONG>(app.GetHeight());
			list->RSSetScissorRects(1, &scissor);

			// --- パイプラインをセット ---
			list->SetGraphicsRootSignature(rootSignature.Get());
			list->SetPipelineState(pipelineState.Get());
			list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// --- 頂点バッファをセット ---
			D3D12_VERTEX_BUFFER_VIEW vbView = vertexBuffer.GetView();
			list->IASetVertexBuffers(0, 1, &vbView);

			// --- 描画（頂点3個, インスタンス1個）---
			list->DrawInstanced(3, 1, 0, 0);
		}

		// ========================
		// ６．バリア：RENDER_TARGET -> PRESENT
		// ========================
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 以前の状態
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // これからの状態
		list->ResourceBarrier(1, &barrier); // バリアをコマンド

		// ========================
		// ７．記録終了
		// ========================
		commandContext.End();

		// ========================
		// ８．実行
		// ========================
		commandContext.Execute();
		
		// ========================
		// ９．表示
		// ========================
		swapChain.Present();

		// ========================
		// １０．GPUの完了を待つ
		// ========================
		fence.Signal(commandContext.GetQueue());
		fence.Wait();
    }

    return 0;
}
