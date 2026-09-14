#pragma once

class RenderObject
{
public:
	virtual void Init() = 0;
	virtual void Begin() = 0;

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
	virtual void Destroy() = 0;

public:
	virtual void EventUpdate()		 = 0;
	virtual void EventBeginContext() = 0;
	virtual void EventBeginCamera()	 = 0;
	virtual void EventRender()		 = 0;
	virtual void EventEndCamera()	 = 0;
	virtual void EventEndContext()	 = 0;

public:
	//static vector<RenderObject*>* objects;

	const string vsm = "vs_5_0";
	const string hsm = "hs_5_0";
	const string dsm = "ds_5_0";
	const string gsm = "gs_5_0";
	const string psm = "ps_5_0";
	const string csm = "cs_5_0";
};