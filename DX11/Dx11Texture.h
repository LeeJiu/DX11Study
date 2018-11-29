#pragma once
#include "Dx11Util.h"

class Dx11Texture
{
public:
	Dx11Texture();
	~Dx11Texture();

	bool Init(ID3D11Device* device, WCHAR* filename);
	void Release();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* mTexture;
};

