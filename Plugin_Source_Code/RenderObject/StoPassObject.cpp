#include "../Header.h"
#include "../Utililty/Utility.h"
#include "StoPassObject.h"
#include "../RenderManager.h"
#include "../Gpu/Gpu.h"

vector<StoPassObject*>* StoPassObject::objects = nullptr;


typedef void (*funcInit)(void* instance);
typedef void (*funcUpdate)(void* instance);
typedef void (*funcRender)(void* instance);
typedef void (*funcDestroy)(void* instance);

typedef int (*funcGetId)(void* instance);

//typedef void (*funcIssueUpdate)(int id);
//typedef void (*funcIssueBeginContext)(int id);
//typedef void (*funcIssueBeginCamera)(int id);
//typedef void (*funcIssueRender)(int id);
//typedef void (*funcIssueEndCamera)(int id);
//typedef void (*funcIssueEndContext)(int id);
//
//typedef void (*funcCompute_Tex3D)(void* instance);
//typedef bool (*funcCheckGPUCompleted)(void* instance);
//typedef void (*funcSetTextureFromUnity)(void* instance, void* nativeTexturePtr);
//typedef void (*funcSetLightDataFromUnity)(void* instance, void** ppData);
//typedef void (*funcSetObjDataFromUnity)(void* instance, void** ppData);
////SetObjDataFromUnity
//
//struct IRenderActor
//{
//	funcInit	Init;
//	funcUpdate	Update;
//	funcRender	Render;
//	funcDestroy Destroy;
//	
//	funcGetId		GetId;
//
//	funcIssueUpdate			IssueUpdate;
//	funcIssueBeginContext	IssueBeginContext;
//	funcIssueBeginCamera	IssueBeginCamera;
//	funcIssueRender			IssueRender;			
//	funcIssueEndCamera		IssueEndCamera;
//	funcIssueEndContext		IssueEndContext;
//
//	funcCompute_Tex3D			Compute_Tex3D;
//	funcCheckGPUCompleted		CheckGPUCompleted;
//	funcSetTextureFromUnity		SetTextureFromUnity;
//	funcSetLightDataFromUnity	SetLightDataFromUnity;
//	funcSetObjDataFromUnity		SetObjDataFromUnity	;
//};

extern "C" UNITY_INTERFACE_EXPORT  void* UNITY_INTERFACE_API Create_StoPassObject()
{
	StoPassObject* object = new StoPassObject();

	if (StoPassObject::objects == nullptr)
	{
		StoPassObject::objects = new vector<StoPassObject*>();
	}

	object->id_obj = StoPassObject::objects->size();
	StoPassObject::objects->push_back(object);
	
	if (RenderManager::objects != nullptr)
	{
		RenderManager::objects->push_back(object);
	}

	return object;
}

StoPassObject::IRenderActor* renActor = nullptr;

