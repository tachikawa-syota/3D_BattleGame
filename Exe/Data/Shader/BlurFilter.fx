// ガウスフィルター

// スクリーンの幅
float MapWidth;
// スクリーンの高さ
float MapHeight;
// 混ぜ合わせる際の重み
float weight[8];
// ぼかす方向フラグ
bool isXY;

//****************************************************************
//メインテクスチャー
//****************************************************************
sampler Maintexture : register(s0);  //ブラーをかける元となるテクスチャー

texture	SrcTexture;	// ぼかす対象となるテクスチャ
sampler TextureSampler2 = sampler_state{
	Texture = <SrcTexture>;
	MinFilter= LINEAR;		// リニアフィルタ（縮小時）
	MagFilter= LINEAR;		// リニアフィルタ（拡大時）
	MipFilter= NONE;		// ミップマップ
};

//****************************************************************
//入力用の構造体
//****************************************************************
struct VS_OUTPUT {
	float4 Pos : POSITION;  //頂点の座標
	float2 Tex : TEXCOORD0; //テクセル
};

//****************************************************************
//バーテックスシェーダー
//****************************************************************
VS_OUTPUT VS( VS_OUTPUT In) //テクセル
{
    //構造体の初期化
    VS_OUTPUT Out = (VS_OUTPUT)0;
     
	// 行列変換しないのでそのままピクセルシェーダーに渡す
    Out.Pos = In.Pos; 
    Out.Tex = In.Tex;
       
    return Out;
}

//****************************************************************
//ピクセルシェーダー
//****************************************************************
float4 PS(VS_OUTPUT In ) : COLOR0
{
	float4	Color = (float4)0;

	if (isXY)    //X方向にぼかす
	{
		// テクスチャ座標を補正
		In.Tex.x = In.Tex.x + 0.5f / MapWidth;
		In.Tex.y = In.Tex.y + 0.5f / MapHeight;

		Color = weight[0] * tex2D(TextureSampler2, In.Tex);
		Color += weight[1]

			* (tex2D(TextureSampler2, In.Tex + float2(+2.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-2.0f / MapWidth, 0)));
		Color += weight[2]
			* (tex2D(TextureSampler2, In.Tex + float2(+4.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-4.0f / MapWidth, 0)));
		Color += weight[3]
			* (tex2D(TextureSampler2, In.Tex + float2(+6.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-6.0f / MapWidth, 0)));
		Color += weight[4]
			* (tex2D(TextureSampler2, In.Tex + float2(+8.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-8.0f / MapWidth, 0)));
		Color += weight[5]
			* (tex2D(TextureSampler2, In.Tex + float2(+10.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-10.0f / MapWidth, 0)));
		Color += weight[6]
			* (tex2D(TextureSampler2, In.Tex + float2(+12.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-12.0f / MapWidth, 0)));
		Color += weight[7]
			* (tex2D(TextureSampler2, In.Tex + float2(+14.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-14.0f / MapWidth, 0)));
	}
	else{
		// テクスチャ座標のずれをなくす
		In.Tex.x = In.Tex.x + 0.5 / MapWidth;
		In.Tex.y = In.Tex.y + 0.5 / MapHeight;

		Color = weight[0] * tex2D(TextureSampler2, In.Tex);
		Color += weight[1]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +2.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -2.0f / MapHeight)));
		Color += weight[2]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +4.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -4.0f / MapHeight)));
		Color += weight[3]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +6.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -6.0f / MapHeight)));
		Color += weight[4]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +8.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -8.0f / MapHeight)));
		Color += weight[5]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +10.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -10.0f / MapHeight)));
		Color += weight[6]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +12.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -12.0f / MapHeight)));
		Color += weight[7]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +14.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -14.0f / MapHeight)));
	}

	return Color;
}

//****************************************************************
//テクニック宣言
//****************************************************************
// Shader_Chass_default TShader
technique BlurFilter
{
   pass P0
   {
      VertexShader = compile vs_3_0 VS();
      PixelShader  = compile ps_3_0 PS();   
   }     
}