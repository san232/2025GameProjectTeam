#pragma once
#include "pch.h"

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
    XMFLOAT4 Color;
};

struct ConstantBuffer
{
    XMMATRIX Projection;
};

class SpriteRenderer
{
    DECLARE_SINGLE(SpriteRenderer);
public:
    bool Init();
    void CleanUp();

    void Begin(float renderWidth = 0.0f, float renderHeight = 0.0f);
    void End();

    // dx, dy, dw, dh = destination
    // sx, sy, sw, sh = source
    // texWidth, texHeight = full texture size
    void Draw(ID3D11ShaderResourceView* srv, float dx, float dy, float dw, float dh,
        float sx, float sy, float sw, float sh, float texWidth, float texHeight,
        bool flipX = false, XMFLOAT4 tintColor = XMFLOAT4(1, 1, 1, 1));
        
    // Draw rectangle (for colliders/debug)
    void DrawRect(float dx, float dy, float dw, float dh, XMFLOAT4 color);
    void DrawFilledRect(float dx, float dy, float dw, float dh, XMFLOAT4 color);

private:
    ComPtr<ID3D11VertexShader> m_vs;
    ComPtr<ID3D11PixelShader> m_ps;
    ComPtr<ID3D11InputLayout> m_layout;
    ComPtr<ID3D11Buffer> m_vb; // Dynamic vertex buffer
    ComPtr<ID3D11Buffer> m_cb;
    ComPtr<ID3D11SamplerState> m_sampler;
    
    ComPtr<ID3D11BlendState> m_blendState;
    ComPtr<ID3D11DepthStencilState> m_depthState;
    ComPtr<ID3D11RasterizerState> m_rasterState;

    ComPtr<ID3D11PixelShader> m_psColor; // For DrawRect
};
