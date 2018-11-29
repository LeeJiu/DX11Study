#pragma once
#include "Dx11Util.h"
#include "Timer.h"

class Dx11Core
{
public:
	Dx11Core(HINSTANCE hInstance);
	virtual ~Dx11Core();

	HINSTANCE	CoreInst() const;
	HWND		MainWnd() const;
	float		AspectRatio() const;

	int	Run();

	// Framework Mathods
	virtual bool Init();
	virtual void Release();
	virtual void OnResize();
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// Mouse input handling function
	virtual void OnMouseDown(WPARAM btnState, int x, int y) {}
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {}
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {}

protected:
	bool InitMainWindow();
	bool InitDirect3D();

	void CalculateFrameStats();

protected:
	HINSTANCE	mhCoreInst;		// ���� ���α׷� �ν��Ͻ� �ڵ�
	HWND		mhMainWnd;		// ���� ������ �ڵ�
	bool		mCorePaused;	// ���� ���α׷��� �Ͻ� ����?
	bool		mMinimized;		// �ּ�ȭ?
	bool		mMaximized;		// �ִ�ȭ?
	bool		mResizing;		// ũ�� ������?
	UINT		m4xMsaaQuality;	// 4X MSAA �� ǰ�� ����

	Timer mTimer;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;

	std::wstring mMainWndCaption;	// ������ �̸�

	D3D_DRIVER_TYPE md3dDriverType;	// ��ġ ����

	// ������ ũ��
	int mClientWidth;
	int mClientHeight;

	// 4x msaa ��� �� true
	bool mEnable4xMsaa;
};

