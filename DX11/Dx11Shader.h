#pragma once
#include "Dx11Util.h"


class Dx11Shader
{
public:
	Dx11Shader();
	~Dx11Shader();

	bool Init(ID3D11Device* device, HWND hwnd, WCHAR* filename);
	void Release();
	bool Render(ID3D11DeviceContext* immediateContext, int indexCount,
		const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);

private:
	bool InitShader(ID3D11Device* device, HWND hwnd, WCHAR* filename);
	void ReleaseShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename);

	bool SetShaderParams(ID3D11DeviceContext* immediateContext,
		const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix);
	void RenderShader(ID3D11DeviceContext* immediateContext, int indexCount);

private:
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
};
