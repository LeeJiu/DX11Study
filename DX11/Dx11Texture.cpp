#include "Dx11Texture.h"



Dx11Texture::Dx11Texture()
{
}

Dx11Texture::~Dx11Texture()
{
}

bool Dx11Texture::Init(ID3D11Device * device, WCHAR * filename)
{
	// Load texture
	HR(D3DX11CreateShaderResourceViewFromFile(device, filename, nullptr, nullptr, &mTexture, nullptr));

	return true;
}

void Dx11Texture::Release()
{
	if (mTexture)
		SafeRelease(mTexture);
}

ID3D11ShaderResourceView * Dx11Texture::GetTexture()
{
	return mTexture;
}
