#include "../Header.h"
#include "../Utililty/Utility.h"
#include "../RenderManager.h"
#include "../Gpu/Gpu.h"

#include "NavTestObject.h"

vector<NavTestObject*>* NavTestObject::objects = nullptr;

extern "C" UNITY_INTERFACE_EXPORT  void* UNITY_INTERFACE_API CreateRenderObject_NavTestObject()
{
	NavTestObject* object = new NavTestObject();

	if (NavTestObject::objects == nullptr)
	{
		NavTestObject::objects = new vector<NavTestObject*>();
	}

	object->id_obj = NavTestObject::objects->size();
	NavTestObject::objects->push_back(object);

	if (RenderManager::objects != nullptr)
	{
		RenderManager::objects->push_back(object);
	}

	return object;
}

NavTestObject::IRenderActor* renActor = nullptr;

extern "C"  UNITY_INTERFACE_EXPORT NavTestObject::IRenderActor* UNITY_INTERFACE_API GetRenderActor_NavTestObject()
{
	if (renActor == nullptr)
	{
		renActor = new NavTestObject::IRenderActor();
	}

	renActor->Init =
		[](void* instance)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->Init();
		};

	renActor->Begin =
		[](void* instance)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->Begin();
		};

	renActor->Update =
		[](void* instance)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->Update();
		};

	renActor->Render =
		[](void* instance)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->Render();
		};

	renActor->Destroy =
		[](void* instance)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->Destroy();
		};


	renActor->GetId =
		[](void* instance)
		{
			NavTestObject* object = (NavTestObject*)instance;

			return object->id_obj;
		};



	renActor->IssueUpdate =
		[](int id)
		{
			if (NavTestObject::objects == nullptr) { return; }

			NavTestObject* object = (*NavTestObject::objects)[id];

			if (object != nullptr)
			{
				object->EventUpdate();			
			}

		};

	renActor->IssueBeginContext =
		[](int id)
		{
			if (NavTestObject::objects == nullptr) { return; }

			NavTestObject* object = (*NavTestObject::objects)[id];

			if (object != nullptr)
			{
				object->EventBeginContext();
			}

		};

	renActor->IssueBeginCamera =
		[](int id)
		{
			if (NavTestObject::objects == nullptr) { return; }

			NavTestObject* object = (*NavTestObject::objects)[id];

			if (object != nullptr)
			{
				object->EventBeginCamera();
			}

		};

	renActor->IssueRender =
		[](int id)
		{
			if (NavTestObject::objects == nullptr) { return; }

			NavTestObject* object = (*NavTestObject::objects)[id];

			if (object != nullptr)
			{
				object->EventRender();
			}

		};


	renActor->IssueEndCamera =
		[](int id)
		{
			if (NavTestObject::objects == nullptr) { return; }

			NavTestObject* object = (*NavTestObject::objects)[id];

			if (object != nullptr)
			{
				object->EventEndCamera();
			}

		};

	renActor->IssueEndContext =
		[](int id)
		{
			if (NavTestObject::objects == nullptr) { return; }

			NavTestObject* object = (*NavTestObject::objects)[id];

			if (object != nullptr)
			{
				object->EventEndContext();
			}

		};	

	renActor->SetTextureFromUnity =
		[](void* instance, void* ptrTex, int idx)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->SetTexureFromUnity(ptrTex, idx);
		};

	renActor->SetTextureModeId =
		[](void* instance, int id)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->renMode_texViewer_id = id;
		};

	renActor->SendDataPtrToUnity =
		[](void* instance, void** ppData0, void** ppData1, void** ppData2)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->SendDataPtrToUnity(ppData0, ppData1, ppData2);
			
		};

	renActor->SendUnitCount =
		[](void* instance, int count)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->count_unit = count;

		};

	renActor->SendPtrTargetPos =
		[](void* instance, void** ppData0, void** ppData1, void** ppData2, void** ppData3, void** ppData4)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->SendPtrTargetPos(ppData0, ppData1, ppData2, ppData3, ppData4);
		};

	renActor->SendComputeDoneEvent =
		[](void* instance, void** pHandle)
		{
			NavTestObject* object = (NavTestObject*)instance;
			*pHandle = object->hComputeDoneEvent;
		};
	
	renActor->SendMoveDoneEvent =
		[](void* instance, void* handle)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->hMoveDoneEvent = (HANDLE)handle;

			int a = 0;
		};

	renActor->SetRenderDebugPosEnable =
		[](void* instance, void** ppData)
		{
			NavTestObject* object = (NavTestObject*)instance;
			*ppData = (void*)&(object->bRender_debugPos);
	
			int a = 0;
		};



	renActor->IssueUpdate_rectIn =
		[](int id)
		{
			if (NavTestObject::objects == nullptr) { return; }

			NavTestObject* object = (*NavTestObject::objects)[id];

			if (object != nullptr)
			{
				object->Update_RectIn();
			}

		};


	renActor->SendPtrRectIn =
		[](void* instance, void** ppData0, void** ppData1, void** ppData2, void** ppData3)
		{
			NavTestObject* object = (NavTestObject*)instance;
			object->SendPtrRectIn(ppData0, ppData1, ppData2, ppData3);
		};


	renActor->SendRectInDoneEvent =
		[](void* instance, void** pHandle)
		{
			NavTestObject* object = (NavTestObject*)instance;
			*pHandle = object->hRectInDoneEvent;
		};


	renActor->GetHandle =
		[](void* instance, void** pHandle, int id)
		{
			NavTestObject* object = (NavTestObject*)instance;
			*pHandle = (*(object->handles))[id];

		};

	return renActor;
}

NavTestObject::NavTestObject()
{

}

