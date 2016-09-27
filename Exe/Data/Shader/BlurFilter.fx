// �K�E�X�t�B���^�[

// �X�N���[���̕�
float MapWidth;
// �X�N���[���̍���
float MapHeight;
// �������킹��ۂ̏d��
float weight[8];
// �ڂ��������t���O
bool isXY;

//****************************************************************
//���C���e�N�X�`���[
//****************************************************************
sampler Maintexture : register(s0);  //�u���[�������錳�ƂȂ�e�N�X�`���[

texture	SrcTexture;	// �ڂ����ΏۂƂȂ�e�N�X�`��
sampler TextureSampler2 = sampler_state{
	Texture = <SrcTexture>;
	MinFilter= LINEAR;		// ���j�A�t�B���^�i�k�����j
	MagFilter= LINEAR;		// ���j�A�t�B���^�i�g�厞�j
	MipFilter= NONE;		// �~�b�v�}�b�v
};

//****************************************************************
//���͗p�̍\����
//****************************************************************
struct VS_OUTPUT {
	float4 Pos : POSITION;  //���_�̍��W
	float2 Tex : TEXCOORD0; //�e�N�Z��
};

//****************************************************************
//�o�[�e�b�N�X�V�F�[�_�[
//****************************************************************
VS_OUTPUT VS( VS_OUTPUT In) //�e�N�Z��
{
    //�\���̂̏�����
    VS_OUTPUT Out = (VS_OUTPUT)0;
     
	// �s��ϊ����Ȃ��̂ł��̂܂܃s�N�Z���V�F�[�_�[�ɓn��
    Out.Pos = In.Pos; 
    Out.Tex = In.Tex;
       
    return Out;
}

//****************************************************************
//�s�N�Z���V�F�[�_�[
//****************************************************************
float4 PS(VS_OUTPUT In ) : COLOR0
{
	float4	Color = (float4)0;

	if (isXY)    //X�����ɂڂ���
	{
		// �e�N�X�`�����W��␳
		In.Tex.x = In.Tex.x + 0.5f / MapWidth;
		In.Tex.y = In.Tex.y + 0.5f / MapHeight;

		Color = weight[0] * tex2D(TextureSampler2, In.Tex);
		Color += weight[1]

			* (tex2D(TextureSampler2, In.Tex + float2(+2.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-2.0f / MapWidth, 0)));
		Color += weight[2]
			* (tex2D(TextureSampler2, In.Tex + float2(+4.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-4.0f / MapWidth, 0)));
		Color += weight[3]
			* (tex2D(TextureSampler2, In.Tex + float2(+6.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-6.0f / MapWidth, 0)));
		Color += weight[4]
			* (tex2D(TextureSampler2, In.Tex + float2(+8.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-8.0f / MapWidth, 0)));
		Color += weight[5]
			* (tex2D(TextureSampler2, In.Tex + float2(+10.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-10.0f / MapWidth, 0)));
		Color += weight[6]
			* (tex2D(TextureSampler2, In.Tex + float2(+12.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-12.0f / MapWidth, 0)));
		Color += weight[7]
			* (tex2D(TextureSampler2, In.Tex + float2(+14.0f / MapWidth, 0))
			+ tex2D(TextureSampler2, In.Tex + float2(-14.0f / MapWidth, 0)));
	}
	else{
		// �e�N�X�`�����W�̂�����Ȃ���
		In.Tex.x = In.Tex.x + 0.5 / MapWidth;
		In.Tex.y = In.Tex.y + 0.5 / MapHeight;

		Color = weight[0] * tex2D(TextureSampler2, In.Tex);
		Color += weight[1]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +2.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -2.0f / MapHeight)));
		Color += weight[2]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +4.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -4.0f / MapHeight)));
		Color += weight[3]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +6.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -6.0f / MapHeight)));
		Color += weight[4]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +8.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -8.0f / MapHeight)));
		Color += weight[5]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +10.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -10.0f / MapHeight)));
		Color += weight[6]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +12.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -12.0f / MapHeight)));
		Color += weight[7]
			* (tex2D(TextureSampler2, In.Tex + float2(0, +14.0f / MapHeight))
			+ tex2D(TextureSampler2, In.Tex + float2(0, -14.0f / MapHeight)));
	}

	return Color;
}

//****************************************************************
//�e�N�j�b�N�錾
//****************************************************************
// Shader_Chass_default TShader
technique BlurFilter
{
   pass P0
   {
      VertexShader = compile vs_3_0 VS();
      PixelShader  = compile ps_3_0 PS();   
   }     
}