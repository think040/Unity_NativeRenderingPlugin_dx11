using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.UI;
using UnityEngine.AI;
//using Unity.AI.Navigation;

unsafe public class StoPassTest : ActorObject
{

#region NativeRendering

    private const string DLL_NAME = "RenderingPlugin_D3D11";

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    static extern void* Create_StoPassObject();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    private static extern void* GetFuction_StoPassObject();

    void* native_instance = null;
    IntPtr native_instance_ptr;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcInit(void* instance);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcUpdate(void* instance);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcRender(void* instance);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcDestroy(void* instance);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate int funcGetId(void* instance);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcIssueUpdate(int id);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcIssueBeginContext(int id);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcIssueBeginCamera(int id);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcIssueRender(int id);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcIssueEndCamera(int id);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcIssueEndContext(int id);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcCompute_Tex3D (void* instance);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate bool funcCheckGPUCompleted (IntPtr instance);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSetTextureFromUnity(void* instance, void* nativeTexturePtr);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSetLightDataFromUnity(void* instance, void** ppData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSetObjDataFromUnity(void* instance, void** ppData);
  

    [StructLayout(LayoutKind.Sequential)]
    struct IRenderActor
    {
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcInit Init;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcUpdate Update;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcRender Render;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcDestroy Destroy;


        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcGetId GetId;


        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcIssueUpdate IssueUpdate;



        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcIssueBeginContext IssueBeginContext;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcIssueBeginCamera IssueBeginCamera;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcIssueRender IssueRender;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcIssueEndCamera IssueEndCamera;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcIssueEndContext IssueEndContext;


        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcCompute_Tex3D Compute_Tex3D;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcCheckGPUCompleted CheckGPUCompleted;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSetTextureFromUnity SetTextureFromUnity;


        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSetLightDataFromUnity SetLightDataFromUnity;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSetObjDataFromUnity SetObjDataFromUnity;

    };

    struct PtrRenderActor
    {
        public IntPtr Init;
        public IntPtr Update;
        public IntPtr Render;
        public IntPtr Destroy;

        public IntPtr GetId;

        public IntPtr IssueUpdate;

        public IntPtr IssueBeginContext;
        public IntPtr IssueBeginCamera;
        public IntPtr IssueRender;        
        public IntPtr IssueEndCamera;
        public IntPtr IssueEndContext;

        public IntPtr Compute_Tex3D;
        public IntPtr CheckGPUCompleted;
        public IntPtr SetTextureFromUnity;        
        public IntPtr SetLightDataFromUnity;        
        public IntPtr SetObjDataFromUnity;
    };

    static IRenderActor native_func;
    //static IntPtr? ptr_native_func = null; 
    static void* ptr_native_func = null;
    int id_native = -1;

#endregion

    void Awake()
    {
        
    }
 
    void Start()
    {
        //var navData = NavMesh.CalculateTriangulation();
        
        

    }

   
    public override void Init()
    {      
        if(ptr_native_func == null)     
        {
            //ptr_native_func = (IntPtr)GetFuction_StoPassObject();
            ptr_native_func = GetFuction_StoPassObject();

            native_func = Marshal.PtrToStructure<IRenderActor>((IntPtr)ptr_native_func);

            {
                PtrRenderActor* ptr = (PtrRenderActor*)ptr_native_func;           
            }
        }
    
    }

    bool bRender_ready = false;

    struct ObjectData
    {
        public float4x4 W;
        public float4 data;
    };

    struct CameraData
    {
        public float4x4 V;
        public float4x4 C;
    };

    struct LightData
    {
        public float4 pos;
        public float4 dir;
    };

    CameraData* ptrCameraData;
    LightData* ptrLightData;
    ObjectData* ptrObjData;

    public Texture2D tex2d;
    public Light light;
    public Transform[] tr_mesh;

    public override void Begin()
    {
        native_instance = Create_StoPassObject();
        native_instance_ptr = (IntPtr)native_instance;


        {
            native_func.Init(native_instance);
            id_native = native_func.GetId(native_instance);
        }

        {
            IntPtr ptr = tex2d.GetNativeTexturePtr();
            native_func.SetTextureFromUnity(native_instance, ptr.ToPointer());            
        }

        {         
            fixed (LightData** ptr = &ptrLightData)
            {
                native_func.SetLightDataFromUnity(native_instance, (void**)ptr);
            }

            fixed (ObjectData** ptr = &ptrObjData)
            {
                native_func.SetObjDataFromUnity(native_instance, (void**)ptr);
            }
        }


        bRender_ready = true;
    }

    public override void Enable()
    {
        //RenderManager.BeginContextRender    += BeginContextRender;
        //RenderManager.BeginCameraRender     += BeginCameraRender;
        //RenderManager.OnRenderCam           += Render;
        //RenderManager.EndCameraRender       += EndCameraRender;
        //RenderManager.EndContextRender      += EndContextRender;        
    }

    public override void Disable()
    {
        //RenderManager.EndContextRender      -= EndContextRender;
        //RenderManager.EndCameraRender       -= EndCameraRender;        
        //RenderManager.OnRenderCam           -= Render;                
        //RenderManager.BeginCameraRender     -= BeginCameraRender;
        //RenderManager.BeginContextRender    -= BeginContextRender;
    }

    public override void Finish()
    {
        
        {
            native_func.Destroy(native_instance);
        }
    }

    int tessFactor = 0;

    // Update is called once per frame
    void Update()
    {
        if(!bRender_ready) { return; }

        //{
        //    native_func.Update(native_instance);
        //}

        //{
        //    native_func.Render(native_instance);
        //}

        //{
        //    int id = native_func.GetId(native_instance);
        //    native_func.IssueRender(id);
        //}

        //{
        //    CommandBuffer cmd = CommandBufferPool.Get();
        //    int id = native_func.GetId(native_instance);
        //    IntPtr ptr = Marshal.GetFunctionPointerForDelegate<funcIssueRender>(native_func.IssueRender);
        //    cmd.IssuePluginEvent(ptr, id);
        //
        //
        //    Graphics.ExecuteCommandBuffer(cmd);
        //    cmd.Release();
        //}

        {
            if (Input.GetKeyDown(KeyCode.T))
            {
                
                tessFactor = (++tessFactor % 4);

            }
        }


        {
            {
                LightData* data = ptrLightData;
                Transform tr = light.transform;
                data->pos = new float4(tr.position, 1.0f);
                data->dir = new float4(math.rotate(tr.rotation, new float3(0.0f, 0.0f, -1.0f)), 0.0f);
            }
            
            for (int i = 0; i < 2; i++)
            {
                ObjectData* data = ptrObjData;
                Transform tr = tr_mesh[i];
                (data + i)->W = RenderUtil.GetWfromL(tr);
                //(data + i)->data = new float4(3 + 1, 0.0f, 0.0f, 0.0f);
                (data + i)->data = new float4(tessFactor + 1, 0.0f, 0.0f, 0.0f);
            }
        }

        //{
        //    native_func.Update(native_instance);
        //}

        {
            CommandBuffer cmd = CommandBufferPool.Get();
            //int id = native_func.GetId(native_instance);
            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueUpdate;
            cmd.IssuePluginEvent(ptr, id_native);

            //GraphicsFence fence = cmd.CreateGraphicsFence(GraphicsFenceType.AsyncQueueSynchronisation, SynchronisationStageFlags.AllGPUOperations);

            Graphics.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);

            //AsyncGPUReadback.WaitAllRequests();

            //StartCoroutine(WaitForGPGPU(fence));

            int a = 10;
            int b = a + 10;
        
        }


        //bool wait = native_func.CheckGPUCompleted(native_instance_ptr);
        //StartCoroutine(WaitForGPUNative());

        //{
        //   native_func.Compute_Tex3D(native_instance);
        //}

    }