void NavTestObject::Init()
{
	auto device = Gpu::device;
	auto context = Gpu::context;

	{
		handles = (HANDLE(*)[ID_HANDEL::Count])malloc(sizeof(HANDLE) * ID_HANDEL::Count);
	}

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

		wstrAssetPath = path;
		strAssetPath = WStringToString(path);
	}

	{
		//mesh = Mesh::CreateCubeMesh();
		//mesh = Mesh::CreateTorusMesh();
	}

	{
		InitResource();
		InitTexture();

		InitShader();
	}

	{
		InitTerrain();
	}

	{
		Load_NormalHeightMap_Baked();
		Load_AlphaMap();
		Load_HoleMap();
		Load_Terrain_Texture();
	}

	
	
}

void NavTestObject::Begin()
{
	{
		InitUnitSimple();
		InitDebugPos();
	}

	{
		Init_targetCompute();
	}

	{
		Init_RectIn();
	}
}

void NavTestObject::Update()
{
	UpdateTarget();
}

void NavTestObject::Render()
{	
	HRESULT hr = S_OK;

	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;
	auto cmdList = RenderManager::cmdList;
	
	{
		//Render_TextureViewer();
	}

	{
		RenderTerrain();
		RenderUnitSimple();
		RenderDebugPos();
	}

	{
		hr = dcontext->FinishCommandList(true, &cmdList);
		context->ExecuteCommandList(cmdList, true);
	}

}

void NavTestObject::Destroy()
{
	this->id_obj = -1;
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

	for (int i = 0; i < ID_HANDEL::Count; i++)
	{
		CloseHandle((*handles)[i]);
	}
	free(handles);


	string str = string("NavTestObject_Destroy()_id") + std::to_string(id_obj);
	NativeLog(str.c_str());
}

void NavTestObject::EventUpdate()
{
	Update();
	int a = 0;
}

void NavTestObject::EventBeginContext()
{

}

void NavTestObject::EventBeginCamera()
{

}

void NavTestObject::EventRender()
{
	Render();
}

void NavTestObject::EventEndCamera()
{

}

void NavTestObject::EventEndContext()
{

}

int NavTestObject::InitShader()
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

	wstring cshaderRootPath = shaderRootPath + L"NavTest\\00_CShader\\";
	wstring gshaderRootPath = shaderRootPath + L"NavTest\\01_GShader\\";
	
	//shaderRootPath += L"NavTest\\01_GShader\\";

#define ROW_MAJOR "0"
#define COLUMN_MAJOR "1"

	D3D_SHADER_MACRO macros[] = {
		//{"ROW_MAJOR", NULL}, 
		{"COLUMN_MAJOR", NULL},
		{"USE_TEXTURE", NULL},
		{ NULL, NULL } };
	
	{
		shaderRootPath = gshaderRootPath;

		{
			wstring shaderFileName = L"00_TextureViewer.hlsl";
			ID3DBlob* blob;

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "VShader", vsm);
				hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshader_texViewer);

				int a = 0;
			}

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "PShader", psm);
				hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pshader_texViewer);

				int a = 0;
			}
		}

		{
			wstring shaderFileName = L"01_TerrainSimple.hlsl";
			ID3DBlob* blob;

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "VShader", vsm);
				hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshader_terrain);

				int a = 0;
			}

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "HShader", hsm);
				hr = device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &hshader_terrain);

				int a = 0;
			}

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "DShader", dsm);
				hr = device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &dshader_terrain);

				int a = 0;
			}

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "GShader", gsm);
				hr = device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &gshader_terrain);

				int a = 0;
			}

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "PShader", psm);
				hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pshader_terrain);

				int a = 0;
			}
		}

		{
			wstring shaderFileName = L"02_UnitSimple.hlsl";
			ID3DBlob* blob;

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "VShader", vsm);
				hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshader_unit);

				int a = 0;
			}

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "PShader", psm);
				hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pshader_unit);

				int a = 0;
			}
		}

		{
			wstring shaderFileName = L"03_DebugPos.hlsl";
			ID3DBlob* blob;

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "VShader", vsm);
				hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vshader_debug);

				int a = 0;
			}

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, nullptr, "PShader", psm);
				hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pshader_debug);

				int a = 0;
			}
		}
	}

	{
		shaderRootPath = cshaderRootPath;

		{
			wstring shaderFileName = L"00_Target_trW.hlsl";
			ID3DBlob* blob;

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "CS_TrW", csm);
				hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &cshader_trW);

				int a = 0;
			}
		}

		{
			wstring shaderFileName = L"01_Target_targetPos.hlsl";
			ID3DBlob* blob;

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "CS_TargetPos", csm);
				hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &cshader_targetPos);

				int a = 0;
			}
		}

		{
			wstring shaderFileName = L"02_RectIn.hlsl";
			ID3DBlob* blob;

			{
				blob = Gpu::CompileShader(shaderRootPath + shaderFileName, macros, "CS_RectIn", csm);
				hr = device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &cshader_rectIn);

				int a = 0;
			}
		}
	}
	


	return 0;
}

int NavTestObject::InitResource()
{
	{
		renMode_texViewer = new GPU_Resource::COBuffer<XMVECTOR>();
	}


	return 0;
}

