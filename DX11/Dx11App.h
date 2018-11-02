#pragma once
#include "Dx11Core.h"

class Dx11App : public Dx11Core
{
public:
	Dx11App(HINSTANCE hInstance);
	~Dx11App();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
};

