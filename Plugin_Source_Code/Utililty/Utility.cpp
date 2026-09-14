#pragma once
#include "Utility.h"
#include "../Gpu/Gpu.h"

ID3DBlob* CompileShader(
	const wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const string& entrypoint,
	const string& target)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	return byteCode;
}

HRESULT CompileShaderAndSave(const wstring& filename, const D3D_SHADER_MACRO* defines, const string& entrypoint, const string& target, const wstring& saveFilename)
{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	std::ofstream outFile(saveFilename, std::ios::out | std::ios::binary);
	if (!outFile) {
		byteCode->Release();
		return E_FAIL;
	}

	outFile.write((char*)byteCode->GetBufferPointer(), byteCode->GetBufferSize());
	outFile.close();	

	byteCode->Release();

	return hr;
}


HRESULT ReadShaderBlob(const wstring& fileName, char** buffer, size_t* psize) 
{
	// 1. 파일 열기
	std::ifstream inFile(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!inFile) return E_FAIL;

	// 2. 파일 크기 확인 및 버퍼 할당
	std::streamsize size = inFile.tellg();
	inFile.seekg(0, std::ios::beg);
	*psize = size;
	
	//std::vector<char> buffer(size);
	*buffer = (char*)malloc(size);

	if (!inFile.read(*buffer, size)) return E_FAIL;	

	return S_OK;
}





extern LogCallback _logFunc = nullptr;

void RegisterLogCallback(LogCallback cb)
{
	_logFunc = cb;
}

void NativeLog(const char* message)
{
	if (_logFunc != nullptr)
	{
		_logFunc(message);
	}
}

void DoSomethingAndLog()
{
	NativeLog("C++ Native");
}

std::string GetExecutableDirectory() {
	wchar_t buffer[MAX_PATH];
	// 1. 현재 실행 파일의 전체 경로를 얻어옴 (ex: C:\Project\Bin\Game.exe)
	GetModuleFileNameW(NULL, buffer, MAX_PATH);

	// 2. 경로 문자열에서 파일 이름만 제거하여 폴더 경로만 추출
	fs::path exePath(buffer);
	//return exePath.parent_path().wstring(); // C:\Project\Bin 반환
	return exePath.parent_path().string(); // C:\Project\Bin 반환
}


std::string WStringToString(const std::wstring& wstr) {
	if (wstr.empty()) return "";

	size_t sizeNeeded = 0;
	// 필요한 버퍼 크기 구하기
	wcstombs_s(&sizeNeeded, nullptr, 0, wstr.c_str(), 0);

	std::string str(sizeNeeded - 1, '\0'); // '\0' 자리를 제외한 크기로 할당
	wcstombs_s(nullptr, &str[0], sizeNeeded, wstr.c_str(), sizeNeeded);

	return str;
}

// 2. string -> wstring 변환
std::wstring StringToWString(const std::string& str) {
	if (str.empty()) return L"";

	size_t sizeNeeded = 0;
	// 필요한 버퍼 크기 구하기
	mbstowcs_s(&sizeNeeded, nullptr, 0, str.c_str(), 0);

	std::wstring wstr(sizeNeeded - 1, L'\0');
	mbstowcs_s(nullptr, &wstr[0], sizeNeeded, str.c_str(), sizeNeeded);

	return wstr;
}