int NavTestObject::InitTexture()
{
	auto device = Gpu::device;

	ID3D11ShaderResourceView* srv;
	HRESULT hr = S_OK;

	//{
	//	D3D11_SAMPLER_DESC sampDesc;
	//	memset(&sampDesc, 0, sizeof(sampDesc));
	//	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//	sampDesc.MinLOD = 0;
	//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//
	//	hr = device->CreateSamplerState(&sampDesc, &sampler);
	//}

	{
		D3D11_SAMPLER_DESC sampDesc;
		memset(&sampDesc, 0, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = device->CreateSamplerState(&sampDesc, &sampler);
	}

	return 0;
}

void NavTestObject::SetTexureFromUnity(void* nativeTexturePtr, int idx)
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
			//srdc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//srdc.Format = DXGI_FORMAT_R16_FLOAT;
			// 
			//srdc.Format = DXGI_FORMAT_R16_UNORM;	//for heightMap
			//srdc.Format = DXGI_FORMAT_R16_SNORM;	//for heightMap
			//srdc.Format = DXGI_FORMAT_R8_UNORM;	//for holeMap

			srdc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srdc.Texture2D.MipLevels = 1;
			//srdc.Texture2D.MostDetailedMip = 1;
		

			hr = device->CreateShaderResourceView(tex, &srdc, &srv_tex2d[idx]);
		}

		int a = 0;

		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			srv_tex2d[idx]->GetDesc(&desc);

			a = 0;
		}

		
	}
}

void NavTestObject::Render_TextureViewer()
{
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	{
		auto data = (XMVECTOR*)renMode_texViewer->data;
		data->m128_f32[0] = 0;

		renMode_texViewer->WriteToBuffer(context);
		renMode_texViewer->CopyToBuffer(dcontext);
	}
	
	{		
		dcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			

		ID3D11Buffer* cbrs[] = { 
			(ID3D11Buffer*)RenderManager::camInfo_Buffer->value,
			(ID3D11Buffer*)renMode_texViewer->value};
		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)RenderManager::perCam_Buffer->srv,
			(ID3D11ShaderResourceView*)srv_tex2d[renMode_texViewer_id],
			//(ID3D11ShaderResourceView*)tex_diffuse0->srv,
			//(ID3D11ShaderResourceView*)tex_diffuse1->srv,
			//(ID3D11ShaderResourceView*)tex_diffuse2->srv,
		};

		ID3D11SamplerState* sams[] = { sampler };

		dcontext->VSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->VSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->PSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetSamplers(0, _countof(sams), sams);


		dcontext->VSSetShader(vshader_texViewer, nullptr, 0);
		dcontext->HSSetShader(nullptr, nullptr, 0);
		dcontext->DSSetShader(nullptr, nullptr, 0);
		dcontext->GSSetShader(nullptr, nullptr, 0);
		dcontext->PSSetShader(pshader_texViewer, nullptr, 0);


		dcontext->Draw(6, 0);
	}
}

void NavTestObject::InitTerrain()
{
	auto context = Gpu::context;

	terrainSize = XMVectorSet(256.0f, 1.0f, 256.0f, 0.0f);
	tileCount = XMVectorSet(64.0f, 1.0f, 64.0f, 1.0f);
	//tileCount = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	layerSize = XMVectorSet(32.0f, 8.0f, 32.0f, 32.0f);


	//vtxBuffer_idxBuffer
	{
		vector<XMVECTOR> vtx;
		vector<USHORT> idx;

		int cx = tileCount.m128_f32[0];
		int cz = tileCount.m128_f32[2];

		float dx = terrainSize.m128_f32[0] / cx;
		float dz = terrainSize.m128_f32[2] / cz;

		{
			for (int i = 0; i < cz + 1; i++)
			{
				for (int j = 0; j < cx + 1; j++)
				{
					vtx.push_back(XMVectorSet(dx * j, 0.0f, dz * i, 0.0f));
				}
			}
		}

		{
			for (int i = 0; i < cz; i++)
			{
				for (int j = 0; j < cx; j++)
				{
					int i0 = (i + 0) * (cx + 1);
					int i1 = (i + 1) * (cx + 1);
					int j0 = (j + 0);
					int j1 = (j + 1);

					idx.push_back(i0 + j0);
					idx.push_back(i1 + j0);
					idx.push_back(i1 + j1);
					idx.push_back(i0 + j1);
				}
			}
		}


		{
			int count = vtx.size();
			vtxBuffer_terrain = new GPU_Resource::ROBuffer<VertexData>(count);
			VertexData* data = (VertexData*)(vtxBuffer_terrain->data);
			for (int i = 0; i < count; i++)
			{
				data[i].posL = vtx[i];
			}

			vtxBuffer_terrain->WriteToBuffer(context);
			vtxBuffer_terrain->CopyToBuffer(context);
		}

		{
			int count = idx.size();
			idxBuffer_terrain = new GPU_Resource::ROBuffer<USHORT>(count, D3D11_BIND_INDEX_BUFFER);
			USHORT* data = (USHORT*)(idxBuffer_terrain->data);
			for (int i = 0; i < count; i++)
			{
				data[i] = idx[i];
			}

			idxBuffer_terrain->WriteToBuffer(context);
			idxBuffer_terrain->CopyToBuffer(context);
		}

		
	}

	//TerrainData_ObjectData
	{
		{
			terrainDataBuffer = new GPU_Resource::COBuffer<TerrainData>();
			TerrainData* data = (TerrainData*)(terrainDataBuffer->data);
			data->terrainSize = terrainSize;
			data->tileCount = tileCount;
			data->tileSize = terrainSize / tileCount;
			data->layerSize = layerSize;

			terrainDataBuffer->WriteToBuffer(context);
			terrainDataBuffer->CopyToBuffer(context);
		}

		{
			objDataBuffer = new GPU_Resource::COBuffer<ObjectData>();
			ObjectData* data = (ObjectData*)(objDataBuffer->data);
			data->W = XMMatrixIdentity();
			data->data = XMVectorZero();

			objDataBuffer->WriteToBuffer(context);
		}
	}


	//LightData
	{
		lightDataBuffer = new GPU_Resource::COBuffer<LightData>();
	}
	
}

