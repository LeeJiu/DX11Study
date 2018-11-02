#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <D3DX11.h>
#include <DirectXMath.h>
#include <string>
#include <cassert>


#define SafeRelease(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#define SafeDelete(p) { if(p) { delete (p); (p) = nullptr; } }
#define SafeDeleteArr(p) { if(p) { delete[] (p); (p) = nullptr; } }



// dx error check
// dxsdk 가 windows 10 에 포함되면서
// DXTrace()가 사라짐
// -> FormatMessage() & MessageBox() 로 대체해서 에러 메세지 띄움
// 출처: https://wergia.tistory.com/6 [베르의 프로그래밍 노트]
#if defined(DEBUG) || defined(_DEBUG)
#ifndef HR
#define HR(x)                                            \
    {                                                        \
        HRESULT hr = (x);                                    \
        if(FAILED(hr))                                       \
        {                                                    \
            LPWSTR output;                                   \
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |       \
                FORMAT_MESSAGE_IGNORE_INSERTS      |         \
                FORMAT_MESSAGE_ALLOCATE_BUFFER,              \
                NULL,                                        \
                hr,                                          \
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   \
                (LPTSTR) &output,                            \
                0,                                           \
                NULL);                                       \
            MessageBox(NULL, output, L"Error", MB_OK);       \
        }                                                    \
    }
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif


namespace Colors
{
	XMGLOBALCONST DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST DirectX::XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST DirectX::XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}