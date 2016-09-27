
// ワールド行列
float4x4 W;

// ライトのビュー変換行列
float4x4 LV;

// ライトの射影変換行列
float4x4 LP;

struct VS_OUTPUT
{
	// 頂点の座標
   float4 Pos   : POSITION; 

   // テクセル座標
   float4 Tex   : TEXCOORD0;    
};

//****************************************************************
// バーテックスシェーダー
//****************************************************************
VS_OUTPUT ShadowTexMakeVS(float4 Pos : POSITION) //テクセル
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//頂点の座標を行列変換する
	float4x4 mat;
	mat = mul(W, LV);
	mat = mul(mat, LP);
	Out.Pos = mul(Pos, mat);

	// テクスチャ座標を頂点に合わせる
	Out.Tex = Out.Pos;

	return Out;
}

//****************************************************************
// ピクセルシェーダー
//****************************************************************
float4 ShadowTexMakePS(VS_OUTPUT In) : COLOR
{
	float4 FinalColor;

	FinalColor = In.Tex.z / In.Tex.w;

   return FinalColor;
}

//****************************************************************
// テクニック
//****************************************************************
technique ZShadowMap
{
   pass P0
   {
      VertexShader = compile vs_2_0 ShadowTexMakeVS();
      PixelShader  = compile ps_2_0 ShadowTexMakePS();   
   }     
}