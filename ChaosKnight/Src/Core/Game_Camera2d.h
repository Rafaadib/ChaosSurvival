
#pragma once


class Game_Camera2d {
public:
	Game_Camera2d(int camerawidth, int cameraheight);

	void CameraUpdate(Vector2 PositionTarget);

	void CameraBeginMode();
	void CameraEndMode();

	Vector2 GetWorldMouseposition();

private:
	Camera2D PlayerCamera;
	int CameraWidth;
	int CameraHeight;
};
