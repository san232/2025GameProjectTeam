#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
bool ResourceManager::Init()
{
	wchar_t buf[MAX_PATH] = {}; // windows �ִ� ��� ����
	::GetModuleFileNameW(nullptr, buf, MAX_PATH); // ���� �������� exe ��� buf�� ����   
	fs::path exeDir = fs::path(buf).parent_path();                //  buf ��ü ��θ� path ��ü�� ���� ���丮�� ����
	fs::path resourceDir = exeDir.parent_path() / L"build" / L"Resource\\"; // release����϶� build �ѹ��� ���̴°� ����
	m_resourcePath = resourceDir.native();

	RegisterTexture();
	RegisterGDI();

	FMOD::System_Create(&m_pSoundSystem); // �ý��� �����Լ�
	if (m_pSoundSystem != nullptr)
		m_pSoundSystem->init(64, FMOD_INIT_NORMAL, nullptr);

	RegisterSound();
	return true;
}

void ResourceManager::Release()
{
	// texture delete
	std::unordered_map<wstring, Texture*>::iterator iter;
	for (iter = m_mapTexture.begin(); iter != m_mapTexture.end(); ++iter)
		SAFE_DELETE(iter->second);
	m_mapTexture.clear();

	ReleaseGDI();
	ReleaseFonts();
	std::unordered_map<wstring, SoundInfo*>::iterator iterSound;
	for (iterSound = m_mapSounds.begin(); iterSound != m_mapSounds.end(); ++iterSound)
		SAFE_DELETE(iterSound->second);
	m_mapSounds.clear();

	m_pSoundSystem->close();
	m_pSoundSystem->release();
}

void ResourceManager::RegisterSound()
{

}

void ResourceManager::FmodUpdate()
{
	if (m_pSoundSystem)
		m_pSoundSystem->update();
}
void ResourceManager::LoadSound(const wstring& _key, const wstring& _path, bool _isLoop)
{
	if (FindSound(_key) || !m_pSoundSystem)
		return;
	wstring strFilePath = m_resourcePath;
	strFilePath += _path;

	// wstring to string
	std::string str;
	str.assign(strFilePath.begin(), strFilePath.end());

	// �������� ���� ����
	FMOD_MODE eMode = FMOD_LOOP_NORMAL; // �ݺ� ���
	if (!_isLoop)
		eMode = FMOD_DEFAULT; // ���� 1���� ���
	FMOD::Sound* p = nullptr;

	// BGM�� stream, �ƴϸ� sound
	// ���丮�Լ�
	//// ���� ��ü�� ����� ���� system��.
	//						//���ϰ��,  FMOD_MODE, NULL, &sound
	FMOD_RESULT r = _isLoop
		? m_pSoundSystem->createStream(str.c_str(), eMode, nullptr, &p)
		: m_pSoundSystem->createSound(str.c_str(), eMode, nullptr, &p);

	if (r != FMOD_OK || !p)
		return;

	SoundInfo* pSound = new SoundInfo;
	pSound->IsLoop = _isLoop;
	pSound->pSound = p;
	m_mapSounds.insert({ _key, pSound });

}

void ResourceManager::Play(const wstring& _key)
{
	SoundInfo* pSound = FindSound(_key);
	if (!pSound)
		return;
	SOUND_CHANNEL eChannel = SOUND_CHANNEL::BGM;
	if (!pSound->IsLoop)
		eChannel = SOUND_CHANNEL::EFFECT;
	// ���� ��� �Լ�. &channel�� � ä���� ���� ����Ǵ��� ������ �ѱ�
	m_pSoundSystem->playSound(pSound->pSound, nullptr, false, &m_pChannel[(UINT)eChannel]);

}

void ResourceManager::Stop(SOUND_CHANNEL _channel)
{
	m_pChannel[(UINT)_channel]->stop();

}

void ResourceManager::Volume(SOUND_CHANNEL _channel, float _vol)
{
	// 0.0 ~ 1.0 ���� ����
	m_pChannel[(UINT)_channel]->setVolume(_vol);

}

void ResourceManager::Pause(SOUND_CHANNEL _channel, bool _ispause)
{
	m_pChannel[(UINT)_channel]->setPaused(_ispause);
}
SoundInfo* ResourceManager::FindSound(const wstring& _key)
{
	std::unordered_map<wstring, SoundInfo*>::iterator iter = m_mapSounds.find(_key);

	if (iter == m_mapSounds.end())
		return nullptr;
	return iter->second;
}


