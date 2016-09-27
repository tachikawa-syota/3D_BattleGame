
// �P�e�N�Z�����㉺���E�ʒu�ւ̃I�t�Z�b�g�l
float2 TexOffset;

// �΂ˋ��x�B�l��傫������Ɣg�̈ړ����x�������Ȃ�B
float  SpringPower = 0.5f;

// �ǉ�����g�̃e�N�Z���ʒu
float2 AddWavePos;                   

// �ǉ�����g�̍���
float  AddWaveHeight;                 

// �o�ߎ���
float g_DeltaTime = 0.4f;			

// �g�}�b�v
sampler tex0 : register(s0);       


struct VS_OUTPUT
{
    float4 Pos       : POSITION;
    float2 Tex       : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos    : POSITION,
    float4 Color : COLOR0,
    float2 Tex : TEXCOORD0)
{
    VS_OUTPUT Out;

    Out.Pos = Pos;
    Out.Tex = Tex;

    return Out;
}

//�g�}�b�v�̍X�V����
float4 PS_WaveMap(VS_OUTPUT In) : COLOR0
{

    //�g�}�b�v���猻�݂̍���( R���� )���A1�O�̍���( G���� )���擾
    float4 Wave = tex2D(tex0, In.Tex);

    float old_hight = Wave.g;
    float now_hight = Wave.r;
    float next_hight;

    //���͂̃e�N�Z���̍������擾
    float H1 = tex2D(tex0, In.Tex + float2(TexOffset.x, 0.0f)).r;
    float H2 = tex2D(tex0, In.Tex + float2(0.0f, TexOffset.y)).r;
    float H3 = tex2D(tex0, In.Tex + float2(-TexOffset.x, 0.0f)).r;
    float H4 = tex2D(tex0, In.Tex + float2(0.0f, -TexOffset.y)).r;

    //���x�̍X�V
    //���͂̃e�N�Z���̍����Ǝ����̍����̍����v�Z����B���̒l�����x�ƂȂ�B
    //���݂̑��x�����Z�B
    float c = SpringPower * SpringPower;
    float t = g_DeltaTime * g_DeltaTime;

    next_hight = (((c * t) * (H1 + H2 + H3 + H4)) + (2 - (4 * c * t)) * now_hight - old_hight) * 0.996f;

    //�e�N�X�`�����W�ƒǉ�����g�̋��������l�ȉ��Ȃ�V�K�ɔg��ǉ�
    if (distance(In.Tex, AddWavePos) < 0.005f)
    {
        next_hight += AddWaveHeight;
    }

    return float4(next_hight, now_hight, 1.0f, 1.0f);
}

//�@���}�b�v�쐬
float4 PS_BumpMap(VS_OUTPUT In) : COLOR0
{
    float4 Out;

    //�㉺���E�̃e�N�Z���ʒu�̍������擾����
    float H1 = tex2D(tex0, In.Tex + float2(TexOffset.x, 0.0f)).r;
    float H2 = tex2D(tex0, In.Tex + float2(0.0f, TexOffset.y)).r;
    float H3 = tex2D(tex0, In.Tex + float2(-TexOffset.x, 0.0f)).r;
    float H4 = tex2D(tex0, In.Tex + float2(0.0f, -TexOffset.y)).r;

    //X �����̍����̕ω��ʂ��v�Z����
    //�g�̍������� -1.0f �` 1.0f �͈̔͂Ŋi�[����Ă���̂� 0.0f �` 1.0f �ɕϊ�����
    float tu = 0.5f * (H3 - H1) + 0.5f;

    //Y �����̍����̕ω��ʂ��v�Z����
    //�g�̍������� -1.0f �` 1.0f �͈̔͂Ŋi�[����Ă���̂� 0.0f �` 1.0f �ɕϊ�����
    float tv = 0.5f * (H4 - H2) + 0.5f;

    //�����}�b�s���O�͍��������g�p����B���̂��߃������ɍ��������i�[����
    Out = float4(tu, tv, 1.0f, tex2D(tex0, In.Tex).r * 0.5f + 0.5f);

    return Out;
}

technique Wave
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS_WaveMap();
    }

    pass P1
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS_BumpMap();
    }
}