
// 遠近射影
float4x4 P;

// ビュー
float4x4 V;

// ワールド
float4x4 W;

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

//テクスチャサンプラ
texture MainTexture;
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Clamp;
	AddressV = Clamp;
};

///頂点シェーダ入力
struct VS_IN{
    float4 Pos		: POSITION;		//頂点座標
    float3 N        : NORMAL;		//法線ベクトル
	float2 Tex		: TEXCOORD;		//UV座標
	float4 Weight	: BLENDWEIGHT;	//WStackに対する重みがx, y, z, wにそれぞれ入っている。
};

/// 頂点シェーダ出力
struct VS_OUTPUT{
	float4 Pos	    : POSITION;  // 頂点座標
	float4 Col      : COLOR0;    // 色情報   
	float3 N        : TEXCOORD0; // 法線ベクトル
	float2 Tex      : TEXCOORD1; // UV座標
	float3 L        : TEXCOORD2; // ライトの方向
	float3 E        : TEXCOORD3; // 視線方向ベクトル
};

//****************************************************************
// バーテックスシェーダー
//****************************************************************
VS_OUTPUT VS(VS_IN In)
{
	// 構造体初期化
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ボーンから座標を取得
	float fWeight[4] = (float[4])In.Weight;
	float4x4 Comb = (float4x4)0;
		float fLastWeight = 1.0;
	Out.Pos = (float4)0;
	Out.N = (float3)0;
	for (int i = 0; i < StackNum - 1; i++) {
		Out.Pos += mul(In.Pos, WStack[i]) * fWeight[i];
		Out.N += mul(In.N, (float3x3)WStack[i]) * fWeight[i];
		fLastWeight -= fWeight[i];
	}

	// 頂点の座標を行列変換する
	Out.Pos += mul(In.Pos, WStack[StackNum - 1]) * fLastWeight;
	Out.Pos = mul(Out.Pos, V);
	Out.Pos = mul(Out.Pos, P);
	Out.N += mul(In.N, (float3x3)WStack[StackNum - 1]) * fLastWeight;
	Out.N = normalize(Out.N);

	// ライト方向で入力されるので、頂点 -> ライト位置とするために逆向きに変換する。なおアプリケーションで必ず正規化すること
	Out.L = -LightDir.xyz;

	// 視線方向ベクトル産出
	Out.E = Out.Pos.xyz - EyePos;

	// テクセルはそのまま保存
	Out.Tex = In.Tex;

	// カラー
	float4 NormalHead = mul(float4(In.Pos + In.N, 1), fLastWeight);
	float3 Normal = normalize(NormalHead.xyz - Out.Pos.xyz);
	float4 Color = AmbientMat + DiffuseMat * max(0, dot(Normal, -normalize(LightDir)));
	Out.Col = Color;

	return Out;
}

struct PS_OUTPUT
{
	float4 Col1 : COLOR0;  //シーンのカラー情報
	float4 Col2 : COLOR1;  //シーンの輝度情報
};

//****************************************************************
// ピクセルシェーダー
//****************************************************************
PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	//スペキュラーの色を計算する
	float3 N = normalize(In.N);
	//頂点 -> ライト位置ベクトル + 頂点 -> 視点ベクトル
	float3 H = normalize(In.L + normalize(In.E));
	//スペキュラーカラーを計算する
	float S = pow(max(0.0f, dot(N, H)), 20.0f) * 0.55f;

	//テクスチャの反映
	Out.Col1 = In.Col * tex2D(MainTextureSampler, In.Tex);

	//輝度情報をセットする
	float p = S * 4.0f;
	Out.Col2 = float4(p, p, p, 1.0f);

	return Out;
}

//****************************************************************
// テクニック
//****************************************************************
technique SkinMeshShader
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
