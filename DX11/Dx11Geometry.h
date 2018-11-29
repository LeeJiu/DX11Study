#pragma once
#include "Dx11Util.h"
#include "Dx11Texture.h"

class Dx11Geometry
{
public:
	Dx11Geometry();
	~Dx11Geometry();

	bool Init(ID3D11Device* device, WCHAR* filename);
	void Release();
	void Render(ID3D11DeviceContext* context);
	
	int GetIndexCount() const;
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitBuffers(ID3D11Device* device);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext* context);
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

private:
	// vertex buffer & index buffer
	ID3D11Buffer*	mVBuffer;
	ID3D11Buffer*	mIBuffer;
	int				mVertexCount;
	int				mIndexCount;

	// Texture
	Dx11Texture*	mTexture;

	// vertex texture type
	struct VertexTex
	{
		XMFLOAT3 Position;
		XMFLOAT2 Texture;
	};
};

