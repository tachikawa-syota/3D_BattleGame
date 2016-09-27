
// ���ߎˉe
float4x4 P;

// �r���[
float4x4 V;

// ���[���h
float4x4 W;

// ���C�g�̃r���[�ϊ��s��
float4x4 LV;
// ���C�g�̎ˉe�ϊ��s��
float4x4 LP;

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

///���_�V�F�[�_����
struct VS_IN{
	float4 Pos		: POSITION;		//���_���W
	float3 N        : NORMAL;		//�@���x�N�g��
	float2 Tex		: TEXCOORD;		//UV���W
	float4 Weight	: BLENDWEIGHT;	//WStack�ɑ΂���d�݂�x, y, z, w�ɂ��ꂼ������Ă���B
};

struct VS_OUTPUT
{
	float4 Pos   : POSITION;     //���_�̍��W
	float4 Tex   : TEXCOORD0;    //�e�N�Z�����W
};


//==============================================
//���f����Shader
//==============================================

// ���_�V�F�[�_
VS_OUTPUT ShadowTexMakeVS(VS_IN In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// �{�[��������W���擾
	float fWeight[4] = (float[4])In.Weight;
	float4x4 Comb = (float4x4)0;
	//	float fLastWeight = 1.0;
	Out.Pos = (float4)0;

	for (int i = 0; i < StackNum - 1; i++) {
		Out.Pos += mul(In.Pos, WStack[i]) * fWeight[i];
//		Out.N += mul(In.N, (float3x3)WStack[i]) * fWeight[i];
//		fLastWeight -= fWeight[i];
	}

	//���_�̍��W���s��ϊ�����
	float4x4 mat;
	mat = mul(W, LV);
	mat = mul(mat, LP);
	Out.Pos = mul(Pos, mat);
	
	Out.Tex = Out.Pos;
	return Out;
}

//==============================================
//���f���e��Shader
//==============================================

float4 ShadowTexMakePS( VS_OUTPUT In ) : COLOR
{
	return In.Tex.z / In.Tex.w;
}

technique ZSkinShadowMap
{
	// �V���G�b�g���e�N�X�`���Ƀ����_�����O
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowTexMakeVS();
		PixelShader = compile ps_3_0 ShadowTexMakePS();
	}
}