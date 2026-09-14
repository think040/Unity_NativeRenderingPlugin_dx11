#pragma once
#include "../Header.h"

class DX11Fence
{
private:
    //ID3D11Device*        m_pDevice;
    //ID3D11DeviceContext* m_pContext;
    ID3D11Query*            eventQuery;
    bool                    bSignaled = false;
public:
    bool                    bCompleted = false;

public:
    DX11Fence();

    // DX12의 CommandQueue->Signal() 역할
    void Signal();

    // DX12의 Fence->GetCompletedValue() 또는 CPU 측 Check 역할 (넌블로킹)
    bool IsCompleted();

    // DX12의 Fence->SetEventOnCompletion() + WaitForSingleObject() 역할 (블로킹 대기)
    void WaitOnCPU();

};