void NavTestObject::Load_NormalHeightMap_Baked()
{
	auto context = Gpu::context;

	const int stride = 16;
	const int width = 1024; //1024;		//1025
	const int height = 1024; //1024;	//1025

	const int rowPitch = stride * width;
	const int size = rowPitch * height;
	const int count = width * height;

	//tex_NHmap_Baked = new Texture2D_Data(revHeap, &heapIdx_rev, DXGI_FORMAT_R32G32B32A32_FLOAT, stride, width, height);
	tex_NHmap_Baked = new GPU_Resource::ROTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, stride, width, height, 0);


	//float* data;
	//data = (float*)tex_hmap->data;

	std::byte* data;
	data = (std::byte*)tex_NHmap_Baked->data;

	{
		//char* filename = "terrain_NHmap_1024.bin";
		const string filename = "terrain_NHmap_1024_01.bin";
		string path = strAssetPath + "Terrain\\" + filename;

		FILE* fp;
		fopen_s(&fp, path.c_str(), "rb");


		if (fp != 0)
		{
			fseek(fp, 0, SEEK_SET);

			long pos;
			pos = ftell(fp);

			{
				fread(data, size, 1, fp);
			}

			pos = ftell(fp);


			int a = 0;
		}

		fclose(fp);
	}

	{
		tex_NHmap_Baked->WriteToTex(context);
		tex_NHmap_Baked->CopyToTex(context);
	}

	//{
	//	UINT pos0[] = { 0, 0, 0 };
	//	UINT pos1[] = { 0, 0, 0 };
	//	UINT count[] = { width, height };
	//
	//	tex_NHmap_Baked->WriteToTex_Region(pos0, pos1, count, context);
	//}
	//
	//{
	//	UINT pos0[] = { 0, 0, 0 };
	//	UINT pos1[] = { 0, 0, 0 };
	//	UINT count[] = { width, height };
	//
	//	tex_NHmap_Baked->CopyToTex_Region(pos0, pos1, count, context);
	//}

	//Debug
	{
		//XMVECTOR* data0 = (XMVECTOR*)tex_NHmap_Baked->data;
		float (*data0)[4] = (float(*)[4])tex_NHmap_Baked->data;
		int count = tex_NHmap_Baked->count;

		float maxW = 0.0f;
		for (int i = 0; i < count; i++)
		{
			//float value = data0->m128_f32[3];
			float value = data0[i][3];
			if (value > maxW)
			{
				maxW = value;
			}
		}

		int a = 0;
	}

}

void NavTestObject::Load_AlphaMap()
{
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	const int stride = 16;
	const int width = 1024; //1024;		//1025
	const int height = 1024; //1024;	//1025

	const int rowPitch = stride * width;
	const int size = rowPitch * height;
	const int count = width * height;

	tex_Alpha = new GPU_Resource::ROTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, stride, width, height, 0);

	//float* data;
	//data = (float*)tex_hmap->data;

	std::byte* data;
	data = (std::byte*)tex_Alpha->data;

	{
		const string filename = "terrain_Alpha_1024.bin";
		string path = strAssetPath + "Terrain\\" + filename;

		FILE* fp;
		fopen_s(&fp, path.c_str(), "rb");

		if (fp != 0)
		{
			fseek(fp, 0, SEEK_SET);

			long pos;
			pos = ftell(fp);

			{
				fread(data, size, 1, fp);
			}

			pos = ftell(fp);


			int a = 0;
		}

		fclose(fp);
	}

	//{
	//	for (int i = 0; i < height; i++)
	//	{
	//		for (int j = 0; j < width; j++)
	//		{
	//			auto color = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	//			tex_Alpha->SetData(i, j, (void*)&color);
	//		}
	//	}
	//}

	{
		tex_Alpha->WriteToTex(context);
		tex_Alpha->CopyToTex(context);
	}

	//{
	//	UINT pos0[] = { 0, 0, 0 };
	//	UINT pos1[] = { 0, 0, 0 };
	//	UINT count[] = { width, height};
	//	
	//	tex_Alpha->WriteToTex_Region(pos0, pos1, count, context);
	//}
	//
	//{
	//	UINT pos0[] = { 0, 0, 0 };
	//	UINT pos1[] = { 0, 0, 0 };
	//	UINT count[] = { width, height};		
	//
	//	tex_Alpha->CopyToTex_Region(pos0, pos1, count, context);
	//}

}

void NavTestObject::Load_HoleMap()
{
	auto context = Gpu::context;

	const int stride = 16;
	const int width = 1024; //1024;		//1025
	const int height = 1024; //1024;	//1025

	const int rowPitch = stride * width;
	const int size = rowPitch * height;
	const int count = width * height;

	tex_Hole = new GPU_Resource::ROTexture2D(DXGI_FORMAT_R32G32B32A32_FLOAT, stride, width, height, 0);

	//float* data;
	//data = (float*)tex_hmap->data;

	std::byte* data;
	data = (std::byte*)tex_Hole->data;

	{
		const string filename = "terrain_Hole_1024.bin";
		string path = strAssetPath + "Terrain\\" + filename;

		FILE* fp;
		fopen_s(&fp, path.c_str(), "rb");


		if (fp != 0)
		{
			fseek(fp, 0, SEEK_SET);

			long pos;
			pos = ftell(fp);

			{
				fread(data, size, 1, fp);
			}

			pos = ftell(fp);


			int a = 0;
		}

		fclose(fp);
	}

	{
		tex_Hole->WriteToTex(context);
		tex_Hole->CopyToTex(context);
	}

	//{
	//	UINT pos0[] = { 0, 0, 0 };
	//	UINT pos1[] = { 0, 0, 0 };
	//	UINT count[] = { width, height };
	//
	//	tex_Hole->WriteToTex_Region(pos0, pos1, count, context);
	//}
	//
	//{
	//	UINT pos0[] = { 0, 0, 0 };
	//	UINT pos1[] = { 0, 0, 0 };
	//	UINT count[] = { width, height };
	//
	//	tex_Hole->CopyToTex_Region(pos0, pos1, count, context);
	//}
}

