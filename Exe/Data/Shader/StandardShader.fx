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
texture MainTexture; // メインテクスチャー
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};

//****************************************************************
//シャドウマップ
//****************************************************************
texture ShadowMapTexture; // シャドウマップテクスチャー
sampler ShadowMapSampler = sampler_state
{
    Texture = < ShadowMapTexture >;
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

//****************************************************************
//入力用の構造体
//****************************************************************
struct VS_IN {
	float4 Pos : POSITION;         //頂点の座標
	float3 Normal : NORMAL;        //法線ベクトル
	float2 Tex : TEXCOORD0;        //テクセル
};

//****************************************************************
//入出力用の構造体
//****************************************************************
struct VS_OUTPUT
{
   float4 Pos   : POSITION;     //頂点の座標
   float4 Col   : COLOR0;       //頂点カラー
   float2 Tex   : TEXCOORD0;    //テクセル座標
   float4 posforps : TEXCOORD1;		// テクスチャ座標(出力値)
};

//****************************************************************
//バーテックスシェーダー
//****************************************************************
VS_OUTPUT VS( VS_IN In) //テクセル
{

    //構造体の初期化
    VS_OUTPUT Out = (VS_OUTPUT)0;
     
    //頂点の座標を行列変換する
    float4x4 mat;
    mat  = mul( W, V );
    mat  = mul( mat, P );
    Out.Pos = mul( In.Pos, mat );
   
    //ピクセルシェーダー用に座標を保存してく
    Out.posforps = In.Pos;
   
    //テクセルはそのまま格納する
    Out.Tex = In.Tex;
   
    //照明を逆方向にする。(注意１)
    float3 L = -normalize( LightDir.xyz );
   
    //法線ベクトル。
    float3 N = normalize( In.Normal.xyz );

    //照明と法線からdot関数により、内積を計算する。またmax関数により頂点カラーの最小値を環境光に抑えるように設定する。(注意２)
    Out.Col = max( AmbientMat, dot(N, L) );
       
    return Out;
}

//****************************************************************
//ピクセルシェーダー
//****************************************************************
float4 PS( in VS_OUTPUT In ) : COLOR0
{  
    float4 Out_Color = (float4)0;
    Out_Color = In.Col * float4(1,1,1,1);
     
    //テクスチャの反映
	Out_Color = Out_Color * tex2D(MainTextureSampler, In.Tex);
    
   //====================================
   //シャドウマップが理解できないなら触らない事
   //====================================
   
   // ライトの目線によるワールドビュー射影変換をする
   float4 ZCalcTex = mul( In.posforps, W );
   ZCalcTex  = mul( ZCalcTex, LV );
   ZCalcTex  = mul( ZCalcTex, LP );
   
   // ライト目線によるZ値の再算出
   float ZValue = ZCalcTex.z / ZCalcTex.w;
   
   // 射影空間のXY座標をテクスチャ座標に変換
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   
   // リアルZ値抽出
   float SM_Z = tex2D( ShadowMapSampler, TransTexCoord ).z;
   
   //影と判定した場所に影をつける
   if( ZValue > SM_Z+0.0029f ){
     Out_Color = Out_Color * 0.5f; 
    }
      
   return Out_Color;
}

//****************************************************************
//テクニック宣言
//****************************************************************
technique StandardShader
{
   pass P0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();   
   }     
}