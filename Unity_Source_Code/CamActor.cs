using System.Collections;
using Unity.Mathematics;
using UnityEngine;

public class CamActor : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        StartCoroutine(ZoomInOut(transform));

        StartCoroutine(MoveParallel(transform, 1));

        StartCoroutine(RotSpin(transform));

        StartCoroutine(RotOrbit(transform));

    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public static KeyCode key_orbit = KeyCode.Z;    //KeyCode.V     //KeyCode.LeftShift
    public static KeyCode key_spin = KeyCode.C;       //KeyCode.B     //KeyCode.LeftAlt
    public static int delta = 5;  //50

    public static float pacePlane = 50.0f;
    public static float paceZoom = 15.0f;
    


    public IEnumerator ZoomInOut(Transform camTrans, float pace = 15.0f)
    {
        while (true)
        {
            //float delta = Input.GetAxis("Mouse ScrollWheel") * 400.0f * pace * Time.deltaTime;
            //float delta = Input.GetAxis("Mouse ScrollWheel") * 400.0f * CamManager.paceZoom * Time.fixedDeltaTime;
            float delta = Input.GetAxis("Mouse ScrollWheel") * 400.0f * paceZoom * Time.deltaTime;

            float3 zaxis = math.rotate(camTrans.rotation, new float3(0.0f, 0.0f, 1.0f));
            float3 pos = camTrans.position;
            pos = pos + zaxis * delta;

            camTrans.position = pos;

            yield return null;
        }
    }

    public IEnumerator MoveParallel(Transform camTrans, int mode = 0)
    {
        Camera cam = camTrans.gameObject.GetComponent<Camera>();

        while (true)
        {
            Vector3 camPos = Vector3.zero;
            float camDelta = 0.05f;

            float y0 = 0.025f;

            //float yScale = bUseYScale ? camTrans.position.y : 1.0f;

            //camDelta = y0 + (0.025f * CamManager.pacePlane) * camTrans.position.y * Time.deltaTime;
            //camDelta = y0 + (0.025f * CamManager.pacePlane) * yScale * Time.deltaTime;

            camDelta = y0 + (0.025f * pacePlane) * math.abs(camTrans.position.y) * Time.deltaTime;

            //camDelta = 1.0f;

            {
                if (Input.GetKey(KeyCode.W))
                {
                    camPos.y += camDelta;
                }

                if (Input.GetKey(KeyCode.A))
                {
                    camPos.x -= camDelta;
                }

                if (Input.GetKey(KeyCode.S))
                {
                    camPos.y -= camDelta;
                }

                if (Input.GetKey(KeyCode.D))
                {
                    camPos.x += camDelta;
                }
            }

            float3 xaxis;
            float3 yaxis;
            float3 zaxis;

            if (mode == 0)
            {
                xaxis = math.rotate(camTrans.rotation, new float3(1.0f, 0.0f, 0.0f));
                zaxis = math.rotate(camTrans.rotation, new float3(0.0f, 0.0f, 1.0f));
            }
            else
            {
                xaxis = math.rotate(camTrans.rotation, new float3(1.0f, 0.0f, 0.0f));
                yaxis = new float3(0.0f, 1.0f, 0.0f);
                zaxis = math.cross(xaxis, yaxis);
            }

            float3 localPos = xaxis * camPos.x + zaxis * camPos.y;
            camTrans.position += (Vector3)localPos;


            yield return null;
        }
    }

    public IEnumerator RotSpin(Transform camTrans)
    {
        Camera cam = camTrans.gameObject.GetComponent<Camera>();

        float3 prePos = float3.zero;
        float3 curPos = float3.zero;

        float3 angle = float3.zero;

        bool down = false;

        quaternion q = quaternion.identity;
        quaternion preRot = quaternion.identity;

        while (true)
        {
            if (Input.GetMouseButtonDown(1) && Input.GetKey(key_spin))
            {
                prePos = Input.mousePosition;
                preRot = camTrans.rotation;
                down = true;
            }

            if (down)
            {
                curPos = Input.mousePosition;
                angle = (curPos - prePos) * 0.1f;

                q = math.mul(math.mul(
                    quaternion.AxisAngle(new float3(0.0f, 1.0f, 0.0f), math.radians(angle.x)),
                    preRot),
                    quaternion.AxisAngle(new float3(1.0f, 0.0f, 0.0f), math.radians(-angle.y)));

                camTrans.rotation = q;
            }

            if (Input.GetMouseButtonUp(1) || !Input.GetKey(key_spin))
            {
                down = false;
            }

            yield return null;
        }

    }

    public IEnumerator RotOrbit(Transform camTrans)
    {
        Camera cam = camTrans.gameObject.GetComponent<Camera>();

        float3 prePos = float3.zero;
        float3 curPos = float3.zero;

        float3 angle = float3.zero;

        bool down = false;

        Ray ray = new Ray();
        float3 center = float3.zero;
        float radius = 1.0f;
        float3 centerNormal = float3.zero;

        quaternion preRot = quaternion.identity;

        float3 rayDirInView = float3.zero;
        float3 rayDirInWorld = float3.zero;

        quaternion q = quaternion.identity;

        while (true)
        {
            if (Input.GetMouseButtonDown(1) && Input.GetKey(key_orbit))
            {
                prePos = Input.mousePosition;
                preRot = camTrans.rotation;

                ray = cam.ScreenPointToRay(prePos);

                float3x3 mat = new float3x3(camTrans.rotation);
                float3 rayDir = ray.direction.normalized;
                rayDirInView = math.mul(math.transpose(mat), rayDir);

                {
                    float3 n = new float3(0.0f, -1.0f, 0.0f);
                    float3 p0 = new float3(0.0f, 0.0f, 0.0f);

                    float3 d = math.normalize(ray.direction);
                    float3 l0 = ray.origin;

                    float k0 = math.dot(n, d);
                    float k1 = math.dot(n, (p0 - l0));

                    float t = 0.0f;
                    t = (math.abs(k0) < 0.1f) ? 10.0f : (k1 / k0);

                    float3 l1 = l0 + t * d;
                    radius = math.distance(l1, camTrans.position);
                    center = l1;

                    down = true;
                }

                //{
                //    float3 n = new float3(0.0f, -1.0f, 0.0f);
                //    float3 p0 = new float3(0.0f, 0.0f, 0.0f);
                //
                //    float3 d = math.normalize(ray.direction);
                //    float3 l0 = ray.origin;
                //
                //    float t = (math.dot(n, (p0 - l0)) / math.dot(n, d));
                //    float3 l1 = l0 + t * d;
                //    radius = math.distance(l1, camTrans.position);
                //    center = l1;
                //
                //    //Debug.Log("radius : " + radius.ToString());
                //    //Debug.Log("center : " + center.ToString());
                //
                //    down = true;
                //}
            }

            {
                if (down)
                {
                    curPos = Input.mousePosition;
                    angle = (curPos - prePos) * 0.1f;

                    q = math.mul(math.mul(
                        quaternion.AxisAngle(new float3(0.0f, 1.0f, 0.0f), math.radians(-angle.x)),
                        preRot),
                        quaternion.AxisAngle(new float3(1.0f, 0.0f, 0.0f), math.radians(angle.y)));

                    camTrans.rotation = q;

                    float3x3 mat = new float3x3(q);
                    rayDirInWorld = math.mul(mat, rayDirInView);

                    camTrans.position = center + radius * (-rayDirInWorld);
                }
            }

            if (Input.GetMouseButtonUp(1) || !Input.GetKey(key_orbit))
            {
                down = false;
            }

            yield return null;
        }
    }
}