extern "C"  UNITY_INTERFACE_EXPORT StoPassObject::IRenderActor* UNITY_INTERFACE_API GetFuction_StoPassObject()
{
	if (renActor == nullptr)
	{
		renActor = new StoPassObject::IRenderActor();
	}

	renActor->Init =
		[](void* instance)
		{
			StoPassObject* object = (StoPassObject*)instance;
			object->Init();
		};

	renActor->Update =
		[](void* instance)
		{
			StoPassObject* object = (StoPassObject*)instance;
			object->Update();
		};

	renActor->Render =
		[](void* instance)
		{
			StoPassObject* object = (StoPassObject*)instance;
			object->Render();
		};

	renActor->Destroy =
		[](void* instance)
		{
			StoPassObject* object = (StoPassObject*)instance;
			object->Destroy();
		};


	renActor->GetId =
		[](void* instance)
		{
			StoPassObject* object = (StoPassObject*)instance;

			return object->id_obj;
		};



	renActor->IssueUpdate =
		[](int id)
		{
			if (StoPassObject::objects == nullptr) { return; }


			StoPassObject* object = (*StoPassObject::objects)[id];

			if (object != nullptr)
			{
				//object->EventUpdate();
				object->Compute_Tex3D();
			}
			
		};

	renActor->IssueBeginContext =
		[](int id)
		{
			if (StoPassObject::objects == nullptr) { return; }

			StoPassObject* object = (*StoPassObject::objects)[id];

			if (object != nullptr)
			{
				object->EventBeginContext();
			}
			
		};

	renActor->IssueBeginCamera =
		[](int id)
		{
			if (StoPassObject::objects == nullptr) { return; }

			StoPassObject* object = (*StoPassObject::objects)[id];

			if (object != nullptr)
			{
				object->EventBeginCamera();
			}
			
		};

	renActor->IssueRender =
		[](int id)
		{
			if (StoPassObject::objects == nullptr) { return; }

			StoPassObject* object = (*StoPassObject::objects)[id];

			if (object != nullptr)
			{
				object->EventRender();
			}
			
		};


	renActor->IssueEndCamera =
		[](int id)
		{
			if (StoPassObject::objects == nullptr) { return; }

			StoPassObject* object = (*StoPassObject::objects)[id];

			if (object != nullptr)
			{
				object->EventEndCamera();
			}
			
		};

	renActor->IssueEndContext =
		[](int id)
		{
			if (StoPassObject::objects == nullptr) { return; }

			StoPassObject* object = (*StoPassObject::objects)[id];

			if (object != nullptr)
			{
				object->EventEndContext();
			}
			
		};

	renActor->Compute_Tex3D =
		[](void* instance)
		{
			StoPassObject* object = (StoPassObject*)instance;
			object->Compute_Tex3D();
		};
	
	renActor->CheckGPUCompleted =
		[](void* instance)
		{
			StoPassObject* object = (StoPassObject*)instance;
			return object->fence->IsCompleted();
		};

	renActor->SetTextureFromUnity =
		[](void* instance, void* ptrTex)
		{
			StoPassObject* object = (StoPassObject*)instance;
			return object->SetTexureFromUnity(ptrTex);
		};

	renActor->SetLightDataFromUnity = 
		[](void* instance, void** ppData)
		{
			StoPassObject* object = (StoPassObject*)instance;
			return object->SetLightDataFromUnity(ppData);
		};

	renActor->SetObjDataFromUnity =
		[](void* instance, void** ppData)
		{
			StoPassObject* object = (StoPassObject*)instance;
			return object->SetObjDataFromUnity(ppData);
		};

	return renActor;
}


void StoPassObject::Init()
{
	auto device = Gpu::device;
	auto context = Gpu::context;

	{
		string str = string("StoPassObject_Init()_id") + std::to_string(id_obj);
		NativeLog(str.c_str());
	}
	
	{
		//string str = string(RenderManager::exePath);
		
		//NativeLog(.)
	}
	
	{
		//mesh = Mesh::CreateCubeMesh();
		mesh = Mesh::CreateTorusMesh();
	}
	
	{
		InitResource();
		InitTexture();
		
		InitShader();		
	}
	
	
	{
		fence = new DX11Fence();
	}
	
	{
		D3D11_QUERY_DESC desc = {};
		desc.Query = D3D11_QUERY_EVENT; // GPU의 실행 타임라인을 추적하는 이벤트 타입
		desc.MiscFlags = 0;
	
		device->CreateQuery(&desc, &fence_read_back);
	}
	
}

void StoPassObject::Begin()
{

}

void StoPassObject::Update()
{
	string str = string("StoPassObject_Update()_id") + std::to_string(id_obj);
	NativeLog(str.c_str());

	auto context = Gpu::context;

	//{
	//	auto data = (Light*)lightBuffer->data;
	//	lightBuffer->WriteToBuffer(context);
	//}	

	//{
	//	auto data = (Object*)objBuffer->data;
	//	objBuffer->WriteToBuffer(context);
	//}
}

