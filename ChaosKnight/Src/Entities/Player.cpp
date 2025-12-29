
#include <raylib.h>
#include <raymath.h>

#include "Player.h"
#include "Utilities/nameSpaceAmba.h"

Character_2d::Character_2d(Texture2D Sprite, int framewidth, int frameheight, int framecount, float frametime)
	: AnimSprite(Sprite), FrameWidth(framewidth), FrameHeight(frameheight), FrameCount(framecount)  {
	
	float recX = 0.0f;
	float recY = 0.0f;

	this->Position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
	this->Speed = Atribut::movepeed;

	this->FrameTime = 1.0f / frametime;
	this->CurrentFrame = 0;
	this->Timer = 0.0f;

	this->SourceRec = { recX, recY, (float)FrameWidth, (float)FrameHeight };
	
}

void Character_2d::ControlCharacter() {
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		Position.x += Speed * GetFrameTime();
		if (SourceRec.width < 0) {
			SourceRec.width *= -1;
		}
	}

	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
		Position.x -= Speed * GetFrameTime();
		if (SourceRec.width > 0) {
			SourceRec.width *= -1;
		}
	}

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
		Position.y -= Speed * GetFrameTime();
	}

	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
		Position.y += Speed * GetFrameTime();
	}

}

//frame Animation
void Character_2d::AnimUpdate() {
	Timer += GetFrameTime();

	if (Hit_Timer > 0) {
		Hit_Timer -= GetFrameTime();
	}

	int StartFrame = 0;

	if (Timer >= FrameTime) {
		Timer = 0.0f;
		CurrentFrame++;

		if (CurrentFrame >= FrameCount) {
			CurrentFrame = StartFrame;
		}

		SourceRec.x = (float)CurrentFrame * FrameWidth;
	}
}

void Character_2d::TakeDamage(float damage) {
	if (Hit_Timer <= 0) {
		CurrentHealth -= damage;

		if (CurrentHealth < 0) {
			CurrentHealth = 0;
		}
		Hit_Timer = 1.0f;
	}
}

void Character_2d::AnimDraw(Vector2 SPosition, float Scale, Color textureDef) {
	Rectangle Desrec = {
		SPosition.x,
		SPosition.y,
		(float)FrameWidth * Scale, 
		(float)FrameHeight * Scale
	};


	Desrec.x = Position.x;
	Desrec.y = Position.y;

	float Rotation = 0.0f;
	Vector2 SOrigin = {
		(float)FrameWidth * Scale / 2.0f,
		(float)FrameHeight * Scale / 2.0f
	};

	DrawTexturePro(AnimSprite, SourceRec, Desrec, SOrigin, Rotation, textureDef);
}

void Character_2d::AnimReset() {
	CurrentFrame = 0;
	Timer = 0.0f;
	SourceRec.x = 0.0f;
}

Rectangle Character_2d::GetRect() {
	return { Position.x - 20, Position.y - 20, 56, 64 };
}

Vector2 Character_2d::DisplayPosition()	{

	return Position;
}
