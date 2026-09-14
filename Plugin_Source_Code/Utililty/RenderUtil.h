#include "../Header.h"
//#include "Pipeline/Pipeline.h"
#include "../GPU_Resource/GPU_Resource.h"

using namespace DirectX;
using namespace GPU_Resource;

#ifndef  RENDERUTL_H
#define RENDERUTL_H

class RenderUtil
{
public:
	static inline XMMATRIX GetLtoW(XMVECTOR pos, XMVECTOR rot, XMVECTOR sca);

    static inline XMMATRIX GetWtoV(XMVECTOR pos, XMVECTOR rot);
    static inline XMMATRIX GetVtoC(XMVECTOR fi, int type = 0);
    static inline XMMATRIX GetNtoS(XMVECTOR pos, XMVECTOR sca);

    static inline XMMATRIX GetStoN(XMVECTOR pos, XMVECTOR sca);
    static inline XMMATRIX GetCtoV(XMVECTOR fi, int type = 0);
    static inline XMMATRIX GetVtoW(XMVECTOR pos, XMVECTOR rot);

    //
    static inline void GetWtoV(XMVECTOR* pos, XMVECTOR* rot, XMMATRIX* V);
    static inline void GetVtoC(XMVECTOR* fi, int type, XMMATRIX* C);
    static inline void GetNtoS(XMVECTOR* pos, XMVECTOR* sca, XMMATRIX* S);

    static inline void GetStoN(XMVECTOR* pos, XMVECTOR* sca, XMMATRIX* N);
    static inline void GetCtoV(XMVECTOR* fi, int type, XMMATRIX* V);
    static inline void GetVtoW(XMVECTOR* pos, XMVECTOR* rot, XMMATRIX* W);

    //
    static inline XMMATRIX GetNtoT();
    static inline void GetNtoT(XMMATRIX* T);

    //
    static inline void GetPosition_WtoS(
        XMVECTOR* posIn,
        XMVECTOR posW, XMVECTOR rotW,
        XMVECTOR fi, bool type,
        XMVECTOR posS, XMVECTOR scaS,
        XMVECTOR* posOut, bool zNormal = true);

    static inline void GetRay_StoW(
        XMVECTOR* posIn,
        XMVECTOR posS, XMVECTOR scaS,
        XMVECTOR fi, bool type,
        XMVECTOR posW, XMVECTOR rotW,
        XMVECTOR* rayOrigin, XMVECTOR* rayDirection);

    static inline void GetRay_StoW(
        XMVECTOR* posIn,
        XMVECTOR posS, XMVECTOR scaS,
        XMVECTOR fi, int type,
        XMVECTOR posW, XMVECTOR rotW,
        XMVECTOR* rayOrigin, XMVECTOR* rayDirection);

    static inline void GetPosition_WtoS(
        XMVECTOR* posIn,
        XMVECTOR* posW, XMVECTOR* rotW,
        XMVECTOR* fi, bool type,
        XMVECTOR* posS, XMVECTOR* scaS,
        XMVECTOR* posOut, bool zNormal = true);

    static inline void GetRay_StoW(
        XMVECTOR* posIn,
        XMVECTOR* posS, XMVECTOR* scaS,
        XMVECTOR* fi, bool type,
        XMVECTOR* posW, XMVECTOR* rotW,
        XMVECTOR* rayOrigin, XMVECTOR* rayDirection);

    //
    static inline XMVECTOR Vector3Transform(XMVECTOR vec, XMMATRIX mat);

    static inline void Vector3Transform(XMVECTOR* vec, XMMATRIX* mat, XMVECTOR* vecOut);
};

class Mesh
{
    Mesh();
    ~Mesh();

public:
    struct Vertex
    {
        XMVECTOR pos;
        XMVECTOR nom;
        XMVECTOR tan;
        XMVECTOR uv;
        XMVECTOR boneI;
        XMVECTOR boneW;
    };


    vector<Vertex>* vtx;
    vector<WORD>* idx;

    //ID3D11Buffer* vtxBuffer;
    //ID3D11Buffer* idxBuffer;

	ROBuffer<WORD>* idxBuffer;
	ROBuffer<Vertex>* vtxBuffer;

	

    int vCount;
    int iCount;
    int insCount;

