
#pragma once

#include <raylib.h>


//membuat system bebasis ECS di gunakan untuk muush dan projectile yang spwan terus meneurs
// ini hanyalah entity (hanya objek tanpa nilai)
//ingat darah ket* (hanya object tanpa nilai)

using EntityID = int;

//core Component (core of the core)
struct  TransformComponent{
	Vector2 PositionComp;
	float RotationComp;
	float ScaleComp;

};

//Movement Component
struct VelocityComponent {
	Vector2 DirectionComp;
	float SpeedComp;
	bool IsMovingComp;
};

//rendering component
struct Sprite_2d {
	Texture2D TextureComp;
	Rectangle SourceRecComp;
	Vector2 OriginCommp;
	Color ColorComp;
	bool ActiveComp;

	//untuk frame wak
	bool isAnimated;
	int CurrentFrameComp;
	float FrameTimerComp;
	float FrameSpeedComp;
	float MaxFrameComp;
	float FrameWidthComp;
	float FrameHeightComp;
};

//component logic(mikir kids)
enum class TagComponentID {
	NONE = 0,
	PLAYER,
	ENEMY,
	PLAYER_PROJECTILE,
	XP,
	DMG_TEXT,
	GROWN_DECORATION
};


//duar nabark
struct  ColliderComponent{
	float RadiusComp;
	TagComponentID TagComp;
	bool Active;
	float Damage;
	
};

//kekebalan tubuh++
struct HealthComponent {
	float HealthComp;
	float CurrenHealthComp;
	float IFrameTimer;

};

//durasi hidup
struct LifeTimeComponent {
	float LifeTimeComp;
	float MaxLifeTimeComp;
	bool Expired;
};

//Specific component(ytta)
struct EnemyDataComponent {
	bool IsElite;
	float Xp_die;
};

//hitung xp dari musuh yang mati wak
struct XpComponent {
	int ValueXP;
};

//indikasi damage
struct TextDamage {
	int Damage;
	Color ColorDamage;
	float FontText;
};
