/**===========================================================================

 File   : アンリットシェーダー [Unlit.hlsl]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/
#include "Common.hlsli"

VSOutput VSMain(VSInput input) {
    // VSOutput output {}; // 古い０初期化文として解釈されてしまう
    VSOutput output = (VSOutput) 0; // HLSL流のゼロ初期化
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}

float4 PSMain(VSOutput input) : SV_TARGET { // ピクセルシェーダの出力先＝レンダーターゲット
    return input.color; // 頂点間の color はラスタライズ時に自動補間される
}
