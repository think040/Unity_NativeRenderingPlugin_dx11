#include "Header.h"
#include "PlatformBase.h"
#include "RenderManager.h"
#include "Utililty/Utility.h"
#include "Gpu/Gpu.h"

#include "RenderObject/StoPassObject.h"

IUnityInterfaces* s_UnityInterfaces = NULL;
IUnityGraphics* s_Graphics = NULL;
IUnityGraphicsD3D11* s_d3d11 = nullptr;

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	s_UnityInterfaces = unityInterfaces;
	s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
	s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);

	s_d3d11 = s_UnityInterfaces->Get<IUnityGraphicsD3D11>();

	//if (s_d3d11 == nullptr)
	//{
	//	NativeLog("s_d3d11 == nullptr");
	//}


	// Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" UNITY_INTERFACE_EXPORT void  UNITY_INTERFACE_API UnityPluginUnload()
{
	s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

static void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{	
	if (eventType == kUnityGfxDeviceEventInitialize)
	{
		
	}

	if (eventType == kUnityGfxDeviceEventShutdown)
	{		
		int a = 10;
		int b = 20;
		int c = a + b;

	}
}


extern "C" UNITY_INTERFACE_EXPORT void  UNITY_INTERFACE_API Start_Plugin()
{
	NativeLog("Start_Plugin");

	Gpu::Init(s_UnityInterfaces);
	RenderManager::Init();

	int a = 0;
}

extern "C" UNITY_INTERFACE_EXPORT void  UNITY_INTERFACE_API Finish_Plugin()
{
	NativeLog("Finish_Plugin");

	RenderManager::Finish();
}

static void UNITY_INTERFACE_API OnRenderEvent(int eventID)
{	
	RenderManager::Render();
}




extern "C" UNITY_INTERFACE_EXPORT void  UNITY_INTERFACE_API SetOnEditor(bool _onEditor)
{
	RenderManager::onEditor = _onEditor;

	int a = 0;
}

extern "C" UNITY_INTERFACE_EXPORT void  UNITY_INTERFACE_API SetAssetPath(const char* path)
{
	if (path == nullptr) { return; }

	//RenderManager::assetPath = wstring(path);
	RenderManager::assetPath = string(path);

	int a = 0;
}



extern "C" UNITY_INTERFACE_EXPORT UnityRenderingEvent  UNITY_INTERFACE_API GetRenderEventFunc()
{
	return OnRenderEvent;
}

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API GetPerCam_ptr(void** ppData)
{
	auto buffer = RenderManager::perCam_Buffer;
	if (buffer == nullptr) { return; }
	
	*ppData = buffer->data;
}



extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API SetRastMode(int mode)
{
	RenderManager::rastMode = mode;

	int a = 0;
}




bool RenderManager::onEditor;
string RenderManager::assetPath;
string RenderManager::exePath;
int RenderManager::rastMode = 0;


ID3D11CommandList* RenderManager::cmdList = nullptr;
vector<RenderObject*>* RenderManager::objects = nullptr;


ROBuffer<PerCamera>* RenderManager::perCam_Buffer = nullptr;
COBuffer<XMVECTOR>* RenderManager::camInfo_Buffer = nullptr;


int RenderManager::maxCamCount = 8;

UINT RenderManager::idx_cam = 0;

void RenderManager::Init()
{
	exePath = GetExecutableDirectory();

	{
		string str = std::format("assetPath : {}", assetPath);			
		NativeLog(str.c_str());
	}

	{
		string str = std::format("exePath : {}", exePath);
		NativeLog(str.c_str());
	}

	{
		//perCam_Buffer = new COBuffer<PerCamera>();
		perCam_Buffer = new ROBuffer<PerCamera>(maxCamCount);
		camInfo_Buffer = new COBuffer<XMVECTOR>();
	}

	{
		rtv_perCam = new void* [maxCamCount];
		dsv_perCam = new void* [maxCamCount];

		for (int i = 0; i < maxCamCount; i++)
		{
			rtv_perCam[i] = nullptr;
			dsv_perCam[i] = nullptr;
		}
	}

	//{
	//	renTexInfo = new void** [maxCamCount];
	//
	//	for (int i = 0; i < maxCamCount; i++)
	//	{
	//		renTexInfo[i] = new void* [RenTexID::Count];
	//		for (int j = 0; j < RenTexID::Count; j++)
	//		{
	//			renTexInfo[i][j] = nullptr;
	//		}
	//	}
	//}

	//{
	//	camInfo = new unordered_map<CameraType, PerCamera>();
	//}

	{
		objects = new vector<RenderObject*>();
	}

	{
		Init_RenderState();
	}
}



void RenderManager::Render()
{
	int a = 10;
	int b = 20;
	int c = a + b;

	//NativeLog("OnRenderEvent()");

	HRESULT hr = S_OK;

	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;
		
	{	
		IssueBeginContext();

		{
			auto ptrCam = (PerCamera*)RenderManager::perCam_Buffer->data;
			int count = RenderManager::perCam_Buffer->count;

			{
				perCam_Buffer->WriteToBuffer(context);
				perCam_Buffer->CopyToBuffer(context);
			}

			for (int i = 0; i < count; i++)
			{
				bool bEnable = ptrCam[i].data.m128_f32[0] == 1 ? true : false;

				if (bEnable)
				{
					string str = "CameraId : " + std::to_string(i);
					RenderManager::perCam = (ptrCam + i);

					{
						idx_cam = i;
						auto ptr_id = (float*)(camInfo_Buffer->data);
						*ptr_id = (float)i;

						camInfo_Buffer->WriteToBuffer(context);
						camInfo_Buffer->CopyToBuffer(context);
					}

					{
						//NativeLog(str.c_str());
					}

					{
						RenderManager::BeginRenderTexture();
					}
					
					{
						IssueBeginCamera();
						IssueRender();
						IssueEndCamera();
					}

					{
						RenderManager::EndRenderTexture();
					}

					{
						//NativeLog(str.c_str());
					}
				}
			}
		}

		IssueEndContext();
	}


}

void RenderManager::Finish()
{
	if(objects != nullptr)
	{
		delete objects;
		objects = nullptr;
	}

	DeleteTargetTexture();
}


int RenderManager::Init_RenderState()
{
	HRESULT hr;
	auto device = Gpu::device;

	{
		D3D11_RASTERIZER_DESC rdesc = {};
		rdesc.FrontCounterClockwise = false;
		rdesc.DepthBias = 0;
		rdesc.DepthBiasClamp = 0.0f;
		rdesc.SlopeScaledDepthBias = 0.0f;
		rdesc.DepthClipEnable = false;
		rdesc.ScissorEnable = false;
		rdesc.MultisampleEnable = true;
		rdesc.AntialiasedLineEnable = false;

		//rdesc.CullMode = D3D11_CULL_NONE;

		rdesc.FillMode = D3D11_FILL_SOLID;
		{
			rdesc.CullMode = D3D11_CULL_BACK;
			hr = device->CreateRasterizerState(&rdesc, &rastSolid);
			rdesc.CullMode = D3D11_CULL_FRONT;
			hr = device->CreateRasterizerState(&rdesc, &rastSolid_cull_front);
		}

		rdesc.FillMode = D3D11_FILL_WIREFRAME;
		{
			rdesc.CullMode = D3D11_CULL_BACK;
			hr = device->CreateRasterizerState(&rdesc, &rastWire);
			rdesc.CullMode = D3D11_CULL_FRONT;
			hr = device->CreateRasterizerState(&rdesc, &rastWire_cull_front);
		}


	}

	return 0;
}



void RenderManager::IssueUpdate()
{	
	for (auto obj : *objects)
	{
		obj->EventUpdate();
	}
}

void RenderManager::IssueBeginContext()
{
	for (auto obj : *objects)
	{
		obj->EventBeginContext();
	}
}

void RenderManager::IssueBeginCamera()
{
	for (auto obj : *objects)
	{
		obj->EventBeginCamera();
	}
}

void RenderManager::IssueRender()
{
	//if (objects == nullptr) { return; }
	
	for (auto obj : *objects)
	{
		//if (obj == nullptr) { return; }
		obj->EventRender();
	}
}

void RenderManager::IssueEndCamera()
{
	for (auto obj : *objects)
	{
		obj->EventEndCamera();
	}
}

void RenderManager::IssueEndContext()
{
	for (auto obj : *objects)
	{
		obj->EventEndContext();
	}
}


void* RenderManager::roTex_rtv = nullptr;
void* RenderManager::roTex_dsv = nullptr;

void** RenderManager::rtv_perCam = nullptr;
void** RenderManager::dsv_perCam = nullptr;

void*** RenderManager::renTexInfo = nullptr;	//final

ID3D11Texture2D* RenderManager::tex_rtv = nullptr;
ID3D11Texture2D* RenderManager::tex_dsv = nullptr;
ID3D11RenderTargetView* RenderManager::rtv = nullptr;
ID3D11DepthStencilView* RenderManager::dsv = nullptr;
ID3D11Texture2D* RenderManager::tex_rtv0 = nullptr;
ID3D11Texture2D* RenderManager::tex_dsv0 = nullptr;
ID3D11RenderTargetView* RenderManager::rtv0 = nullptr;
ID3D11DepthStencilView* RenderManager::dsv0 = nullptr;


int RenderManager::preWidth = 0;
int RenderManager::preHeight = 0;
int RenderManager::currentWidth = 0;
int RenderManager::currentHeight = 0;

D3D11_VIEWPORT RenderManager::viewPort;

ID3D11RasterizerState* RenderManager::rastSolid = nullptr;
ID3D11RasterizerState* RenderManager::rastWire = nullptr;

ID3D11RasterizerState* RenderManager::rastSolid_cull_front = nullptr;
ID3D11RasterizerState* RenderManager::rastWire_cull_front = nullptr;

ID3D11RasterizerState* RenderManager::rastState = nullptr;

PerCamera* RenderManager::perCam = nullptr;


int RenderManager::BeginRenderTexture()
{
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	{
		context->OMGetRenderTargets(1, &rtv0, &dsv0);

		{
			ID3D11Resource* pRes = nullptr;
			D3D11_RENDER_TARGET_VIEW_DESC desc_rtv;
			rtv0->GetResource(&pRes);
			rtv0->GetDesc(&desc_rtv);

			ID3D11Texture2D* pTexture = nullptr;
			if (SUCCEEDED(pRes->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture)))
			{
				D3D11_TEXTURE2D_DESC desc;
				pTexture->GetDesc(&desc); // 이제 가로, 세로, 포맷 등을 알 수 있음
				//pTexture->Release();			

				tex_rtv0 = pTexture;

				//currentWidth  = desc.Width;
				//currentHeight = desc.Height;

				{
					float* ptrSize = (float*)&(perCam->pixelSize);
					ptrSize[2] = currentWidth  = desc.Width;
					ptrSize[3] = currentHeight = desc.Height;
				}				
				
			}
			//pRes->Release();
		}

		{
			ID3D11Resource* pRes = nullptr;
			D3D11_DEPTH_STENCIL_VIEW_DESC desc_dsv;
			dsv0->GetResource(&pRes);
			dsv0->GetDesc(&desc_dsv);

			ID3D11Texture2D* pTexture = nullptr;
			if (SUCCEEDED(pRes->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&pTexture)))
			{
				D3D11_TEXTURE2D_DESC desc;
				pTexture->GetDesc(&desc); // 이제 가로, 세로, 포맷 등을 알 수 있음
				//pTexture->Release();

				tex_dsv0 = pTexture;
			}
			//pRes->Release();
		}

	}

	{
		UpdateTargetTexture();
	}	

	ID3D11RasterizerState* rastStates[][2] = {
		{rastSolid, rastSolid_cull_front},
		{rastWire,  rastSolid_cull_front},
		{rastSolid,	rastWire_cull_front},
		{rastWire,	rastWire_cull_front}
	};

	int rastMode = RenderManager::rastMode;
	rastState = rastStates[0][0];

	{
		auto ptr_id = (float*)camInfo_Buffer->data;
	
		auto ptrCam = ((PerCamera*)(perCam_Buffer->data)) + (int)(*ptr_id);
		
		if (ptrCam->data.m128_f32[1] == 1)	//Game_View
		{
			rastState = rastStates[rastMode][0];
			//rastState = rastWire;
		}
		else if (ptrCam->data.m128_f32[1] == 2)	//Scene_View
		{
			//rastState = rastSolid_cull_front;
			rastState = rastStates[rastMode][1];
		}
	}

	//{
	//	auto ptr_id = (int*)camInfo_Buffer->data;
	//
	//	auto data = (PerCamera*)perCam_Buffer->data;
	//
	//	if (data[0].data.m128_f32[0] == 1)	//Game_View
	//	{
	//		rastState = rastStates[rastMode][0];
	//		//rastState = rastWire;
	//	}
	//	else if (data[1].data.m128_f32[0] == 1)	//Scene_View
	//	{
	//		//rastState = rastSolid_cull_front;
	//		rastState = rastStates[rastMode][1];
	//	}
	//}

	{
		InitTargetTexture();
	}

	return 0;
}

