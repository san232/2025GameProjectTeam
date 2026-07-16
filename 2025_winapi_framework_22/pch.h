#pragma once
// ǥ�� ���
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<tchar.h>
#include<memory>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<set>
#include<string>
#include<algorithm>
#include<ctime>
#include<cmath>
#include<format>
#include<assert.h>
#include<filesystem>

namespace fs = std::filesystem;

// My ���
#include "Defines.h"
#include "Vec2.h"
#include "Enums.h"
#include "TimeManager.h"
#include "GDISelector.h"

// ���� ���̺귯��
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

// DirectX 11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using Microsoft::WRL::ComPtr;

// ���� using
using std::vector;
using std::list;
using std::map;
using std::wstring;

// ���� define��
#define PI 3.14159265358979f
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


// �޸� ��
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


// ������ �ܼ�â
#ifdef _DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif
#include<iostream>
using std::cout;
using std::endl;