    static Mesh* CreateCubeMesh(float size = 1.0f, int insCount = 1)
    {
        Mesh* mesh = new Mesh();
        vector<Vertex>* mvtx = mesh->vtx;
        vector<WORD>* midx = mesh->idx;

        float r0 = size * 0.5f;
        int s0 = 4;
        int s1 = 4;

        Vertex vtx[4];
        {
			{
				vtx[0].pos = XMVectorSet(+r0, +r0, -r0, +1.0f);
				vtx[1].pos = XMVectorSet(+r0, -r0, -r0, +1.0f);
				vtx[2].pos = XMVectorSet(-r0, -r0, -r0, +1.0f);
				vtx[3].pos = XMVectorSet(-r0, +r0, -r0, +1.0f);
			
			}

			//{
			//	vtx[0].pos = XMVectorSet(+r0, +r0, 0.5f, +1.0f);
			//	vtx[1].pos = XMVectorSet(+r0, -r0, 0.5f, +1.0f);
			//	vtx[2].pos = XMVectorSet(-r0, -r0, 0.5f, +1.0f);
			//	vtx[3].pos = XMVectorSet(-r0, +r0, 0.5f, +1.0f);
			//}

            {
                vtx[0].nom = XMVectorSet(+0.0f, +0.0f, -1.0f, +0.0f);
                vtx[1].nom = XMVectorSet(+0.0f, +0.0f, -1.0f, +0.0f);
                vtx[2].nom = XMVectorSet(+0.0f, +0.0f, -1.0f, +0.0f);
                vtx[3].nom = XMVectorSet(+0.0f, +0.0f, -1.0f, +0.0f);
            }

            {
                vtx[0].tan = XMVectorSet(-1.0f, +0.0f, +0.0f, +0.0f);
                vtx[1].tan = XMVectorSet(-1.0f, +0.0f, +0.0f, +0.0f);
                vtx[2].tan = XMVectorSet(-1.0f, +0.0f, +0.0f, +0.0f);
                vtx[3].tan = XMVectorSet(-1.0f, +0.0f, +0.0f, +0.0f);
            }

            {
                vtx[0].uv = XMVectorSet(+0.0f, +0.0f, +0.0f, +0.0f);
                vtx[1].uv = XMVectorSet(+0.0f, +1.0f, +0.0f, +0.0f);
                vtx[2].uv = XMVectorSet(+1.0f, +1.0f, +0.0f, +0.0f);
                vtx[3].uv = XMVectorSet(+1.0f, +0.0f, +0.0f, +0.0f);
            }
        }

        WORD idx[6];
        {
            idx[0] = 0;
            idx[1] = 2;
            idx[2] = 3;

            idx[3] = 2;
            idx[4] = 0;
            idx[5] = 1;
        }

        XMVECTOR rot[6];
        {
            rot[0] = XMQuaternionIdentity();
            rot[1] = XMQuaternionRotationAxis(XMVectorSet(+0.0f, +1.0, +0.0f, +0.0), XMConvertToRadians(+90.0f));
            rot[2] = XMQuaternionRotationAxis(XMVectorSet(+0.0f, +1.0, +0.0f, +0.0), XMConvertToRadians(+180.0f));
            rot[3] = XMQuaternionRotationAxis(XMVectorSet(+0.0f, +1.0, +0.0f, +0.0), XMConvertToRadians(+270.0f));
            rot[4] = XMQuaternionRotationAxis(XMVectorSet(+1.0f, +0.0, +0.0f, +0.0), XMConvertToRadians(+90.0f));
            rot[5] = XMQuaternionRotationAxis(XMVectorSet(+1.0f, +0.0, +0.0f, +0.0), XMConvertToRadians(-90.0f));
        }

		//{
		//	rot[0] = XMQuaternionIdentity();
		//	rot[1] = XMQuaternionIdentity();
		//	rot[2] = XMQuaternionIdentity();
		//	rot[3] = XMQuaternionIdentity();
		//	rot[4] = XMQuaternionIdentity();
		//	rot[5] = XMQuaternionIdentity();
		//}

        for (int i = 0; i < 6; i++)
        {
            XMMATRIX R = XMMatrixRotationQuaternion(rot[i]);
            for (int j = 0; j < 4; j++)
            {
                Vertex v0 = vtx[j];
                Vertex v1;
                v1.pos = XMVector4Transform(v0.pos, R);
                v1.nom = XMVector4Transform(v0.nom, R);
                v1.tan = XMVector4Transform(v0.tan, R);
                v1.uv = v0.uv;
                mvtx->push_back(v1);
            }
        }

        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                WORD i0 = idx[j];
                WORD i1;
                i1 = i * 4 + i0;
                midx->push_back(i1);
            }
        }

        {
            mesh->insCount = insCount;
            mesh->vCount = mesh->vtx->size();
            mesh->iCount = mesh->idx->size();
        }

		{
			mesh->CreateBuffer();
		}

		return mesh;
    }

	static Mesh* CreateTorusMesh(float radiusOut = 1.0f, float radiusIn = 2.0f, int slice4Cone = 6.0f, int slice4Circle = 6.0f, int insCount = 1)
	{
		Mesh* mesh = new Mesh();
		vector<Vertex>* mvtx = mesh->vtx;
		vector<WORD>* midx = mesh->idx;

		float r0 = radiusOut;
		float r1 = radiusIn;
		int s0 = 4 * slice4Cone;
		int s1 = 4 * slice4Circle;
		int c0 = s0 + 1;
		int c1 = s1 + 1;
		float dtheta = (float)XMConvertToRadians(360.0f / s0);
		float dphi = (float)XMConvertToRadians(360.0f / s1);
		float theta = 0.0f;
		float phi = 0.0f;

		for (int i = 0; i < c0; i = i + 1)
		{
			theta = i * dtheta;
			for (int j = 0; j < c1; j = j + 1)
			{
				phi = j * dphi;

				float sinT;
				float cosT;
				float sinP;
				float cosP;

				XMScalarSinCos(&sinT, &cosT, theta);
				XMScalarSinCos(&sinP, &cosP, phi);

				XMVECTOR r;
				XMVECTOR vTheta;
				XMVECTOR vPhi;

				{
					r = (
						XMVectorSet(+r0, +r0, +r0, 0.0f) *
						XMVectorSet(+sinT, +cosT, +sinT, 0.0f) +
						XMVectorSet(+r1, +0.0f, +r1, 0.0f)) *
						XMVectorSet(+sinP, +1.0f, +cosP, 1.0f);

					vTheta =
						XMVectorSet(+r0, +r0, +r0, 1.0f) *
						XMVectorSet(+cosT, -sinT, +cosT, 1.0f) *
						XMVectorSet(+sinP, +1.0f, +cosP, 1.0f);

					vPhi = (
						XMVectorSet(+r0, +0.0f, +r0, 1.0f) *
						XMVectorSet(+sinT, +0.0f, +sinT, 1.0f) +
						XMVectorSet(+r1, +0.0f, +r1, 0.0f)) *
						XMVectorSet(+cosP, +0.0f, -sinP, 1.0f);
				}


				XMVECTOR pos = r;
				XMVECTOR normal = XMVector3Normalize(XMVector3Cross(vTheta, vPhi));
				XMVECTOR tangent = XMVector3Normalize(vPhi);
				//XMVECTOR uv = XMVectorSet((float)i / (float)s0, (float)j / (float)s1, 0.0f, 0.0f);            
				XMVECTOR uv = XMVectorSet((float)j / (float)s1, (float)i / (float)s0, 0.0f, 0.0f);

				Vertex vtx;
				vtx.pos = pos;
				vtx.nom = normal;
				vtx.tan = tangent;
				vtx.uv = uv;

				mvtx->push_back(vtx);
			}
		}

		for (int i = 0; i < s0; i = i + 1)
		{
			for (int j = 0; j < s1; j = j + 1)
			{
				//int i0 = i * s1;
				//int i1 = (i + 1) * s1;
				//int j0 = j;
				//int j1 = (j + 1) % s1;

				int i0 = i * c1;
				int i1 = (i + 1) * c1;
				int j0 = j;
				int j1 = (j + 1);

				int v00 = i0 + j0;
				int v10 = i1 + j0;
				int v01 = i0 + j1;
				int v11 = i1 + j1;

				midx->push_back(v00);
				midx->push_back(v11);
				midx->push_back(v01);

				midx->push_back(v11);
				midx->push_back(v00);
				midx->push_back(v10);
			}
		}

		{
			mesh->insCount = insCount;
			mesh->vCount = mesh->vtx->size();
			mesh->iCount = mesh->idx->size();
		}

		{
			mesh->CreateBuffer();
		}

		return mesh;
	}

	void CreateBuffer()
	{
		auto context = Gpu::context;

		//idxBuffer = new ROBuffer<WORD>(iCount, D3D11_BIND_INDEX_BUFFER);
		idxBuffer = new ROBuffer<WORD>(iCount, D3D11_BIND_INDEX_BUFFER);
		//vtxBuffer = new ROBuffer<Vertex>(vCount, D3D11_BIND_VERTEX_BUFFER);
		vtxBuffer = new ROBuffer<Vertex>(vCount);
		
		
		{
			auto data = (WORD*)idxBuffer->data;
		
			for (int i = 0; i < iCount; i++)
			{
				data[i] = (*idx)[i];
			}
		
			idxBuffer->WriteToBuffer(context);
			idxBuffer->CopyToBuffer(context);
		}
		
		{
			auto data = (Vertex*)vtxBuffer->data;
		
			for (int i = 0; i < vCount; i++)
			{
				data[i] = (*vtx)[i];
			}
		
			vtxBuffer->WriteToBuffer(context);
			vtxBuffer->CopyToBuffer(context);
		}
	}
};

