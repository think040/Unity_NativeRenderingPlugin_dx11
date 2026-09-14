#ifndef UTILITYCS_HLSL
#define UTILITYCS_HLSL

#if defined(ROW_MAJOR)
    #pragma pack_matrix(row_major)
#elif defined(COLUMN_MAJOR)
    #pragma pack_matrix(column_major)
#else

#endif

#define v3c0 _m00_m10_m20
#define v3c1 _m01_m11_m21
#define v3c2 _m02_m12_m22
#define v3c3 _m03_m13_m23

#define v4c0 _m00_m10_m20_m30
#define v4c1 _m01_m11_m21_m31
#define v4c2 _m02_m12_m22_m32
#define v4c3 _m03_m13_m23_m33

#define v3r0 _m00_m01_m02
#define v3r1 _m10_m11_m12
#define v3r2 _m20_m21_m22
#define v3r3 _m30_m31_m32

#define v4r0 _m00_m01_m02_m03
#define v4r1 _m10_m11_m12_m13
#define v4r2 _m20_m21_m22_m23
#define v4r3 _m30_m31_m32_m33

#define f3zero float3(0.0f, 0.0f, 0.0f);
#define f3x3I float3x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
#define f3x3Zero float3x3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
#define f4x4I float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)
#define f4x4Zero float4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
#define qI float4(0.0f, 0.0f, 0.0f, 1.0f);

struct Triangle
{
    static bool TestLineSegmentToTriangle(float2x3 lineSeg, float3x3 tri, out float3 result)
    {
        float3 r = float3(0.0f, 0.0f, 0.0f);
        float3x3 M = float3x3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        M.v3r0 = lineSeg.v3r0 - lineSeg.v3r1;
        M.v3r1 = tri.v3r1 - tri.v3r0;
        M.v3r2 = tri.v3r2 - tri.v3r0;
        float3 v = lineSeg.v3r0 - tri.v3r0;

		//optimized
        float3 area1 = cross(M.v3r1, M.v3r2);
        float3 area2 = cross(M.v3r0, v);
        float det = dot(M.v3r0, area1);
        r.x = dot(v, area1) / det;
        r.y = dot(M.v3r2, area2) / det;
        r.z = dot(M.v3r1, -area2) / det;

        result = r;
        if ((r.x >= 0.0f && r.x <= 1.0f) &&
			(r.y >= 0.0f && r.y <= 1.0f) &&
			(r.z >= 0.0f && r.z <= 1.0f) &&
			(r.y + r.z >= 0.0f && r.y + r.z <= 1.0f))
        {
            return true;
        }

        return false;
    }

    static bool TestTriangleToTriangleOutPlane(float3x3 triA, float3x3 triB)
    {
        float3 result = float3(0.0f, 0.0f, 0.0f);
        float2x3 lineSeg = float2x3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

		//lineSegA_triB
        lineSeg.v3r0 = triA.v3r0;
        lineSeg.v3r1 = triA.v3r1;
        if (Triangle::TestLineSegmentToTriangle(lineSeg, triB, result))
        {
            return true;
        }

        lineSeg.v3r0 = triA.v3r1;
        lineSeg.v3r1 = triA.v3r2;
        if (Triangle::TestLineSegmentToTriangle(lineSeg, triB, result))
        {
            return true;
        }

        lineSeg.v3r0 = triA.v3r2;
        lineSeg.v3r1 = triA.v3r0;
        if (Triangle::TestLineSegmentToTriangle(lineSeg, triB, result))
        {
            return true;
        }

		//triA_lineSegB
        lineSeg.v3r0 = triB.v3r0;
        lineSeg.v3r1 = triB.v3r1;
        if (Triangle::TestLineSegmentToTriangle(lineSeg, triA, result))
        {
            return true;
        }

        lineSeg.v3r0 = triB.v3r1;
        lineSeg.v3r1 = triB.v3r2;
        if (Triangle::TestLineSegmentToTriangle(lineSeg, triA, result))
        {
            return true;
        }

        lineSeg.v3r0 = triB.v3r2;
        lineSeg.v3r1 = triB.v3r0;
        if (Triangle::TestLineSegmentToTriangle(lineSeg, triA, result))
        {
            return true;
        }

        return false;
    }

