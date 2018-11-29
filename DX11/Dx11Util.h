#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>
#include <d3dx11async.h>
#include <d3dx11tex.h>

#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <windowsx.h>

#include <vector>

#include "MathHelper.h"

using namespace std;
using namespace DirectX::PackedVector;


#define SafeRelease(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#define SafeDelete(p) { if(p) { delete (p); (p) = nullptr; } }
#define SafeDeleteArr(p) { if(p) { delete[] (p); (p) = nullptr; } }


// dx error check
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
	XMGLOBALCONST XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	XMGLOBALCONST XMVECTORF32 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };

	XMGLOBALCONST XMVECTORF32 Silver = { 0.75f, 0.75f, 0.75f, 1.0f };
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = { 0.69f, 0.77f, 0.87f, 1.0f };
}

struct VertexColor
{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};