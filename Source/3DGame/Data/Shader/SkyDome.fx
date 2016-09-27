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
texture MainTexture;       //���C���e�N�X�`���[
sampler TextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
};

//****************************************************************
// ���͗p�̍\����
//****************************************************************
struct VS_IN {
	// ���_�̍��W
	float4 Pos : POSITION;

	// �@���x�N�g��
	float3 N : NORMAL;

	// �e�N�Z��
	float2 Tex : TEXCOORD0;
};

//****************************************************************
// ���o�͗p�̍\����
//****************************************************************
struct VS_OUTPUT
{
	// ���_�̍��W
	float4 Pos   : POSITION;

	// ���_�J���[
	float4 Col   : COLOR0;

	// �e�N�Z�����W
	float2 Tex   : TEXCOORD0;

	// �e�N�X�`�����W(�o�͒l)
	float4 posforps : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 Col1 : COLOR0;  //�V�[���̃J���[���
	float4 Col2 : COLOR1;  //�V�[���̋P�x���
};

//****************************************************************
// �o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
VS_OUTPUT VS( VS_IN In) //�e�N�Z��
{
    // �\���̂̏�����
	VS_OUTPUT Out = (VS_OUTPUT)0;

	// ���_�̍��W���s��ϊ�����
	float4x4 mat;
	mat = mul(W, V);
	mat = mul(mat, P);
	Out.Pos = mul(In.Pos, mat);

	Out.Tex = In.Tex;
   
    // �s�N�Z���V�F�[�_�[�p�ɍ��W��ۑ����Ă�
    Out.posforps = In.Pos;
   
    // �e�N�Z���͂��̂܂܊i�[����
    Out.Tex = In.Tex;
   
    // �Ɩ����t�����ɂ���
	float3 L = -normalize(LightDir.xyz);
   
    // �@���x�N�g��
    float3 N = normalize( In.N.xyz );

    //�Ɩ��Ɩ@������dot�֐��ɂ��A���ς��v�Z����B�܂�max�֐��ɂ�蒸�_�J���[�̍ŏ��l�������ɗ}����悤�ɐݒ肷��
	Out.Col = max(AmbientMat, dot(N, L));
       
    return Out;
}

//****************************************************************
// �s�N�Z���V�F�[�_�[
//****************************************************************
PS_OUTPUT PS(VS_OUTPUT In )
{  
    PS_OUTPUT Out;
	
	// �e�N�X�`���̔��f
	Out.Col1 = In.Col * tex2D(TextureSampler, In.Tex);
	Out.Col2 = float4(0, 0, 0, 0);
   
	return Out;
}

//****************************************************************
// �e�N�j�b�N�錾
//****************************************************************
technique SkyDome
{
   pass P0
   {
      VertexShader = compile vs_2_0 VS();
      PixelShader  = compile ps_2_0 PS();   
   }     
}