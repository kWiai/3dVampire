#pragma once
cbuffer global : register(b5)
{
    float4 gConst[32];
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


VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    float x1 = -0.98 + gConst[0].x;      // Фиксированная X-координата нижнего левого угла
    float x = -0.98;
    float y = 0.8;
    float y1 =  0.9;

    float x3 = -0.4;      // Фиксированная X-координата нижнего левого угла
    float x2 = -0.98;
    float y2 = 0.9;
    float y3 = 0.91;

    float x5 = -0.4;      // Фиксированная X-координата нижнего левого угла
    float x4 = -0.98;
    float y4 = 0.79;
    float y5 = 0.8;

    float x7 = -0.98;      // Фиксированная X-координата нижнего левого угла
    float x6 = -0.988;
    float y6 = 0.79;
    float y7 = 0.91;
    
    float x9 = -0.393;      // Фиксированная X-координата нижнего левого угла
    float x8 = -0.4;
    float y8 = 0.79;
    float y9 = 0.91;



    // Вершины квада (два треугольника)
    float2 quad[30] = {
        float2(x, y),  // Нижний левый
        float2(x, y1),  // Верхний левый
        float2(x1, y),  // Нижний правый

        float2(x1, y),  // Нижний правый (повтор)
        float2(x, y1),   // Верхний левый (повтор)
        float2(x1, y1), // Верхний правый

        float2(x2, y2),   // Нижний левый
        float2(x2, y3),  // Верхний левый
        float2(x3, y2),  // Нижний правый

        float2(x3, y2),  // Нижний правый (повтор)
        float2(x2, y3),   // Верхний левый (повтор)
        float2(x3, y3), // Верхний правый

        float2(x4, y4),   // Нижний левый
        float2(x4, y5),  // Верхний левый
        float2(x5, y4),  // Нижний правый

        float2(x5, y4),  // Нижний правый (повтор)
        float2(x4, y5),   // Верхний левый (повтор)
        float2(x5, y5), // Верхний правый

        float2(x6, y6),   // Нижний левый
        float2(x6, y7),  // Верхний левый
        float2(x7, y6),  // Нижний правый

        float2(x7, y6),  // Нижний правый (повтор)
        float2(x6, y7),   // Верхний левый (повтор)
        float2(x7, y7), // Верхний правый

        float2(x8, y8),   // Нижний левый
        float2(x8, y9),  // Верхний левый
        float2(x9, y8),  // Нижний правый

        float2(x9, y8),  // Нижний правый (повтор)
        float2(x8, y9),   // Верхний левый (повтор)
        float2(x9, y9) // Верхний правый
    };
    float4 viewPos = mul(float4(quad[vID], 0, 1.0f), view[0]);
    float4 projPos = mul(viewPos, proj[0]);
    output.wpos = float4(quad[vID], 0, 1.0f);
    output.pos = float4(quad[vID], 0, 1.0f);

    return output;
}