
#pragma once

class Character_2d {
public:
	Character_2d(Texture2D Sprite, int framewidth, int frameheight, int framecount, float frametime);

	//player control
	Vector2 Position;
	float Speed;
	void ControlCharacter();

	//animation sprite
	void AnimUpdate();
	void AnimDraw(Vector2 SPosition, float Scale, Color textureDef);
	inline void AnimReset();
	Rectangle SourceRec;

	//helath player
	float MaxHealth = 115;
	float CurrentHealth = 115;

	//playerxp
	int CurrentXp = 0;
	int Maxp = 100;

	//player stats
	float DamageMultiplier = 1.0f;
	int BulletCount = 1;
	int BulletBounce = 0;

	float Hit_Timer = 0.0f;
	inline bool IsVunerability() {
		return  Hit_Timer > 0;
	}

	void TakeDamage(float damage);
	inline Rectangle GetRect();

	//display position in the screen
	inline Vector2 DisplayPosition();

private:
	Texture2D AnimSprite;
	int FrameWidth;
	int FrameHeight;
	int FrameCount;
	float FrameTime;

	//reset
	int CurrentFrame;
	float Timer;

};
