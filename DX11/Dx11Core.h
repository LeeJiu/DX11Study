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
	HINSTANCE	mhCoreInst;		// 응용 프로그램 인스턴스 핸들
	HWND		mhMainWnd;		// 메인 윈도우 핸들
	bool		mCorePaused;	// 응용 프로그램이 일시 정지?
	bool		mMinimized;		// 최소화?
	bool		mMaximized;		// 최대화?
	bool		mResizing;		// 크기 조정중?
	UINT		m4xMsaaQuality;	// 4X MSAA 의 품질 수준

	Timer mTimer;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;

	std::wstring mMainWndCaption;	// 윈도우 이름

	D3D_DRIVER_TYPE md3dDriverType;	// 장치 종류

	// 윈도우 크기
	int mClientWidth;
	int mClientHeight;

	// 4x msaa 사용 시 true
	bool mEnable4xMsaa;
};

