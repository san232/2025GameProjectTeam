#pragma once
// �ܺο��� ���� ����? X
// inst �θ� ������ �����Ǿ�� ��? �ƴϸ� ��ġ�Ǿ����? => �Ȱ��� ��
// �������� �θ� �� �־�ߵ�? => #include 
//#include "Defines.h"
//#include "Object.h"
class Core
{
	DECLARE_SINGLE(Core);
public:
	bool Init(HWND _hWnd);
	void GameLoop();
	void CleanUp();

	// DX11 Accessors
	ID3D11Device* GetDevice() const { return m_device.Get(); }
	ID3D11DeviceContext* GetContext() const { return m_context.Get(); }
	IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() const { return m_rtv.Get(); }
	const HWND& GetHwnd() const { return m_hWnd; }

private:
	void MainUpdate();
	void MainRender();

private:
	HWND m_hWnd;

	// DX11 Objects
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RenderTargetView> m_rtv;
};


//DECLARE_SINGLE(Core);
//private:
//	Core() {}
//public:
//	static Core* GetInst()
//	{
//		static Core inst;
//		return &inst;
//	}

//#include "bullet.h"
//class Player;
//class Core
//{
//private:
//	Core() {}
//public:
//	static Core* GetInst()
//	{
//		if (nullptr == m_pInst)
//			m_pInst = new Core;
//		return m_pInst;
//	}
//	static void DestroyInst()
//	{
//
//	}
//private:
//	Bullet m_bullet;
//	Player* m_player;
//	static Core* m_pInst;
//};