inline XMMATRIX RenderUtil::GetLtoW(XMVECTOR pos, XMVECTOR rot, XMVECTOR sca)
{
	XMMATRIX W = XMMatrixIdentity();

	XMMATRIX R = XMMatrixRotationQuaternion(rot);
	W.r[0] = sca.m128_f32[0] * R.r[0];
	W.r[1] = sca.m128_f32[1] * R.r[1];
	W.r[2] = sca.m128_f32[2] * R.r[2];
	W.r[3] = XMVectorSet(pos.m128_f32[0], pos.m128_f32[1], pos.m128_f32[2], 1.0f);

	return W;
}

XMMATRIX RenderUtil::GetWtoV(XMVECTOR pos, XMVECTOR rot)
{
	XMMATRIX V = XMMatrixIdentity();

	//XMVECTOR* pos = &trCam->posL;
	//XMVECTOR* rot = &trCam->rotL;

	XMMATRIX R = XMMatrixRotationQuaternion(rot);
	XMMATRIX Rt = XMMatrixTranspose(R);

	pos = -pos;
	V = Rt;
	V.r[3] = XMVectorSet(
		XMVector3Dot(R.r[0], pos).m128_f32[0],
		XMVector3Dot(R.r[1], pos).m128_f32[0],
		XMVector3Dot(R.r[2], pos).m128_f32[0],
		1.0f);

	return V;
}