void StoPassObject::Render()
{
	string str = string("StoPassObject_Render()_id") + std::to_string(id_obj);
	NativeLog(str.c_str());

	HRESULT hr = S_OK;

	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;
	auto cmdList = RenderManager::cmdList;


	{
		objBuffer->WriteToBuffer(context);
		lightBuffer->WriteToBuffer(context);		
	}

	{		
		objBuffer->CopyToBuffer(dcontext);
		lightBuffer->CopyToBuffer(dcontext);
	}	


	//int i = 0.0f;
	for (int i = 0; i < 2; i++)
	{
		{
			auto data = (RenderInfo*)renInfoBuffer->data;
			data->data = XMVectorSet(i, 0.0f, 0.0f, 0.0f);

			renInfoBuffer->WriteToBuffer(context);
			renInfoBuffer->CopyToBuffer(dcontext);
		}
	
		//{
		//	RenderSO();
		//	RenderAuto();
		//}

		{
			RenderSO();
			RenderIndex();
		}

		{
			hr = dcontext->FinishCommandList(true, &cmdList);
			context->ExecuteCommandList(cmdList, true);
		}
	}


	{
		D3D11_QUERY_DATA_SO_STATISTICS soResult = {};

		while (S_OK != context->GetData(pQuery, &soResult, sizeof(D3D11_QUERY_DATA_SO_STATISTICS), 0)) {}
		int vtxCount = soResult.NumPrimitivesWritten * 3;
		{
			stoBuffer->ReadFromBuffer(context);
		}
	}

	{
		vtxBuffer_so->ReadFromBuffer(context);
	}
	
}

void StoPassObject::Destroy()
{
	this->id_obj = -1;

	{
		//if (objects != nullptr)
		//{
		//	{
		//		auto it = std::find(objects->begin(), objects->end(), this);
		//
		//		if (it != objects->end())
		//		{
		//			objects->erase(it);
		//		}
		//	}
		//
		//	if (objects->size() == 0)
		//	{
		//		delete objects;
		//		objects = nullptr;
		//	}
		//}

		if (objects != nullptr)
		{
			delete objects;
			objects = nullptr;		
		}
	}

	//{
	//	auto objs = RenderManager::objects;
	//
	//	if (objs != nullptr)
	//	{
	//		{
	//			auto it = std::find(objs->begin(), objs->end(), this);
	//
	//			if (it != objs->end())
	//			{
	//				objs->erase(it);
	//			}
	//		}			
	//	}
	//}
	

	string str = string("StoPassObject_Destroy()_id") + std::to_string(id_obj);
	NativeLog(str.c_str());
}


void StoPassObject::EventUpdate()
{
	string str = std::format("StoPassObject_EventUpdate()_id{}", id_obj);

	//NativeLog(str.c_str());

	//Update();
}

void StoPassObject::EventBeginContext()
{
	string str = std::format("StoPassObject_EventBeginContext()_id{}", id_obj);

	//Update();

	//NativeLog(str.c_str());
}

void StoPassObject::EventBeginCamera()
{
	string str = std::format("StoPassObject_EventBeginCamera()_id{}", id_obj);

	//NativeLog(str.c_str());
}

void StoPassObject::EventRender()
{
	string str = std::format("StoPassObject_EventRender()_id{}", id_obj);

	//NativeLog(str.c_str());	

	Render();
}

void StoPassObject::EventEndCamera()
{
	string str = std::format("StoPassObject_EventEndCamera()_id{}", id_obj);

	//NativeLog(str.c_str());
}

void StoPassObject::EventEndContext()
{
	string str = std::format("StoPassObject_EventEndContext()_id{}", id_obj);

	//NativeLog(str.c_str());
}


