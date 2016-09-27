
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

// ���s�����̕����x�N�g��
float4 LightDir;

// ���_�x�N�g��
float4 EyePos;

// ���f���̃A���r�G���g
float4 AmbientMat;

// ���f���̃f�B�t���[�Y
float4 DiffuseMat;

//****************************************************************
// ���C���e�N�X�`���[
//****************************************************************
texture MainTexture;		// ���C���e�N�X�`���[
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
	AddressU = Clamp;
	AddressV = Clamp;
};

//****************************************************************
// �V���h�E�}�b�v
//****************************************************************
texture ShadowMapTexture;	// �V���h�E�}�b�v�e�N�X�`���[
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
	// ���W
   float4 Pos : POSITION;  

   // �J���[
   float4 Col : COLOR0;    

   // UV���W
   float2 Tex : TEXCOORD0; 

   // �@���x�N�g��
   float3 N   : TEXCOORD1; 

   // ���C�g�̕���
   float3 L   : TEXCOORD2; 

   // ���_�̕���
   float3 E   : TEXCOORD3; 
};

VS_OUTPUT VS(float4 Pos    : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD0)
{
	// �\���̂̏�����
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ���_�̍��W���s��ϊ�����
	float4x4 mat;
	mat = mul(W, V);
	mat = mul(mat, P);
	Out.Pos = mul(Pos, mat);

	// �s�N�Z���V�F�[�_�[�p�ɍ��W��ۑ����Ă�
	Out.Tex = Tex;

	//���C�g�����œ��͂����̂ŁA���_ -> ���C�g�ʒu�Ƃ��邽�߂ɋt�����ɕϊ�����B�Ȃ��A�v���P�[�V�����ŕK�����K�����邱��
	Out.L = -normalize(LightDir.xyz);

	//�I�u�W�F�N�g�̖@���x�N�g���𐳋K������
	Out.N = normalize(Normal.xyz);

	//���C�g�x�N�g���Ɩ@���x�N�g���̓��ς��v�Z���A�v�Z���ʂ̐F�̍Œ�l������( AmbientMat )�ɐ�������
	Out.Col = max(AmbientMat + DiffuseMat, dot(Out.N, Out.L));
	
	//���_ -> ���_ �ւ̃x�N�g�����v�Z
	Out.E = EyePos.xyz - Pos.xyz;
	
	return Out;
}

struct PS_OUTPUT
{
	float4 Col1 : COLOR0;  //�V�[���̃J���[���
	float4 Col2 : COLOR1;  //�V�[���̋P�x���
};

PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	//�X�y�L�����[�̐F���v�Z����
	float3 N = normalize( In.N );
	//���_ -> ���C�g�ʒu�x�N�g�� + ���_ -> ���_�x�N�g��
	float3 H = normalize( In.L + normalize( In.E ) );
	//�X�y�L�����[�J���[���v�Z����
	float S = pow(max(0.0f, dot(N, H)), 20.0f) * 0.55f;

	//�X�y�L�����[�J���[�����Z����   
	Out.Col1 = (In.Col * tex2D(MainTextureSampler, In.Tex) + S);

	//�P�x�����Z�b�g����	
	float p = S * 4.0f;
	Out.Col2 = float4(p, p, p, 1.0f);

	//====================================
	// �ȉ��V���h�E�}�b�v
	//====================================

	// ���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
//	float4 ZCalcTex = mul(In.Tex, W);
	float4 ZCalcTex = mul(In.Tex, LV);
	ZCalcTex = mul(ZCalcTex, LP);

	// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
	float ZValue = ZCalcTex.z / ZCalcTex.w;

	// �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x / ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y / ZCalcTex.w) * 0.5f;

	// ���A��Z�l���o
	float SM_Z = tex2D(ShadowSampler, TransTexCoord).x;

	// �e�Ɣ��肵���ꏊ�ɉe������
	if (ZValue > SM_Z + 0.005f) {
		Out.Col1 = Out.Col1 * 0.5f;
	}

	return Out;
}

//****************************************************************
// �e�N�j�b�N // compile��2_0���g��
//****************************************************************
technique LightBloom
{
   pass P0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();
   }
}