XMMATRIX RenderUtil::GetVtoC(XMVECTOR fi, int type)
{
	XMMATRIX C = XMMatrixIdentity();

	if (type == 0)
	{
		float _fov = fi.m128_f32[0];
		float _aspect = fi.m128_f32[1];
		float _near = fi.m128_f32[2];
		float _far = fi.m128_f32[3];

		{
			float angle = XMConvertToRadians(_fov * 0.5f);

			float _fn = +_far / (_far - _near);
			float cotFov = 1.0f / tan(angle);
			//float cotFov = 1.0f / tan(_fov * 0.5f);
			C.r[0].m128_f32[0] = (1.0f / _aspect) * cotFov;
			C.r[1].m128_f32[1] = cotFov;

			C.r[2].m128_f32[2] = _fn;
			C.r[3].m128_f32[2] = -_near * _fn;
			C.r[2].m128_f32[3] = 1.0f;
			C.r[3].m128_f32[3] = 0.0f;
		}

		//{
		//	float angle = XMConvertToRadians(_fov);
		//
		//	C = XMMatrixPerspectiveFovLH(angle, _aspect, _near, _far);
		//}		
	}
	else
	{
		float _hv = fi.m128_f32[0];
		float _aspect = fi.m128_f32[1];
		float _near = fi.m128_f32[2];
		float _far = fi.m128_f32[3];

		{
			XMVECTOR s = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f) / XMVectorSet(_aspect * _hv, _hv, _far - _near, 1.0f);
			XMVECTOR t = XMVectorSet(0.0f, 0.0f, -_near, 1.0f);

			C.r[0].m128_f32[0] = s.m128_f32[0];
			C.r[1].m128_f32[1] = s.m128_f32[1];
			C.r[2].m128_f32[2] = s.m128_f32[2];
			C.r[3] = s * t;
		}

		//{
		//	float _v = 2.0f * _hv;
		//
		//	C = XMMatrixOrthographicLH(_aspect * _v, _v, _near, _far);
		//}

	}

	return C;
}

