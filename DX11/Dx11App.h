#pragma once
#include "Dx11Core.h"
#include "Dx11Shader.h"
#include "Dx11Geometry.h"


class Dx11App : public Dx11Core
{
public:
	Dx11App(HINSTANCE hInstance);
	~Dx11App();

	bool Init();
	void Release();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	float mTheta;
	float mPhi;
	float mRadius;

	POINT mLastMousePos;

	Dx11Shader*		mShader;
	Dx11Geometry*	mGeometry;
};

