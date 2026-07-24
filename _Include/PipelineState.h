/**===========================================================================

 File   : パイプラインステート [PipelineState.h]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef PIPLINE_STATE_H

#include <d3d12.h>
#include <wrl/client.h>

class Shader;
class RootSignature;

// 描画設定を固めたパイプラインステート
class PipelineState {
public:
	bool Initialize(ID3D12Device* device, RootSignature* rootSignature, Shader& vertexShader, Shader& pixelShader);

	ID3D12PipelineState* Get() const { return m_pipelineState.Get(); }

private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12PipelineState> m_pipelineState;
};

#endif // !PIPLINE_STATE_H
