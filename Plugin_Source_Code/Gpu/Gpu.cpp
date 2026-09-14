#include "../Header.h"
#include "Gpu.h"
#include "../GPU_Resource/GPU_Resource.h"

using namespace GPU_Resource;

ID3D11Device* Gpu::device = nullptr;
ID3D11DeviceContext* Gpu::context = nullptr;
ID3D11DeviceContext* Gpu::dcontext = nullptr;

UINT Gpu::m4xMsaaQuality = 0;
UINT Gpu::msaaCount = 4;




Gpu::Gpu()
{

}


Gpu::~Gpu()
{

}

int Gpu::Init(IUnityInterfaces* interfaces)
{
	HRESULT hr = S_OK;

	{
		IUnityGraphicsD3D11* d3d = interfaces->Get<IUnityGraphicsD3D11>();
		device = d3d->GetDevice();
	}
	
	{
		device->GetImmediateContext(&context);
		hr = device->CreateDeferredContext(0, &dcontext);
	}


	{
		hr = device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	}

	{
		//Init_RenderState();
	}

	return 0;
}

ID3DBlob* Gpu::CompileShader(
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
