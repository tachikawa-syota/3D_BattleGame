
// ���[���h�s��
float4x4 W;

// ���C�g�̃r���[�ϊ��s��
float4x4 LV;

// ���C�g�̎ˉe�ϊ��s��
float4x4 LP;

struct VS_OUTPUT
{
	// ���_�̍��W
   float4 Pos   : POSITION; 

   // �e�N�Z�����W
   float4 Tex   : TEXCOORD0;    
};

//****************************************************************
// �o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
VS_OUTPUT ShadowTexMakeVS(float4 Pos : POSITION) //�e�N�Z��
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//���_�̍��W���s��ϊ�����
	float4x4 mat;
	mat = mul(W, LV);
	mat = mul(mat, LP);
	Out.Pos = mul(Pos, mat);

	// �e�N�X�`�����W�𒸓_�ɍ��킹��
	Out.Tex = Out.Pos;

	return Out;
}

//****************************************************************
// �s�N�Z���V�F�[�_�[
//****************************************************************
float4 ShadowTexMakePS(VS_OUTPUT In) : COLOR
{
	float4 FinalColor;

	FinalColor = In.Tex.z / In.Tex.w;

   return FinalColor;
}

//****************************************************************
// �e�N�j�b�N
//****************************************************************
technique ZShadowMap
{
   pass P0
   {
      VertexShader = compile vs_2_0 ShadowTexMakeVS();
      PixelShader  = compile ps_2_0 ShadowTexMakePS();   
   }     
}