    static bool TestTriangleToTriangleSimple(float3x3 triA, float3x3 triB, float4x4 BtoA)
    {
        triB.v3r0 = mul(float4(triB.v3r0, 1.0f), BtoA).xyz;
        triB.v3r1 = mul(float4(triB.v3r1, 1.0f), BtoA).xyz;
        triB.v3r2 = mul(float4(triB.v3r2, 1.0f), BtoA).xyz;

        if (Triangle::TestTriangleToTriangleOutPlane(triA, triB))
        {
            return true;
        }

        return false;
    }
};

struct Plane
{
    float2x3 plane;

    static bool TestPointToPlaneInOut(float3 pos, float2x3 plane)
    {
        if (dot(plane.v3r0, pos - plane.v3r1) <= 0.0f)
        {
            return true;
        }

        return false;
    }
};

struct Quaternion
{
    static float4 identity()
    {
        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    static float4 axisAngle(float3 axis, float angle)
    {
        float sin;
        float cos;
        sincos(0.5f * angle, sin, cos);
        return float4(sin * axis, cos);
    }
	
    static float4 conjugate(float4 q)
    {
        return float4(-q.xyz, q.w);
    }
	        
    static float4 mul(float4 b, float4 a)
    {
        return float4(b.xyz * a.w + b.w * a.xyz + cross(a.xyz, b.xyz), -dot(b.xyz, a.xyz) + b.w * a.w);
    }
	       
    static float3 rotate(float3 v, float4 q)
    {
        float3 t = 2.0f * cross(q.xyz, v);
        return v + cross(q.xyz, t) + q.w * t;
    }
	        
    static float3x3 toMat(float4 q)
    {
        float _2xx = 2.0f * q.x * q.x;
        float _2yy = 2.0f * q.y * q.y;
        float _2zz = 2.0f * q.z * q.z;

        float _2xy = 2.0f * q.x * q.y;
        float _2yz = 2.0f * q.y * q.z;
        float _2zx = 2.0f * q.z * q.x;

        float _2wx = 2.0f * q.w * q.x;
        float _2wy = 2.0f * q.w * q.y;
        float _2wz = 2.0f * q.w * q.z;

        float3x3 R;
        R._m00 = 1.0f - _2yy - _2zz;
        R._m11 = 1.0f - _2zz - _2xx;
        R._m22 = 1.0f - _2yy - _2xx;

        R._m10 = _2xy - _2wz;
        R._m01 = _2xy + _2wz;

        R._m20 = _2zx + _2wy;
        R._m02 = _2zx - _2wy;

        R._m21 = _2yz - _2wx;
        R._m12 = _2yz + _2wx;

        return R;
    }
    
    static float3x3 toMat_row(float4 q)
    {
        float _2xx = 2.0f * q.x * q.x;
        float _2yy = 2.0f * q.y * q.y;
        float _2zz = 2.0f * q.z * q.z;

        float _2xy = 2.0f * q.x * q.y;
        float _2yz = 2.0f * q.y * q.z;
        float _2zx = 2.0f * q.z * q.x;

        float _2wx = 2.0f * q.w * q.x;
        float _2wy = 2.0f * q.w * q.y;
        float _2wz = 2.0f * q.w * q.z;

        float3x3 R;
        R._m00 = 1.0f - _2yy - _2zz;
        R._m11 = 1.0f - _2zz - _2xx;
        R._m22 = 1.0f - _2yy - _2xx;

        R._m10 = _2xy - _2wz;
        R._m01 = _2xy + _2wz;

        R._m20 = _2zx + _2wy;
        R._m02 = _2zx - _2wy;

        R._m21 = _2yz - _2wx;
        R._m12 = _2yz + _2wx;

        return R;
    }
    
    static float3x3 toMat_column(float4 q)        //for_Unity
    {
        float _2xx = 2.0f * q.x * q.x;
        float _2yy = 2.0f * q.y * q.y;
        float _2zz = 2.0f * q.z * q.z;

        float _2xy = 2.0f * q.x * q.y;
        float _2yz = 2.0f * q.y * q.z;
        float _2zx = 2.0f * q.z * q.x;

        float _2wx = 2.0f * q.w * q.x;
        float _2wy = 2.0f * q.w * q.y;
        float _2wz = 2.0f * q.w * q.z;

        float3x3 R;
        R._m00 = 1.0f - _2yy - _2zz;
        R._m11 = 1.0f - _2zz - _2xx;
        R._m22 = 1.0f - _2yy - _2xx;

        R._m10 = _2xy + _2wz;
        R._m01 = _2xy - _2wz;

        R._m20 = _2zx - _2wy;
        R._m02 = _2zx + _2wy;

        R._m21 = _2yz + _2wx;
        R._m12 = _2yz - _2wx;

        return R;
    }
	
    static float4 fromMat(float3x3 m)
    {
        float4 q;
		
        float trace = m._m00 + m._m11 + m._m22;
        if (trace >= 0)
        {
            q.w = 0.5f * sqrt(trace + 1);
            q.z = (-m._m10 + m._m01) / (4 * q.w);
            q.y = (+m._m20 - m._m02) / (4 * q.w);
            q.x = (-m._m21 + m._m12) / (4 * q.w);
        }
        else
        {
            float dia[3];

            dia[0] = m._m00;
            dia[1] = m._m11;
            dia[2] = m._m22;
			
            float max = dia[0];
            for (int i = 1; i < 3; i++)
            {
                if (max < dia[i])
                {
                    max = dia[i];
                }
            }

            if (max == m._m00)
            {
                q.x = (0.5f) * sqrt(+m._m00 - m._m11 - m._m22 + 1);
                q.y = (+m._m10 + m._m01) / (4 * q.x);
                q.z = (+m._m20 + m._m02) / (4 * q.x);
                q.w = (-m._m21 + m._m12) / (4 * q.x);
            }
            else if (max == m._m11)
            {
                q.y = (0.5f) * sqrt(-m._m00 + m._m11 - m._m22 + 1);
                q.x = (+m._m10 + m._m01) / (4 * q.y);
                q.w = (+m._m20 - m._m02) / (4 * q.y);
                q.z = (+m._m12 + m._m21) / (4 * q.y);
            }
            else if (max == m._m22)
            {
                q.z = (0.5f) * sqrt(-m._m00 - m._m11 + m._m22 + 1);
                q.w = (-m._m10 + m._m01) / (4 * q.z);
                q.x = (+m._m20 + m._m02) / (4 * q.z);
                q.y = (+m._m21 + m._m12) / (4 * q.z);
            }

        }
        return q;
    }
    
    static float4 fromMat(float3 r0, float3 r1, float3 r2)
    {
        float4 q;
        float3x3 m;
        m.v3r0 = r0;
        m.v3r1 = r1;
        m.v3r2 = r2;
        
        q = fromMat(m);
        
        return q;
    }
	
    static float4 slerp(float4 a, float4 b, float t)
    {
        float4 q;
		
        float dt = dot(a, b);
        if (dt < 0.0f)
        {
            dt = -dt;
            b = -b;
        }

        if (dt < 0.9995f)
        {
            float angle = acos(dt);
            float si = rsqrt(1.0f - dt * dt);
            float wa = sin(angle * (1.0f - t)) * si;
            float wb = sin(angle * t) * si;
            return float4(a * wa + b * wb);
        }
        else
        {
            return normalize(lerp(a, b, t));
        }
    }
    
    
    //Test
    static float4 mul0(float4 a, float4 b)
    {
        return float4(a.xyz * b.w + a.w * b.xyz + cross(a.xyz, b.xyz), -dot(a.xyz, b.xyz) + a.w * b.w);
    }
    
    static float3 rotate0(float4 q, float3 v)
    {
        float3 t = 2.0f * cross(q.xyz, v);
        return v + cross(q.xyz, t) + q.w * t;
    }
    
    static float3x3 toMat0(float4 q)
    {
        float _2xx = 2.0f * q.x * q.x;
        float _2yy = 2.0f * q.y * q.y;
        float _2zz = 2.0f * q.z * q.z;

        float _2xy = 2.0f * q.x * q.y;
        float _2yz = 2.0f * q.y * q.z;
        float _2zx = 2.0f * q.z * q.x;

        float _2wx = 2.0f * q.w * q.x;
        float _2wy = 2.0f * q.w * q.y;
        float _2wz = 2.0f * q.w * q.z;

        float3x3 R;
        R._m00 = 1.0f - _2yy - _2zz;
        R._m11 = 1.0f - _2zz - _2xx;
        R._m22 = 1.0f - _2yy - _2xx;

        R._m10 = _2xy + _2wz;
        R._m01 = _2xy - _2wz;

        R._m20 = _2zx - _2wy;
        R._m02 = _2zx + _2wy;

        R._m21 = _2yz + _2wx;
        R._m12 = _2yz - _2wx;

        return R;
    }
	
    static float4 fromMat0(float3x3 m)
    {
        float4 q;
		
        float trace = m._m00 + m._m11 + m._m22;
        if (trace >= 0)
        {
            q.w = 0.5f * sqrt(trace + 1);
            q.z = (+m._m10 - m._m01) / (4 * q.w);
            q.y = (-m._m20 + m._m02) / (4 * q.w);
            q.x = (+m._m21 - m._m12) / (4 * q.w);
        }
        else
        {
            float dia[3];

            dia[0] = m._m00;
            dia[1] = m._m11;
            dia[2] = m._m22;
			
            float max = dia[0];
            for (int i = 1; i < 3; i++)
            {
                if (max < dia[i])
                {
                    max = dia[i];
                }
            }

            if (max == m._m00)
            {
                q.x = (0.5f) * sqrt(+m._m00 - m._m11 - m._m22 + 1);
                q.y = (+m._m10 + m._m01) / (4 * q.x);
                q.z = (+m._m20 + m._m02) / (4 * q.x);
                q.w = (+m._m21 - m._m12) / (4 * q.x);
            }
            else if (max == m._m11)
            {
                q.y = (0.5f) * sqrt(-m._m00 + m._m11 - m._m22 + 1);
                q.x = (+m._m10 + m._m01) / (4 * q.y);
                q.w = (-m._m20 + m._m02) / (4 * q.y);
                q.z = (+m._m12 + m._m21) / (4 * q.y);
            }
            else if (max == m._m22)
            {
                q.z = (0.5f) * sqrt(-m._m00 - m._m11 + m._m22 + 1);
                q.w = (+m._m10 - m._m01) / (4 * q.z);
                q.x = (+m._m20 + m._m02) / (4 * q.z);
                q.y = (+m._m21 + m._m12) / (4 * q.z);
            }

        }
        return q;
    }
    
    static float4 fromMat0(float3 c0, float3 c1, float3 c2)
    {
        float4 q;
        float3x3 m;
        m.v3c0 = c0;
        m.v3c1 = c1;
        m.v3c2 = c2;
        
        q = fromMat0(m);
        
        return q;
    }
			
};

struct DualQuaternion
{
    float4 real;
    float4 dual;
      
