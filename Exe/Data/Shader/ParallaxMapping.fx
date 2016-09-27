
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

// 視点位置ベクトル
float4 EyePos;

// モデルのアンビエント
float4 AmbientMat;

// 高さの最大値。大きくするほど高くなる。
float  m_Param = 0.036f;


sampler s0 : register(s0);        //オブジェクトのテクスチャー
//****************************************************************
// メインテクスチャー
//****************************************************************
texture MainTexture;		//メインテクスチャー
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
	AddressU = Clamp;
	AddressV = Clamp;
};

//****************************************************************
// シャドウマップテクスチャー
//****************************************************************
texture NormalMapTexture;	//シャドウマップテクスチャー
sampler NormalMapSampler = sampler_state {
	Texture = < NormalMapTexture >;
	AddressU = Clamp;
	AddressV = Clamp;
	MagFilter = LINEAR;
	MinFilter = LINEAR;
	MipFilter = NONE;
};

struct VS_OUTPUT
{
   float4 Pos      : POSITION;  //頂点座標
   float2 Tex      : TEXCOORD0; //テクセル座標
   float3 Eye      : TEXCOORD1; //頂点座標系での視線方向ベクトル
   float3 Light    : TEXCOORD2; //頂点座標系での頂点 → ライト位置ベクトル
};

VS_OUTPUT VS( float3 Pos      : POSITION,   //頂点座標
              float3 Tangent  : TANGENT0,   //接線ベクトル
              float3 Binormal : BINORMAL0,  //従法線ベクトル
              float3 Normal   : NORMAL,     //法線ベクトル
              float2 Tex      : TEXCOORD0   //テクセル
              )
{
   VS_OUTPUT Out;
   
   // 頂点の座標を行列変換する
   float4x4 mat;
   mat = mul(W, V);
   mat = mul(mat, P);
   Out.Pos = mul(float4(Pos, 1.0f), mat);

   Out.Tex = Tex;
   
   //視線ベクトルを計算
   float3 Eye = normalize( EyePos.xyz - Pos.xyz );
   
   //視線ベクトルを頂点座標系に変換する
   Out.Eye.x = dot( Eye, Tangent );
   Out.Eye.y = dot( Eye, Binormal );
   Out.Eye.z = dot( Eye, Normal );
   Out.Eye = normalize( Out.Eye );
   
   //オブジェクトの頂点座標 -> ライトの位置ベクトル に変換する
   float3 Light = -LightDir.xyz;

   //ライトベクトルを頂点座標系に変換する
   Out.Light.x = dot( Light, Tangent );
   Out.Light.y = dot( Light, Binormal );
   Out.Light.z = dot( Light, Normal );
   Out.Light = normalize( Out.Light );
   
   return Out;
}

float4 PS( VS_OUTPUT In ) : COLOR
{
   //高さマップを参照し、高さを取得する
   //高さ情報はアルファ成分に格納されている
//	float h = tex2D(NormalMapSampler, In.Tex).a;
   
   //テクセルを頂点座標系での視線ベクトル方向に重みをつけてずらす。
 //  float2 Tex = In.Tex + m_Param * h * In.Eye.xy;
   
   //法線マップを参照し、法線を取得する
   //法線マップは -1.0f ～ 1.0f の値を 0.0f ～ 1.0f の範囲に保存してあるので変換する
   float3 Normal = 2.0f * tex2D(NormalMapSampler, In.Tex).xyz - 1.0f;
      
   //フォンシェーディングによるスペキュラーの色を計算する
   //頂点 -> ライト位置ベクトル + 頂点 -> 視点ベクトル
   float3 H = normalize( In.Light + In.Eye );
   
   //スペキュラーカラーを計算する
   float S = pow( max( 0.0f, dot( Normal, H ) ), 4.0f ) * 0.55f;
   
   //マテリアル色を決定する
   return tex2D(s0, In.Tex) * max(AmbientMat, dot(Normal, In.Light)) + S;
}

technique PM
{
   pass P0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();
   }
}