
// ワールド行列
float4x4 W;

// ビュー行列
float4x4 V;

// 投影行列
float4x4 P;

// ライトのビュー変換行列
float4x4 LV;

// ライトの射影変換行列
float4x4 LP;

// 平行光源の方向ベクトル
float4 LightDir;

// 視点ベクトル
float4 EyePos;

// モデルのアンビエント
float4 AmbientMat;

// モデルのディフューズ
float4 DiffuseMat;

//****************************************************************
// メインテクスチャー
//****************************************************************
texture MainTexture;		// メインテクスチャー
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
	AddressU = Clamp;
	AddressV = Clamp;
};

//****************************************************************
// シャドウマップ
//****************************************************************
texture ShadowMapTexture;	// シャドウマップテクスチャー
sampler ShadowSampler = sampler_state {
	Texture = < ShadowMapTexture >;
	AddressU = Clamp;
	AddressV = Clamp;
	MagFilter = LINEAR;
	MinFilter = LINEAR;
	MipFilter = NONE;
};

struct VS_OUTPUT
{
	// 座標
   float4 Pos : POSITION;  

   // カラー
   float4 Col : COLOR0;    

   // UV座標
   float2 Tex : TEXCOORD0; 

   // 法線ベクトル
   float3 N   : TEXCOORD1; 

   // ライトの方向
   float3 L   : TEXCOORD2; 

   // 視点の方向
   float3 E   : TEXCOORD3; 
};

VS_OUTPUT VS(float4 Pos    : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD0)
{
	// 構造体の初期化
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// 頂点の座標を行列変換する
	float4x4 mat;
	mat = mul(W, V);
	mat = mul(mat, P);
	Out.Pos = mul(Pos, mat);

	// ピクセルシェーダー用に座標を保存してく
	Out.Tex = Tex;

	//ライト方向で入力されるので、頂点 -> ライト位置とするために逆向きに変換する。なおアプリケーションで必ず正規化すること
	Out.L = -normalize(LightDir.xyz);

	//オブジェクトの法線ベクトルを正規化する
	Out.N = normalize(Normal.xyz);

	//ライトベクトルと法線ベクトルの内積を計算し、計算結果の色の最低値を環境光( AmbientMat )に制限する
	Out.Col = max(AmbientMat + DiffuseMat, dot(Out.N, Out.L));
	
	//頂点 -> 視点 へのベクトルを計算
	Out.E = EyePos.xyz - Pos.xyz;
	
	return Out;
}

struct PS_OUTPUT
{
	float4 Col1 : COLOR0;  //シーンのカラー情報
	float4 Col2 : COLOR1;  //シーンの輝度情報
};

PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	//スペキュラーの色を計算する
	float3 N = normalize( In.N );
	//頂点 -> ライト位置ベクトル + 頂点 -> 視点ベクトル
	float3 H = normalize( In.L + normalize( In.E ) );
	//スペキュラーカラーを計算する
	float S = pow(max(0.0f, dot(N, H)), 20.0f) * 0.55f;

	//スペキュラーカラーを加算する   
	Out.Col1 = (In.Col * tex2D(MainTextureSampler, In.Tex) + S);

	//輝度情報をセットする	
	float p = S * 4.0f;
	Out.Col2 = float4(p, p, p, 1.0f);

	//====================================
	// 以下シャドウマップ
	//====================================

	// ライトの目線によるワールドビュー射影変換をする
//	float4 ZCalcTex = mul(In.Tex, W);
	float4 ZCalcTex = mul(In.Tex, LV);
	ZCalcTex = mul(ZCalcTex, LP);

	// ライト目線によるZ値の再算出
	float ZValue = ZCalcTex.z / ZCalcTex.w;

	// 射影空間のXY座標をテクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x / ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y / ZCalcTex.w) * 0.5f;

	// リアルZ値抽出
	float SM_Z = tex2D(ShadowSampler, TransTexCoord).x;

	// 影と判定した場所に影をつける
	if (ZValue > SM_Z + 0.005f) {
		Out.Col1 = Out.Col1 * 0.5f;
	}

	return Out;
}

//****************************************************************
// テクニック // compileは2_0を使う
//****************************************************************
technique LightBloom
{
   pass P0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();
   }
}