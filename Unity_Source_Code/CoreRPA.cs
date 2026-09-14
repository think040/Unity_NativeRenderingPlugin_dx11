using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Unity.Mathematics;
using UnityEditor;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.Windows.WebCam;
using static Unity.Burst.Intrinsics.X86.Avx;



[CreateAssetMenu(fileName = "CoreRPA", menuName = "RenderPipeline/CoreRPA")]
public class CoreRPA : RenderPipelineAsset<CoreRP>
{

    public Color _clearColor = Color.green;
    public static Color clearColor;

    protected override RenderPipeline CreatePipeline()
    {
        clearColor = _clearColor;
        return new CoreRP();
    }
}

public class CoreRP : RenderPipeline
{
    protected void Render0(ScriptableRenderContext context, List<Camera> cams)
    {

#if UNITY_EDITOR
        if (EditorApplication.isPlaying)
#endif
        {
            {
                RenderManager.PreContexRender(context, cams);
                //BeginContextRendering(context, cams);

                foreach (var cam in cams)
                {
                    RenderManager.PreCameraRender(context, cam);
                    //BeginCameraRendering(context, cam);
                  
                    {
                        RenderManager.RenderCam(context, cam);
                    }
                
                    //EndCameraRendering(context, cam);
                    RenderManager.PostCameraRender(context, cam);
                }
                
               //EndContextRendering(context, cams);
               RenderManager.PostContextRender(context, cams);
            }

            {
                context.Submit();

                int a = 10;
                int b = a + 10;
            }
        }

    }

    
    protected override void Render(ScriptableRenderContext context, List<Camera> cams)
    {

#if UNITY_EDITOR
        if (EditorApplication.isPlaying)
#endif
        {
            //Debug.Log($"List<Camera>_count : {cams.Count}");      
           
            {
                {
                    RenderManager.SetUpCamera(cams);
                }

                {
                    RenderManager.Render(context);
                }
            }

            {
                context.Submit();

                int a = 10;
                int b = a + 10;
            }
        }

    }
}

unsafe public static class RenderManager
{  
    static RenderManager()
    {
       
    }

    public const int maxCamCount = 8;
    //public static Camera[] camToIdx = null;
    public static Dictionary<Camera, int> camToIdx = null;
    public static Dictionary<CameraType, int> camTypeToIdx = null;


    public static int camCount = 0;
    

    public static void Init()
    {
        {
            //camToIdx = new Camera[maxCamCount];
            camToIdx = new Dictionary<Camera, int>();
            camTypeToIdx = new Dictionary<CameraType, int>();
        }

        fixed (PerCamera** pptr = &ptrPerCam)
        {
            GetPerCam_ptr((void**)pptr);

            int a = 0;
        }
    }

    public static void SetUpCamera0(List<Camera> cams)
    {
        SetPerCamEnableAll(false);        
       

        for (int i = 0; i < cams.Count; i++)
        {       
            Camera cam = cams[i];
            if (camToIdx.ContainsKey(cam))
            {
                {
                    int idx = camToIdx[cam];
                    SetPerCamPorperty(cam, idx);
                }
            }
            else
            {
                if (camCount < maxCamCount)
                {
                    camToIdx.Add(cam, camCount);
                    SetPerCamPorperty(cam, camCount);                    

                    camCount++;
                }              
            }                       
        }
    }

    public static void SetUpCamera(List<Camera> cams)
    {
        SetPerCamEnableAll(false);


        for (int i = 0; i < cams.Count; i++)
        {
            Camera cam = cams[i];
            if (camTypeToIdx.ContainsKey(cam.cameraType))
            {
                {
                    int idx = camTypeToIdx[cam.cameraType];
                    SetPerCamPorperty(cam, idx);
                }
            }
            else
            {
                if (camCount < maxCamCount)
                {
                    camTypeToIdx.Add(cam.cameraType, camCount);
                    SetPerCamPorperty(cam, camCount);

                    camCount++;
                }
            }
        }
    }

    public struct PerCamera
    {
        public float4x4 V;
        public float4x4 C;
        public float4x4 S;
        public float4x4 CV;
        public float4 dirW_view;
        public float4 posW_view;
        public float4 pixelSize;
        public float4 data;
    }

    public const string DLL_NAME = "RenderingPlugin_D3D11";

    public static PerCamera* ptrPerCam = null;