void ResourceManager::RegisterGDI()
{
	// BRUSH
	// BRUSH
	m_Brushs[(UINT)BrushType::HOLLOW] = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	m_Brushs[(UINT)BrushType::RED] = (HBRUSH)::CreateSolidBrush(RGB(255, 0, 0));
	m_Brushs[(UINT)BrushType::GREEN] = (HBRUSH)::CreateSolidBrush(RGB(0, 255, 0));
	m_Brushs[(UINT)BrushType::BLUE] = (HBRUSH)::CreateSolidBrush(RGB(0, 0, 255));
	m_Brushs[(UINT)BrushType::WHITE] = (HBRUSH)::CreateSolidBrush(RGB(255, 255, 255));
	m_Brushs[(UINT)BrushType::BLACK] = (HBRUSH)::CreateSolidBrush(RGB(0, 0, 0));
	m_Brushs[(UINT)BrushType::YELLOW] = (HBRUSH)::CreateSolidBrush(RGB(255, 255, 0));
	m_Brushs[(UINT)BrushType::CYAN] = (HBRUSH)::CreateSolidBrush(RGB(0, 255, 255));
	m_Brushs[(UINT)BrushType::MAGENTA] = (HBRUSH)::CreateSolidBrush(RGB(255, 0, 255));
	m_Brushs[(UINT)BrushType::GRAY] = (HBRUSH)::CreateSolidBrush(RGB(128, 128, 128));
	m_Brushs[(UINT)BrushType::LIGHTGRAY] = (HBRUSH)::CreateSolidBrush(RGB(211, 211, 211));
	m_Brushs[(UINT)BrushType::ORANGE] = (HBRUSH)::CreateSolidBrush(RGB(255, 165, 0));
	m_Brushs[(UINT)BrushType::BROWN] = (HBRUSH)::CreateSolidBrush(RGB(165, 42, 42));
	m_Brushs[(UINT)BrushType::PURPLE] = (HBRUSH)::CreateSolidBrush(RGB(128, 0, 128));
	m_Brushs[(UINT)BrushType::VIOLET] = (HBRUSH)::CreateSolidBrush(RGB(238, 130, 238));
	m_Brushs[(UINT)BrushType::PINK] = (HBRUSH)::CreateSolidBrush(RGB(255, 192, 203));
	m_Brushs[(UINT)BrushType::LIGHTGREEN] = (HBRUSH)::CreateSolidBrush(RGB(61, 235, 61));

	// PEN 
	m_Pens[(UINT)PenType::RED] = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_Pens[(UINT)PenType::GREEN] = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_Pens[(UINT)PenType::BLUE] = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_Pens[(UINT)PenType::WHITE] = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	m_Pens[(UINT)PenType::BLACK] = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_Pens[(UINT)PenType::YELLOW] = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	m_Pens[(UINT)PenType::CYAN] = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	m_Pens[(UINT)PenType::MAGENTA] = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	m_Pens[(UINT)PenType::GRAY] = ::CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	m_Pens[(UINT)PenType::LIGHTGRAY] = ::CreatePen(PS_SOLID, 1, RGB(211, 211, 211));
	m_Pens[(UINT)PenType::ORANGE] = ::CreatePen(PS_SOLID, 1, RGB(255, 165, 0));
	m_Pens[(UINT)PenType::BROWN] = ::CreatePen(PS_SOLID, 1, RGB(165, 42, 42));
	m_Pens[(UINT)PenType::PURPLE] = ::CreatePen(PS_SOLID, 1, RGB(128, 0, 128));
	m_Pens[(UINT)PenType::VIOLET] = ::CreatePen(PS_SOLID, 1, RGB(238, 130, 238));
	m_Pens[(UINT)PenType::PINK] = ::CreatePen(PS_SOLID, 1, RGB(255, 192, 203));
	m_Pens[(UINT)PenType::LIGHTGREEN] = ::CreatePen(PS_SOLID, 1, RGB(173, 255, 47));


	// ��Ʈ ���
}

void ResourceManager::ReleaseGDI()
{
	for (int i = 0; i < (UINT)PenType::END; ++i)
		::DeleteObject(m_Pens[i]);
	for (int i = 1; i < (UINT)BrushType::END; ++i)
		// Hollow �����ϰ�
		::DeleteObject(m_Brushs[i]);
	for (int i = 0; i < (UINT)FontType::END; ++i)
		::DeleteObject(m_Fonts[i]);
}

bool ResourceManager::RegisterFontFile(const wstring& _path)
{
	wstring fontPath = m_resourcePath;
	fontPath += _path;
	if (!(AddFontResourceExW(fontPath.c_str(), FR_PRIVATE, 0) > 0))
		return false;
	m_vecFontFiles.push_back(fontPath);
	return true;
}

void ResourceManager::ReleaseFonts()
{
	for (const auto& path : m_vecFontFiles)
		::RemoveFontResourceExW(path.c_str(), FR_PRIVATE, 0);
	m_vecFontFiles.clear();
}

