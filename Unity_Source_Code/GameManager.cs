using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections.Generic;

using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Rendering;



unsafe public class GameManager : MonoBehaviour
{
    public int vSyncCount = 1;
    public int fps = 60;

    public const string DLL_NAME = "RenderingPlugin_D3D11";

    [DllImport(DLL_NAME)]
    private static extern IntPtr GetRenderEventFunc();

    [DllImport(DLL_NAME)]
    private static extern IntPtr Start_Plugin();

    [DllImport(DLL_NAME)]
    private static extern IntPtr Finish_Plugin();   

    [DllImport(DLL_NAME)]
    private static extern void SetOnEditor(bool onEditor);

    //[DllImport(DLL_NAME, CharSet = CharSet.Unicode)]
    //private static extern void SetAssetPath([MarshalAs(UnmanagedType.LPWStr)] string path);

    [DllImport(DLL_NAME, CharSet = CharSet.Auto)]
    private static extern void SetAssetPath([MarshalAs(UnmanagedType.LPStr)] string path);


    [DllImport(DLL_NAME)]
    private static extern void SetRastMode(int mode);

    public GameObject instance;

    public NavigationTest navTest;


    public int count;
   
    List<ActorObject> actors;
  


    void Awake()
    {
        QualitySettings.vSyncCount = vSyncCount;
        Application.targetFrameRate = fps;

#if UNITY_EDITOR
        SetOnEditor(true);

        string projectRoot = Directory.GetParent(Application.dataPath).FullName;

        // 2. Native Plugin으로 전달
        //Debug.Log($"Sending path to Native: {projectRoot}");
        SetAssetPath(projectRoot);
#else
        SetOnEditor(false);
#endif

        Start_Plugin();

        {
            RenderManager.Init();
        }

        {    
            actors = new List<ActorObject>();
        }


        for(int i = 0; i < count; i++)
        {
            var gameObject = GameObject.Instantiate(instance);
            gameObject.SetActive(true);
            
            actors.Add(gameObject.GetComponent<StoPassTest>());
        }

        if(navTest != null) 
        {
            //navTest.gameObject.SetActive(true);
            actors.Add(navTest);
        }

        foreach (var actor in actors)
        {
            actor.Init();
        }


    }

    void OnEnable()
    {
        foreach (var actor in actors)
        {
            actor.Enable();
        }

        RenderManager.OnRender += Render;
    }
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {        
        foreach (var actor in actors)
        {
            actor.Begin();
        }
    }

    void OnDisable()
    {
        //foreach (var obj in stoPassTest)
        //{
        //    obj.Disable();
        //}

        foreach (var actor in actors)
        {
            actor.Disable();
        }

        RenderManager.OnRender -= Render;
    }

    private void OnDestroy()
    {
        Finish_Plugin();

        //foreach (var obj in stoPassTest)
        //{
        //    obj.Finish();
        //}

        foreach (var actor in actors)
        {
            actor.Finish();
        }
    }

    int rastMode = 0;

    // Update is called once per frame
    void Update()
    {
        //CommandBuffer cmd = new CommandBuffer();
        //
        //cmd.IssuePluginEvent(GetRenderEventFunc(), 1);
        //
        //
        //Graphics.ExecuteCommandBuffer(cmd);
        //cmd.Release();

        if (Input.GetKeyDown(KeyCode.Space))
        {
            rastMode = (++rastMode) % 4;

            SetRastMode(rastMode);
        }
    }


    unsafe private void Render(ScriptableRenderContext context, CommandBuffer cmd)
    {       
        {            
            cmd.IssuePluginEvent(GetRenderEventFunc(), 0);


            int a = 10;
            int b = 20;
            int c = a + b;
        }
    }
}