void NavTestObject::Load_Terrain_Texture()
{
	wstring root = wstrAssetPath + L"Terrain\\";

	{
		wstring path = root + L"GroundStones01.png";
		//wstring path = root + L"WK_Standard_Units.png";		

		tex_diffuse0 = new GPU_Resource::ROTexture2D(path);
	}

	{
		wstring path = root + L"adesert_mntn4_d.png";

		tex_diffuse1 = new GPU_Resource::ROTexture2D(path);
	}

	{
		wstring path = root + L"Vol_36_5_Base_Color.png";

		tex_diffuse2 = new GPU_Resource::ROTexture2D(path);
	}

	//Debug
	{
		srv_tex2d[0] = (ID3D11ShaderResourceView*)tex_diffuse0->srv;
		srv_tex2d[1] = (ID3D11ShaderResourceView*)tex_diffuse1->srv;
		srv_tex2d[2] = (ID3D11ShaderResourceView*)tex_diffuse2->srv;
	}

	//{
	//	srv_tex2d[0] = (ID3D11ShaderResourceView*)tex_Alpha->srv;
	//}

}

void NavTestObject::SendDataPtrToUnity(void** ppData0, void** ppData1, void** ppData2)
{
	if (lightDataBuffer != nullptr)
	{
		*ppData0 = lightDataBuffer->data;
	}

	if (objDataBuffer != nullptr)
	{
		*ppData1 = objDataBuffer->data;
	}

	if (unitObjBuffer != nullptr)
	{
		*ppData2 = unitObjBuffer->data;
	}

	int a = 0;
}

void NavTestObject::RenderTerrain()
{
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	{
		objDataBuffer->WriteToBuffer(context);
		lightDataBuffer->WriteToBuffer(context);
	}

	{
		objDataBuffer->CopyToBuffer(dcontext);
		lightDataBuffer->CopyToBuffer(dcontext);
	}

	//{
	//	dcontext->CopyResource(vtxBuffer_so->value, stoBuffer->value);
	//	vtxBuffer_so->CopyFromBuffer(dcontext);
	//}

	{
		//dcontext->IASetInputLayout(inpLayout);
		dcontext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		dcontext->IASetIndexBuffer((ID3D11Buffer*)idxBuffer_terrain->value, DXGI_FORMAT_R16_UINT, 0);


		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)RenderManager::camInfo_Buffer->value,
				(ID3D11Buffer*)terrainDataBuffer->value,
				(ID3D11Buffer*)objDataBuffer->value,
				(ID3D11Buffer*)lightDataBuffer->value };
		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)RenderManager::perCam_Buffer->srv,
			(ID3D11ShaderResourceView*)vtxBuffer_terrain->srv,

			(ID3D11ShaderResourceView*)tex_NHmap_Baked->srv,
			(ID3D11ShaderResourceView*)tex_Alpha->srv,
			(ID3D11ShaderResourceView*)tex_Hole->srv,

			(ID3D11ShaderResourceView*)tex_diffuse0->srv,
			(ID3D11ShaderResourceView*)tex_diffuse1->srv,
			(ID3D11ShaderResourceView*)tex_diffuse2->srv
		};	

		ID3D11SamplerState* sams[] = { sampler };

		dcontext->VSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->VSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->HSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->HSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->DSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->DSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->GSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->GSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->PSSetShaderResources(0, _countof(srvs), srvs);

		dcontext->PSSetSamplers(0, _countof(sams), sams);


		dcontext->VSSetShader(vshader_terrain, nullptr, 0);
		dcontext->HSSetShader(hshader_terrain, nullptr, 0);
		dcontext->DSSetShader(dshader_terrain, nullptr, 0);
		dcontext->GSSetShader(gshader_terrain, nullptr, 0);
		dcontext->PSSetShader(pshader_terrain, nullptr, 0);

		

		int count_idx = idxBuffer_terrain->count;		
		dcontext->DrawIndexedInstanced(count_idx, 1, 0, 0, 0);

	}

}

void NavTestObject::InitUnitSimple()
{
	auto context = Gpu::context;

	{
		mesh_unit = Mesh::CreateCubeMesh();
	}
	
	{
		unitObjBuffer = new ROBuffer<ObjectData>(count_unit);	
		auto data = (ObjectData*)unitObjBuffer->data;

		for (int i = 0; i < count_unit; i++)
		{
			(data + i)->W = XMMatrixIdentity();
			(data + i)->data = XMVectorZero();
		}

		unitObjBuffer->WriteToBuffer(context);
		unitObjBuffer->CopyToBuffer(context);
	}
}

