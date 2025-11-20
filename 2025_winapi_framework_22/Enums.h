#pragma once
enum class Layer
{
	DEFAULT,
	BACKGROUND,
	// 밑에 변경 가능
	PLAYER,
	EVIDENCE,
	DEFAULTENEMY,
	INVISIBLEENEMY,
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