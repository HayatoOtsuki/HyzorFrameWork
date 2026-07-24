/**===========================================================================

 File   : ルートシグネチャ [RootSignature.h]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 シェーダに渡す定数やテクスチャの口を定義

 ===========================================================================**/
#ifndef ROOTSIGNATURE_H

#include <d3d12.h>
#include <wrl/client.h>

// シェーダーへの入力受け渡し仕様
class RootSignature {
public:
	bool Initialize(ID3D12Device* device);
	
	ID3D12RootSignature* Get() const { return m_rootSignature.Get(); }

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12RootSignature> m_rootSignature;
};

#endif // !ROOTSIGNATURE_H