int RenderManager::EndRenderTexture()
{
	auto context = Gpu::context;

	{
		context->ResolveSubresource(tex_rtv0, 0, tex_rtv, 0, DXGI_FORMAT_R8G8B8A8_UNORM);
		context->ResolveSubresource(tex_dsv0, 0, tex_dsv, 0, DXGI_FORMAT_D24_UNORM_S8_UINT);
	}


	return 0;
}

void RenderManager::CreateTargetTexture0(int w, int h)
{
	HRESULT hr = S_OK;
	auto device = Gpu::device;	

	auto ReleaseTex =
		[](ROTexture2D** tex)
		{
			if (*tex != nullptr)
			{
				delete *tex;
				*tex = nullptr;
			}
		};

	//delete (ROTexture2D*)roTex_rtv;
	//delete (ROTexture2D*)roTex_dsv;

	ReleaseTex((ROTexture2D**)&roTex_rtv);
	ReleaseTex((ROTexture2D**)&roTex_dsv);

	roTex_rtv = (ROTexture2D*)new ROTexture2D(DXGI_FORMAT_R8G8B8A8_UNORM, sizeof(DWORD), w, h, 0, false, true, true, true, false);
	roTex_dsv = (ROTexture2D*)new ROTexture2D(DXGI_FORMAT_D24_UNORM_S8_UINT, sizeof(DWORD), w, h, 0, false, false, true, false, true);


	tex_rtv = (ID3D11Texture2D*)((ROTexture2D*)roTex_rtv)->value;
	tex_dsv = (ID3D11Texture2D*)((ROTexture2D*)roTex_dsv)->value;

	rtv = (ID3D11RenderTargetView*)((ROTexture2D*)roTex_rtv)->rtv;
	dsv = (ID3D11DepthStencilView*)((ROTexture2D*)roTex_dsv)->dsv;
}

