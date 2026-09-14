#pragma once
#include "../Header.h"
//#include "../GPU_Resource/GPU_Resource.h"
//#include "../../../source/Utility.h"

#ifndef GPU_H
#define GPU_H

using namespace DirectX;




class Gpu
{
public:
	Gpu();
	~Gpu();

	static int Init(IUnityInterfaces* interfaces);


public:
	static ID3D11Device* device;
	static ID3D11DeviceContext* context;
	static ID3D11DeviceContext* dcontext;


	static bool      m4xMsaaState;
	static UINT      m4xMsaaQuality;
	static UINT		 msaaCount;
	static bool		useMsaa;	

public:
	static ID3DBlob* CompileShader(
		const wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const string& entrypoint,
		const string& target);

public:
	
};


#endif