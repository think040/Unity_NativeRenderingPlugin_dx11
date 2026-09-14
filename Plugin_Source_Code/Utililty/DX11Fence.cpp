#include "../Header.h"
#include "DX11Fence.h"
#include "../Gpu/Gpu.h"
#include "Utility.h"

DX11Fence::DX11Fence()
{
	auto device = Gpu::device;
	auto context = Gpu::context;

	// 1. DX12의 Fence 역할을 할 DX11의 Event Query 생성
	D3D11_QUERY_DESC desc = {};
	desc.Query = D3D11_QUERY_EVENT; // GPU의 실행 타임라인을 추적하는 이벤트 타입
	desc.MiscFlags = 0;

	device->CreateQuery(&desc, &eventQuery);
}

// DX12의 CommandQueue->Signal() 역할
void DX11Fence::Signal()
{
	auto context = Gpu::context;

	// GPU 명령 스트림에 "여기까지 실행되면 알려줘"라는 마커(End)를 삽입
	context->End(eventQuery);

	// 명령이 커맨드 버퍼에만 머물지 않고 GPU로 즉시 전송되도록 Flush 호출 (매우 중요)
	context->Flush();
	bSignaled = true;
	bCompleted = false;
}

// DX12의 Fence->GetCompletedValue() 또는 CPU 측 Check 역할 (넌블로킹)
bool DX11Fence::IsCompleted()
{
	auto context = Gpu::context;

	if (!bSignaled)
	{
		return false;
	}
	// GetData의 4번째 인자에 D3D11_ASYNC_GETDATA_DONOTFLUSH를 주면 
	// CPU를 멈추지 않고(Non-blocking) 현재 GPU가 통과했는지 여부만 TRUE/FALSE로 반환합니다.	
	HRESULT hr = context->GetData(eventQuery, nullptr, 0, D3D11_ASYNC_GETDATA_DONOTFLUSH);
	bSignaled = false;

	if (hr == S_OK)
	{
		NativeLog("m_bCompleted = true");
		bCompleted = true;
	}
	else
	{
		NativeLog("m_bCompleted = false");
		bCompleted = false;
	}

	return bCompleted; // S_OK면 GPU가 이 지점을 통과한 것임 (S_FALSE면 아직 진행 중)
}

// DX12의 Fence->SetEventOnCompletion() + WaitForSingleObject() 역할 (블로킹 대기)
void DX11Fence::WaitOnCPU()
{
	auto context = Gpu::context;

	if (!bSignaled) return;

	// GPU가 Query 마커를 통과할 때까지 CPU 스레드를 무한 루프로 대기시킴 (동기식)
	while (context->GetData(eventQuery, nullptr, 0, 0) == S_FALSE)
	{
		// CPU 점유율 폭발을 막기 위해 잠시 대기
		Sleep(0);

		NativeLog("Sleep(0)");
	}

	NativeLog("AWake()");
}