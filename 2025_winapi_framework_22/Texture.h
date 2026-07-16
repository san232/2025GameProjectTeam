#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
public:
    Texture();
    ~Texture();

public:
    LONG GetWidth() const { return m_width; }
    LONG GetHeight() const { return m_height; }
    ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

public:
    void Load(const wstring& _filePath);

private:
    ComPtr<ID3D11ShaderResourceView> m_srv;
    ComPtr<ID3D11Texture2D>          m_tex;
    LONG m_width;
    LONG m_height;
};

