/**===========================================================================

 File   : シェーダーコンパイル [Shader.cpp]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "Shader.h"

#include <Windows.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

bool Shader::CompileFromFile(const wchar_t* path, const wchar_t* entryPoint, const wchar_t* target) {
    // DXCのユーティリティとコンパイラを生成
    ComPtr<IDxcUtils>     utils;
    ComPtr<IDxcCompiler3> compiler;
    if (FAILED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)))) { return false; }
    if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)))) { return false; }

    // #include "Common.hlsli" を解決するための標準ハンドラ
    ComPtr<IDxcIncludeHandler> includeHandler;
    utils->CreateDefaultIncludeHandler(&includeHandler);

    // hlslファイルをUTF-8として読み込む（日本語コメントもこれでOK）
    UINT32 codePage = DXC_CP_UTF8;
    ComPtr<IDxcBlobEncoding> sourceBlob;
    if (FAILED(utils->LoadFile(path, &codePage, &sourceBlob))) {
        OutputDebugStringA("Shader: hlslファイルを開けませんでした\n");
        return false;
    }

    DxcBuffer sourceBuffer {};
    sourceBuffer.Ptr = sourceBlob->GetBufferPointer();
    sourceBuffer.Size = sourceBlob->GetBufferSize();
    sourceBuffer.Encoding = DXC_CP_UTF8;

    // コンパイル引数（コマンドラインのdxc.exeと同じ書式）
    LPCWSTR args[] =
    {
        L"-E", entryPoint,   // エントリポイント関数名
        L"-T", target,       // ターゲットプロファイル（vs_6_0 等）
        L"-I", L"_Shader",
#if defined(_DEBUG)
        L"-Zi",              // デバッグ情報を付与
        L"-Qembed_debug",    // デバッグ情報をバイナリに埋め込む
        L"-Od",              // 最適化なし
#else
        L"-O3",              // 最適化最大
#endif
    };

    // コンパイル実行
    ComPtr<IDxcResult> result;
    HRESULT hr = compiler->Compile(
        &sourceBuffer,
        args, _countof(args),
        includeHandler.Get(),
        IID_PPV_ARGS(&result));
    if (FAILED(hr)) {
        return false;
    }

    // エラー/警告の文字列を取り出して出力
    ComPtr<IDxcBlobUtf8> errors;
    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
    if (errors != nullptr && errors->GetStringLength() > 0) {
        OutputDebugStringA(errors->GetStringPointer());
    }

    // コンパイルの成否
    HRESULT status = S_OK;
    result->GetStatus(&status);
    if (FAILED(status)) {
        return false;
    }

    // コンパイル済みバイトコード(DXIL)を取り出す
    hr = result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&m_blob), nullptr);
    return SUCCEEDED(hr);
}

D3D12_SHADER_BYTECODE Shader::GetByteCode() const {
    D3D12_SHADER_BYTECODE byteCode {};
    byteCode.pShaderBytecode = m_blob->GetBufferPointer();
    byteCode.BytecodeLength = m_blob->GetBufferSize();
    return byteCode;
}
