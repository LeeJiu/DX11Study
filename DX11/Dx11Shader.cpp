#include "Dx11Shader.h"



Dx11Shader::Dx11Shader()
	: mVertexShader(nullptr), mPixelShader(nullptr),
	mLayout(nullptr), mMatrixBuffer(nullptr), mSamplerState(nullptr)
{
}


Dx11Shader::~Dx11Shader()
{
}

bool Dx11Shader::Init(ID3D11Device * device, HWND hwnd, WCHAR* filename)
{
	// vs 와 ps 초기화
	if (!InitShader(device, hwnd, filename)) 
		return false;

	return true;
}

void Dx11Shader::Release()
{
	// vs 와 ps, 그외 관련된 것들 반환
	ReleaseShader();
}

bool Dx11Shader::Render(ID3D11DeviceContext * context, int indexCount,
	const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix,
	ID3D11ShaderResourceView* texture)
{
	// 렌더링에 사용할 셰이더 인자
	if (!SetShaderParams(context, worldMatrix, viewMatrix, projMatrix, texture)) return false;

	// 셰이더를 이용하여 준비된 버퍼를 그린다.
	RenderShader(context, indexCount);

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
	D3D11_SAMPLER_DESC samplerDesc;

	// 초기화
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// vs 컴파일
	hr = D3DX11CompileFromFile(filename, nullptr, nullptr, "TextureVS", "vs_5_0",
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

	// ps 컴파일
	hr = D3DX11CompileFromFile(filename, nullptr, nullptr, "TexturePS", "ps_5_0",
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

	// 버퍼로부터 vs 생성
	hr = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), nullptr, &mVertexShader);
	if (FAILED(hr)) return false;
	// 버퍼로부터 ps 생성
	hr = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), nullptr, &mPixelShader);
	if (FAILED(hr)) return false;


	// 정점 입력 레이아웃 서술
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 레이아웃의 요소 개수
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 정점 입력 레이아웃 생성
	hr = device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(hr)) return false;

	// 더 이상 사용되지 않는 vs buffer 와 ps buffer 해제
	SafeRelease(vertexShaderBuffer);
	SafeRelease(pixelShaderBuffer);

	// vs 에 있는 행렬 상수 버퍼 서술
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(XMMATRIX);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 생성 -> vs 상수 버퍼에 접근
	hr = device->CreateBuffer(&matrixBufferDesc, nullptr, &mMatrixBuffer);
	if (FAILED(hr)) return false;

	// sampler state 서술
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDesc, &mSamplerState);
	if (FAILED(hr)) return false;

	return true;
}

void Dx11Shader::ReleaseShader()
{
	// sampler state 해제
	if (mSamplerState)
		SafeRelease(mSamplerState);

	// 상수 버퍼 해제 
	if (mMatrixBuffer) 
		SafeRelease(mMatrixBuffer);

	// 레이아웃 해제 
	if (mLayout) 
		SafeRelease(mLayout);

	// ps 해제 
	if (mPixelShader) 
		SafeRelease(mPixelShader);

	// vs 해제 
	if (mVertexShader) 
		SafeRelease(mVertexShader);
}

void Dx11Shader::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * filename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// 에러 메세지를 담고 있는 문자열 버퍼의 포인터
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 메세지의 길이
	bufferSize = errorMessage->GetBufferSize();


	// 에러 메세지를 씁니다. 
	fout.open("shader-error.txt");
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}
	fout.close();

	// 에러 메세지 반환
	SafeRelease(errorMessage);

	// 컴파일 에러 알림 메세지
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", filename, MB_OK);
}

bool Dx11Shader::SetShaderParams(ID3D11DeviceContext * context,
	const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projMatrix,
	ID3D11ShaderResourceView* texture)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	unsigned int bufferNumber;

	// matrix transpose -> 셰이더에서 사용 가능
	// 셰이더에서는 매트릭스의 방향이 다르기 때문에 전치해야함.
	XMMATRIX wvp = XMMatrixTranspose(worldMatrix * viewMatrix * projMatrix);
	
	// 상수 버퍼 사용 시작(잠금)
	context->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	// 상수 버퍼에 데이터 카피
	memcpy(mappedResource.pData, &wvp, sizeof(XMMATRIX));
	// 상수 버퍼 사용 끝(잠금 해제)
	context->Unmap(mMatrixBuffer, 0);

	// vs 에서의 상수 버퍼 위치
	bufferNumber = 0;

	// 상수 버퍼 갱신
	context->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);
	context->PSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);
	
	// ps 에 shader texture resource 설정
	context->PSSetShaderResources(0, 1, &texture);

	return true;
}

void Dx11Shader::RenderShader(ID3D11DeviceContext * context, int indexCount)
{
	// 정점 입력 레이아웃 설정
	context->IASetInputLayout(mLayout);

	// vs, ps 설정
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	// ps 에 sampler state 설정
	context->PSSetSamplers(0, 1, &mSamplerState);

	// draw
	context->DrawIndexed(indexCount, 0, 0);
}
