using Microsoft.Win32.SafeHandles;
using System;
using System.Collections;
using System.Drawing.Drawing2D;
//using System.Numerics;
using System.Runtime.InteropServices;
using System.Threading;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Experimental.GlobalIllumination;
using UnityEngine.Rendering;
using UnityEngine.U2D;

using Random = Unity.Mathematics.Random;

public unsafe class NavigationTest : ActorObject
{
    #region NativeRendering

    enum ID_HANDEL : int
    {
        TargetPos = 0,

        RectIn = 1,

        Count = 2
    };

    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern uint WaitForSingleObject(IntPtr hHandle, uint dwMilliseconds);

    // Win32 Constants
    private const uint INFINITE = 0xFFFFFFFF; // 무한대 대기 타임아웃 값
    private const uint WAIT_OBJECT_0 = 0x00000000;
    private const uint WAIT_FAILED = 0xFFFFFFFF;

    private const string DLL_NAME = "RenderingPlugin_D3D11";

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    static extern void* CreateRenderObject_NavTestObject();

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    private static extern void* GetRenderActor_NavTestObject();

    void* native_instance = null;
    IntPtr native_instance_ptr;

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcInit(void* instance);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcBegin(void* instance);

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
    public delegate void funcSetTextureFromUnity(void* instance, void* nativeTexturePtr, int idx);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSetTextureModeId (void* instance, int id);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSendDataPtrToUnity(void* instance, void** ppData0, void** ppData1, void** ppData2);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSendUnitCount(void* instance, int count_unit);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSendPtrTargetPos(void* instance, void** ppData0, void** ppData1, void** ppData2, void** ppData3, void** ppData4);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSendComputeDoneEvent(void* instance, void** pHandle);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSendMoveDoneEvent(void* instance, void* handle);


    
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSetRenderDebugPosEnable(void* instance, void** ppData);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcIssueUpdate_rectIn(int id);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]

    public delegate void funcSendPtrRectIn (void* instance, void** ppData0, void** ppData1, void** ppData2, void** ppData3);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcSendRectInDoneEvent(void* instance, void** pHandle);


    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void funcGetHandle (void* instance, void** pHandle, int id);

    


    [StructLayout(LayoutKind.Sequential)]
    struct IRenderActor
    {
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcInit Init;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcInit Begin;

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
        public funcSetTextureFromUnity SetTextureFromUnity;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSetTextureModeId SetTextureModeId;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSendDataPtrToUnity SendDataPtrToUnity;
     
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSendUnitCount SendUnitCount;

  
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSendPtrTargetPos SendPtrTargetPos;

      
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSendComputeDoneEvent SendComputeDoneEvent;

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSendMoveDoneEvent SendMoveDoneEvent;


        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSetRenderDebugPosEnable SetRenderDebugPosEnable;


        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcIssueUpdate_rectIn IssueUpdate_rectIn;


        

        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSendPtrRectIn SendPtrRectIn;


        //funcSendRectInDoneEvent
        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcSendRectInDoneEvent SendRectInDoneEvent;


        [MarshalAs(UnmanagedType.FunctionPtr)]
        public funcGetHandle GetHandle;  
    };

    struct PtrRenderActor
    {
        public IntPtr Init;
        public IntPtr Begin;
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

        public IntPtr SetTextureFromUnity;
        public IntPtr SetTextureModeId;

        public IntPtr SendDataPtrToUnity;

        public IntPtr SendUnitCount;


        public IntPtr SendPtrTargetPos;


        public IntPtr SendComputeDoneEvent;
        public IntPtr SendMoveDoneEvent;


        public IntPtr SetRenderDebugPosEnable;


        public IntPtr IssueUpdate_rectIn;

        public IntPtr SendPtrRectIn;

        public IntPtr SendRectInDoneEvent;


        public IntPtr GetHandle;
    };

    static IRenderActor native_func;
    //static IntPtr? ptr_native_func = null; 
    static void* ptr_native_func = null;
    int id_native = -1;

    #endregion


    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    Random random;

    public override void Init()
    {
        if (ptr_native_func == null)
        {
            //ptr_native_func = (IntPtr)GetFuction_StoPassObject();
            ptr_native_func = GetRenderActor_NavTestObject();

            native_func = Marshal.PtrToStructure<IRenderActor>((IntPtr)ptr_native_func);

            {
                PtrRenderActor* ptr = (PtrRenderActor*)ptr_native_func;
            }

            native_instance = CreateRenderObject_NavTestObject();
            native_instance_ptr = (IntPtr)native_instance;


            {
                native_func.Init(native_instance);
                id_native = native_func.GetId(native_instance);
            }       
        }

        {
            random = new Random();
            random.InitState();
        }
    }

    public static bool bRender_ready = false;


    public int count_unit = 1;
    public GameObject prefab_unit;
    GameObject[] units;
    UnitSimple[] unitActors;

    public override void Begin()
    {
        //native_instance = CreateRenderObject_NavTestObject();
        //native_instance_ptr = (IntPtr)native_instance;
        //
        //
        //{
        //    native_func.Init(native_instance);
        //    id_native = native_func.GetId(native_instance);
        //}

        {
            //TestTexture();
        }

        //{       
        //    fixed(LightData** ptr0 = &ptrLightData)             
        //    fixed(ObjectData** ptr1 = &ptrObjData)
        //    fixed(ObjectData** ptr2 = &ptrUnitObjData)
        //    {
        //       native_func.SendDataPtrToUnity(native_instance, (void**)ptr0, (void**)ptr1, (void**)ptr2);
        //    }           
        //}     

        {
            native_func.SendUnitCount(native_instance, count_unit);
        }

        {
            native_func.Begin(native_instance);
        }

        {
            fixed (LightData** ptr0 = &ptrLightData)
            fixed (ObjectData** ptr1 = &ptrObjData)
            fixed (ObjectData** ptr2 = &ptrUnitObjData)
            {
                native_func.SendDataPtrToUnity(native_instance, (void**)ptr0, (void**)ptr1, (void**)ptr2);
            }
        }

        {
            fixed (float4** ptr0 = &ptrTargetPos)
            fixed (float4x4** ptr1 = &ptrTrM)
            fixed (int** ptr2 = &ptrRandomUnit)
            fixed (TargetComputeInfo** ptr3 = &ptrTargetInfo)
            fixed (int** ptr4 = &ptrSelectData)
            {
                //native_func.SendPtrTargetPos(native_instance, (void**)ptr0, (void**)ptr1, (void**)ptr2, (void**)ptr3, (void**)ptr4);
                native_func.SendPtrTargetPos(native_instance, (void**)ptr0, (void**)ptr1, (void**)ptr2, (void**)ptr3, (void**)ptr4);
            }
        }

        {
            fixed(bool** ptr = &ptrRenderDebugPosEnable)
            {
                native_func.SetRenderDebugPosEnable(native_instance, (void**)ptr);
            }
        }


        {
            fixed(RectInInfo** ptr0 = &ptrRectInInfo)            
            fixed(float4x4** ptr1 = &ptrSCV_data)            
            fixed(float4** ptr2 = &ptrPos_data)            
            fixed(int** ptr3 = &ptrInRect_data)
            {
                native_func.SendPtrRectIn(native_instance, (void**)ptr0, (void**)ptr1, (void**)ptr2, (void**)ptr3);
            }
        }

        {
            units = new GameObject[count_unit];
            unitActors = new UnitSimple[count_unit];
        
            for(int i = 0; i < count_unit; i++)
            {
                units[i] = GameObject.Instantiate(prefab_unit);
                units[i].SetActive(true);
                unitActors[i] = units[i].GetComponent<UnitSimple>();
                unitActors[i].iid = i;             
            }
            
            for(int i = 0; i < count_unit; i++)
            {
                Transform tr = units[i].transform;
                float3 origin = new float3(0.0f, 0.0f, 0.0f);
                int cx = 16;
                float sx = 1.5f;
                float sz = 1.5f;
                
                tr.position = origin + new float3((i % cx) * sx, 0.5f, (i / cx) * sz);

                unitActors[i].targetPos = tr.position;
            }
        }


        //StartCoroutine(UpdateTarget());


        //{
        //
        //    void* handle = null;      
        //    native_func.SendComputeDoneEvent(native_instance, &handle);
        //    hComputeDone = (IntPtr)handle;
        //}
    
        //{
        //
        //    void* handle = null;
        //    native_func.SendRectInDoneEvent(native_instance, &handle);
        //    hRectInDone = (IntPtr)handle;
        //}


        {
            void* handle = null;
            native_func.GetHandle(native_instance, &handle, (int)ID_HANDEL.TargetPos);
            hComputeDone = (IntPtr)handle;
        }

        {
            void* handle = null;
            native_func.GetHandle(native_instance, &handle, (int)ID_HANDEL.RectIn);
            hRectInDone = (IntPtr)handle;
        }



        {
            {
                moveEvent = new AutoResetEvent(false);
                hMoveEvnet = moveEvent.SafeWaitHandle.DangerousGetHandle();

                native_func.SendMoveDoneEvent(native_instance, hMoveEvnet.ToPointer());
            }    
        }

        StartCoroutine(SelectAction());
        StartCoroutine(MoveAction());
        //routine_moveAction = MoveAction();

        bRender_ready = true;


        for (int i = 0; i < count_unit; i++)
        {
            Transform tr = units[i].transform;
            unitActors[i].targetPos = tr.position;
        }

    }

    IntPtr hComputeDone;

    IntPtr hRectInDone;

    IEnumerator routine_moveAction;

    public Texture2D tex2d;

    public Terrain terrain;
    public Texture tex;  
    public RenderTexture hMap;

   
    public int mode_texViewer = 0;

    void TestTexture()
    {
       
        {
            IntPtr ptr = tex2d.GetNativeTexturePtr();
            native_func.SetTextureFromUnity(native_instance, ptr.ToPointer(), 0);
        }
        
        {
            var tData = terrain.terrainData;
            tex = tData.terrainLayers[0].diffuseTexture;
            IntPtr ptr = tex.GetNativeTexturePtr();
            native_func.SetTextureFromUnity(native_instance, ptr.ToPointer(), 1);
        }
       
        {
            var tData = terrain.terrainData;
            tex = tData.terrainLayers[1].diffuseTexture;
            IntPtr ptr = tex.GetNativeTexturePtr();
            native_func.SetTextureFromUnity(native_instance, ptr.ToPointer(), 2);
        }

        ////HeightMap
        //{
        //    var tData = terrain.terrainData;
        //    hMap = tData.heightmapTexture;
        //    IntPtr ptr = hMap.GetNativeTexturePtr();
        //    native_func.SetTextureFromUnity(native_instance, ptr.ToPointer(), 3);
        //}

        ////Alphamap
        //{
        //    var tData = terrain.terrainData;
        //    tex = tData.alphamapTextures[0];
        //    IntPtr ptr = tex.GetNativeTexturePtr();
        //    native_func.SetTextureFromUnity(native_instance, ptr.ToPointer(), 4);
        //}

        ////HoleMap
        //{
        //    var tData = terrain.terrainData;
        //    tex = tData.holesTexture;
        //    IntPtr ptr = tex.GetNativeTexturePtr();
        //    native_func.SetTextureFromUnity(native_instance, ptr.ToPointer(), 5);
        //}


    }

    public override void Disable()
    {
        
    }

    public override void Enable()
    {
       
    }

    public override void Finish()
    {
        {
            native_func.Destroy(native_instance);
        }
    }

    IEnumerator  UpdateTarget()
    {
        while(!bRender_ready) { yield return null; }

        while(true)
        {
            Targeting();

            //yield return new WaitForFixedUpdate();
            yield return new WaitForEndOfFrame();
        }

    }

    void Targeting()
    {
        //Texture_Viewer_Test
        {
            if (Input.GetKeyDown(KeyCode.H))
            {
                mode_texViewer = (++mode_texViewer) % 3;
            }

            {
                native_func.SetTextureModeId(native_instance, mode_texViewer);
            }
        }

        {
            {
                LightData* data = ptrLightData;
                Transform tr = light.transform;
                data->pos = new float4(tr.position, 1.0f);
                data->dir = new float4(math.rotate(tr.rotation, new float3(0.0f, 0.0f, -1.0f)), 0.0f);
            }


            {
                ObjectData* data = ptrObjData;
                Transform tr = terrain.transform;
                data->W = RenderUtil.GetWfromL(tr);
                data->data = new float4(0.0f, 0.0f, 0.0f, 0.0f);
            }

            for (int i = 0; i < count_unit; i++)
            {
                ObjectData* data = ptrUnitObjData;
                Transform tr = units[i].transform;
                data[i].W = RenderUtil.GetWfromL(tr);
                data[i].data = new float4(0.0f, 0.0f, 0.0f, 0.0f);


                //(data + i)->W = RenderUtil.GetWfromL(tr);
                //(data + i)->data = new float4(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }

        {
            {
                float4* data = ptrTargetPos;
            }

            {
                float4x4* data = ptrTrM;
                for (int i = 0; i < count_unit; i++)
                {
                    var tr = units[i].transform;
                    data[i].c0 = new float4(tr.position, 0.0f);
                    data[i].c1 = ((quaternion)tr.rotation).value;
                    data[i].c2 = new float4(tr.localScale, 0.0f);
                    //data[i].c3 = float4.zero;
                    data[i].c3 = unitActors[i].radius;
                }
            }

            {
                int* data = ptrRandomUnit;

                for (int i = 0; i < count_unit; i++)
                {
                    data[i] = random.NextInt(0, 7);
                }
            }

            {
                TargetComputeInfo* data = ptrTargetInfo;
                var tr = terrain.transform;
                data->T = RenderUtil.GetVfromW(tr.position, tr.rotation);
            }
        }


        {
            CommandBuffer cmd = CommandBufferPool.Get();

            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueUpdate;
            cmd.IssuePluginEvent(ptr, id_native);

            Graphics.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }

    }

    // Update is called once per frame
    void Update()
    {
        if (!bRender_ready) { return; }

        //Texture_Viewer_Test
        {
            if (Input.GetKeyDown(KeyCode.H))
            {
                mode_texViewer = (++mode_texViewer) % 3;
            }

            {
                native_func.SetTextureModeId(native_instance, mode_texViewer);
            }
        }

        {
            {
                LightData* data = ptrLightData;
                Transform tr = light.transform;
                data->pos = new float4(tr.position, 1.0f);
                data->dir = new float4(math.rotate(tr.rotation, new float3(0.0f, 0.0f, -1.0f)), 0.0f);
            }


            {
                ObjectData* data = ptrObjData;
                Transform tr = terrain.transform;
                data->W = RenderUtil.GetWfromL(tr);
                data->data = new float4(0.0f, 0.0f, 0.0f, 0.0f);
            }

            for (int i = 0; i < count_unit; i++)
            {
                ObjectData* data = ptrUnitObjData;
                Transform tr = units[i].transform;
                data[i].W = RenderUtil.GetWfromL(tr);
                data[i].data = new float4(0.0f, 0.0f, 0.0f, 0.0f);


                //(data + i)->W = RenderUtil.GetWfromL(tr);
                //(data + i)->data = new float4(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }

        {
            {
                float4* data = ptrTargetPos;
            }

            {
                float4x4* data = ptrTrM;
                for (int i = 0; i < count_unit; i++)
                {
                    var tr = units[i].transform;
                    data[i].c0 = new float4(tr.position, 0.0f);
                    data[i].c1 = ((quaternion)tr.rotation).value;
                    data[i].c2 = new float4(tr.localScale, 0.0f);
                    //data[i].c3 = float4.zero;
                    data[i].c3 = unitActors[i].radius;
                }
            }

            {
                int* data = ptrRandomUnit;

                for (int i = 0; i < count_unit; i++)
                {
                    data[i] = random.NextInt(0, 7);
                }
            }

            {
                TargetComputeInfo* data = ptrTargetInfo;
                var tr = terrain.transform;
                data->T = RenderUtil.GetVfromW(tr.position, tr.rotation);
            }
        }


        {
            if (Input.GetKeyDown(KeyCode.F))
            {
                *ptrRenderDebugPosEnable = !(*ptrRenderDebugPosEnable);
            }

            {
                CommandBuffer cmd = CommandBufferPool.Get();

                IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueUpdate;
                cmd.IssuePluginEvent(ptr, id_native);

                Graphics.ExecuteCommandBuffer(cmd);
                CommandBufferPool.Release(cmd);
            }


            {
                uint waitResult = WaitForSingleObject(hComputeDone, INFINITE); // Max 1000ms

                if (waitResult == 0) // WAIT_OBJECT_0
                {
                    // 동기화 완료: C++에서 안전하게 Readback된 데이터 사용
                    Debug.Log("GPU Compute & Worker Thread Readback Completed!");
                }
                else
                {
                    Debug.LogWarning("Timeout waiting for GPU Compute completion.");
                }
            }


            //{
            //    IntPtr nativeEventHandle;
            //    void* pHandle = null;
            //    //IntPtr* pHandle = null;
            //    native_func.SendComputeDoneEvent(native_instance, &pHandle);
            //    nativeEventHandle = (IntPtr)pHandle;
            //
            //    SafeWaitHandle safeHandle = new SafeWaitHandle(nativeEventHandle, ownsHandle: false);
            //
            //    using (AutoResetEvent waitEvent = new AutoResetEvent(false) { SafeWaitHandle = safeHandle })
            //    {
            //        // kernel32.dll 직접 호출 대신 C# 메서드 사용 (타 플랫폼 및 IL2CPP에 훨씬 안전)
            //        bool isSignaled = waitEvent.WaitOne(1000);
            //    }
            //}


        }
    }

    struct ObjectData
    {
        public float4x4 W;
        public float4 data;
    };
 
    struct LightData
    {
        public float4 pos;
        public float4 dir;
    };

    public Light light;

    LightData* ptrLightData;
    ObjectData* ptrObjData;
    ObjectData* ptrUnitObjData;


    struct TargetComputeInfo
    {
        float4 t1_t0;
        public float4x4 T;
        float4 countInfo;
    };

    public static float4* ptrTargetPos;
    float4x4* ptrTrM;
    int* ptrRandomUnit;
    TargetComputeInfo* ptrTargetInfo;

    public static int* ptrSelectData;

    bool* ptrRenderDebugPosEnable;


    struct RectInInfo
    {
        public float4 rect;
        public float4 data;
    };


    RectInInfo* ptrRectInInfo;

    float4x4* ptrSCV_data;

    float4* ptrPos_data;

    int* ptrInRect_data;


    private AutoResetEvent moveEvent;   // 작업 요청용 (Set 후 자동으로 Reset됨)
    //private ManualResetEvent moveEvent; // 종료 요청용 (Set 후 수동으로 Reset 전까지 유지됨)

    IntPtr hMoveEvnet;

    public static KeyCode key_muti_select = KeyCode.E;
    public static KeyCode key_hold = KeyCode.Q;

    public void SetSelectAll(bool value)
    {
        for (int i = 0; i < count_unit; i++)
        {
            ptrSelectData[i] = value ? 1 : 0;
        }
    }



    public RectTransform rtTrSelect;


    void Test_RectIn(Rect rect)
    {
        {
            var camMain = Camera.main;
            var mainCamTr = camMain.transform;
            float4x4 S;
            float4x4 CV;

            {
                var data = ptrRectInInfo;

                data->rect = new float4(rect.x, rect.y, rect.width, rect.height);
                data->data = new float4(0, count_unit, 0.0f, 0.0f);
            }            

            {
                RenderUtil.GetMat_SfromW(mainCamTr, camMain, out S, out CV);

                var data = ptrSCV_data;
                data[0] = S;
                data[1] = CV;                
            }

            {
                var data = ptrPos_data;
                for (int i = 0; i < count_unit; i++)
                {
                    data[i] = new float4(unitActors[i].transform.position, 0.0f);
                }                
            }                 
        }

        {
            CommandBuffer cmd = CommandBufferPool.Get();

            IntPtr ptr = ((PtrRenderActor*)ptr_native_func)->IssueUpdate_rectIn;
            cmd.IssuePluginEvent(ptr, id_native);

            Graphics.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }


        {
            uint waitResult = WaitForSingleObject(hRectInDone, INFINITE); // Max 1000ms

            if (waitResult == 0) // WAIT_OBJECT_0
            {
                // 동기화 완료: C++에서 안전하게 Readback된 데이터 사용
                Debug.Log("GPU Compute & Worker Thread Readback Completed!");
            }
            else
            {
                Debug.LogWarning("Timeout waiting for GPU Compute completion.");
            }
        }

        {
            for (int i = 0; i < count_unit; i++)
            {
                //if (ptrInRect_data[i] == 1)
                //{
                //    ptrSelectData[i] = 1;
                //}

                ptrSelectData[i] = ptrInRect_data[i];
            }
            
        }

    }


    IEnumerator SelectAction()
    {
        bool down = false;
        float3 rp0 = float3.zero;
        float3 rp1 = float3.zero;

        LayerMask lmask = LayerMask.GetMask("Unit");

        {
            rtTrSelect.anchorMin = new Vector2(0.0f, 0.0f);
            rtTrSelect.anchorMax = new Vector2(0.0f, 0.0f);
            rtTrSelect.pivot = new Vector2(0.0f, 0.0f);
        }

        while (!bRender_ready) { yield return null; }

 
        while (true)
        {
            //if (Input.GetMouseButtonDown(0))
            if (Input.GetMouseButton(0) && down == false)
            {       
                //Ray ray = mainCam.ScreenPointToRay(Input.mousePosition);
                Ray ray = RenderUtil.GetRay_WfromS(Input.mousePosition, Camera.main);
                RaycastHit hit;

                //if (Physics.Raycast(ray, out hit, 500.0f, lmask))
                if (Physics.Raycast(ray.origin, ray.direction, out hit, 2000.0f, lmask))
                {
                    var hActor = hit.transform.GetComponent<UnitSimple>();

                    if (hActor != null)
                    {                    
                        {
                            if (Input.GetKey(key_muti_select))
                            {
                                if (hActor.isSelected)
                                { hActor.isSelected = false; }
                                else
                                { hActor.isSelected = true; }
                            }
                            else
                            {
                                SetSelectAll(false);
                                hActor.isSelected = true;
                            }     
                        }
                    }
                }
                else
                {
                    SetSelectAll(false);
                }


                {
                    down = true;
                    rp0 = Input.mousePosition;
                }
            }


            if (down)
            {
                rp1 = Input.mousePosition;
                //if (math.distance(rp0, rp1) > 1000.0f)
                if (math.distance(rp0, rp1) > 1.0f)
                {
                    Rect rt = new Rect(math.min(rp0, rp1).xy, math.abs(rp1 - rp0).xy);
                    //rt = new Rect(new Vector2(100.0f, 100.0f), new Vector2(100.0f, 100.0f));
                    rtTrSelect.offsetMin = rt.min;
                    rtTrSelect.offsetMax = rt.max;

                    //rectIn.rect = rt;
                    //rectIn.Test();

                    //{
                    //    GameManager.instance.Update_SelectBuffer();
                    //    GameManager.torusMan.Compute();
                    //}

                    Test_RectIn(rt);              
                }
            }

            if (Input.GetMouseButtonUp(0))
            {
                down = false;
                rtTrSelect.offsetMin = float2.zero;
                rtTrSelect.offsetMax = float2.zero;
            }

            if (Input.GetKeyDown(KeyCode.R))
            {
                SetSelectAll(true);
            }

            yield return null;
        }
    }

    IEnumerator MoveAction()
    {
        KeyCode key_orbit = CamActor.key_orbit;
        KeyCode key_spin = CamActor.key_spin;

        while (!bRender_ready) { yield return null; }

        while (true)
        {         
            if (Input.GetMouseButton(1) && !Input.GetKey(key_orbit) && !Input.GetKey(key_spin))
            {
                //Ray ray = mainCam.ScreenPointToRay(Input.mousePosition);
                Ray ray = RenderUtil.GetRay_WfromS(Input.mousePosition, Camera.main);
                RaycastHit hit;

                if (Physics.Raycast(ray, out hit, 500, LayerMask.GetMask("Terrain", "Unit")))
                {
                    float3 movePos = hit.point;
                    float3 moveNom = hit.normal;
                    Transform htr = hit.transform;
                    UnitSimple hactor = htr.GetComponent<UnitSimple>();
                    
                    
                    for (int i = 0; i < count_unit; i++)
                    {
                        UnitSimple uactor = unitActors[i];

                        if(uactor.isSelected)
                        {
                            uactor.MoveTo(movePos);
                        }                  
                    }                 
                }
            }
     
            yield return null;
            //yield return new WaitForEndOfFrame();
        }
    }

}
