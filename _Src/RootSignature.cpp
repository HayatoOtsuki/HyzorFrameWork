/**===========================================================================

 File   : ルートシグネチャ [RootSignature.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "RootSignature.h"

#include <Windows.h>
#include <d3dcommon.h>

bool RootSignature::Initialize(ID3D12Device* device) {
    // 空のルートシグネチャ設定（パラメータもサンプラも無し）
    D3D12_ROOT_SIGNATURE_DESC desc {};
    desc.NumParameters = 0;
    desc.pParameters = nullptr;
    desc.NumStaticSamplers = 0;
    desc.pStaticSamplers = nullptr;
    // 頂点入力レイアウト(IA)を使う許可
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // シリアライズ（バイナリ化）してから生成する2段構え
    ComPtr<ID3DBlob> blob;
    ComPtr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeRootSignature(
        &desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error);
    if (FAILED(hr)) {
        if (error != nullptr) {
            OutputDebugStringA(static_cast<const char*>(error->GetBufferPointer()));
        }
        return false;
    }

    hr = device->CreateRootSignature(
        0,
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        IID_PPV_ARGS(&m_rootSignature));
    return SUCCEEDED(hr);
}
