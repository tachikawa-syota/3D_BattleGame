
// ���ߎˉe
float4x4 P;

// �r���[
float4x4 V;

// ���[���h
float4x4 W;

// �s��X�^�b�N
float4x4 WStack[4];

// ���f���̃A���r�G���g
float4 AmbientMat;

// ���f���̃f�B�t���[�Y�F
float4 DiffuseMat;

// ���C�g����
float3 LightDir;

// ��������
float3 EyePos;

// 1���_�ɑΐ��X�^�b�N��
int StackNum;

//�e�N�X�`���T���v��
texture MainTexture;
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU = Clamp;
	AddressV = Clamp;
};

///���_�V�F�[�_����
struct VS_IN{
    float4 Pos		: POSITION;		//���_���W
    float3 N        : NORMAL;		//�@���x�N�g��
	float2 Tex		: TEXCOORD;		//UV���W
	float4 Weight	: BLENDWEIGHT;	//WStack�ɑ΂���d�݂�x, y, z, w�ɂ��ꂼ������Ă���B
};

/// ���_�V�F�[�_�o��
struct VS_OUTPUT{
	float4 Pos	    : POSITION;  // ���_���W
	float4 Col      : COLOR0;    // �F���   
	float3 N        : TEXCOORD0; // �@���x�N�g��
	float2 Tex      : TEXCOORD1; // UV���W
	float3 L        : TEXCOORD2; // ���C�g�̕���
	float3 E        : TEXCOORD3; // ���������x�N�g��
};

//****************************************************************
// �o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
VS_OUTPUT VS(VS_IN In)
{
	// �\���̏�����
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// �{�[��������W���擾
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

	// ���_�̍��W���s��ϊ�����
	Out.Pos += mul(In.Pos, WStack[StackNum - 1]) * fLastWeight;
	Out.Pos = mul(Out.Pos, V);
	Out.Pos = mul(Out.Pos, P);
	Out.N += mul(In.N, (float3x3)WStack[StackNum - 1]) * fLastWeight;
	Out.N = normalize(Out.N);

	// ���C�g�����œ��͂����̂ŁA���_ -> ���C�g�ʒu�Ƃ��邽�߂ɋt�����ɕϊ�����B�Ȃ��A�v���P�[�V�����ŕK�����K�����邱��
	Out.L = -LightDir.xyz;

	// ���������x�N�g���Y�o
	Out.E = Out.Pos.xyz - EyePos;

	// �e�N�Z���͂��̂܂ܕۑ�
	Out.Tex = In.Tex;

	// �J���[
	float4 NormalHead = mul(float4(In.Pos + In.N, 1), fLastWeight);
	float3 Normal = normalize(NormalHead.xyz - Out.Pos.xyz);
	float4 Color = AmbientMat + DiffuseMat * max(0, dot(Normal, -normalize(LightDir)));
	Out.Col = Color;

	return Out;
}

struct PS_OUTPUT
{
	float4 Col1 : COLOR0;  //�V�[���̃J���[���
	float4 Col2 : COLOR1;  //�V�[���̋P�x���
};

//****************************************************************
// �s�N�Z���V�F�[�_�[
//****************************************************************
PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;

	//�X�y�L�����[�̐F���v�Z����
	float3 N = normalize(In.N);
	//���_ -> ���C�g�ʒu�x�N�g�� + ���_ -> ���_�x�N�g��
	float3 H = normalize(In.L + normalize(In.E));
	//�X�y�L�����[�J���[���v�Z����
	float S = pow(max(0.0f, dot(N, H)), 20.0f) * 0.55f;

	//�e�N�X�`���̔��f
	Out.Col1 = In.Col * tex2D(MainTextureSampler, In.Tex);

	//�P�x�����Z�b�g����
	float p = S * 4.0f;
	Out.Col2 = float4(p, p, p, 1.0f);

	return Out;
}

//****************************************************************
// �e�N�j�b�N
//****************************************************************
technique SkinMeshShader
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
