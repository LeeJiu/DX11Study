
cbuffer MatrixBuffer : register(b0)
{
	matrix worldViewProj;
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

// Vertex Shader : 3차원 공간으로 변환한다.
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// 정점 위치 계산
	vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);

	// 입력 색상 저장
	vout.Color = vin.Color;

	return vout;
}

// Pixel Shader : 화면에 각 픽셀들을 실제로 그린다.
float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