    //
    void set(float4 _real, float4 _dual)
    {
        real = _real;
        dual = _dual;
    }
    
    //
    void toRigidParam(out float4 r, out float3 t)
    {
        r = 0.0f;
        t = 0.0f;

        {
            r = real;
            float4 _real = float4(-real.xyz, +real.w);
            t = 2.0f * float3(cross(dual.xyz, _real.xyz) + _real.xyz * dual.w + dual.xyz * _real.w);
        }
    }
     
    void toRigidParam(out float3x3 R, out float3 t)
    {
        R = 0.0f;
        t = 0.0f;
            
        {
            float4 r = 0.0f;
            toRigidParam(r, t);
            R = Quaternion::toMat(r);
        }
    }
     
    void toRigidParam(out float4x4 M)
    {
        float3x3 R = 0.0f;
        float3 t = 0.0f;
        M = 0.0f;

        {
            toRigidParam(R, t);
            M.v4r0 = float4(R.v3r0, 0.0f);
            M.v4r1 = float4(R.v3r1, 0.0f);
            M.v4r2 = float4(R.v3r2, 0.0f);
            M.v4r3 = float4(t, 1.0f);
        }
    }
    
    //           
    void fromRigidParam(float4 r, float3 t)
    {
        real = r;
        dual = 0.5f * Quaternion::mul(r, float4(t, 0.0f));
    }
      