XMMATRIX RenderUtil::GetNtoS(XMVECTOR pos, XMVECTOR sca)
{
	XMMATRIX S = XMMatrixIdentity();

	float x = pos.m128_f32[0];
	float y = pos.m128_f32[1];
	float z = pos.m128_f32[2];

	float w_2 = sca.m128_f32[0] * 0.5f;
	float h_2 = sca.m128_f32[1] * 0.5f;
	//float d_2 = sca.m128_f32[2] * 0.5f;
	float d = sca.m128_f32[2];

	{
		//XMVECTOR s = XMVectorSet(w_2, h_2, d_2, 1.0f);
		//XMVECTOR t = XMVectorSet(x, y, z, 0.0f) + s;
		XMVECTOR s = XMVectorSet(w_2, h_2, d, 1.0f);
		XMVECTOR t = XMVectorSet(x, y, z, 0.0f) + XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f) * s;


		s = XMVectorSet(+1.0f, -1.0f, +1.0f, +0.0f) * s;

		S.r[3] = t;
		S.r[0].m128_f32[0] = s.m128_f32[0];
		S.r[1].m128_f32[1] = s.m128_f32[1];
		S.r[2].m128_f32[2] = s.m128_f32[2];

		//S.r[3].m128_f32[3] = 1.0f;
	}

	return S;
}

XMMATRIX RenderUtil::GetStoN(XMVECTOR pos, XMVECTOR sca)
{
	XMMATRIX N = XMMatrixIdentity();

	float x = pos.m128_f32[0];
	float y = pos.m128_f32[1];
	float z = pos.m128_f32[2];

	float w_2 = sca.m128_f32[0] * 0.5f;
	float h_2 = sca.m128_f32[1] * 0.5f;
	//float d_2 = sca.m128_f32[2] * 0.5f;
	float d = sca.m128_f32[2];

	{
		//XMVECTOR s = XMVectorSet(w_2, h_2, d_2, 1.0f);
		//XMVECTOR t = XMVectorSet(x, y, z, 0.0f) + s;
		XMVECTOR s = XMVectorSet(w_2, h_2, d, 1.0f);
		XMVECTOR t = XMVectorSet(x, y, z, 0.0f) + XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f) * s;

		t = -t;
		s = XMVectorSet(+1.0f, -1.0f, +1.0f, -1.0f) / s;

		N.r[0].m128_f32[0] = s.m128_f32[0];
		N.r[1].m128_f32[1] = s.m128_f32[1];
		N.r[2].m128_f32[2] = s.m128_f32[2];
		N.r[3] = s * t;

		//N.r[3].m128_f32[3] = 1.0f;
	}

	return N;
}



XMMATRIX RenderUtil::GetCtoV(XMVECTOR fi, int type)
{
	XMMATRIX V = XMMatrixIdentity();

	if (type == 0)
	{
		float _fov = fi.m128_f32[0];
		float _aspect = fi.m128_f32[1];
		float _near = fi.m128_f32[2];
		float _far = fi.m128_f32[3];

		{
			float angle = XMConvertToRadians(_fov * 0.5f);

			float tanFov = tan(angle);
			V.r[0].m128_f32[0] = _aspect * tanFov;
			V.r[1].m128_f32[1] = tanFov;

			V.r[2].m128_f32[2] = 0.0f;
			V.r[3].m128_f32[2] = 1.0f;
			V.r[2].m128_f32[3] = -(_far - _near) / (_far * _near);
			V.r[3].m128_f32[3] = (_far) / (_far * _near);
		}

		//{
		//	float angle = XMConvertToRadians(_fov);
		//
		//	XMMATRIX C = XMMatrixPerspectiveFovLH(angle, _aspect, _near, _far);
		//	XMVECTOR det = XMMatrixDeterminant(C);
		//	V = XMMatrixInverse(&det, C);
		//}		
	}
	else
	{
		float _hv = fi.m128_f32[0];
		float _aspect = fi.m128_f32[1];
		float _near = fi.m128_f32[2];
		float _far = fi.m128_f32[3];

		{
			XMVECTOR t = XMVectorSet(0.0f, 0.0f, _near, 1.0f);
			XMVECTOR s = XMVectorSet(_hv, _hv, (_far - _near), 1.0f) * XMVectorSet(_aspect, 1.0f, 1.0f, 1.0f);

			V.r[3] = t;
			V.r[0].m128_f32[0] = s.m128_f32[0];
			V.r[1].m128_f32[1] = s.m128_f32[1];
			V.r[2].m128_f32[2] = s.m128_f32[2];
		}

		//{
		//	float _v = 2.0f * _hv;
		//
		//	XMMATRIX C = XMMatrixOrthographicLH(_aspect * _v, _v, _near, _far);
		//	XMVECTOR det = XMMatrixDeterminant(C);
		//	V = XMMatrixInverse(&det, C);
		//}
	}


	return V;
}

