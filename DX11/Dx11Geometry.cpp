#include "Dx11Geometry.h"



Dx11Geometry::Dx11Geometry() 
	: mVBuffer(nullptr), mIBuffer(nullptr), mTexture(nullptr)
{
}


Dx11Geometry::~Dx11Geometry()
{
}

bool Dx11Geometry::Init(ID3D11Device * device, WCHAR * filename)
{
	HR(InitBuffers(device));

	HR(LoadTexture(device, filename));

	return true;
}

void Dx11Geometry::Release()
{
	ReleaseTexture();
	ReleaseBuffers();
}

void Dx11Geometry::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

int Dx11Geometry::GetIndexCount() const
{
	return mIndexCount;
}

ID3D11ShaderResourceView * Dx11Geometry::GetTexture()
{
	return mTexture->GetTexture();
}

bool Dx11Geometry::InitBuffers(ID3D11Device * device)
{
	// Create vertex buffer
	VertexTex vertices[] =
	{
		// front	/ 0 1 2 3
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f)},
		{ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f)},
		// back		/ 4 5 6 7
		{ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT2(1.0f, 1.0f)},
		{ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT2(1.0f, 0.0f)},
		{ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT2(0.0f, 0.0f)},
		{ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT2(0.0f, 1.0f)},
		// left		/ 8 9 10 11
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT2(0.0f, 1.0f) },
		// right	/ 12 13 14 15
		{ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT2(1.0f, 1.0f) },
		// top		/ 16 17 18 19
		{ XMFLOAT3(-0.5f, +0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, +0.5f, +0.5f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+0.5f, +0.5f, +0.5f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+0.5f, +0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
		// bottom	/ 20 21 22 23
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, +0.5f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+0.5f, -0.5f, +0.5f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(+0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f) }
	};

	mVertexCount = sizeof(vertices) / sizeof(VertexTex);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexTex) * mVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;

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
		8, 10, 9,
		8, 11, 10,
		// right face
		12, 13, 14,
		12, 14, 15,
		// top face
		16, 17, 18,
		16, 18, 19,
		// bottom face
		20, 22, 21,
		20, 23, 22
	};

	mIndexCount = sizeof(indices) / sizeof(UINT);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	
	HR(device->CreateBuffer(&ibd, &iinitData, &mIBuffer));

	return true;
}

void Dx11Geometry::ReleaseBuffers()
{
	if (mVBuffer)
		SafeRelease(mVBuffer);

	if (mIBuffer)
		SafeRelease(mIBuffer);
}

void Dx11Geometry::RenderBuffers(ID3D11DeviceContext * context)
{
	UINT stride = sizeof(VertexTex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVBuffer, &stride, &offset);
	context->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Dx11Geometry::LoadTexture(ID3D11Device * device, WCHAR * filename)
{
	mTexture = new Dx11Texture();

	HR(mTexture->Init(device, filename));

	return true;
}

void Dx11Geometry::ReleaseTexture()
{
	if (mTexture)
		SafeRelease(mTexture);
}
