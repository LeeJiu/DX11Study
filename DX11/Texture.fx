// Globals ==========================================
// �ؽ��� �ڿ� -> �𵨿� �ؽ��� �׸� �� ���
Texture2D shaderTexture;
// ������ ���̵��� �̷���� ��
// ��� �ؽ����� �ȼ��� ���������� ������ �� �ְ� ����
SamplerState sampleType;
//===================================================

cbuffer MatrixBuffer : register(b0)
{
	matrix worldViewProj;
};

struct VertexIn
{
	float3 Pos  : POSITION;
	float2 Tex	: TEXCOORD0;
};

struct PixelIn
{
	float4 Pos	: SV_POSITION;
	float2 Tex	: TEXCOORD0;
};

//struct VertexOut
//{
//	float4 Pos  : SV_POSITION;
//	float4 Color : COLOR;
//};

// Textrue Vertex Shader
PixelIn TextureVS(VertexIn vin)
{
	PixelIn pout;

	pout.Pos = mul(float4(vin.Pos, 1.0f), worldViewProj);

	pout.Tex = vin.Tex;

	return pout;
}

// Texture Pixel Shader
float4 TexturePS(PixelIn pin) : SV_Target
{
	float4 textureColor;

	// ������ ǥ�� UV��ǥ�� �� �ȼ� ���� �����ϰ� ��ȯ
	textureColor = shaderTexture.Sample(sampleType, pin.Tex);

	return textureColor;
}

technique11 TextureTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
