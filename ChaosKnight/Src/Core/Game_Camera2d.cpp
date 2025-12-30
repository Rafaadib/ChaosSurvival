

#include <raylib.h>
#include <raymath.h>

#include "Game_Camera2d.h"

Game_Camera2d::Game_Camera2d(int width, int height)
	: CameraWidth(width), CameraHeight(height) {

	PlayerCamera.target = { 0.0f, 0.0f };
	PlayerCamera.offset = { (float)width / 2.0f, (float)height / 2.0f };
	PlayerCamera.rotation = 0.0f;
	PlayerCamera.zoom = 1.0f;
}

//target ke player cik
void Game_Camera2d::CameraUpdate(Vector2 PositionTarget) {
	PlayerCamera.target = PositionTarget;
}

void Game_Camera2d::CameraBeginMode() {
	BeginMode2D(PlayerCamera);
}

void Game_Camera2d::CameraEndMode()	{
	EndMode2D();
}

Vector2 Game_Camera2d::GetWorldMouseposition() {
	
	return GetScreenToWorld2D(GetMousePosition(), PlayerCamera);
}