int StoPassObject::InitShader()
{
	HRESULT hr = S_OK;
	auto device = Gpu::device;
	
	wstring shaderRootPath;

	if (RenderManager::onEditor)
	{
		shaderRootPath = StringToWString(RenderManager::assetPath) + L"\\Assets\\Plugins\\x86_64\\HLSL\\";
	}
	else
	{
		shaderRootPath = StringToWString(RenderManager::exePath) + L"\\HLSL\\";
	}

	NativeLog(WStringToString(shaderRootPath).c_str());

	{
		InitShader_Compute(shaderRootPath);
		InitResourece_Compute();
	}

	shaderRootPath += L"StoPass\\01_GShader\\";

	//shaderRootPath += L"StoPipeline\\01_GShader\\";

#define ROW_MAJOR "0"
#define COLUMN_MAJOR "1"

	D3D_SHADER_MACRO macros[] = {
		//{"ROW_MAJOR", NULL}, 
		{"COLUMN_MAJOR", NULL},
		{"USE_TEXTURE", NULL},
		{ NULL, NULL } };

	//StoPass
	{
		//wstring shaderFileName = L"StoPass.hlsl";
		wstring shaderFileName = L"StoPass_PNT.hlsl";
		ID3DBlob* blob;

		{

			blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "VShader", vsm);
			hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshader_so);

			int a = 0;
		}

		{

			blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "HShader", hsm);
			hr = device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &hshader_so);

			int a = 0;
		}

		{

			blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "DShader", dsm);
			hr = device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &dshader_so);

			int a = 0;
		}

		{
			D3D11_SO_DECLARATION_ENTRY soe[] =
			{
					{ 0, "PosW", 0, 0, 4, 0 },
					{ 0, "NomW", 0, 0, 4, 0 },
					{ 0, "TanW", 0, 0, 4, 0 },
					{ 0, "Uv", 0, 0, 4, 0 },
			};
			UINT stride[] = { sizeof(Vertex_SO) };

			ID3DBlob* pGSBlob = CompileShader(shaderRootPath + shaderFileName, macros, "GShader", gsm);

			hr = device->CreateGeometryShaderWithStreamOutput(
				pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(),
				soe, _countof(soe),
				stride, _countof(stride),
				D3D11_SO_NO_RASTERIZED_STREAM, nullptr, &gshader_so);

			int a = 0;
		}
	}

	//StoPass_Auto
	{
		wstring shaderFileName = L"StoPass_Auto.hlsl";
		ID3DBlob* blob;

		{
			blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "VShader", vsm);
			hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshader_auto);

			//float4 posW : PosW;
			//float4 nomW : NomW;
			//float4 tanW : TanW;
			//float4 uv : Uv;

			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "PosW",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NomW",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TanW",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "Uv",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			hr = device->CreateInputLayout(layout, numElements, blob->GetBufferPointer(), blob->GetBufferSize(), &inpLayout);

			int a = 0;
		}

		{
			blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "PShader", psm);
			hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pshader_auto);

			int a = 0;
		}




	}

	//StoPass_Index
	{
		wstring shaderFileName = L"StoPass_Index.hlsl";
		ID3DBlob* blob;

		{
			blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "VShader", vsm);
			hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshader_index);

			int a = 0;
		}

		//{
		//	blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "PShader", psm);
		//	hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pshader_index);
		//
		//	int a = 0;
		//}

		{
			wstring csoFileName = L"StoPass_Index_PShader.cso";
			CompileShaderAndSave(shaderRootPath + shaderFileName, macros, "PShader", psm, shaderRootPath + csoFileName);
			char* buffer = nullptr;
			size_t size;

			ReadShaderBlob(shaderRootPath + csoFileName, &buffer, &size);
			hr = device->CreatePixelShader(buffer, size, nullptr, &pshader_index);

			if (buffer != nullptr)
			{
				free(buffer);
			}

		}
	}

	return 0;
}

int StoPassObject::InitResource()
{
	HRESULT  hr = S_OK;
	auto device = Gpu::device;
	auto context = Gpu::context;

	{		
		lightBuffer = new GPU_Resource::COBuffer<Light>();
	}

	{
		renInfoBuffer = new GPU_Resource::COBuffer<RenderInfo>();
	}

	{
		objBuffer = new GPU_Resource::ROBuffer<Object>(2);
	}

	{
		//mesh->iCount
		int count = tessTriCount[3] * mesh->iCount;

		stoBuffer = new Buffer(
			sizeof(Vertex_SO), count, D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT,
			true, false, false, false, false);

		//stoBuffer = new Buffer(
		//	sizeof(Vertex_SO), count, D3D11_BIND_STREAM_OUTPUT,
		//	true, false, false, false, false);
	}

	{
		int count = tessTriCount[3] * mesh->iCount;
		idxBuffer_so = new Buffer(
			sizeof(DWORD), count, D3D11_BIND_INDEX_BUFFER,
			false, false, true, false, false);

		auto data = (DWORD*)idxBuffer_so->data;
		for (int i = 0; i < count; i++)
		{
			data[i] = i;
		}

		idxBuffer_so->WriteToBuffer(context);
		idxBuffer_so->CopyToBuffer(context);
	}

	{
		int count = tessTriCount[3] * mesh->iCount;
		vtxBuffer_so = new Buffer(
			sizeof(Vertex_SO), count, D3D11_BIND_SHADER_RESOURCE,
			true, false, false, true, false);
	}

	{
		D3D11_QUERY_DESC querydesc;
		memset(&querydesc, 0, sizeof(querydesc));
		querydesc.Query = D3D11_QUERY_SO_STATISTICS;
		querydesc.MiscFlags = 0;
		hr = device->CreateQuery(&querydesc, &pQuery);
	}

	return 0;
}

