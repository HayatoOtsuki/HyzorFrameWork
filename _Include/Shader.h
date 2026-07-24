/**===========================================================================

 File   : シェーダーコンパイル [Shader.h]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#ifndef SHADER_H

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl/client.h>

// HLSLを実行時コンパイルしてバイトコードを保持するクラス
class Shader {
public:
	// Path : hlsl, entryPoint : 関数名, target : "VS_6_0"等
	bool CompileFromFile(const wchar_t* path, const wchar_t* entryPoint, const wchar_t* target);
	
	D3D12_SHADER_BYTECODE GetByteCode() const;

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDxcBlob> m_blob; // コンパイル済みバイトコード
};

#endif // !SHADER_H
