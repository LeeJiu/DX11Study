#include "Dx11Shader.h"



Dx11Shader::Dx11Shader()
{
}


Dx11Shader::~Dx11Shader()
{
}

bool Dx11Shader::Init(ID3D11Device * device, HWND hwnd, WCHAR* filename)
{
	// vs �� ps �ʱ�ȭ
	if (!InitShader(device, hwnd, filename)) 
		return false;

	return true;
}

void Dx11Shader::Release()
{
	// vs �� ps, �׿� ���õ� �͵� ��ȯ
	ReleaseShader();
}

bool Dx11Shader::Render(ID3D11DeviceContext * immediateContext, int indexCount, 
	const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	// �������� ����� ���̴� ����
	if (!SetShaderParams(immediateContext, worldMatrix, viewMatrix, projMatrix)) return false;

	// ���̴��� �̿��Ͽ� �غ�� ���۸� �׸���.
	RenderShader(immediateContext, indexCount);

	return true;
}

bool Dx11Shader::InitShader(ID3D11Device * device, HWND hwnd, WCHAR * filename)
{
	HRESULT hr;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// �ʱ�ȭ
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// vs ������
	hr = D3DX11CompileFromFile(filename, nullptr, nullptr, "VS", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &vertexShaderBuffer,
		&errorMessage, nullptr);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"VertexShader");
		}
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// ps ������
	hr = D3DX11CompileFromFile(filename, nullptr, nullptr, "PS", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, &pixelShaderBuffer,
		&errorMessage, nullptr);

	if (FAILED(hr))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, L"PixelShader");
		}
		else
		{
			MessageBox(hwnd, filename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	// ���۷κ��� vs ����
	hr = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), nullptr, &mVertexShader);
	if (FAILED(hr)) return false;
	// ���۷κ��� ps ����
	hr = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), nullptr, &mPixelShader);
	if (FAILED(hr)) return false;


	// ���� �Է� ���̾ƿ� ����
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// ���̾ƿ��� ��� ����
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// ���� �Է� ���̾ƿ� ����
	hr = device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(hr)) return false;

	// �� �̻� ������ �ʴ� vs buffer �� ps buffer ����
	SafeRelease(vertexShaderBuffer);
	SafeRelease(pixelShaderBuffer);

	// vs �� �ִ� ��� ��� ���� ����
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(XMMATRIX);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ���� -> vs ��� ���ۿ� ����
	hr = device->CreateBuffer(&matrixBufferDesc, nullptr, &mMatrixBuffer);
	if (FAILED(hr)) return false;

	return true;
}

void Dx11Shader::ReleaseShader()
{
	// ��� ���� ���� 
	if (mMatrixBuffer) 
		SafeRelease(mMatrixBuffer);

	// ���̾ƿ� ���� 
	if (mLayout) 
		SafeRelease(mLayout);

	// ps ���� 
	if (mPixelShader) 
		SafeRelease(mPixelShader);

	// vs ���� 
	if (mVertexShader) 
		SafeRelease(mVertexShader);
}

void Dx11Shader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * filename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// ���� �޼����� ��� �ִ� ���ڿ� ������ ������
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// �޼����� ����
	bufferSize = errorMessage->GetBufferSize();


	// ���� �޼����� ���ϴ�. 
	fout.open("shader-error.txt");
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	fout.close();

	// ���� �޼��� ��ȯ
	SafeRelease(errorMessage);

	// ������ ���� �˸� �޼���
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", filename, MB_OK);
}

bool Dx11Shader::SetShaderParams(ID3D11DeviceContext * immediateContext, 
	const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	unsigned int bufferNumber;

	// matrix transpose -> ���̴����� ��� ����
	// ���̴������� ��Ʈ������ ������ �ٸ��� ������ ��ġ�ؾ���.
	XMMATRIX wvp = XMMatrixTranspose(worldMatrix * viewMatrix * projMatrix);
	
	// ��� ���� ��� ����(���)
	immediateContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// ��� ���ۿ� ������ ī��
	memcpy(mappedResource.pData, &wvp, sizeof(XMMATRIX));
	// ��� ���� ��� ��(��� ����)
	immediateContext->Unmap(mMatrixBuffer, 0);

	// vs ������ ��� ���� ��ġ
	bufferNumber = 0;

	// ��� ���� ����
	immediateContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);
	immediateContext->PSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	return true;
}

void Dx11Shader::RenderShader(ID3D11DeviceContext * immediateContext, int indexCount)
{
	// ���� �Է� ���̾ƿ� ����
	immediateContext->IASetInputLayout(mLayout);

	// vs, ps ����
	immediateContext->VSSetShader(mVertexShader, nullptr, 0);
	immediateContext->PSSetShader(mPixelShader, nullptr, 0);

	// draw
	immediateContext->DrawIndexed(indexCount, 0, 0);
}