    void fromRigidParam(float3x3 R, float3 t)
    {
        float4 r = Quaternion::fromMat(R);
        fromRigidParam(r, t);
    }
       
    void fromRigidParam(float4x4 M)
    {
        float3x3 R = 0.0f;
        float3 t = 0.0f;
        R.v3r0 = M.v3r0;
        R.v3r1 = M.v3r1;
        R.v3r2 = M.v3r2;
        t = M.v3r3;
        fromRigidParam(R, t);
    }
    
    //    
    static DualQuaternion conjugate0(DualQuaternion q)
    {
        DualQuaternion p;
        p.real = float4(-q.real.xyz, +q.real.w);
        p.dual = float4(-q.dual.xyz, +q.dual.w);
            
        return p;
    }
        
    static DualQuaternion conjugate1(DualQuaternion q)
    {
        DualQuaternion p;
        p.real = float4(-q.real.xyz, +q.real.w);
        p.dual = float4(+q.dual.xyz, -q.dual.w);
        
        return p;
    }
    
    //       
    static DualQuaternion mul(DualQuaternion b, DualQuaternion a)
    {
        DualQuaternion p;

        p.real = Quaternion::mul(b.real, a.real);
        p.dual =
            Quaternion::mul(b.real, a.dual) +
            Quaternion::mul(b.dual, a.real);
                    
        return p;
    }
    
