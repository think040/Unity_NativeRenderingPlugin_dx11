using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

using TMPro;
using UnityEngine;
using UnityEngine.UI;


public class NativeLogger : MonoBehaviour
{
    // 델리게이트 정의 (C++의 const char*는 C#에서 string으로 매핑됨)
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void LogDelegate(IntPtr message);

    [DllImport("RenderingPlugin_D3D11")]
    private static extern void RegisterLogCallback(LogDelegate cb);

    [DllImport("RenderingPlugin_D3D11")]
    private static extern void DoSomethingAndLog();

    // GC 방지를 위해 멤버 변수로 유지
    static private LogDelegate _logDelegate;

    public static TextMeshProUGUI text;
    public TextMeshProUGUI _text;

    private static ConcurrentQueue<string> logQueue = new ConcurrentQueue<string>();
  

    private StringBuilder sb = new StringBuilder();

    [SerializeField] private int maxLines = 100;
    private int currentLineCount = 0;

    void Awake()
    {        
        _logDelegate = new LogDelegate(LogFromNative);
        RegisterLogCallback(_logDelegate);

        text = _text;
        //GameObject obj = GameObject.Find("Debug_Native");

        int a = 0;
    }

    void Start()
    {
        // 2. 테스트 호출
        //DoSomethingAndLog();
    }

    static bool bUse = true;

    private void Update()
    {
        bool hasNewLog = false;

        // 1. 큐에 쌓인 모든 로그를 한 번에 StringBuilder에 추가
        while (logQueue.TryDequeue(out string message))
        {
            sb.AppendLine(message);
            currentLineCount++;
            hasNewLog = true;
        }

        // 2. 로그가 새로 추가되었을 때만 UI 갱신
        if (hasNewLog)
        {
            // (선택) 너무 길어지면 오래된 로그 삭제
            if (currentLineCount > maxLines)
            {
                // 간단한 구현을 위해 전체 초기화 후 재생성하거나, 
                // 실 서비스용이라면 sb.Remove를 사용해 앞부분을 잘라낼 수 있습니다.
                if (sb.Length > 5000)
                {
                    sb.Clear();
                    //bUse = false;
                }
                
                currentLineCount = 0;

                
            }

            text.SetText(sb.ToString());
        }
    }

    [AOT.MonoPInvokeCallback(typeof(LogDelegate))]
    static void LogFromNative(IntPtr messagePtr)
    {
        string message = Marshal.PtrToStringAnsi(messagePtr);

        string timestamp = DateTime.Now.ToString("HH:mm:ss.fff");

        // TMP 색상 태그 적용 (시간은 은색, 메시지는 흰색)
        string formattedLog = $"<color=yellow>[Native]</color> <color=#FF0000>[{timestamp}]</color> {message}";
        Debug.Log(formattedLog);

        if(bUse)
        {
            logQueue.Enqueue(formattedLog);
        }       

        //Debug.Log($"<color=yellow>[Native]</color> {message}");
        //logQueue.Enqueue(message);

        int b = 0;
    }
}
