#include "Dx11Geometry.h"



Dx11Geometry::Dx11Geometry()
{
}


Dx11Geometry::~Dx11Geometry()
{
}

bool Dx11Geometry::Init(ID3D11Device* device)
{
	// Create vertex buffer
	Vertex vertices[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), (const XMFLOAT4)Colors::White },
		{ XMFLOAT3(-0.5f, +0.5f, -0.5f), (const XMFLOAT4)Colors::Black },
		{ XMFLOAT3(+0.5f, +0.5f, -0.5f), (const XMFLOAT4)Colors::Red },
		{ XMFLOAT3(+0.5f, -0.5f, -0.5f), (const XMFLOAT4)Colors::Green },
		{ XMFLOAT3(-0.5f, -0.5f, +0.5f), (const XMFLOAT4)Colors::Blue },
		{ XMFLOAT3(-0.5f, +0.5f, +0.5f), (const XMFLOAT4)Colors::Yellow },
		{ XMFLOAT3(+0.5f, +0.5f, +0.5f), (const XMFLOAT4)Colors::Cyan },
		{ XMFLOAT3(+0.5f, -0.5f, +0.5f), (const XMFLOAT4)Colors::Magenta }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(device->CreateBuffer(&vbd, &vinitData, &mVBuffer));


	// Create the index buffer
	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(device->CreateBuffer(&ibd, &iinitData, &mIBuffer));

	mIndexCount = 36;

	return true;
}

void Dx11Geometry::Release()
{
	if(mVBuffer)
		SafeRelease(mVBuffer);

	if(mIBuffer)
		SafeRelease(mIBuffer);
}

void Dx11Geometry::Render(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Dx11Geometry::GetIndexCount() const
{
	return mIndexCount;
}
