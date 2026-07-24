/**===========================================================================

 File   : コモンシェーダー [Common.hlsl]

 Author : Hayato Otsuki
 Date   : 2026/7/24
 -----------------------------------------------------------------------------

 ===========================================================================**/

struct VSInput {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput {
    float4 position : SV_POSITION; // 頂点シェーダが必ず出すクリップ空間座標（GPUが画面座標に変換する）
    float4 color : COLOR;
};
