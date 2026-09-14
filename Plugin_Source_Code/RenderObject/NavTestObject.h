#pragma once
#include "RenderObject.h"
#include "../GPU_Resource/GPU_Resource.h"
#include "../Utililty/RenderUtil.h"

using namespace GPU_Resource;

class NavTestObject : public RenderObject
{
public:
	NavTestObject();
public:
	virtual void Init();
	virtual void Begin();
public:
	virtual void Update();
	virtual void Render();

public:
	virtual void Destroy();

public:
	static vector<NavTestObject*>* objects;
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

	enum ID_HANDEL : int
	{
		TargetPos = 0,		
		RectIn = 1,
		Count = 2
	};


	typedef void (*funcInit)(void* instance);
	typedef void (*funcBegin)(void* instance);
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

	typedef void (*funcSetTextureFromUnity)(void* instance, void* nativeTexturePtr, int idx);
	typedef void (*funcSetTextureModeId)(void* instance, int id);
	typedef void (*funcSendDataPtrToUnity)(void* instance, void** ppData0, void** ppData1, void** ppData2);
	typedef void (*funcSendUnitCount)(void* instance, int count_unit);

	typedef void (*funcSendPtrTargetPos)(void* instance, void** ppData0, void** ppData1, void** ppData2, void** ppData3, void** ppData4);
	typedef void (*funcSendComputeDoneEvent)(void* instance, void** pHandle);
	typedef void (*funcSendMoveDoneEvent)(void* instance, void* handle);

	typedef void (*funcSetRenderDebugPosEnable)(void* instance, void** ppData);

	typedef void (*funcIssueUpdate_rectIn)(int id);
	typedef void (*funcSendPtrRectIn)(void* instance, void** ppData0, void** ppData1, void** ppData2, void** ppData3);
	typedef void (*funcSendRectInDoneEvent)(void* instance, void** pHandle);

	typedef void (*funcGetHandle)(void* instance, void** pHandle, int id);
	
	
	struct IRenderActor
	{
		funcInit	Init;
		funcBegin	Begin;
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

		funcSetTextureFromUnity	SetTextureFromUnity;
		funcSetTextureModeId	SetTextureModeId;
		funcSendDataPtrToUnity	SendDataPtrToUnity;
		funcSendUnitCount		SendUnitCount;

		funcSendPtrTargetPos	SendPtrTargetPos;
		funcSendComputeDoneEvent	SendComputeDoneEvent;
		funcSendMoveDoneEvent		SendMoveDoneEvent;

		funcSetRenderDebugPosEnable		SetRenderDebugPosEnable;

		funcIssueUpdate_rectIn			IssueUpdate_rectIn;
		funcSendPtrRectIn				SendPtrRectIn;
		funcSendRectInDoneEvent			SendRectInDoneEvent;

		funcGetHandle					GetHandle;
	};

public:
	ID3D11VertexShader* vshader_texViewer;
	ID3D11PixelShader* pshader_texViewer;

public:
	ID3D11VertexShader*		vshader_terrain;
	ID3D11HullShader*		hshader_terrain;
	ID3D11DomainShader*		dshader_terrain;
	ID3D11GeometryShader*	gshader_terrain;
	ID3D11PixelShader*		pshader_terrain;

public:
	ID3D11VertexShader* vshader_unit;
	ID3D11PixelShader*	pshader_unit;

public:
	ID3D11VertexShader* vshader_debug;
	ID3D11PixelShader*	pshader_debug;

public:
	ID3D11ComputeShader* cshader_trW;
	ID3D11ComputeShader* cshader_targetPos;
	ID3D11ComputeShader* cshader_rectIn;

public:
	ID3D11ShaderResourceView* srv_tex2d[6];
	void SetTexureFromUnity(void* nativeTexturePtr, int idx);

public:
	ID3D11SamplerState* sampler = nullptr;

	void Render_TextureViewer();

public:
	GPU_Resource::COBuffer<XMVECTOR>* renMode_texViewer;
	int renMode_texViewer_id = 0;

public:
	struct VertexData
	{
		XMVECTOR posL;
	};

	struct TerrainData
	{
		XMVECTOR tileSize;
		XMVECTOR terrainSize;
		XMVECTOR tileCount;
		XMVECTOR layerSize;
	};