    static float3 transform(float3 v, DualQuaternion q)
    {
        float4 r;
        float3 t;
        q.toRigidParam(r, t);
        v = Quaternion::rotate(v, r) + t;
    
        return v;
    }
           
    static DualQuaternion smul(float s, DualQuaternion a)
    {
        DualQuaternion p;
    
        p.real = s * a.real;
        p.dual = s * a.dual;
        
        return p;
    }
           
    static DualQuaternion smul(DualQuaternion a, float s)
    {
        DualQuaternion p;
    
        p.real = a.real * s;
        p.dual = a.dual * s;
        
        return p;
    }
    
    //    
    //static float dot(DualQuaternion a, DualQuaternion b)
    //{
    //    return dot(a.real, b.real);
    //}
     
    static DualQuaternion Normalize(DualQuaternion q0)
    {
        DualQuaternion q1 = q0;

        float mag = dot(q0.real, q0.real);

        if (mag > 0.000001f)
        {
                //assert
        }
        q1.real *= 1.0f / mag;
        q1.dual *= 1.0f / mag;

        return q1;
    }
   
    static DualQuaternion Lerp(DualQuaternion a, DualQuaternion b, float dt)
    {
        DualQuaternion q;

        float3 ta;
        float4 ra;
        float3 tb;
        float4 rb;
        float3 t;
        float4 r;

        a.toRigidParam(ra, ta);
        b.toRigidParam(rb, tb);

        t = lerp(ta, tb, dt);
        r = Quaternion::slerp(ra, rb, dt);

        q.fromRigidParam(r, t);

        return q;
    }
    
    //   
    void toScrewParam(out float o, out float d, out float3 l, out float3 m)
    {
        o = 0.0f;
        d = 0.0f;
        l = 0.0f;
        m = 0.0f;

        {
            float i_s = 1.0f / length(real.xyz);

            o = +2.0f * acos(real.w);
            d = -2.0f * dual.w * i_s;
            l = real.xyz * i_s;
            m = (dual.xyz - l * d * real.w * 0.5f) * i_s;
        }
    }
    
    void fromScrewParam(float o, float d, float3 l, float3 m)
    {
        float o_2 = 0.5f * o;
        float d_2 = 0.5f * d;
        float sin = 0.0f;
        float cos = 0.0f;

        {
            sincos(o_2, sin, cos);

            real.xyz = sin * l;
            real.w = cos;

            dual.xyz = sin * m + d_2 * cos * l;
            dual.w = -d_2 * sin;
        }
    }
    
    //           
    static DualQuaternion scLerp(DualQuaternion a, DualQuaternion b, float t)
    {
        DualQuaternion q;
        DualQuaternion dq;
        float o = 0.0f;
        float d = 0.0f;
        float3 l = 0.0f;
        float3 m = 0.0f;
                   
        //DualQuaternion b = (dot(a.real, b.real) >= 0.0f ? b : DualQuaternion::smul((-1.0f), b));
        DualQuaternion _b = b;
        
        if (dot(a.real, b.real) < 0.0f)
        {
            _b = DualQuaternion::smul((-1.0f), b);
        }
        DualQuaternion _a = DualQuaternion::conjugate0(a);
        
        dq = DualQuaternion::mul(_b, _a);
        
        if (dq.real.w < 0.9f)
        {
            dq.toScrewParam(o, d, l, m);
            o = o * t;
            d = d * t;
            dq.fromScrewParam(o, d, l, m);
        
            q = DualQuaternion::mul(dq, a);
        }
        else
        {
            q = DualQuaternion::Lerp(a, b, t);
        }
                        
        return q;
    }
    