XMMATRIX RenderUtil::GetVtoW(XMVECTOR pos, XMVECTOR rot)
{
	XMMATRIX W = XMMatrixIdentity();

	XMMATRIX R = XMMatrixRotationQuaternion(rot);

	W = R;
	W.r[3] = pos;

	W.r[3].m128_f32[3] = 1.0f;

	return W;
}


void RenderUtil::GetWtoV(XMVECTOR* pos, XMVECTOR* rot, XMMATRIX* V)
{
	*V = XMMatrixIdentity();

	XMMATRIX R = XMMatrixRotationQuaternion(*rot);
	XMMATRIX Rt = XMMatrixTranspose(R);

	XMVECTOR npos = -*pos;
	*V = Rt;
	V->r[3] = XMVectorSet(
		XMVector3Dot(R.r[0], npos).m128_f32[0],
		XMVector3Dot(R.r[1], npos).m128_f32[0],
		XMVector3Dot(R.r[2], npos).m128_f32[0],
		1.0f);
}

void RenderUtil::GetVtoC(XMVECTOR* fi, int type, XMMATRIX* C)
{
	*C = XMMatrixIdentity();

	if (type == 0)
	{
		float _fov = fi->m128_f32[0];
		float _aspect = fi->m128_f32[1];
		float _near = fi->m128_f32[2];
		float _far = fi->m128_f32[3];

		{
			float angle = XMConvertToRadians(_fov * 0.5f);

			float _fn = +_far / (_far - _near);
			float cotFov = 1.0f / tan(angle);
			//float cotFov = 1.0f / tan(_fov * 0.5f);
			C->r[0].m128_f32[0] = (1.0f / _aspect) * cotFov;
			C->r[1].m128_f32[1] = cotFov;

			C->r[2].m128_f32[2] = _fn;
			C->r[3].m128_f32[2] = -_near * _fn;
			C->r[2].m128_f32[3] = 1.0f;
			C->r[3].m128_f32[3] = 0.0f;
		}
	}
	else
	{
		float _hv = fi->m128_f32[0];
		float _aspect = fi->m128_f32[1];
		float _near = fi->m128_f32[2];
		float _far = fi->m128_f32[3];

		{
			XMVECTOR s = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f) / XMVectorSet(_aspect * _hv, _hv, _far - _near, 1.0f);
			XMVECTOR t = XMVectorSet(0.0f, 0.0f, -_near, 1.0f);

			C->r[0].m128_f32[0] = s.m128_f32[0];
			C->r[1].m128_f32[1] = s.m128_f32[1];
			C->r[2].m128_f32[2] = s.m128_f32[2];
			C->r[3] = s * t;
		}
	}
}

void RenderUtil::GetNtoS(XMVECTOR* pos, XMVECTOR* sca, XMMATRIX* S)
{
	*S = XMMatrixIdentity();

	float x = pos->m128_f32[0];
	float y = pos->m128_f32[1];
	float z = pos->m128_f32[2];

	float w_2 = sca->m128_f32[0] * 0.5f;
	float h_2 = sca->m128_f32[1] * 0.5f;
	float d = sca->m128_f32[2];

	{
		XMVECTOR s = XMVectorSet(w_2, h_2, d, 1.0f);
		XMVECTOR t = XMVectorSet(x, y, z, 0.0f) + XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f) * s;


		s = XMVectorSet(+1.0f, -1.0f, +1.0f, +0.0f) * s;

		S->r[3] = t;
		S->r[0].m128_f32[0] = s.m128_f32[0];
		S->r[1].m128_f32[1] = s.m128_f32[1];
		S->r[2].m128_f32[2] = s.m128_f32[2];

		//S->r[3].m128_f32[3] = 1.0f;
	}
}