	struct ObjectData
	{
		XMMATRIX W;
		XMVECTOR data;
	};

	struct LightData
	{
		XMVECTOR pos;
		XMVECTOR dir;
	};

	XMVECTOR terrainSize;
	XMVECTOR tileCount;
	XMVECTOR layerSize;	

	GPU_Resource::ROBuffer<VertexData>* vtxBuffer_terrain = nullptr;
	GPU_Resource::ROBuffer<USHORT>* idxBuffer_terrain = nullptr;

	GPU_Resource::COBuffer<TerrainData>* terrainDataBuffer = nullptr;
	GPU_Resource::COBuffer<ObjectData>* objDataBuffer = nullptr;

	GPU_Resource::COBuffer<LightData>* lightDataBuffer = nullptr;


	void InitTerrain();

public:
	
	string strAssetPath;
	wstring wstrAssetPath;


	GPU_Resource::ROTexture2D* tex_NHmap_Baked = nullptr;
	GPU_Resource::ROTexture2D* tex_Alpha = nullptr;
	GPU_Resource::ROTexture2D* tex_Hole = nullptr;

	GPU_Resource::ROTexture2D* tex_diffuse0 = nullptr;
	GPU_Resource::ROTexture2D* tex_diffuse1 = nullptr;
	GPU_Resource::ROTexture2D* tex_diffuse2 = nullptr;

	void Load_NormalHeightMap_Baked();

	void Load_AlphaMap();

	void Load_HoleMap();

	void Load_Terrain_Texture();

public:
	void SendDataPtrToUnity(void** ppData0, void** ppData1, void** ppData2);

	void RenderTerrain();

public:
	void InitUnitSimple();

	void RenderUnitSimple();

	int count_unit = 1;
	Mesh* mesh_unit = nullptr;

	GPU_Resource::ROBuffer<ObjectData>* unitObjBuffer = nullptr;

public:
	bool bRender_debugPos = true;

	void InitDebugPos();

	void RenderDebugPos();

	Mesh* mesh_debug = nullptr;

public:
	struct TargetComputeInfo
	{
		XMVECTOR t1_t0;
		XMMATRIX T;
		XMVECTOR countInfo;
	};

	void Init_targetCompute();

	void SendPtrTargetPos(void** ppData0, void** ppData1, void** ppData2, void** ppData3, void** ppData4);

	GPU_Resource::COBuffer<TargetComputeInfo>* tc_info_Buffer;

	GPU_Resource::ROBuffer<XMMATRIX>* trM_Buffer;

	GPU_Resource::RWBuffer<XMMATRIX>* trW_Buffer;
	GPU_Resource::RWBuffer<XMVECTOR>* circle_Buffer;
	GPU_Resource::RWBuffer<XMVECTOR>* terrainArea_Buffer;

	GPU_Resource::ROBuffer<XMVECTOR>* block_Buffer;
	GPU_Resource::ROBuffer<int>* random_Buffer;
	
	GPU_Resource::RWBuffer<XMVECTOR>* targetPos_Buffer;
	GPU_Resource::RWBuffer<XMMATRIX>* tc_debug_Buffer;

	//Debug_Pos
	GPU_Resource::ROBuffer<XMMATRIX>* WBuffer_debug;

	GPU_Resource::ROBuffer<int>* select_Buffer;


	ID3D11Query* fence_targetPos = nullptr;
	
	void UpdateTarget();

	HANDLE (*handles)[ID_HANDEL::Count];

	HANDLE  hComputeDoneEvent = nullptr;
	HANDLE  hMoveDoneEvent = nullptr;

	HANDLE  hRectInDoneEvent = nullptr;

public:

	struct RectInInfo
	{
		XMVECTOR rect;
		XMVECTOR data;
	};
	
	GPU_Resource::COBuffer<RectInInfo>*		rectInInfo		;	
	GPU_Resource::ROBuffer<XMMATRIX>*		SCV_Buffer		;
	GPU_Resource::ROBuffer<XMVECTOR>*		pos_Buffer		;
	GPU_Resource::RWBuffer<int>*			inRect_Buffer	;

	void Init_RectIn();

	void Update_RectIn();

	void SendPtrRectIn(void** ppData0, void** ppData1, void** ppData2, void** ppData3);
};