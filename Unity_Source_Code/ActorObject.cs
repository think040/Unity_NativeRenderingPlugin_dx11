using UnityEngine;

public abstract class ActorObject : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public  abstract void Init();
 
    public  abstract void Enable();
  
    public  abstract void Begin();
   
    public  abstract void Disable();
   
    public  abstract void Finish();

}
