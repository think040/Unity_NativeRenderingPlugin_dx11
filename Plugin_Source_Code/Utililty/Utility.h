#pragma once
//#ifndef UTILITY_H
//#define UTILITY_H

#include <string>


#include <d3d11_4.h>
#include <dxgi1_4.h>
#include <D3dcompiler.h>
#include "../Unity/IUnityInterface.h"


#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace std;

ID3DBlob* CompileShader(
	const wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const string& entrypoint,
	const string& target);

HRESULT CompileShaderAndSave(
	const wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const string& entrypoint,
	const string& target,
	const wstring& saveFilename);

HRESULT ReadShaderBlob(const wstring& fileName, char** buffer, size_t* psize);



// 유니티 로그 함수를 담을 포인터 타입 정의
typedef void (*LogCallback)(const char*);
//typedef void (*LogCallback)(void*);


// 1. 유니티에서 로그 함수 주소를 전달받음
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API RegisterLogCallback(LogCallback cb);


// 2. C++ 내부에서 로그를 남기고 싶을 때 호출할 함수
void NativeLog(const char* message);


extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API DoSomethingAndLog();



#include <windows.h>
#include <iostream>
#include <fstream>
#include <filesystem> 
// C++17 이상 권장

namespace fs = std::filesystem;

std::string GetExecutableDirectory();

//void releaseCom(IUnknown* comObject)
//{
//	if (comObject != nullptr)
//	{
//		comObject->Release();
//		comObject = nullptr;
//	}
//
//}

//#endif

std::string WStringToString(const std::wstring& wstr);

// 2. string -> wstring 변환
std::wstring StringToWString(const std::string& str);