void NavTestObject::RenderUnitSimple()
{
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	{
		unitObjBuffer->WriteToBuffer(context);	
		select_Buffer->WriteToBuffer(context);
	}

	{
		unitObjBuffer->CopyToBuffer(dcontext);
		select_Buffer->CopyToBuffer(dcontext);
	}

	//{
	//	dcontext->CopyResource(vtxBuffer_so->value, stoBuffer->value);
	//	vtxBuffer_so->CopyFromBuffer(dcontext);
	//}

	{
		Mesh* mesh = mesh_unit;

		//dcontext->IASetInputLayout(inpLayout);
		dcontext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dcontext->IASetIndexBuffer((ID3D11Buffer*)mesh->idxBuffer->value, DXGI_FORMAT_R16_UINT, 0);


		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)RenderManager::camInfo_Buffer->value,				
				(ID3D11Buffer*)lightDataBuffer->value };
		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)RenderManager::perCam_Buffer->srv,
			(ID3D11ShaderResourceView*)mesh->vtxBuffer->srv,
			(ID3D11ShaderResourceView*)unitObjBuffer->srv,
			(ID3D11ShaderResourceView*)select_Buffer->srv
		};

		ID3D11SamplerState* sams[] = { sampler };

		dcontext->VSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->VSSetShaderResources(0, _countof(srvs), srvs);	
		dcontext->PSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->PSSetShaderResources(0, _countof(srvs), srvs);

		dcontext->PSSetSamplers(0, _countof(sams), sams);


		dcontext->VSSetShader(vshader_unit, nullptr, 0);
		dcontext->HSSetShader(nullptr, nullptr, 0);
		dcontext->DSSetShader(nullptr, nullptr, 0);
		dcontext->GSSetShader(nullptr, nullptr, 0);
		dcontext->PSSetShader(pshader_unit, nullptr, 0);



		int count_idx = mesh->idxBuffer->count;	
		dcontext->DrawIndexedInstanced(count_idx, count_unit, 0, 0, 0);

	}

}

void NavTestObject::InitDebugPos()
{
	auto context = Gpu::context;

	{
		mesh_debug = Mesh::CreateTorusMesh(0.1f, 0.2f);
	}
}



void NavTestObject::RenderDebugPos()
{
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	if (!bRender_debugPos) { return; }
	
	{
		Mesh* mesh = mesh_debug;

		//dcontext->IASetInputLayout(inpLayout);
		dcontext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dcontext->IASetIndexBuffer((ID3D11Buffer*)mesh->idxBuffer->value, DXGI_FORMAT_R16_UINT, 0);


		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)RenderManager::camInfo_Buffer->value,
				(ID3D11Buffer*)lightDataBuffer->value };
		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)RenderManager::perCam_Buffer->srv,
			(ID3D11ShaderResourceView*)mesh->vtxBuffer->srv,
			(ID3D11ShaderResourceView*)targetPos_Buffer->srv
		};

		ID3D11SamplerState* sams[] = { sampler };

		dcontext->VSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->VSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->PSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->PSSetShaderResources(0, _countof(srvs), srvs);

		dcontext->PSSetSamplers(0, _countof(sams), sams);


		dcontext->VSSetShader(vshader_debug, nullptr, 0);
		dcontext->HSSetShader(nullptr, nullptr, 0);
		dcontext->DSSetShader(nullptr, nullptr, 0);
		dcontext->GSSetShader(nullptr, nullptr, 0);
		dcontext->PSSetShader(pshader_debug, nullptr, 0);



		int count_idx = mesh->idxBuffer->count;
		dcontext->DrawIndexedInstanced(count_idx, count_unit, 0, 0, 0);

	}
}


void NavTestObject::Init_targetCompute()
{
	auto context = Gpu::context;
	auto device = Gpu::device;

	{
		hComputeDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		hRectInDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

		(*handles)[ID_HANDEL::TargetPos] = hComputeDoneEvent;
		(*handles)[ID_HANDEL::RectIn] = hRectInDoneEvent;
		
	}

	{
		tc_info_Buffer = new COBuffer<TargetComputeInfo>();

		trM_Buffer = new ROBuffer<XMMATRIX>(count_unit);

		trW_Buffer = new RWBuffer<XMMATRIX>(count_unit);
		circle_Buffer = new RWBuffer<XMVECTOR>(count_unit);
		terrainArea_Buffer = new RWBuffer<XMVECTOR>(count_unit);

		block_Buffer = new ROBuffer<XMVECTOR>(8);
		random_Buffer = new ROBuffer<int>(count_unit);
		
		targetPos_Buffer = new RWBuffer<XMVECTOR>(count_unit);
		tc_debug_Buffer = new RWBuffer<XMMATRIX>(count_unit);
	}

	{
		select_Buffer = new ROBuffer<int>(count_unit);

		auto data = (int*)select_Buffer->data;
		for (int i = 0; i < count_unit; i++)
		{
			data[i] = 0;
		}
	}

	{
		D3D11_QUERY_DESC desc = {};
		desc.Query = D3D11_QUERY_EVENT; // GPU의 실행 타임라인을 추적하는 이벤트 타입
		desc.MiscFlags = 0;

		device->CreateQuery(&desc, &fence_targetPos);
	}

	{
		auto data = (TargetComputeInfo*)tc_info_Buffer->data;
		data->countInfo = XMVectorSet(count_unit, 0.0f, 0.0f, 0.0f);
		//data->T = RenderUtil::GetWtoV(XMVectorSet(-128.0f, 0.0f, -128.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
		data->t1_t0 = 1024.0f / 256.0f * XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);

		tc_info_Buffer->WriteToBuffer(context);
	}

	//{
	//	auto data0 = (XMMATRIX*)trM_Buffer->
	//	auto data = (XMVECTOR*)circle_Buffer->data;
	//	for (int i = 0; i < count_unit; i++)
	//	{
	//		data[i] = XMVectorSet(0.0f, 0.0f, 0.0, agentInfo[i].radius);
	//	}
	//
	//	circle_Buffer->WriteToGpu();
	//}

	{
		{
			auto data = (XMVECTOR*)block_Buffer->data;

			float r = 1.0f;

			data[0] = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f) * r;
			data[1] = XMVectorSet(+0.0f, 0.0f, -1.0f, 0.0f) * r;
			data[2] = XMVectorSet(+1.0f, 0.0f, -1.0f, 0.0f) * r;

			data[3] = XMVectorSet(-1.0f, 0.0f, +0.0f, 0.0f) * r;

			data[4] = XMVectorSet(+1.0f, 0.0f, +0.0f, 0.0f) * r;

			data[5] = XMVectorSet(-1.0f, 0.0f, +1.0f, 0.0f) * r;
			data[6] = XMVectorSet(+0.0f, 0.0f, +1.0f, 0.0f) * r;
			data[7] = XMVectorSet(+1.0f, 0.0f, +1.0f, 0.0f) * r;

			block_Buffer->WriteToBuffer(context);
		}

		//block_Buffer->CopyToGpu();
	}

}