void RenderManager::CreateTargetTexture1(int w, int h)
{
	HRESULT hr = S_OK;
	auto device = Gpu::device;

	auto ReleaseTex =
		[](ROTexture2D** tex)
		{
			if (*tex != nullptr)
			{
				delete* tex;
				*tex = nullptr;
			}
		};
	
	void** pptr_rtv = &rtv_perCam[idx_cam];
	void** pptr_dsv = &dsv_perCam[idx_cam];

	ReleaseTex((ROTexture2D**)pptr_rtv);
	ReleaseTex((ROTexture2D**)pptr_dsv);

	*pptr_rtv = (ROTexture2D*)new ROTexture2D(DXGI_FORMAT_R8G8B8A8_UNORM, sizeof(DWORD), w, h, 0, false, true, true, true, false);
	*pptr_dsv = (ROTexture2D*)new ROTexture2D(DXGI_FORMAT_D24_UNORM_S8_UINT, sizeof(DWORD), w, h, 0, false, false, true, false, true);


	tex_rtv = (ID3D11Texture2D*)((ROTexture2D*)*pptr_rtv)->value;
	tex_dsv = (ID3D11Texture2D*)((ROTexture2D*)*pptr_dsv)->value;

	rtv = (ID3D11RenderTargetView*)((ROTexture2D*)*pptr_rtv)->rtv;
	dsv = (ID3D11DepthStencilView*)((ROTexture2D*)*pptr_dsv)->dsv;

}

