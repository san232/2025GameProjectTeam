#pragma once
enum class Layer
{
	DEFAULT,
	BACKGROUND,
	// 밑에 변경 가능
	EVIDENCE,
	DEFAULTENEMY,
	CORE,
	PLAYER,
	VFX,
	PLAYERBULLET,
	ENEMYBULLET,
	DEFAULTEND,
	INVISIBLEENEMY,
	END
};

enum class OwnerType
{
	DEFAULT = 0,
	END
};

enum class PenType
{
	RED, GREEN, END
};

enum class BrushType
{
	HOLLOW, RED, GREEN, END
};

enum class FontType
{
	UI, TITLE, END
};

enum class PlayMode
{
	Once, Loop, Counted
};

enum class PlayerStateType 
{
	Idle, Move, Rolling, Dead
};

enum class EnemyStateType
{
	Idle, Move, Attack, Hit, Dead
};

enum class EffectType
{
	PLAYER_HIT, ENEMY_HIT, ENEMY_DEAD, END
};