    static DualQuaternion scLerp_debug(DualQuaternion a, DualQuaternion b, float t, out float3 rp, out float3 rb, out float3 rl)
    {
        DualQuaternion q;
        DualQuaternion dq;
        float o = 0.0f;
        float d = 0.0f;
        float3 l = 0.0f;
        float3 m = 0.0f;
    
        //DualQuaternion b = (dot(a.real, b.real) >= 0.0f ? b : DualQuaternion::smul((-1.0f), b));
        DualQuaternion _b = b;
        if (dot(a.real, b.real) < 0.0f)
        {
            _b = DualQuaternion::smul((-1.0f), b);
        }
        DualQuaternion _a = DualQuaternion::conjugate0(a);
        
        dq = DualQuaternion::mul(_b, _a);
    
        if (dq.real.w < 1.0f)
        {
            dq.toScrewParam(o, d, l, m);
            o = o * t;
            d = d * t;
            dq.fromScrewParam(o, d, l, m);
    
            q = DualQuaternion::mul(dq, a);
        }
        else
        {
            q = DualQuaternion::Lerp(a, b, t);
        }
    
        //Debug
        float4 rot;
        float3 tb;
        float3 p;
        
        p = cross(l, m);
        dq.toRigidParam(rot, tb);
    
        rp = DualQuaternion::transform(p, a);
        rb = DualQuaternion::transform((tb - d * l), a);
        rl = DualQuaternion::transform((p + d * l), a);
    
        return q;
    }
    
   
    //Test
    void fromRigidParam0(float4 r, float3 t)
    {
        real = r;
        dual = 0.5f * Quaternion::mul0(float4(t, 0.0f), r);
    }
    
    static DualQuaternion mul0(DualQuaternion a, DualQuaternion b)
    {
        DualQuaternion p;

        p.real = Quaternion::mul0(a.real, b.real);
        p.dual =
            Quaternion::mul0(a.real, b.dual) +
            Quaternion::mul0(a.dual, b.real);
                    
        return p;
    }
    
    static float3 transform0(DualQuaternion q, float3 v)
    {
        float4 r;
        float3 t;
        q.toRigidParam(r, t);
        v = Quaternion::rotate0(r, v) + t;
    
        return v;
    }
    
    static DualQuaternion scLerp0(DualQuaternion a, DualQuaternion b, float t)
    {
        DualQuaternion q;
        DualQuaternion dq;
        float o = 0.0f;
        float d = 0.0f;
        float3 l = 0.0f;
        float3 m = 0.0f;
                   
        //DualQuaternion b = (dot(a.real, b.real) >= 0.0f ? b : DualQuaternion::smul((-1.0f), b));
        DualQuaternion _b = b;
        
        if (dot(a.real, b.real) < 0.0f)
        {
            _b = DualQuaternion::smul((-1.0f), b);
        }
        DualQuaternion _a = DualQuaternion::conjugate0(a);
        
        dq = DualQuaternion::mul0(_a, _b);
        
        if (dq.real.w < 0.9f)
        {
            dq.toScrewParam(o, d, l, m);
            o = o * t;
            d = d * t;
            dq.fromScrewParam(o, d, l, m);
        
            q = DualQuaternion::mul0(a, dq);
        }
        else
        {
            q = DualQuaternion::Lerp(a, b, t);
        }
                        
        return q;
    }
    
