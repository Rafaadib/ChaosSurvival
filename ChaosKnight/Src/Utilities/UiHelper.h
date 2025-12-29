
#pragma once


#include <raylib.h>

class UI {
public:
	//nbuat fungsi static cik
	static void DrawBarUI(Vector2 uipos, float current, float maxm, float scale, Texture2D filltex, Texture2D frametex) {
		float percentbar = current / maxm;

		//batasi agar tidak negatif
		if (percentbar < 0.0f) {
			percentbar = 0.0f;
		}
		if (percentbar > 1.0f) {
			percentbar = 1.0f;
		}

		DrawTextureEx(frametex, uipos, 0.0f, scale, WHITE);

		Rectangle SourcrecUI = {
			0.0f,
			0.0f,
			(float)filltex.width * percentbar,
			(float)filltex.height
		};

		Rectangle DestRecUI = {
			uipos.x,
			uipos.y,
			((float)filltex.width * percentbar) * scale,
			(float)filltex.height * scale

		};

		Vector2 OriginUI = { 0.0f, 0.0f };
		DrawTexturePro(filltex, SourcrecUI, DestRecUI, OriginUI, 0.0f, WHITE);
	}


};
