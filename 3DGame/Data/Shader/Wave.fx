
// １テクセル分上下左右位置へのオフセット値
float2 TexOffset;

// ばね強度。値を大きくすると波の移動速度が速くなる。
float  SpringPower = 0.5f;

// 追加する波のテクセル位置
float2 AddWavePos;                   

// 追加する波の高さ
float  AddWaveHeight;                 

// 経過時間
float g_DeltaTime = 0.4f;			

// 波マップ
sampler tex0 : register(s0);       


struct VS_OUTPUT
{
    float4 Pos       : POSITION;
    float2 Tex       : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos    : POSITION,
    float4 Color : COLOR0,
    float2 Tex : TEXCOORD0)
{
    VS_OUTPUT Out;

    Out.Pos = Pos;
    Out.Tex = Tex;

    return Out;
}

//波マップの更新処理
float4 PS_WaveMap(VS_OUTPUT In) : COLOR0
{

    //波マップから現在の高さ( R成分 )情報、1つ前の高さ( G成分 )を取得
    float4 Wave = tex2D(tex0, In.Tex);

    float old_hight = Wave.g;
    float now_hight = Wave.r;
    float next_hight;

    //周囲のテクセルの高さを取得
    float H1 = tex2D(tex0, In.Tex + float2(TexOffset.x, 0.0f)).r;
    float H2 = tex2D(tex0, In.Tex + float2(0.0f, TexOffset.y)).r;
    float H3 = tex2D(tex0, In.Tex + float2(-TexOffset.x, 0.0f)).r;
    float H4 = tex2D(tex0, In.Tex + float2(0.0f, -TexOffset.y)).r;

    //速度の更新
    //周囲のテクセルの高さと自分の高さの差を計算する。この値が速度となる。
    //現在の速度を加算。
    float c = SpringPower * SpringPower;
    float t = g_DeltaTime * g_DeltaTime;

    next_hight = (((c * t) * (H1 + H2 + H3 + H4)) + (2 - (4 * c * t)) * now_hight - old_hight) * 0.996f;

    //テクスチャ座標と追加する波の距離が一定値以下なら新規に波を追加
    if (distance(In.Tex, AddWavePos) < 0.005f)
    {
        next_hight += AddWaveHeight;
    }

    return float4(next_hight, now_hight, 1.0f, 1.0f);
}

//法線マップ作成
float4 PS_BumpMap(VS_OUTPUT In) : COLOR0
{
    float4 Out;

    //上下左右のテクセル位置の高さを取得する
    float H1 = tex2D(tex0, In.Tex + float2(TexOffset.x, 0.0f)).r;
    float H2 = tex2D(tex0, In.Tex + float2(0.0f, TexOffset.y)).r;
    float H3 = tex2D(tex0, In.Tex + float2(-TexOffset.x, 0.0f)).r;
    float H4 = tex2D(tex0, In.Tex + float2(0.0f, -TexOffset.y)).r;

    //X 方向の高さの変化量を計算する
    //波の高さ情報は -1.0f ～ 1.0f の範囲で格納されているので 0.0f ～ 1.0f に変換する
    float tu = 0.5f * (H3 - H1) + 0.5f;

    //Y 方向の高さの変化量を計算する
    //波の高さ情報は -1.0f ～ 1.0f の範囲で格納されているので 0.0f ～ 1.0f に変換する
    float tv = 0.5f * (H4 - H2) + 0.5f;

    //視差マッピングは高さ情報も使用する。そのためα成分に高さ情報を格納する
    Out = float4(tu, tv, 1.0f, tex2D(tex0, In.Tex).r * 0.5f + 0.5f);

    return Out;
}

technique Wave
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS_WaveMap();
    }

    pass P1
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS_BumpMap();
    }
}