void RenderUtil::GetStoN(XMVECTOR* pos, XMVECTOR* sca, XMMATRIX* N)
{
	*N = XMMatrixIdentity();

	float x = pos->m128_f32[0];
	float y = pos->m128_f32[1];
	float z = pos->m128_f32[2];

	float w_2 = sca->m128_f32[0] * 0.5f;
	float h_2 = sca->m128_f32[1] * 0.5f;
	float d = sca->m128_f32[2];

	{
		XMVECTOR s = XMVectorSet(w_2, h_2, d, 1.0f);
		XMVECTOR t = XMVectorSet(x, y, z, 0.0f) + XMVectorSet(1.0f, 1.0f, 0.0f, 1.0f) * s;

		t = -t;
		s = XMVectorSet(+1.0f, -1.0f, +1.0f, -1.0f) / s;

		N->r[0].m128_f32[0] = s.m128_f32[0];
		N->r[1].m128_f32[1] = s.m128_f32[1];
		N->r[2].m128_f32[2] = s.m128_f32[2];
		N->r[3] = s * t;
	}
}

void RenderUtil::GetCtoV(XMVECTOR* fi, int type, XMMATRIX* V)
{
	*V = XMMatrixIdentity();

	if (type == 0)
	{
		float _fov = fi->m128_f32[0];
		float _aspect = fi->m128_f32[1];
		float _near = fi->m128_f32[2];
		float _far = fi->m128_f32[3];

		{
			float angle = XMConvertToRadians(_fov * 0.5f);

			float tanFov = tan(angle);
			V->r[0].m128_f32[0] = _aspect * tanFov;
			V->r[1].m128_f32[1] = tanFov;

			V->r[2].m128_f32[2] = 0.0f;
			V->r[3].m128_f32[2] = 1.0f;
			V->r[2].m128_f32[3] = -(_far - _near) / (_far * _near);
			V->r[3].m128_f32[3] = (_far) / (_far * _near);
		}
	}
	else
	{
		float _hv = fi->m128_f32[0];
		float _aspect = fi->m128_f32[1];
		float _near = fi->m128_f32[2];
		float _far = fi->m128_f32[3];

		{
			XMVECTOR t = XMVectorSet(0.0f, 0.0f, _near, 1.0f);
			XMVECTOR s = XMVectorSet(_hv, _hv, (_far - _near), 1.0f) * XMVectorSet(_aspect, 1.0f, 1.0f, 1.0f);

			V->r[3] = t;
			V->r[0].m128_f32[0] = s.m128_f32[0];
			V->r[1].m128_f32[1] = s.m128_f32[1];
			V->r[2].m128_f32[2] = s.m128_f32[2];
		}
	}
}

void RenderUtil::GetVtoW(XMVECTOR* pos, XMVECTOR* rot, XMMATRIX* W)
{
	*W = XMMatrixIdentity();

	XMMATRIX R = XMMatrixRotationQuaternion(*rot);

	*W = R;
	W->r[3] = *pos;

	W->r[3].m128_f32[3] = 1.0f;

}

inline XMMATRIX RenderUtil::GetNtoT()
{
	XMMATRIX T = XMMatrixIdentity();

	T.r[0] = XMVectorSet(+0.5f, +0.0f, +0.0f, +0.0f);
	T.r[1] = XMVectorSet(+0.0f, -0.5f, +0.0f, +0.0f);
	T.r[2] = XMVectorSet(+0.0f, +0.0f, +1.0f, +0.0f);
	T.r[3] = XMVectorSet(+0.5f, +0.5f, +0.0f, +1.0f);

	return T;
}

inline void RenderUtil::GetNtoT(XMMATRIX* T)
{
	*T = XMMatrixIdentity();

	T->r[0] = XMVectorSet(+0.5f, +0.0f, +0.0f, +0.0f);
	T->r[1] = XMVectorSet(+0.0f, -0.5f, +0.0f, +0.0f);
	T->r[2] = XMVectorSet(+0.0f, +0.0f, +1.0f, +0.0f);
	T->r[3] = XMVectorSet(+0.5f, +0.5f, +0.0f, +1.0f);
}


