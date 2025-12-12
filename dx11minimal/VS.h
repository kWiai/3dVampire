cbuffer global : register(b5)
{
    float4 gConst[4096];
};

cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
};

cbuffer camera : register(b3)
{
    float4x4 world[2];
    float4x4 view[2];
    float4x4 proj[2];
};

cbuffer drawMat : register(b2)
{
    float4x4 model;
    float hilight;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float4 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

float3 rotY(float3 pos, float a)
{
    float3x3 m =
    {
        cos(a), 0, sin(a),
        0, 1, 0,
        -sin(a), 0, cos(a)
    };
    pos = mul(pos, m);
    return pos;
}

VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    // Параметры сетки
    float gridSize = gConst[0].x;        // Размер сетки (например, 10x10)
    float spacing = gConst[0].y;         // Расстояние между квадратами
    float height = gConst[0].z;          // Высота (Y координата)

    // 1. Определяем квадрат и вершину
    int quadIndex = vID / 6;
    int vertexInQuad = vID % 6;

    // 2. Базовые вершины квадрата в 3D (XZ плоскость, Y=0)
    float3 baseVerts[6] = {
        float3(0, 0, 0), float3(1, 0, 0), float3(0, 0, 1),  // Треугольник 1
        float3(1, 0, 0), float3(1, 0, 1), float3(0, 0, 1)   // Треугольник 2
    };

    float3 localPos = baseVerts[vertexInQuad];

    // 3. Позиция в сетке
    int gridX = quadIndex % (int)gridSize;
    int gridZ = quadIndex / (int)gridSize;

    // 4. Мировые координаты в 3D
    float worldX = (gridX + localPos.x) * spacing - (gridSize * spacing) / 2.0f;
    float worldY = height;                                    // Y координата
    float worldZ = (gridZ + localPos.z) * spacing - (gridSize * spacing) / 2.0f;

    float4 pos = float4(worldX, worldY, worldZ, 1.0f);

    output.pos = mul(pos, mul(view[0], proj[0]));
    output.uv = float2(localPos.x, localPos.z);  // UV по X и Z

    return output;
}