void ResourceManager::RegisterFont(FontType _type, const wstring& _name, int _height, int _weight, bool _italic, int _quality)
{

	HFONT h = ::CreateFont(_height, 0, 0, 0, _weight, _italic, false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, _quality, DEFAULT_PITCH || FF_DONTCARE, _name.c_str());
	m_Fonts[(UINT)_type] = h;
}


void ResourceManager::RegisterTexture()
{
	LoadTexture(L"Player", L"Texture\\Player.bmp");
	LoadTexture(L"PlayerBullet", L"Texture\\PlayerBullet.bmp");
	LoadTexture(L"BoomEnemy", L"Texture\\BoomEnemy.bmp");
	LoadTexture(L"Ghost", L"Texture\\Ghost.bmp");
	LoadTexture(L"Cat", L"Texture\\Cat.bmp");
	LoadTexture(L"Snail", L"Texture\\Snail.bmp");
	LoadTexture(L"SnailBullet", L"Texture\\SnailBullet.bmp");
	LoadTexture(L"CatBullet", L"Texture\\CatBullet.bmp");
	LoadTexture(L"Zombie", L"Texture\\Zombie.bmp");
	LoadTexture(L"Title", L"Texture\\TitleScene.bmp");
	LoadTexture(L"PlayerAttackEffect", L"Texture\\PlayerAttackEffect.bmp");
	LoadTexture(L"PlayerHitEffect", L"Texture\\PlayerHitEffect.bmp");
	LoadTexture(L"EnemyHitEffect", L"Texture\\EnemyHitEffect.bmp");
	LoadTexture(L"PlayerLevelUpEffect", L"Texture\\PlayerLevelUpEffect.bmp");
	LoadTexture(L"PlayerRollingEffect", L"Texture\\PlayerRollingEffect.bmp");
	LoadTexture(L"DashBoss", L"Texture\\DashBoss.bmp");
	LoadTexture(L"DashBossHitEffect", L"Texture\\DashBossHitEffect.bmp");
	LoadTexture(L"DashBossAttackEffect", L"Texture\\DashBossAttackEffect.bmp");
	LoadTexture(L"CatBulletDeadEffect", L"Texture\\CatBulletDeadEffect.bmp");
	LoadTexture(L"WizardBullet", L"Texture\\WizardBullet.bmp");
	LoadTexture(L"Wizard", L"Texture\\Wizard.bmp");
	LoadTexture(L"Conch", L"Texture\\Conch.bmp");
	LoadTexture(L"ConchBullet", L"Texture\\ConchBullet.bmp");
	LoadTexture(L"AttackPowerUpIcon", L"Texture\\AttackPowerUpIcon.bmp");
	LoadTexture(L"AttackSpeedUpIcon", L"Texture\\AttackSpeedUpIcon.bmp");
	LoadTexture(L"HpUpIcon", L"Texture\\HpUpIcon.bmp");
	LoadTexture(L"RollingCooltimeDownIcon", L"Texture\\RollingCooltimeDownIcon.bmp");
	LoadTexture(L"ConchBulletDeadEffect", L"Texture\\ConchBulletDeadEffect.bmp");
	LoadTexture(L"Necromancer", L"Texture\\Necromancer.bmp");
	LoadTexture(L"NecromancerBullet", L"Texture\\NecromancerBullet.bmp");
	LoadTexture(L"Slime", L"Texture\\Slime.bmp");
	LoadTexture(L"SlimeBullet", L"Texture\\SlimeBullet.bmp");
	LoadTexture(L"Map", L"Texture\\Map.bmp");
	LoadTexture(L"Worm", L"Texture\\Worm.bmp");
	LoadTexture(L"WormBullet", L"Texture\\WormBullet.bmp");
	LoadTexture(L"WormBulletDeadEffect", L"Texture\\WormBulletDeadEffect.bmp");
}

void ResourceManager::LoadTexture(const wstring& _key, const wstring& _path)
{
	Texture* pTex = GetTexture(_key);
	// ã������ ����
	if (nullptr != pTex)
		return;
	// ó���� �����Ŵ� ��� ã�Ƽ�
	wstring texPath = m_resourcePath;
	texPath += _path;

	// ����
	pTex = new Texture;
	pTex->Load(texPath); // �ؽ�ó ��ü �ε�
	pTex->SetKey(_key); // Ű ��� ����
	pTex->SetRelativePath(texPath);
	m_mapTexture.insert({ _key,pTex }); // �ʿ� ����
}

Texture* ResourceManager::GetTexture(const wstring& _key)
{
	auto iter = m_mapTexture.find(_key);
	if (iter != m_mapTexture.end())
		return iter->second;
	return nullptr;
}