int StoPassObject::InitTexture()
{
	auto device = Gpu::device;

	ID3D11ShaderResourceView* srv;
	HRESULT hr = S_OK;

	{
		wstring path; // = exeDir + L"\\Assets\\WK_Standard_Units.png";

		if (RenderManager::onEditor)
		{
			path = StringToWString(RenderManager::assetPath) + L"\\Assets\\Plugins\\x86_64\\Assets\\";
		}
		else
		{
			path = StringToWString(RenderManager::exePath) + L"\\Assets\\";
		}

		path += L"WK_Standard_Units.png";

		roTex_srv = new ROTexture2D(path);
		srv_tex2d = (ID3D11ShaderResourceView*)roTex_srv->srv;
	}

	{
		D3D11_SAMPLER_DESC sampDesc;
		memset(&sampDesc, 0, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = device->CreateSamplerState(&sampDesc, &sampler);
	}



	return 0;
}



int StoPassObject::RenderSO()
{
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	{
		//{
		//	//auto data = (Object*)objBuffer->data;
		//	objBuffer->WriteToBuffer(context);
		//}

		//Draw		
		//objBuffer->CopyToBuffer(dcontext);		

		{
			ID3D11Buffer* cbrs[] = {				
				(ID3D11Buffer*)renInfoBuffer->value };
			ID3D11ShaderResourceView* srvs[] = {
				(ID3D11ShaderResourceView*)mesh->vtxBuffer->srv,
				(ID3D11ShaderResourceView*)objBuffer->srv };
			//ID3D11SamplerState* sams[] = { sampler };

			dcontext->IASetIndexBuffer((ID3D11Buffer*)mesh->idxBuffer->value, DXGI_FORMAT_R16_UINT, 0);
			//dcontext->IASetIndexBuffer((ID3D11Buffer*)idxBuffer->value, DXGI_FORMAT_R16_UINT, 0);
			dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

			dcontext->VSSetConstantBuffers(0, _countof(cbrs), cbrs);
			dcontext->VSSetShaderResources(0, _countof(srvs), srvs);
			dcontext->HSSetConstantBuffers(0, _countof(cbrs), cbrs);
			dcontext->HSSetShaderResources(0, _countof(srvs), srvs);
			dcontext->DSSetConstantBuffers(0, _countof(cbrs), cbrs);
			dcontext->DSSetShaderResources(0, _countof(srvs), srvs);
			dcontext->GSSetConstantBuffers(0, _countof(cbrs), cbrs);
			dcontext->GSSetShaderResources(0, _countof(srvs), srvs);


			dcontext->VSSetShader(vshader_so, nullptr, 0);
			dcontext->HSSetShader(hshader_so, nullptr, 0);
			dcontext->DSSetShader(dshader_so, nullptr, 0);
			dcontext->GSSetShader(gshader_so, nullptr, 0);
			dcontext->PSSetShader(nullptr, nullptr, 0);


			UINT offset = 0;
			ID3D11Buffer* soTarget[] = { (ID3D11Buffer*)(stoBuffer->value) };
			dcontext->SOSetTargets(1, soTarget, &offset);
				dcontext->Begin(pQuery);
					dcontext->DrawIndexed(mesh->iCount, 0, 0);
				dcontext->End(pQuery);
			dcontext->SOSetTargets(0, nullptr, nullptr);

			stoBuffer->CopyFromBuffer(dcontext);


			int a = 0;
		}

	}


	return 0;
}

int StoPassObject::RenderAuto()
{
	auto dcontext = Gpu::dcontext;

	{
	
		//camBuffer->CopyToBuffer(dcontext);
		//lightBuffer->CopyToBuffer(dcontext);
	}


	{
		dcontext->IASetInputLayout(inpLayout);
		dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11Buffer* buffers[] = { (ID3D11Buffer*)stoBuffer->value };
		UINT stride = sizeof(Vertex_SO);
		UINT offset = 0;
		dcontext->IASetVertexBuffers(0, _countof(buffers), buffers, &stride, &offset);

		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)RenderManager::camInfo_Buffer->value,
				(ID3D11Buffer*)lightBuffer->value,
				(ID3D11Buffer*)renInfoBuffer->value };
		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)RenderManager::perCam_Buffer->srv,			
			(ID3D11ShaderResourceView*)srv_tex2d0,
			(ID3D11ShaderResourceView*)srv_tex2d
		};

		ID3D11SamplerState* sams[] = { sampler };

		dcontext->VSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->VSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->PSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetSamplers(0, _countof(sams), sams);

		dcontext->VSSetShader(vshader_auto, nullptr, 0);
		dcontext->HSSetShader(nullptr, nullptr, 0);
		dcontext->DSSetShader(nullptr, nullptr, 0);
		dcontext->GSSetShader(nullptr, nullptr, 0);
		dcontext->PSSetShader(pshader_auto, nullptr, 0);

		dcontext->DrawAuto();
	}



	return 0;
}

