#pragma once
#include "Dx11Util.h"

class Dx11Geometry
{
public:
	Dx11Geometry();
	~Dx11Geometry();

	bool Init(ID3D11Device* device);
	void Release();
	void Render(ID3D11DeviceContext* context);
	int GetIndexCount() const;

private:
	// vertex buffer & index buffer
	ID3D11Buffer*	mVBuffer;
	ID3D11Buffer*	mIBuffer;
	int				mIndexCount;
};

