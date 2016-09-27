// ���[���h�s��
float4x4 W;

// �r���[�s��
float4x4 V;

// ���e�s��
float4x4 P;

// ���C�g�̃r���[�ϊ��s��
float4x4 LV;

// ���C�g�̎ˉe�ϊ��s��
float4x4 LP;

// ���f���̃A���r�G���g
float4 AmbientMat;

// ���C�g�̕���
float4 LightDir;			

//****************************************************************
//���C���e�N�X�`���[
//****************************************************************
texture MainTexture; // ���C���e�N�X�`���[
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};

//****************************************************************
//�V���h�E�}�b�v
//****************************************************************
texture ShadowMapTexture; // �V���h�E�}�b�v�e�N�X�`���[
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
//���͗p�̍\����
//****************************************************************
struct VS_IN {
	float4 Pos : POSITION;         //���_�̍��W
	float3 Normal : NORMAL;        //�@���x�N�g��
	float2 Tex : TEXCOORD0;        //�e�N�Z��
};

//****************************************************************
//���o�͗p�̍\����
//****************************************************************
struct VS_OUTPUT
{
   float4 Pos   : POSITION;     //���_�̍��W
   float4 Col   : COLOR0;       //���_�J���[
   float2 Tex   : TEXCOORD0;    //�e�N�Z�����W
   float4 posforps : TEXCOORD1;		// �e�N�X�`�����W(�o�͒l)
};

//****************************************************************
//�o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
VS_OUTPUT VS( VS_IN In) //�e�N�Z��
{

    //�\���̂̏�����
    VS_OUTPUT Out = (VS_OUTPUT)0;
     
    //���_�̍��W���s��ϊ�����
    float4x4 mat;
    mat  = mul( W, V );
    mat  = mul( mat, P );
    Out.Pos = mul( In.Pos, mat );
   
    //�s�N�Z���V�F�[�_�[�p�ɍ��W��ۑ����Ă�
    Out.posforps = In.Pos;
   
    //�e�N�Z���͂��̂܂܊i�[����
    Out.Tex = In.Tex;
   
    //�Ɩ����t�����ɂ���B(���ӂP)
    float3 L = -normalize( LightDir.xyz );
   
    //�@���x�N�g���B
    float3 N = normalize( In.Normal.xyz );

    //�Ɩ��Ɩ@������dot�֐��ɂ��A���ς��v�Z����B�܂�max�֐��ɂ�蒸�_�J���[�̍ŏ��l�������ɗ}����悤�ɐݒ肷��B(���ӂQ)
    Out.Col = max( AmbientMat, dot(N, L) );
       
    return Out;
}

//****************************************************************
//�s�N�Z���V�F�[�_�[
//****************************************************************
float4 PS( in VS_OUTPUT In ) : COLOR0
{  
    float4 Out_Color = (float4)0;
    Out_Color = In.Col * float4(1,1,1,1);
     
    //�e�N�X�`���̔��f
	Out_Color = Out_Color * tex2D(MainTextureSampler, In.Tex);
    
   //====================================
   //�V���h�E�}�b�v�������ł��Ȃ��Ȃ�G��Ȃ���
   //====================================
   
   // ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
   float4 ZCalcTex = mul( In.posforps, W );
   ZCalcTex  = mul( ZCalcTex, LV );
   ZCalcTex  = mul( ZCalcTex, LP );
   
   // ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
   float ZValue = ZCalcTex.z / ZCalcTex.w;
   
   // �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   
   // ���A��Z�l���o
   float SM_Z = tex2D( ShadowMapSampler, TransTexCoord ).z;
   
   //�e�Ɣ��肵���ꏊ�ɉe������
   if( ZValue > SM_Z+0.0029f ){
     Out_Color = Out_Color * 0.5f; 
    }
      
   return Out_Color;
}

//****************************************************************
//�e�N�j�b�N�錾
//****************************************************************
technique StandardShader
{
   pass P0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();   
   }     
}