int StoPassObject::RenderIndex()
{
	auto dcontext = Gpu::dcontext;

	{		
		//lightBuffer->CopyToBuffer(dcontext);
	}

	{
		dcontext->CopyResource(vtxBuffer_so->value, stoBuffer->value);
		vtxBuffer_so->CopyFromBuffer(dcontext);
	}

	{
		//dcontext->IASetInputLayout(inpLayout);
		dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dcontext->IASetIndexBuffer((ID3D11Buffer*)idxBuffer_so->value, DXGI_FORMAT_R32_UINT, 0);


		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)RenderManager::camInfo_Buffer->value,
				(ID3D11Buffer*)lightBuffer->value,
				(ID3D11Buffer*)renInfoBuffer->value };
		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)RenderManager::perCam_Buffer->srv,
			(ID3D11ShaderResourceView*)vtxBuffer_so->srv,
			(ID3D11ShaderResourceView*)srv_tex2d0,
			(ID3D11ShaderResourceView*)srv_tex2d
		};

		ID3D11SamplerState* sams[] = { sampler };

		dcontext->VSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->VSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->PSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetSamplers(0, _countof(sams), sams);


		dcontext->VSSetShader(vshader_index, nullptr, 0);
		dcontext->HSSetShader(nullptr, nullptr, 0);
		dcontext->DSSetShader(nullptr, nullptr, 0);
		dcontext->GSSetShader(nullptr, nullptr, 0);
		dcontext->PSSetShader(pshader_index, nullptr, 0);

		auto tessFactor = ((Object*)objBuffer->data)->data.m128_f32[0];

		int count_idx = tessTriCount[(int)tessFactor - 1] * mesh->iCount;
		//int count_idx = tessTriCount[3] * mesh->iCount;
		dcontext->DrawIndexedInstanced(count_idx, 1, 0, 0, 0);

	}



	return 0;
}



void StoPassObject::SetTexureFromUnity(void* nativeTexturePtr)
{
	if (nativeTexturePtr == nullptr)
	{
		return;
	}

	HRESULT hr = S_OK;
	auto device = Gpu::device;

	{
		ID3D11Texture2D* tex = (ID3D11Texture2D*)nativeTexturePtr;
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srdc;
			std::memset(&srdc, 0, sizeof(srdc));
			srdc.Format = desc.Format;
			srdc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srdc.Texture2D.MipLevels = 1;
			//srdc.Texture2D.MostDetailedMip = 1;

			hr = device->CreateShaderResourceView(tex, &srdc, &srv_tex2d0);
		}

		int a = 0;
	}

}

void StoPassObject::SetLightDataFromUnity(void** ppData)
{	
	if (lightBuffer != nullptr)
	{
		*ppData = lightBuffer->data;
	}
}

void StoPassObject::SetObjDataFromUnity(void** ppData)
{
	if (objBuffer != nullptr)
	{
		*ppData = objBuffer->data;
	}
}

int StoPassObject::InitShader_Compute(wstring shaderRootPath)
{
	HRESULT hr = S_OK;
	auto device = Gpu::device;
	ID3DBlob* blob;

	shaderRootPath += L"StoPass\\00_CShader\\";

	NativeLog(WStringToString(shaderRootPath).c_str());

	{
		//const wstring shaderRootPath = wstring(HLSL_FOLDER) + L"TestComputePipe\\";
		wstring shaderFileName = L"CS_Compute_Tex.hlsl";
		const string csm = "cs_5_0";

		blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "CS_Compute_Tex", csm);

		hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &cshader_tex3d);
		if (hr == S_OK) { OutputDebugStringA("create compute shader.\n"); }


		int a = 0;
	}
	
	return 0;
}

