#include "pch.h"
#include "Texture.h"
#include "Core.h"

Texture::Texture()
	: m_width(0)
	, m_height(0)
{
}

Texture::~Texture()
{
}

void Texture::Load(const wstring& _filePath)
{
	HBITMAP hBit = (HBITMAP)::LoadImage(nullptr, _filePath.c_str(), IMAGE_BITMAP
		, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(hBit);
	
	BITMAP bitInfo;
	::GetObject(hBit, sizeof(BITMAP), &bitInfo);
	m_width = bitInfo.bmWidth;
	m_height = bitInfo.bmHeight;

	HDC hDC = ::GetDC(nullptr);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)::SelectObject(hMemDC, hBit);

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_width;
	bmi.bmiHeader.biHeight = -m_height; // top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	std::vector<DWORD> pixels(m_width * m_height);
	::GetDIBits(hMemDC, hBit, 0, m_height, pixels.data(), &bmi, DIB_RGB_COLORS);

	::SelectObject(hMemDC, hOldBit);
	::DeleteDC(hMemDC);
	::ReleaseDC(nullptr, hDC);
	::DeleteObject(hBit);

	for (size_t i = 0; i < pixels.size(); ++i)
	{
		BYTE b = (pixels[i] & 0x000000FF);
		BYTE g = (pixels[i] & 0x0000FF00) >> 8;
		BYTE r = (pixels[i] & 0x00FF0000) >> 16;
		
		if (r == 255 && g == 0 && b == 255)
		{
			pixels[i] = 0x00000000;
		}
		else
		{
			pixels[i] |= 0xFF000000;
		}
	}

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = pixels.data();
	initData.SysMemPitch = m_width * 4;

	ID3D11Device* device = GET_SINGLE(Core)->GetDevice();
	HRESULT hr = device->CreateTexture2D(&desc, &initData, &m_tex);
	assert(SUCCEEDED(hr));

	hr = device->CreateShaderResourceView(m_tex.Get(), nullptr, &m_srv);
	assert(SUCCEEDED(hr));
}