    [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
    extern static void GetPerCam_ptr(void** ppData);

   


    //public static PerCamera perCam;
    public static Color clearColor = new Color(0.75f, 0.75f, 0.75f, 1.0f);
    //public static Color clearColor = new Color(0.0f, 1.0f, 0.0f, 1.0f);    

    public static Action PreRender
    {
        get; set;
    } = () => { };


    public static Action<ScriptableRenderContext, CommandBuffer, List<Camera>> BeginContextRender
    {
        get; set;
    } = (context, cmd, cams) => { };

    public static Action<ScriptableRenderContext, CommandBuffer, Camera> BeginCameraRender
    {
        get; set;
    } = (context, cmd, cam) => { };

    public static Action<ScriptableRenderContext, CommandBuffer, Camera> OnRenderCam
    {
        get; set;
    } = (context, cmd, cam) => { };

    public static Action<ScriptableRenderContext, CommandBuffer, Camera> OnRenderCamAlpha
    {
        get; set;
    } = (context, cmd, cam) => { };

    public static Action<ScriptableRenderContext, CommandBuffer, Camera> OnRenderCamDebug
    {
        get; set;
    } = (context, cmd, cam) => { };

    public static Action<ScriptableRenderContext, CommandBuffer, Camera> OnRenderCamViewport
    {
        get; set;
    } = (context, cmd, cam) => { };


    public static Action<ScriptableRenderContext, CommandBuffer, Camera> EndCameraRender
    {
        get; set;
    } = (context, cmd, cam) => { };

    public static Action<ScriptableRenderContext, CommandBuffer, List<Camera>> EndContextRender
    {
        get; set;
    } = (context, cmd, cams) => { };


    public static void SetPerCamType(int idx, CameraType type)
    {
        var perCam = (ptrPerCam + idx);
        if (perCam != null)
        {
            float* ptr = ((float*)(&(perCam->data)) + 1);
            *ptr = (int)type;
        }
    }


    public static void SetPerCamEnable(int idx, bool bEnable)
    { 
        var perCam = (ptrPerCam + idx);
        if (perCam != null)
        {      
            float* ptr = ((float*)(&(perCam->data)) + 0);
            *ptr = bEnable ? 1 : 0;
        }
    }

    public static void SetPerCamEnableAll(bool bEnable)
    {
        for (int i = 0; i < maxCamCount; i++)
        {
            SetPerCamEnable(i, bEnable);
        }      
    }

    public static void SetPerCamPorperty(Camera cam, int idx = 0)
    {
        float4x4 V = float4x4.zero;
        float4x4 C = float4x4.zero;
        float4x4 S = float4x4.zero;
        float4x4 CV = float4x4.zero;
    
        bool bSceneView = false;
        {
            V = RenderUtil.GetVfromW(cam);    

            if(cam.cameraType == CameraType.Game)
            {
                bSceneView = false;
            }
            else
            if (cam.cameraType == CameraType.SceneView)
            {
                bSceneView = true;
            }

            C = RenderUtil.GetCfromV(cam, bSceneView);
            S = RenderUtil.GetSfromN(cam, bSceneView);


            CV = math.mul(C, V);
            float3 dirW_view = -math.rotate(cam.transform.rotation, new float3(0.0f, 0.0f, 1.0f));
            float3 posW_view = cam.transform.position;

           
            var perCam = (ptrPerCam + idx);
            if(perCam != null)
            {
                perCam->V = V;
                perCam->C = C;
                perCam->S = S;
                perCam->CV = CV;
                perCam->dirW_view = new float4(dirW_view, 0.0f);
                perCam->posW_view = new float4(posW_view, 0.0f);
                //perCam->pixelSize[0] = cam.pixelWidth;
                //perCam->pixelSize[1] = cam.pixelHeight;              
            }

            SetPerCamType(idx, cam.cameraType);
            SetPerCamEnable(idx, true);
        }
    }

    public static void PreContexRender(ScriptableRenderContext context, List<Camera> cams)
    {    

        {
            CommandBuffer cmd = CommandBufferPool.Get();
            BeginContextRender(context, cmd, cams);

            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
    }

    public static void PreCameraRender(ScriptableRenderContext context, Camera cam)
    {

        {
            //SetPerCamPorperty(cam);
        }

        {
            CommandBuffer cmd = CommandBufferPool.Get();
            BeginCameraRender(context, cmd, cam);

            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }   
    }  


    public static void RenderCam(ScriptableRenderContext context, Camera cam)
    {   
        {
            CommandBuffer cmd = CommandBufferPool.Get();

            {
                cmd.ClearRenderTarget(true, true, clearColor);
            }

            {
                OnRenderCam(context, cmd, cam);
            }                          

            context.ExecuteCommandBuffer(cmd);      
            CommandBufferPool.Release(cmd);
        }        
    }

    public static void PostCameraRender(ScriptableRenderContext context, Camera cam)
    {
       
        {
            CommandBuffer cmd = CommandBufferPool.Get();
            EndCameraRender(context, cmd, cam);

            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
    }

    public static void PostContextRender(ScriptableRenderContext context, List<Camera> cams)
    {
        {
            CommandBuffer cmd = CommandBufferPool.Get();
            EndContextRender(context, cmd, cams);

            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }       
    }




    public static Action<ScriptableRenderContext, CommandBuffer> OnRender
    {
        get; set;
    } = (context, cmd) => { };
 
    public static void Render(ScriptableRenderContext context)
    {
        {
            CommandBuffer cmd = CommandBufferPool.Get();  

            {
                OnRender(context, cmd);
            }

            context.ExecuteCommandBuffer(cmd);
            CommandBufferPool.Release(cmd);
        }
    }

}