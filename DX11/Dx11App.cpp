#include "Dx11App.h"

Dx11App::Dx11App(HINSTANCE hInstance) : Dx11Core(hInstance)
{
}

Dx11App::~Dx11App()
{
}

bool Dx11App::Init()
{
	if (!Dx11Core::Init())
		return false;

	return true;
}

void Dx11App::OnResize()
{
	Dx11Core::OnResize();
}

void Dx11App::UpdateScene(float dt)
{
}

void Dx11App::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	md3dImmediateContext->ClearRenderTargetView(
		mRenderTargetView, reinterpret_cast<const float*>(&Colors::Blue));
	md3dImmediateContext->ClearDepthStencilView(
		mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	HR(mSwapChain->Present(0, 0));
}