    IEnumerator WaitForGPGPU(GraphicsFence fence)
    {
        // GPU가 C++ Native에서 제출한 CS 연산을 끝내고 Fence 지점을 통과할 때까지 대기
        while (!fence.passed)
        {
            yield return null; // 다음 프레임까지 양보
        }


        Debug.Log($"fence.pased : " + fence.passed.ToString());

        // [동기화 완료] GPU 연산이 완벽히 끝났으므로 안심하고 C++ 데이터를 읽어옵니다.
        //ReadResultFromNative();
        transform.position = new float3(10.0f, 10.0f, 10.0f);

        int a = 10;
        int b = a + 10;
    }

    IEnumerator WaitForGPUNative()
    {


        // C++ DLL의 IsCompleted()가 true를 반환할 때까지 메인 스레드는 다음 프레임으로 양보
        //while (!native_func.CheckGPUCompleted(native_instance))

        bool wait = native_func.CheckGPUCompleted(native_instance_ptr);
        while (!wait)
        {
            yield return null;
        }

        // [동기화 완료] 완벽한 시점에 버퍼 맵핑 및 데이터 사용
        //ReadBufferData();

        int a = 10;
        int b = a + 10;
    }

    unsafe private void BeginContextRender(ScriptableRenderContext context, CommandBuffer cmd, List<Camera> cams)
    {
        {
            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueBeginContext;
            cmd.IssuePluginEvent(ptr, id_native);

            int a = 10;
            int b = 20;
            int c = a + b;            
        }
    }

    unsafe private void BeginCameraRender(ScriptableRenderContext context, CommandBuffer cmd, Camera cams)
    {
        {
            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueBeginCamera;
            cmd.IssuePluginEvent(ptr, id_native);

            int a = 10;
            int b = 20;
            int c = a + b;

        }
    }

    unsafe private void Render(ScriptableRenderContext context, CommandBuffer cmd, Camera cam)
    {     
        //{       
        //    int id = native_func.GetId(native_instance);
        //    IntPtr ptr = Marshal.GetFunctionPointerForDelegate<funcIssueRender>(native_func.IssueRender);
        //    cmd.IssuePluginEvent(ptr, id);                     
        //}

        {
            //int id = native_func.GetId(native_instance);
            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueRender;
            cmd.IssuePluginEvent(ptr, id_native);


            int a = 10;
            int b = 20;
            int c = a + b;
        }
    }

    unsafe private void EndCameraRender(ScriptableRenderContext context, CommandBuffer cmd, Camera cams)
    {
        {
            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueEndCamera;
            cmd.IssuePluginEvent(ptr, id_native);

            int a = 10;
            int b = 20;
            int c = a + b;

        }
    }

    unsafe private void EndContextRender(ScriptableRenderContext context, CommandBuffer cmd, List<Camera> cams)
    {
        {
            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueEndContext;
            cmd.IssuePluginEvent(ptr, id_native);

            int a = 10;
            int b = 20;
            int c = a + b;

        }
    }
}