void NavTestObject::SendPtrTargetPos(void** ppData0, void** ppData1, void** ppData2, void** ppData3, void** ppData4)
{
	if (targetPos_Buffer != nullptr)
	{
		*ppData0 = targetPos_Buffer->data;
	}

	if (trM_Buffer != nullptr)
	{
		*ppData1 = trM_Buffer->data;
	}

	if (random_Buffer != nullptr)
	{
		*ppData2 = random_Buffer->data;
	}

	if (tc_info_Buffer != nullptr)
	{
		*ppData3 = tc_info_Buffer->data;
	}

	if (select_Buffer != nullptr)
	{
		*ppData4 = select_Buffer->data;
	}

	int a = 0;
}

void NavTestObject::UpdateTarget()
{
	HRESULT hr;

	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;
	ID3D11CommandList* cmdList;

	bool debug = true;


	//{
	//	HANDLE handles[] = { hMoveDoneEvent };
	//
	//	WaitForMultipleObjects(_countof(handles), handles, true, INFINITE);
	//}

	//{		
	//	WaitForSingleObject(hMoveDoneEvent, INFINITE);
	//}

	{
		trM_Buffer->WriteToBuffer(context);
		targetPos_Buffer->WriteToBuffer(context);	
		random_Buffer->WriteToBuffer(context);

		tc_info_Buffer->WriteToBuffer(context);
		block_Buffer->WriteToBuffer(context);
	}

	{
		trM_Buffer->CopyToBuffer(dcontext);
		targetPos_Buffer->CopyToBuffer(dcontext);		
		random_Buffer->CopyToBuffer(dcontext);
	}

	{
		tc_info_Buffer->CopyToBuffer(dcontext);
		block_Buffer->CopyToBuffer(dcontext);
	}

	//00_Target_trW
	{
		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)tc_info_Buffer->value
		};

		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)trM_Buffer->srv,
			(ID3D11ShaderResourceView*)tex_Alpha->srv
		};

		ID3D11UnorderedAccessView* uavs[] = {
			(ID3D11UnorderedAccessView*)trW_Buffer->uav,
			(ID3D11UnorderedAccessView*)circle_Buffer->uav,
			(ID3D11UnorderedAccessView*)terrainArea_Buffer->uav
		};

		dcontext->CSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->CSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->CSSetUnorderedAccessViews(0, _countof(uavs), uavs, nullptr);


		auto cshader = cshader_trW;
		dcontext->CSSetShader(cshader, nullptr, 0);

		const int grCount = 64;
		int dpCount = (count_unit % grCount == 0) ? (count_unit / grCount) : (count_unit / grCount + 1);
		dcontext->Dispatch(dpCount, 1, 1);

		//dcontext->Flush();	
	}

	{
		if (debug)
		{
			trW_Buffer->CopyFromBuffer(dcontext);
			circle_Buffer->CopyFromBuffer(dcontext);
		}

		{
			terrainArea_Buffer->CopyFromBuffer(dcontext);
		}
	}

	{
		dcontext->FinishCommandList(false, &cmdList);
		context->ExecuteCommandList(cmdList, false);
	}


	//01_Target_targetPos
	{
		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)tc_info_Buffer->value
		};

		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)trW_Buffer->srv,
			(ID3D11ShaderResourceView*)circle_Buffer->srv,
		
			(ID3D11ShaderResourceView*)block_Buffer->srv,
			(ID3D11ShaderResourceView*)random_Buffer->srv,
		
			(ID3D11ShaderResourceView*)tex_NHmap_Baked->srv,
			(ID3D11ShaderResourceView*)tex_Alpha->srv,
			(ID3D11ShaderResourceView*)tex_Hole->srv,					
		};
		
		ID3D11UnorderedAccessView* uavs[] = {
			(ID3D11UnorderedAccessView*)targetPos_Buffer->uav,
			(ID3D11UnorderedAccessView*)tc_debug_Buffer->uav,			
		};


		//ID3D11ShaderResourceView* srvs[] = {
		//	
		//	(ID3D11ShaderResourceView*)block_Buffer->srv,
		//	(ID3D11ShaderResourceView*)random_Buffer->srv,
		//
		//	(ID3D11ShaderResourceView*)tex_NHmap_Baked->srv,
		//	(ID3D11ShaderResourceView*)tex_Alpha->srv,
		//	(ID3D11ShaderResourceView*)tex_Hole->srv,
		//		
		//};
		//
		//ID3D11UnorderedAccessView* uavs[] = {
		//	(ID3D11UnorderedAccessView*)targetPos_Buffer->uav,
		//	(ID3D11UnorderedAccessView*)tc_debug_Buffer->uav,		
		//	(ID3D11UnorderedAccessView*)trW_Buffer->uav,
		//	(ID3D11UnorderedAccessView*)circle_Buffer->uav,
		//
		//};


		dcontext->CSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->CSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->CSSetUnorderedAccessViews(0, _countof(uavs), uavs, nullptr);


		auto cshader = cshader_targetPos;
		dcontext->CSSetShader(cshader, nullptr, 0);

		int dpCount = count_unit;
		dcontext->Dispatch(dpCount, 1, 1);

		//dcontext->End(fence_targetPos);
		//dcontext->Flush();
	}

	{
		if (debug)
		{
			tc_debug_Buffer->CopyFromBuffer(dcontext);
		}

		{
			targetPos_Buffer->CopyFromBuffer(dcontext);			
		}
	}

	{
		dcontext->FinishCommandList(false, &cmdList);
		context->ExecuteCommandList(cmdList, false);		
	}

	//{
	//	context->End(fence_targetPos);
	//	context->Flush();
	//}

	//while (context->GetData(fence_targetPos, nullptr, 0, 0) == S_FALSE)
	//{
	//	// CPU 점유율 폭발을 막기 위해 잠시 대기
	//	//Sleep(0);
	//
	//	//NativeLog("Sleep()");
	//}

	{
		if (debug)
		{
			trW_Buffer->ReadFromBuffer(context);
			circle_Buffer->ReadFromBuffer(context);
		}

		{
			terrainArea_Buffer->ReadFromBuffer(context);
		}

		if (debug)
		{
			{
				auto buffer = trW_Buffer;
				auto count = buffer->count;
				auto data = (XMMATRIX*)buffer->data;

				int a = 0;
			}

			{
				auto buffer = circle_Buffer;
				auto count = buffer->count;
				auto data = (XMVECTOR*)buffer->data;

				int a = 0;
			}

			{
				auto buffer = terrainArea_Buffer;
				auto count = buffer->count;
				auto data = (XMVECTOR*)buffer->data;

				int a = 0;
			}
		}
	}

	{
		{
			targetPos_Buffer->ReadFromBuffer(context);			
		}

		{
			SetEvent(hComputeDoneEvent);
		}

		if (debug)
		{
			tc_debug_Buffer->ReadFromBuffer(context);
		}

		if (debug)
		{
			{
				auto buffer = targetPos_Buffer;
				auto count = buffer->count;
				auto data = (XMVECTOR*)buffer->data;

				int a = 0;
			}

			{
				auto buffer = tc_debug_Buffer;
				auto count = buffer->count;
				auto data = (XMMATRIX*)buffer->data;

				int a = 0;
			}
		}
	}
}