    static DualQuaternion scLerp_debug0(DualQuaternion a, DualQuaternion b, float t, out float3 rp, out float3 rb, out float3 rl)
    {
        DualQuaternion q;
        DualQuaternion dq;
        float o = 0.0f;
        float d = 0.0f;
        float3 l = 0.0f;
        float3 m = 0.0f;
    
        //DualQuaternion b = (dot(a.real, b.real) >= 0.0f ? b : DualQuaternion::smul((-1.0f), b));
        DualQuaternion _b = b;
        if (dot(a.real, b.real) < 0.0f)
        {
            _b = DualQuaternion::smul((-1.0f), b);
        }
        DualQuaternion _a = DualQuaternion::conjugate0(a);
        
        dq = DualQuaternion::mul0(_a, _b);
    
        if (dq.real.w < 1.0f)
        {
            dq.toScrewParam(o, d, l, m);
            o = o * t;
            d = d * t;
            dq.fromScrewParam(o, d, l, m);
    
            q = DualQuaternion::mul0(a, dq);
        }
        else
        {
            q = DualQuaternion::Lerp(a, b, t);
        }
    
        //Debug
        float4 rot;
        float3 tb;
        float3 p;
        
        p = cross(l, m);
        dq.toRigidParam(rot, tb);
    
        rp = DualQuaternion::transform0(a, p);
        rb = DualQuaternion::transform0(a, (tb - d * l));
        rl = DualQuaternion::transform0(a, (p + d * l));
    
        return q;
    }
    
};


struct Transform
{
    static float4x4 GetW(
		float3 t, float4 q, float3 s)
    {
        float3x3 R;
        float4x4 M;
 
#if defined(ROW_MAJOR)
    R = Quaternion::toMat_row(q);
    	      
    M.v4r0 = float4(s.x * R.v3r0, 0.0f);
    M.v4r1 = float4(s.y * R.v3r1, 0.0f);
    M.v4r2 = float4(s.z * R.v3r2, 0.0f);
    M.v4r3 = float4(t, 1.0f);
        
#elif defined(COLUMN_MAJOR)
    R = Quaternion::toMat_column(q);
        	      
    M.v4c0 = float4(s.x * R.v3c0, 0.0f);
    M.v4c1 = float4(s.y * R.v3c1, 0.0f);
    M.v4c2 = float4(s.z * R.v3c2, 0.0f);
    M.v4c3 = float4(t, 1.0f);       
#else

#endif		
        return M;
    }
       
};


struct Terrain
{
    static float GetHeight(uint2 id, Texture2D<float4> tex)
    {
        float h;
    
        h = tex[id.xy].w;
           
        return h;
    }

    static float3 GetNormal(uint2 id, Texture2D<float4> tex)
    {
        float3 n;
    
        n = tex[id.xy].xyz;
    
        return n;
    }
        
    static uint2 GetHMapPos(float3 _posW, float4x4 _T, float2 _t1_t0)
    {
        float2 posT;
    
        //float3 temp = mul(float4(_posW, 1.0f), _T).xyz;
        float3 temp = mul(_T, float4(_posW, 1.0f)).xyz;
        float2 p0 = temp.xz;
    
        posT = _t1_t0 * p0;
    
        return (uint2) posT;
    }

    static bool isOutNV(uint2 posT, Texture2D<float4> _alphaTex)
    {
        if (_alphaTex[posT].y != 0.0f)     
        {
            return true;
        }
    
        return false;
    }
    
    static bool isOutNV_Hole(uint2 posT, Texture2D<float4> _holeTex)
    {
        if (_holeTex[posT].w == 0.0f)
        {
            return true;
        }
    
        return false;
    }
    
    static float4 GetTerrainArea(uint2 posT, Texture2D<float4> _alphaTex)
    {
        float4 area = _alphaTex[posT];
        
        //if (area.x == 1.0f)
        //{
        //    area = float4(1.0f, 0.0f, 0.0f, 0.0f);
        //}
        //else if (area.y == 1.0f)
        //{
        //    area = float4(0.0f, 1.0f, 0.0f, 0.0f);
        //}
        //else if (area.z == 1.0f)
        //{
        //    area = float4(0.0f, 0.0f, 1.0f, 0.0f);
        //}
        //else if (area.w == 1.0f)
        //{
        //    area = float4(0.0f, 0.0f, 0.0f, 1.0f);
        //}
        
        return area;
    }

};


////
struct TestTexture
{
    Texture2D<float4> tex[4];
};

struct TestObject
{
    Texture2D<float4> dstTex[4];
    TestTexture tTex;
    void SetTexture(Texture2D<float4> srcTex[3])
    {
        dstTex[0] = srcTex[0];
        dstTex[1] = srcTex[1];
        dstTex[2] = srcTex[0];
        dstTex[3] = srcTex[0];
        
        tTex.tex[0] = dstTex[0];
        tTex.tex[1] = dstTex[1];
        tTex.tex[2] = dstTex[2];
        tTex.tex[3] = dstTex[3];
    }
};

#endif