#pragma once

#ifndef HEADER_H
#define HHADER_H
#include <Windows.h>


//#include <d3d11.h>
#include <d3d11_4.h>
#include <dxgi1_4.h>
#include <D3dcompiler.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

//#include <DirectXMathVector.inl>
//#include <DirectXMathMatrix.inl>
//#include <DirectXMathMisc.inl>
//#include <DirectXMathConvert.inl>



//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

#include <vector>
#include <array>
#include <list>
#include <unordered_map>
#include <queue>
#include <set>

#include <algorithm>
#include <iterator>
#include <functional>

#include <string>
#include <iostream>
#include <chrono>

#include <thread>

#include <math.h>
#include <ppl.h>

//#include "Transform/Transform.h"
//#include "Animation/AnimationClip.h"
//#include "Animation/BoneCurve.h"


#include <DirectXTexP.h>
#include <DirectXTex.h>
#include <DDS.h>
#include <WICTextureLoader11.h>

#include "Unity/IUnityGraphics.h"
#include "Unity/IUnityGraphicsD3D11.h"


#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxgi.lib")

#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

using namespace std;
//wstring shaderRootPath = L"Shaders/";

#define HLSL_FOLDER L"D:\\NativeRenderingPlugin-master\\UnityProject_NativeRendering\\Assets\\Plugins\\x86_64\\HLSL\\"



#endif