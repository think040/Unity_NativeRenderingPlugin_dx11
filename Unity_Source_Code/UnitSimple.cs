using Unity.Mathematics;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.UIElements;

public class UnitSimple : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    public void Init()
    {

    }

    public void Begin()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (!NavigationTest.bRender_ready) { return; }

        if (Input.GetKeyDown(KeyCode.Q) && isSelected)       
        {
            ActState_Idle();
        }
        else
        {
            float3 tPos = targetPos;
            float3 cPos = transform.position;

            tPos.y = 0.0f;
            cPos.y = 0.0f;
            //if (math.distance(tPos, cPos) < 1.0f) 
            if (math.distance(tPos, cPos) < 0.5f)
            {
                ActState_Idle();
            }
            else
            {
                ActState_Run();
            }
        }
        
    }

  

    void ActState_Idle()
    {
        if (!nvAgent.isOnNavMesh)
        {
            return;
        }

        nvAgent.isStopped = true;
        targetPos = transform.position;

        //nvAgent.isStopped = false;
        //nvAgent.SetDestination(targetPos);

        float speed = math.length(nvAgent.velocity);
      
        if (speed < 0.1f)
        {
            //anim.PlayCross("Idle");            
        }
        else
        {
            //anim.PlayCross("Running");       
        }
    }

    void ActState_Run()
    {
        if (!nvAgent.isOnNavMesh)
        {         
            return;
        }

        nvAgent.isStopped = false;
        nvAgent.SetDestination(targetPos);
       

        //anim.PlayCross("Running");  
    }

    public int offsetIdx = 0;

    public int iid;

    public NavMeshAgent nvAgent;

    public float radius
    {
        get
        {
            if(nvAgent == null) { return -1.0f; }
            return nvAgent.radius;
        }
    }

    public void MoveTo(float3 pos)
    {
        if(!NavigationTest.bRender_ready) { return; }

        //nvAgent.SetDestination(pos);

        targetPos = pos;
    }

    public unsafe float3 targetPos
    {
        get
        {
            if (!NavigationTest.bRender_ready) { return float3.zero; }
          
            {
                int idx = offsetIdx + iid;
                return NavigationTest.ptrTargetPos[idx].xyz;
            }      
        }
        set
        {
            if (!NavigationTest.bRender_ready) { return; }

            {
                int idx = offsetIdx + iid;
                NavigationTest.ptrTargetPos[idx] = new float4(value, 0.0f);            
            }             
        }
    }

    public unsafe bool isSelected
    {
        get
        {
            int idx = offsetIdx + iid;
            return NavigationTest.ptrSelectData[idx] == 1 ? true : false;
        }
        set
        {
            int idx = offsetIdx + iid;
            NavigationTest.ptrSelectData[idx] = value ? 1 : 0;
        }
    }

}