void NavTestObject::Init_RectIn()
{
	rectInInfo		= new COBuffer<RectInInfo>();
	SCV_Buffer		= new ROBuffer<XMMATRIX>(2);
	pos_Buffer		= new ROBuffer<XMVECTOR>(count_unit);
	inRect_Buffer	= new RWBuffer<int>(count_unit);

}

void NavTestObject::Update_RectIn()
{
	HRESULT hr;

	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;
	ID3D11CommandList* cmdList;

	bool debug = true;
	
	{
		rectInInfo->WriteToBuffer(context);
		SCV_Buffer->WriteToBuffer(context);
		pos_Buffer->WriteToBuffer(context);
		//inRect_Buffer->WriteToBuffer(context);
	}

	{
		rectInInfo->CopyToBuffer(dcontext);
		SCV_Buffer->CopyToBuffer(dcontext);
		pos_Buffer->CopyToBuffer(dcontext);
		//inRect_Buffer->CopyToBuffer(dcontext);
	}
	
	//02_RectIn
	{
		ID3D11Buffer* cbrs[] = {
				(ID3D11Buffer*)rectInInfo->value
		};

		ID3D11ShaderResourceView* srvs[] = {
			(ID3D11ShaderResourceView*)SCV_Buffer->srv,
			(ID3D11ShaderResourceView*)pos_Buffer->srv
		};

		ID3D11UnorderedAccessView* uavs[] = {
			(ID3D11UnorderedAccessView*)inRect_Buffer->uav,			
		};

		dcontext->CSSetConstantBuffers(0, _countof(cbrs), cbrs);
		dcontext->CSSetShaderResources(0, _countof(srvs), srvs);
		dcontext->CSSetUnorderedAccessViews(0, _countof(uavs), uavs, nullptr);


		auto cshader = cshader_rectIn;
		dcontext->CSSetShader(cshader, nullptr, 0);

		const int grCount = 64;
		int dpCount = (count_unit % grCount == 0) ? (count_unit / grCount) : (count_unit / grCount + 1);
		dcontext->Dispatch(dpCount, 1, 1);

		//dcontext->Flush();	
	}

	{
		inRect_Buffer->CopyFromBuffer(dcontext);
	}

	{
		dcontext->FinishCommandList(false, &cmdList);
		context->ExecuteCommandList(cmdList, false);
	}

	{		
		{
			inRect_Buffer->ReadFromBuffer(context);
		}

		{
			SetEvent(hRectInDoneEvent);
		}

		if (debug)
		{
			auto buffer = inRect_Buffer;
			auto count = buffer->count;
			auto data = (int*)buffer->data;

			int a = 0;
		}
	}	
}

void NavTestObject::SendPtrRectIn(void** ppData0, void** ppData1, void** ppData2, void** ppData3)
{
	if (rectInInfo != nullptr)
	{
		*ppData0 = rectInInfo->data;
	}

	if (SCV_Buffer != nullptr)
	{
		*ppData1 = SCV_Buffer->data;
	}

	if (pos_Buffer != nullptr)
	{
		*ppData2 = pos_Buffer->data;
	}

	if (inRect_Buffer != nullptr)
	{
		*ppData3 = inRect_Buffer->data;
	}

}