void RenderUtil::GetPosition_WtoS(XMVECTOR* posIn, XMVECTOR posW, XMVECTOR rotW, XMVECTOR fi, bool type, XMVECTOR posS, XMVECTOR scaS, XMVECTOR* posOut, bool zNormal)
{
	posIn->m128_f32[3] = 1.0f;

	XMMATRIX VC;
	VC = GetWtoV(posW, rotW) * GetVtoC(fi, type);
	XMMATRIX S;
	S = GetNtoS(posS, scaS);

	XMVECTOR v;
	v = XMVector4Transform(*posIn, VC);
	v = (1.0f / v.m128_f32[3]) * v;

	v = XMVector4Transform(v, S);

	if (!zNormal)
	{
		float d = XMVector3Dot(*posIn - posW, XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotW)).m128_f32[0];
		v.m128_f32[2] = d;
	}

	*posOut = v;
}

void RenderUtil::GetRay_StoW(XMVECTOR* posIn, XMVECTOR posS, XMVECTOR scaS, XMVECTOR fi, bool type, XMVECTOR posW, XMVECTOR rotW, XMVECTOR* rayOrigin, XMVECTOR* rayDirection)
{
	posIn->m128_f32[2] = 0.0f;
	posIn->m128_f32[3] = 1.0f;

	XMMATRIX NV;
	NV = GetStoN(posS, scaS) * GetCtoV(fi, type);
	XMMATRIX W;
	W = GetVtoW(posW, rotW);

	XMVECTOR v0;
	XMVECTOR v1;
	v1 = XMVector4Transform(*posIn, NV);
	v1 = (1.0f / v1.m128_f32[3]) * v1;

	v1 = XMVector4Transform(v1, W);
	v0 = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), W);

	*rayOrigin = v1;
	if (type == 0)
	{
		*rayDirection = XMVector3Normalize(v1 - v0);
	}
	else
	{
		*rayDirection = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotW);
	}

}

void RenderUtil::GetPosition_WtoS(XMVECTOR* posIn, XMVECTOR* posW, XMVECTOR* rotW, XMVECTOR* fi, bool type, XMVECTOR* posS, XMVECTOR* scaS, XMVECTOR* posOut, bool zNormal)
{
	posIn->m128_f32[3] = 1.0f;

	XMMATRIX V;
	XMMATRIX C;
	XMMATRIX S;

	GetWtoV(posW, rotW, &V);
	GetVtoC(fi, type, &C);
	GetNtoS(posS, scaS, &S);

	XMVECTOR v;
	v = XMVector4Transform(XMVector4Transform(*posIn, V), C);
	v = (1.0f / v.m128_f32[3]) * v;

	v = XMVector4Transform(v, S);

	if (!zNormal)
	{
		float d = XMVector3Dot(*posIn - *posW, XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), *rotW)).m128_f32[0];
		v.m128_f32[2] = d;
	}

	*posOut = v;
}

void RenderUtil::GetRay_StoW(XMVECTOR* posIn, XMVECTOR* posS, XMVECTOR* scaS, XMVECTOR* fi, bool type, XMVECTOR* posW, XMVECTOR* rotW, XMVECTOR* rayOrigin, XMVECTOR* rayDirection)
{
	posIn->m128_f32[2] = 0.0f;
	posIn->m128_f32[3] = 1.0f;

	XMMATRIX N;
	XMMATRIX V;
	XMMATRIX W;

	GetStoN(posS, scaS, &N);
	GetCtoV(fi, type, &V);
	GetVtoW(posW, rotW, &W);

	XMVECTOR v0;
	XMVECTOR v1;
	v1 = XMVector4Transform(XMVector4Transform(*posIn, N), V);
	v1 = (1.0f / v1.m128_f32[3]) * v1;

	v1 = XMVector4Transform(v1, W);
	v0 = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), W);

	*rayOrigin = v1;
	if (type == 0)
	{
		*rayDirection = XMVector3Normalize(v1 - v0);
	}
	else
	{
		*rayDirection = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), *rotW);
	}

}


//
XMVECTOR RenderUtil::Vector3Transform(XMVECTOR vec, XMMATRIX mat)
{
	return XMVector3Transform(vec, mat);
}

void RenderUtil::Vector3Transform(XMVECTOR* vec, XMMATRIX* mat, XMVECTOR* vecOut)
{
	*vecOut = XMVector3Transform(*vec, *mat);
}

#endif