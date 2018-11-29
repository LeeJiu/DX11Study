// Globals ==========================================
// 텍스쳐 자원 -> 모델에 텍스쳐 그릴 때 사용
Texture2D shaderTexture;
// 도형에 셰이딩이 이루어질 때
// 어떻게 텍스쳐의 픽셀이 씌여지는지 수정할 수 있게 해줌
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

	// 도형의 표면 UV좌표에 들어갈 픽셀 값을 결정하고 반환
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
