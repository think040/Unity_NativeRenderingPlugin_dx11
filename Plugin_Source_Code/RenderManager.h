#pragma once
#include "Unity/IUnityGraphicsD3D11.h"
#include "Unity/IUnityGraphics.h"
#include "GPU_Resource/GPU_Resource.h"
#include "RenderObject/RenderObject.h"

extern "C" IUnityInterfaces* s_UnityInterfaces;
extern "C" IUnityGraphics* s_Graphics;
extern "C" IUnityGraphicsD3D11* s_d3d11;

using namespace DirectX;
using namespace GPU_Resource;

struct PerCamera
{
    XMMATRIX V;
    XMMATRIX C;
    XMMATRIX S;
    XMMATRIX CV;
    XMVECTOR dirW_view;
    XMVECTOR posW_view;
	XMVECTOR pixelSize;		//0, 1 : pre	// 2, 3 : current
	XMVECTOR data;
};


class RenderManager
{
public:
	static bool onEditor;
	static string assetPath;
	static string exePath;
    
	static ROBuffer<PerCamera>* perCam_Buffer;
	static COBuffer<XMVECTOR>* camInfo_Buffer;

public:
	static void Init();
	static void Render();

	static void Finish();

	static int Init_RenderState();
public:
	static void IssueUpdate();
	static void IssueBeginContext();
	static void IssueBeginCamera();
	static void IssueRender();
	static void IssueEndCamera();
	static void IssueEndContext();

public:
	static int BeginRenderTexture();
	static int EndRenderTexture();

	static void CreateTargetTexture0(int w, int h);
	static void CreateTargetTexture1(int w, int h);
	static void CreateTargetTexture(int w, int h);
	static void DeleteTargetTexture();

	static void UpdateTargetTexture();
	static void InitTargetTexture();

	static ID3D11CommandList* cmdList;
	static vector<RenderObject*>* objects;

	//static ROTexture2D* roTex_rtv;
	//static ROTexture2D* roTex_dsv;
	static void* roTex_rtv;
	static void* roTex_dsv;

	static void** rtv_perCam;	//non_use
	static void** dsv_perCam;	//non_use

	static void*** renTexInfo;

	enum RenTexID : int
	{
		RTV = 0, DSV = 1, Count = 2,
	};
	

	static ID3D11Texture2D* tex_rtv;
	static ID3D11Texture2D* tex_dsv;

	static ID3D11RenderTargetView* rtv;
	static ID3D11DepthStencilView* dsv;

	static ID3D11Texture2D* tex_rtv0;
	static ID3D11Texture2D* tex_dsv0;

	static ID3D11RenderTargetView* rtv0;
	static ID3D11DepthStencilView* dsv0;


	static int preWidth;
	static int preHeight;

	static int currentWidth;
	static int currentHeight;

	static D3D11_VIEWPORT viewPort;
	
	
	static PerCamera* perCam;
	static UINT idx_cam;

	static int maxCamCount;
	
	static int rastMode;

public:
	static ID3D11RasterizerState* rastSolid;
	static ID3D11RasterizerState* rastWire;

	static ID3D11RasterizerState* rastSolid_cull_front;
	static ID3D11RasterizerState* rastWire_cull_front;

	static ID3D11RasterizerState* rastState;


};