int StoPassObject::InitResourece_Compute()
{
	{
		tex3d = new RWTexture3D(DXGI_FORMAT_R32G32B32A32_FLOAT, sizeof(XMVECTOR), width, height, depth, 0, true, true);
	}


	return 0;
}

int StoPassObject::Compute_Tex3D()
{
	HRESULT hr;

	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;
	ID3D11CommandList* cmdList;

	//fence->IsCompleted();

	auto data = (XMVECTOR(*)[HEIGHT][WIDTH])(tex3d->data);

	static int num = 0;
	{
		//auto data = (XMVECTOR(*)[8][8])(tex3d->data);
		//auto data = (XMVECTOR(*)[6][4])(tex3d->data);


		//num = num + 1;
		//num = num % 4;

		for (int i = 0; i < depth; i++)
		{
			for (int j = 0; j < height; j++)
			{
				for (int k = 0; k < width; k++)
				{
					XMVECTOR pixel = XMVectorSet(i, j, k, 0.0f);
					//XMVECTOR pixel = XMVectorZero();
					tex3d->SetData(i, j, k, &pixel);

					//int a = 0;
				}
			}
		}

		int a = 0;

		//{
		//	tex3d->WriteToTex(context);
		//}

		{
			UINT pos0[] = { 0, 0, 0 };
			UINT pos1[] = { 0, 0, 0 };
			UINT count[] = { width, height, depth };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { 4, 4, 4 };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { width / 2, height / 2,  depth / 2 };

			tex3d->WriteToTex_Region(pos0, pos1, count, context);
		}

	}

	{
		//{
		//	tex3d->CopyToTex(dcontext);
		//}

		{
			UINT pos0[] = { 0, 0, 0 };
			UINT pos1[] = { 0, 0, 0 };
			UINT count[] = { width, height, depth };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { 4, 4, 4 };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { width / 2, height / 2,  depth / 2 };

			tex3d->CopyToTex_Region(pos0, pos1, count, dcontext);
		}

		auto cshader = cshader_tex3d;
		dcontext->CSSetShader(cshader, nullptr, 0);

		ID3D11UnorderedAccessView* uavs[] = { (ID3D11UnorderedAccessView*)(tex3d->uav) };
		dcontext->CSSetUnorderedAccessViews(0, 1, uavs, nullptr);

		dcontext->Dispatch(1, 1, 1);

		dcontext->End(fence_read_back);
		dcontext->Flush();

		//{
		//	tex3d->CopyFromTex(dcontext);
		//}

		{
			UINT pos0[] = { 0, 0, 0 };
			UINT pos1[] = { 0, 0, 0 };
			UINT count[] = { width, height, depth };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { 4, 4, 4 };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { width / 2, height / 2,  depth / 2 };

			tex3d->CopyFromTex_Region(pos0, pos1, count, dcontext);
		}
	}

	{
		dcontext->FinishCommandList(false, &cmdList);
		context->ExecuteCommandList(cmdList, true);
	}

	while (context->GetData(fence_read_back, nullptr, 0, 0) == S_FALSE)
	{
		// CPU 점유율 폭발을 막기 위해 잠시 대기
		Sleep(0);
	
		//NativeLog("Sleep()");
	}
	
	{
		//NativeLog("Awake()");
	}	

	{
		//{
		//	tex3d->ReadFromTex(context);
		//}

		{
			UINT pos0[] = { 0, 0, 0 };
			UINT pos1[] = { 0, 0, 0 };
			UINT count[] = { width, height, depth };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { 4, 4, 4 };

			//UINT pos0[] = { 2, 2, 2 };
			//UINT pos1[] = { 2, 2, 2 };
			//UINT count[] = { width / 2, height / 2,  depth / 2 };

			//UINT pos0[] = { 2, 3, 4 };
			//UINT pos1[] = { 0, 0, 0 };
			//UINT count[] = { width / 2, height / 2,  depth / 2 };

			tex3d->ReadFromTex_Region(pos0, pos1, count, context);
		}

		//{
		//	context->Flush();			
		//}
		int id = id_obj;
		int a = 0;

		//{
		//	fence->Signal();
		//
		//	//fence->WaitOnCPU();
		//	fence->IsCompleted();
		//}

	


		return 0;
	}



}
