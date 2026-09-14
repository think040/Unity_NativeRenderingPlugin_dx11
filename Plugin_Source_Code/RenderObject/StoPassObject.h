#pragma once
#include "RenderObject.h"
#include "../GPU_Resource/GPU_Resource.h"
#include "../Utililty/DX11Fence.h"
#include "../Utililty/RenderUtil.h"

using namespace GPU_Resource;

class StoPassObject : public RenderObject
{
public:
	virtual void Init();
	virtual void Begin();
public:
	virtual void Update();
	virtual void Render();

public:
	virtual void Destroy();

public:
	static vector<StoPassObject*>* objects;
	int id_obj = -1;

public:
	virtual void EventUpdate();
	virtual void EventBeginContext();
	virtual void EventBeginCamera();
	virtual void EventRender();
	virtual void EventEndCamera();
	virtual void EventEndContext();

public:
	int InitShader();

	int InitResource();
	int InitTexture();

public:
	int RenderSO();

	int RenderAuto();
	int RenderIndex();

	typedef void (*funcInit)(void* instance);
	typedef void (*funcUpdate)(void* instance);
	typedef void (*funcRender)(void* instance);
	typedef void (*funcDestroy)(void* instance);

	typedef int (*funcGetId)(void* instance);

	typedef void (*funcIssueUpdate)(int id);
	typedef void (*funcIssueBeginContext)(int id);
	typedef void (*funcIssueBeginCamera)(int id);
	typedef void (*funcIssueRender)(int id);
	typedef void (*funcIssueEndCamera)(int id);
	typedef void (*funcIssueEndContext)(int id);

	typedef void (*funcCompute_Tex3D)(void* instance);
	typedef bool (*funcCheckGPUCompleted)(void* instance);
	typedef void (*funcSetTextureFromUnity)(void* instance, void* nativeTexturePtr);
	typedef void (*funcSetLightDataFromUnity)(void* instance, void** ppData);
	typedef void (*funcSetObjDataFromUnity)(void* instance, void** ppData);
	//SetObjDataFromUnity

	struct IRenderActor
	{
		funcInit	Init;
		funcUpdate	Update;
		funcRender	Render;
		funcDestroy Destroy;

		funcGetId		GetId;

		funcIssueUpdate			IssueUpdate;
		funcIssueBeginContext	IssueBeginContext;
		funcIssueBeginCamera	IssueBeginCamera;
		funcIssueRender			IssueRender;
		funcIssueEndCamera		IssueEndCamera;
		funcIssueEndContext		IssueEndContext;

		funcCompute_Tex3D			Compute_Tex3D;
		funcCheckGPUCompleted		CheckGPUCompleted;
		funcSetTextureFromUnity		SetTextureFromUnity;
		funcSetLightDataFromUnity	SetLightDataFromUnity;
		funcSetObjDataFromUnity		SetObjDataFromUnity;
	};

public:
	struct Vertex_SO
	{
		XMVECTOR pos;
		XMVECTOR nom;
		XMVECTOR tan;
		XMVECTOR uv;
	};

public:
	ID3D11VertexShader* vshader_so;
	ID3D11HullShader* hshader_so;
	ID3D11DomainShader* dshader_so;
	ID3D11GeometryShader* gshader_so;

public:
	ID3D11VertexShader* vshader_auto;
	ID3D11PixelShader* pshader_auto;

public:
	ID3D11VertexShader* vshader_index;
	ID3D11PixelShader* pshader_index;
public:
	ID3D11Query* pQuery;

	Buffer* stoBuffer;

	Buffer* idxBuffer_so;
	Buffer* vtxBuffer_so;

	const int tessTriCount[4] = { 1, 6, 13, 24 };

	ID3D11SamplerState* sampler = nullptr;

	ID3D11InputLayout* inpLayout = nullptr;
public:

	struct Object
	{
		XMMATRIX W;
		XMVECTOR data;
	};	

	struct Light
	{
		XMVECTOR pos;
		XMVECTOR dir;
	};

public:
	Mesh* mesh;

	
	GPU_Resource::COBuffer<Light>* lightBuffer;

	struct RenderInfo
	{
		XMVECTOR data;
	};

	GPU_Resource::COBuffer<RenderInfo>* renInfoBuffer;
	GPU_Resource::ROBuffer<Object>* objBuffer;

	UINT idxCount = 3;
	GPU_Resource::ROBuffer<WORD>* idxBuffer;

public:

	ROTexture2D* roTex_srv = nullptr;

	ID3D11ShaderResourceView* srv_tex2d0 = nullptr;
	ID3D11ShaderResourceView* srv_tex2d = nullptr;

	void SetTexureFromUnity(void* nativeTexturePtr);
	void SetLightDataFromUnity(void** ppData);
	void SetObjDataFromUnity(void** ppData);

public:

#define WIDTH	8
#define HEIGHT	8
#define DEPTH	8

	int width = WIDTH;
	int height = HEIGHT;
	int depth = DEPTH;

	RWTexture3D* tex3d = nullptr;

	int InitShader_Compute(wstring shaderRootPath);
	int InitResourece_Compute();


	int Compute_Tex3D();


	ID3D11ComputeShader* cshader_tex3d = nullptr;

	DX11Fence* fence = nullptr;
	ID3D11Query* fence_read_back = nullptr;
	
};