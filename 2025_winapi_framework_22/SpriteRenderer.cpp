#include "pch.h"
#include "SpriteRenderer.h"
#include "Core.h"



bool SpriteRenderer::Init()
{
    ID3D11Device* device = GET_SINGLE(Core)->GetDevice();

    ComPtr<ID3DBlob> vsBlob, psBlob, psColorBlob, errorBlob;

    HRESULT hr = D3DCompileFromFile(L"Sprite.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob) OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        return false;
    }
    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vs);

    hr = D3DCompileFromFile(L"Sprite.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &psBlob, &errorBlob);
    if (FAILED(hr)) return false;
    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_ps);

    hr = D3DCompileFromFile(L"Sprite.hlsl", nullptr, nullptr, "PS_Color", "ps_5_0", 0, 0, &psColorBlob, &errorBlob);
    if (FAILED(hr)) return false;
    device->CreatePixelShader(psColorBlob->GetBufferPointer(), psColorBlob->GetBufferSize(), nullptr, &m_psColor);

    D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    device->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_layout);

    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.Usage = D3D11_USAGE_DYNAMIC;
    vbDesc.ByteWidth = sizeof(Vertex) * 5; 
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&vbDesc, nullptr, &m_vb);

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&cbDesc, nullptr, &m_cb);

    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    device->CreateSamplerState(&sampDesc, &m_sampler);

    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&blendDesc, &m_blendState);

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = FALSE;
    device->CreateDepthStencilState(&dsDesc, &m_depthState);

    D3D11_RASTERIZER_DESC rsDesc = {};
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.CullMode = D3D11_CULL_NONE; 
    device->CreateRasterizerState(&rsDesc, &m_rasterState);

    return true;
}

void SpriteRenderer::CleanUp()
{
}

void SpriteRenderer::Begin(float renderWidth, float renderHeight)
{
    auto context = GET_SINGLE(Core)->GetContext();

    context->IASetInputLayout(m_layout.Get());
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vb.GetAddressOf(), &stride, &offset);

    context->VSSetShader(m_vs.Get(), nullptr, 0);
    context->VSSetConstantBuffers(0, 1, m_cb.GetAddressOf());

    context->PSSetShader(m_ps.Get(), nullptr, 0);
    context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());

    context->OMSetBlendState(m_blendState.Get(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_depthState.Get(), 0);
    context->RSSetState(m_rasterState.Get());

    // Update Projection Matrix
    D3D11_MAPPED_SUBRESOURCE mappedCb;
    context->Map(m_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedCb);
    ConstantBuffer* cb = (ConstantBuffer*)mappedCb.pData;
    
    // Ortho projection for the current render target.
    float width = renderWidth;
    float height = renderHeight;
    if (width <= 0.0f || height <= 0.0f)
    {
        HWND hWnd = GET_SINGLE(Core)->GetHwnd();
        RECT rc;
        GetClientRect(hWnd, &rc);
        width = (float)(rc.right - rc.left);
        height = (float)(rc.bottom - rc.top);
    }
    if (width == 0) width = WINDOW_WIDTH;
    if (height == 0) height = WINDOW_HEIGHT;

    cb->Projection = XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(0, width, height, 0, 0.0f, 1.0f));
    context->Unmap(m_cb.Get(), 0);
}

void SpriteRenderer::End()
{
}

void SpriteRenderer::Draw(ID3D11ShaderResourceView* srv, float dx, float dy, float dw, float dh,
    float sx, float sy, float sw, float sh, float texWidth, float texHeight,
    bool flipX, XMFLOAT4 tintColor)
{
    auto context = GET_SINGLE(Core)->GetContext();
    
    context->PSSetShaderResources(0, 1, &srv);
    context->PSSetShader(m_ps.Get(), nullptr, 0);

    float u0 = sx / texWidth;
    float v0 = sy / texHeight;
    float u1 = (sx + sw) / texWidth;
    float v1 = (sy + sh) / texHeight;

    if (flipX)
    {
        float temp = u0;
        u0 = u1;
        u1 = temp;
    }

    D3D11_MAPPED_SUBRESOURCE mappedVb;
    context->Map(m_vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVb);
    Vertex* v = (Vertex*)mappedVb.pData;

    v[0] = { XMFLOAT3(dx, dy + dh, 0), XMFLOAT2(u0, v1), tintColor }; // BL
    v[1] = { XMFLOAT3(dx, dy, 0), XMFLOAT2(u0, v0), tintColor };      // TL
    v[2] = { XMFLOAT3(dx + dw, dy + dh, 0), XMFLOAT2(u1, v1), tintColor }; // BR
    v[3] = { XMFLOAT3(dx + dw, dy, 0), XMFLOAT2(u1, v0), tintColor };      // TR

    context->Unmap(m_vb.Get(), 0);

    context->Draw(4, 0);
}

void SpriteRenderer::DrawRect(float dx, float dy, float dw, float dh, XMFLOAT4 color)
{
    auto context = GET_SINGLE(Core)->GetContext();
    
    context->PSSetShader(m_psColor.Get(), nullptr, 0);

    D3D11_MAPPED_SUBRESOURCE mappedVb;
    context->Map(m_vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVb);
    Vertex* v = (Vertex*)mappedVb.pData;

    v[0] = { XMFLOAT3(dx, dy, 0), XMFLOAT2(0, 0), color };      // TL
    v[1] = { XMFLOAT3(dx + dw, dy, 0), XMFLOAT2(0, 0), color };      // TR
    v[2] = { XMFLOAT3(dx + dw, dy + dh, 0), XMFLOAT2(0, 0), color }; // BR
    v[3] = { XMFLOAT3(dx, dy + dh, 0), XMFLOAT2(0, 0), color }; // BL
    v[4] = { XMFLOAT3(dx, dy, 0), XMFLOAT2(0, 0), color };      // TL (Close loop)

    context->Unmap(m_vb.Get(), 0);

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    context->Draw(5, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void SpriteRenderer::DrawFilledRect(float dx, float dy, float dw, float dh, XMFLOAT4 color)
{
    auto context = GET_SINGLE(Core)->GetContext();

    context->PSSetShader(m_psColor.Get(), nullptr, 0);

    D3D11_MAPPED_SUBRESOURCE mappedVb;
    context->Map(m_vb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVb);
    Vertex* v = (Vertex*)mappedVb.pData;

    v[0] = { XMFLOAT3(dx, dy + dh, 0), XMFLOAT2(0, 0), color };
    v[1] = { XMFLOAT3(dx, dy, 0), XMFLOAT2(0, 0), color };
    v[2] = { XMFLOAT3(dx + dw, dy + dh, 0), XMFLOAT2(0, 0), color };
    v[3] = { XMFLOAT3(dx + dw, dy, 0), XMFLOAT2(0, 0), color };

    context->Unmap(m_vb.Get(), 0);
    context->Draw(4, 0);
}
