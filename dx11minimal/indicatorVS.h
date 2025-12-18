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
    uint instanceID : TEXCOORD1;
};
float3 rotZ(float3 pos, float a) {
    float3x3 m = {
        cos(a),-sin(a),0,
        sin(a),cos(a),0,
        0,0,1
    };
    pos = mul(pos, m);
    return pos;
}
float3 rotX(float3 pos, float a) {
    float3x3 m = {
        1,0,0,
        0,cos(a),-sin(a),
        0,sin(a),cos(a)
    };
    pos = mul(pos, m);
    return pos;
}
float3 rotY(float3 pos, float a) {
    float3x3 m = {
        cos(a),0,sin(a),
        0,1,0,
        -sin(a),0,cos(a)
    };
    pos = mul(pos, m);
    return pos;
}

VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    // Параметры куба из gConst[0]
    float centerX = gConst[0].x;      // X координата центра куба
    float centerZ = gConst[0].y;      // Z координата центра куба
    float centerY = gConst[0].z;      // Y координата центра (высота)
    float size = 0.15;         // Размер куба (масштаб)
    float angel = gConst[0].w*-1;

    float3 worldCenter = float3(centerX, centerY, centerZ);

    // Вершины куба (36 вершин)
    float3 baseVerts[36] = {
        // передняя сторона (Z = 0)
        float3(0, 0, -1.25), float3(0.5, 0, -1.25), float3(0, 0.5, -1.25),
        float3(0.5, 0, -1.25), float3(0.5, 0.5, -1.25), float3(0, 0.5, -1.25),

        // задняя сторона (Z = 1)
        float3(0, 0, 1.25), float3(0.5, 0, 1.25), float3(0, 0.5, 1.25),
        float3(0.5, 0, 1.25), float3(0.5, 0.5, 1.25), float3(0, 0.5, 1.25),

        // нижняя сторона (Y = 0)
        float3(0, 0, -1.25), float3(0.5, 0, -1.25), float3(0, 0, 1.25),
        float3(0.5, 0, -1.25), float3(0.5, 0, 1.25), float3(0, 0, 1.25),

        // верхняя сторона (Y = 1)
        float3(0, 0.5, -1.25), float3(0.5, 0.5, -1.25), float3(0, 0.5, 1.25),
        float3(0.5, 0.5, -1.25), float3(0.5, 0.5, 1.25), float3(0, 0.5, 1.25),

        // левая сторона (X = 0)
        float3(0, 0, -1.25), float3(0, 0, 1.25), float3(0, 0.5, -1.25),
        float3(0, 0, 1.25), float3(0, 0.5, 1.25), float3(0, 0.5, -1.25),

        // правая сторона (X = 1)
        float3(0.5, 0, -1.25), float3(0.5, 0, 1.25), float3(0.5, 0.5, -1.25),
        float3(0.5, 0, 1.25), float3(0.5, 0.5, 1.25), float3(0.5, 0.5, -1.25)
    };

    // ПРАВИЛЬНО: используем vID для доступа ко всем 36 вершинам
    float3 localPos = baseVerts[vID];
    localPos = rotY(localPos, angel);
    // Преобразуем из [0,1] в [-size, size] и смещаем к центру
    float3 scaledPos = (localPos - 0.5) * 2.0 * size;

    float3 worldPos = scaledPos + worldCenter;
    
    // Трансформации
    
    float4 pos = float4(worldPos, 1.0f);
    pos = mul(pos, mul(view[0], proj[0]));

    output.pos = pos;
    output.uv = float2(localPos.x, localPos.z);
    output.wpos = float4(worldPos, 1.0f);           // Мировая позиция
    output.vpos = mul(float4(worldPos, 1.0f), view[0]);  // Позиция в пространстве вида
    output.instanceID = vID / 6;  // ID грани (0-5) вместо ID вершины

    return output;
}