void RenderManager::CreateTargetTexture(int w, int h)
{
	HRESULT hr = S_OK;
	auto device = Gpu::device;

	if(renTexInfo == nullptr)
	{
		renTexInfo = new void** [maxCamCount];

		for (int i = 0; i < maxCamCount; i++)
		{
			renTexInfo[i] = new void* [RenTexID::Count];
			for (int j = 0; j < RenTexID::Count; j++)
			{
				renTexInfo[i][j] = nullptr;
			}
		}
	}

	{
		auto ReleaseTex =
			[](ROTexture2D** tex)
			{
				if (*tex != nullptr)
				{
					delete* tex;
					*tex = nullptr;
				}
			};

		void*** ppptr = (renTexInfo + idx_cam);

		void** pptr_rtv = (*(ppptr)+RenTexID::RTV);	//  &rtv_perCam[idx_cam];
		void** pptr_dsv = (*(ppptr)+RenTexID::DSV);	//	&dsv_perCam[idx_cam];

		ReleaseTex((ROTexture2D**)pptr_rtv);
		ReleaseTex((ROTexture2D**)pptr_dsv);

		*pptr_rtv = (ROTexture2D*)new ROTexture2D(DXGI_FORMAT_R8G8B8A8_UNORM, sizeof(DWORD), w, h, 0, false, true, true, true, false);
		*pptr_dsv = (ROTexture2D*)new ROTexture2D(DXGI_FORMAT_D24_UNORM_S8_UINT, sizeof(DWORD), w, h, 0, false, false, true, false, true);


		tex_rtv = (ID3D11Texture2D*)((ROTexture2D*)*pptr_rtv)->value;
		tex_dsv = (ID3D11Texture2D*)((ROTexture2D*)*pptr_dsv)->value;

		rtv = (ID3D11RenderTargetView*)((ROTexture2D*)*pptr_rtv)->rtv;
		dsv = (ID3D11DepthStencilView*)((ROTexture2D*)*pptr_dsv)->dsv;
	}

	

}

