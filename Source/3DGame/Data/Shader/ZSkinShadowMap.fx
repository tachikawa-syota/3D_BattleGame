
// 遠近射影
float4x4 P;

// ビュー
float4x4 V;

// ワールド
float4x4 W;

// ライトのビュー変換行列
float4x4 LV;
// ライトの射影変換行列
float4x4 LP;

// 行列スタック
float4x4 WStack[4];

// モデルのアンビエント
float4 AmbientMat;

// モデルのディフューズ色
float4 DiffuseMat;

// ライト方向
float3 LightDir;

// 視線方向
float3 EyePos;

// 1頂点に対数スタック数
int StackNum;

///頂点シェーダ入力
struct VS_IN{
	float4 Pos		: POSITION;		//頂点座標
	float3 N        : NORMAL;		//法線ベクトル
	float2 Tex		: TEXCOORD;		//UV座標
	float4 Weight	: BLENDWEIGHT;	//WStackに対する重みがx, y, z, wにそれぞれ入っている。
};

struct VS_OUTPUT
{
	float4 Pos   : POSITION;     //頂点の座標
	float4 Tex   : TEXCOORD0;    //テクセル座標
};


//==============================================
//モデルのShader
//==============================================

// 頂点シェーダ
VS_OUTPUT ShadowTexMakeVS(VS_IN In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ボーンから座標を取得
	float fWeight[4] = (float[4])In.Weight;
	float4x4 Comb = (float4x4)0;
	//	float fLastWeight = 1.0;
	Out.Pos = (float4)0;

	for (int i = 0; i < StackNum - 1; i++) {
		Out.Pos += mul(In.Pos, WStack[i]) * fWeight[i];
//		Out.N += mul(In.N, (float3x3)WStack[i]) * fWeight[i];
//		fLastWeight -= fWeight[i];
	}

	//頂点の座標を行列変換する
	float4x4 mat;
	mat = mul(W, LV);
	mat = mul(mat, LP);
	Out.Pos = mul(Pos, mat);
	
	Out.Tex = Out.Pos;
	return Out;
}

//==============================================
//モデル影のShader
//==============================================

float4 ShadowTexMakePS( VS_OUTPUT In ) : COLOR
{
	return In.Tex.z / In.Tex.w;
}

technique ZSkinShadowMap
{
	// シルエットをテクスチャにレンダリング
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowTexMakeVS();
		PixelShader = compile ps_3_0 ShadowTexMakePS();
	}
}