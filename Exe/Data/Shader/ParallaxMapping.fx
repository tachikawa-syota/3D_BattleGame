
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

// ���_�ʒu�x�N�g��
float4 EyePos;

// ���f���̃A���r�G���g
float4 AmbientMat;

// �����̍ő�l�B�傫������قǍ����Ȃ�B
float  m_Param = 0.036f;


sampler s0 : register(s0);        //�I�u�W�F�N�g�̃e�N�X�`���[
//****************************************************************
// ���C���e�N�X�`���[
//****************************************************************
texture MainTexture;		//���C���e�N�X�`���[
sampler MainTextureSampler = sampler_state{
	Texture = < MainTexture >;
	MinFilter = ANISOTROPIC;
	MagFilter = ANISOTROPIC;
	AddressU = Clamp;
	AddressV = Clamp;
};

//****************************************************************
// �V���h�E�}�b�v�e�N�X�`���[
//****************************************************************
texture NormalMapTexture;	//�V���h�E�}�b�v�e�N�X�`���[
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
   float4 Pos      : POSITION;  //���_���W
   float2 Tex      : TEXCOORD0; //�e�N�Z�����W
   float3 Eye      : TEXCOORD1; //���_���W�n�ł̎��������x�N�g��
   float3 Light    : TEXCOORD2; //���_���W�n�ł̒��_ �� ���C�g�ʒu�x�N�g��
};

VS_OUTPUT VS( float3 Pos      : POSITION,   //���_���W
              float3 Tangent  : TANGENT0,   //�ڐ��x�N�g��
              float3 Binormal : BINORMAL0,  //�]�@���x�N�g��
              float3 Normal   : NORMAL,     //�@���x�N�g��
              float2 Tex      : TEXCOORD0   //�e�N�Z��
              )
{
   VS_OUTPUT Out;
   
   // ���_�̍��W���s��ϊ�����
   float4x4 mat;
   mat = mul(W, V);
   mat = mul(mat, P);
   Out.Pos = mul(float4(Pos, 1.0f), mat);

   Out.Tex = Tex;
   
   //�����x�N�g�����v�Z
   float3 Eye = normalize( EyePos.xyz - Pos.xyz );
   
   //�����x�N�g���𒸓_���W�n�ɕϊ�����
   Out.Eye.x = dot( Eye, Tangent );
   Out.Eye.y = dot( Eye, Binormal );
   Out.Eye.z = dot( Eye, Normal );
   Out.Eye = normalize( Out.Eye );
   
   //�I�u�W�F�N�g�̒��_���W -> ���C�g�̈ʒu�x�N�g�� �ɕϊ�����
   float3 Light = -LightDir.xyz;

   //���C�g�x�N�g���𒸓_���W�n�ɕϊ�����
   Out.Light.x = dot( Light, Tangent );
   Out.Light.y = dot( Light, Binormal );
   Out.Light.z = dot( Light, Normal );
   Out.Light = normalize( Out.Light );
   
   return Out;
}

float4 PS( VS_OUTPUT In ) : COLOR
{
   //�����}�b�v���Q�Ƃ��A�������擾����
   //�������̓A���t�@�����Ɋi�[����Ă���
//	float h = tex2D(NormalMapSampler, In.Tex).a;
   
   //�e�N�Z���𒸓_���W�n�ł̎����x�N�g�������ɏd�݂����Ă��炷�B
 //  float2 Tex = In.Tex + m_Param * h * In.Eye.xy;
   
   //�@���}�b�v���Q�Ƃ��A�@�����擾����
   //�@���}�b�v�� -1.0f �` 1.0f �̒l�� 0.0f �` 1.0f �͈̔͂ɕۑ����Ă���̂ŕϊ�����
   float3 Normal = 2.0f * tex2D(NormalMapSampler, In.Tex).xyz - 1.0f;
      
   //�t�H���V�F�[�f�B���O�ɂ��X�y�L�����[�̐F���v�Z����
   //���_ -> ���C�g�ʒu�x�N�g�� + ���_ -> ���_�x�N�g��
   float3 H = normalize( In.Light + In.Eye );
   
   //�X�y�L�����[�J���[���v�Z����
   float S = pow( max( 0.0f, dot( Normal, H ) ), 4.0f ) * 0.55f;
   
   //�}�e���A���F�����肷��
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