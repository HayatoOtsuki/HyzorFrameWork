/**===========================================================================

 File   : パイプラインステート [PipelineState.h]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "PipelineState.h"
#include "Shader.h"
#include "RootSignature.h"

bool PipelineState::Initialize(ID3D12Device* device, RootSignature* rootSignature, Shader& vertexShader, Shader& pixelShader) {
	// 頂点１個のメモリ配置をGPUに教える
	D3D12_INPUT_ELEMENT_DESC inputElements[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc {};
    desc.pRootSignature = rootSignature->Get();
    desc.VS = vertexShader.GetByteCode();
    desc.PS = pixelShader.GetByteCode();
    desc.InputLayout = {inputElements, _countof(inputElements)};

    // ラスタライザ（塗りつぶし・背面カリング）
    desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    desc.RasterizerState.FrontCounterClockwise = FALSE;
    desc.RasterizerState.DepthClipEnable = TRUE;

    // ブレンド（不透明・全チャンネル書き込み）
    desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // 深度・ステンシル
    desc.DepthStencilState.DepthEnable = FALSE;
    desc.DepthStencilState.StencilEnable = FALSE;

    desc.SampleMask = UINT_MAX;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // スワップチェインと一致させる
    desc.SampleDesc.Count = 1;

    HRESULT hr = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipelineState));
    return SUCCEEDED(hr);
}