void RenderManager::DeleteTargetTexture()
{
	auto ReleaseTex =
		[](ROTexture2D** tex)
		{
			if (*tex != nullptr)
			{
				delete* tex;
				*tex = nullptr;
			}
		};

	int a = 0;

	if (renTexInfo != nullptr)
	{
		for (int i = 0; i < maxCamCount; i++)
		{
			for (int j = 0; j < RenTexID::Count; j++)
			{
				ReleaseTex((ROTexture2D**)&renTexInfo[i][j]);			
				a = 0;
			}
			delete[] renTexInfo[i];
			renTexInfo[i] = nullptr;
		}
		delete renTexInfo;
		renTexInfo = nullptr;
	}

	a = 0;
	
}

void RenderManager::UpdateTargetTexture()
{	
	{		
		float* ptrSize = (float*)&(perCam->pixelSize);

		{			
			preWidth  = ptrSize[0];
			preHeight = ptrSize[1];
		}

		if (preWidth != currentWidth || preHeight != currentHeight)
		{
			CreateTargetTexture(currentWidth, currentHeight);

			{
				string str = std::format("width : {}, height {}", currentWidth, currentHeight);
				NativeLog(str.c_str());
			}
			
			{
				ptrSize[0] = ptrSize[2];
				ptrSize[1] = ptrSize[3];
			}

			//{
			//	preWidth  = ptrSize[0] = ptrSize[2];
			//	preHeight = ptrSize[1] = ptrSize[3];
			//}

			{
				viewPort.TopLeftX = 0.0f;
				viewPort.TopLeftY = 0.0f;
				//viewPort.TopLeftY = (FLOAT)currentHeight;
				viewPort.Width = (FLOAT)currentWidth;
				viewPort.Height = (FLOAT)currentHeight;
				viewPort.MinDepth = 0.0f;
				viewPort.MaxDepth = 1.0f;
			}		
		}
	}
	

}

void RenderManager::InitTargetTexture()
{
	HRESULT hr = S_OK;
	auto context = Gpu::context;
	auto dcontext = Gpu::dcontext;

	{
		//FLOAT clearColor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
		FLOAT clearColor[] = { 0.0f, 0.75f, 0.75f, 1.0f };

		{
			dcontext->RSSetViewports(1, &RenderManager::viewPort);
			dcontext->RSSetState(RenderManager::rastState);

			//dcontext->OMSetDepthStencilState(dsState, 0);
			//dcontext->OMSetBlendState()
		}

		{
			ID3D11RenderTargetView* rtvs[] = { RenderManager::rtv };
			dcontext->OMSetRenderTargets(_countof(rtvs), rtvs, RenderManager::dsv);
			dcontext->ClearRenderTargetView(RenderManager::rtv, clearColor);
			dcontext->ClearDepthStencilView(RenderManager::dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}

		{
			hr = dcontext->FinishCommandList(true, &cmdList);
			context->ExecuteCommandList(cmdList, true);
		}

	}

}






