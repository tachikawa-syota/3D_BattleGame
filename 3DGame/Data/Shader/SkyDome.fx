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

// モデルのアンビエント
float4 AmbientMat;

// ライトの方向
float4 LightDir;

//****************************************************************
//メインテクスチャー
//****************************************************************
texture MainTexture;       //メインテクスチャー
sampler TextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};

//****************************************************************
// 入力用の構造体
//****************************************************************
struct VS_IN {
	// 頂点の座標
	float4 Pos : POSITION;

	// 法線ベクトル
	float3 N : NORMAL;

	// テクセル
	float2 Tex : TEXCOORD0;
};

//****************************************************************
// 入出力用の構造体
//****************************************************************
struct VS_OUTPUT
{
	// 頂点の座標
	float4 Pos   : POSITION;

	// 頂点カラー
	float4 Col   : COLOR0;

	// テクセル座標
	float2 Tex   : TEXCOORD0;

	// テクスチャ座標(出力値)
	float4 posforps : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 Col1 : COLOR0;  //シーンのカラー情報
	float4 Col2 : COLOR1;  //シーンの輝度情報
};

//****************************************************************
// バーテックスシェーダー
//****************************************************************
VS_OUTPUT VS( VS_IN In) //テクセル
{
    // 構造体の初期化
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// 頂点の座標を行列変換する
	float4x4 mat;
	mat = mul(W, V);
	mat = mul(mat, P);
	Out.Pos = mul(In.Pos, mat);

	Out.Tex = In.Tex;
   
    // ピクセルシェーダー用に座標を保存してく
    Out.posforps = In.Pos;
   
    // テクセルはそのまま格納する
    Out.Tex = In.Tex;
   
    // 照明を逆方向にする
	float3 L = -normalize(LightDir.xyz);
   
    // 法線ベクトル
    float3 N = normalize( In.N.xyz );

    //照明と法線からdot関数により、内積を計算する。またmax関数により頂点カラーの最小値を環境光に抑えるように設定する
	Out.Col = max(AmbientMat, dot(N, L));
       
    return Out;
}

//****************************************************************
// ピクセルシェーダー
//****************************************************************
PS_OUTPUT PS(VS_OUTPUT In )
{  
    PS_OUTPUT Out;
	
	// テクスチャの反映
	Out.Col1 = In.Col * tex2D(TextureSampler, In.Tex);
	Out.Col2 = float4(0, 0, 0, 0);
   
	return Out;
}

//****************************************************************
// テクニック宣言
//****************************************************************
technique SkyDome
{
   pass P0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();   
   }     
}