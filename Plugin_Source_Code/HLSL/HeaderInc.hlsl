
//#define ROW_MAJOR
//#define COLUMN_MAJOR

#if defined(ROW_MAJOR)
    #pragma pack_matrix(row_major)
#elif defined(COLUMN_MAJOR)
    #pragma pack_matrix(column_major)
#else

#endif

struct PerCamera
{
    float4x4 V;
    float4x4 C;
    float4x4 S;
    float4x4 CV;
    float4 dirW_view;
    float4 posW_view;
    float4 pixelSize;
    float4 data;
};

