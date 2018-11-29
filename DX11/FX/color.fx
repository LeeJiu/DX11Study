
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

// Vertex Shader : 3���� �������� ��ȯ�Ѵ�.
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// ���� ��ġ ���
	vout.PosH = mul(float4(vin.PosL, 1.0f), worldViewProj);

	// �Է� ���� ����
	vout.Color = vin.Color;

	return vout;
}

// Pixel Shader : ȭ�鿡 �� �ȼ����� ������ �׸���.
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
