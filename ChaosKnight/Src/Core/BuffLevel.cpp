
#include <raylib.h>

#include "BuffLevel.h"

BuffLevel::BuffLevel() {
	//kosong rek
}

int BuffLevel::UpdateAndChoice(Vector2 MousePosition, const std::vector<BuffDefinition>& optionbuff) {

	int startX = 1126 / 2 - 350;
	int spacing = 250;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		for (int i = 0; i < optionbuff.size(); i++) {
			int posX = startX + (i * spacing);
			Rectangle cardrect = { (float)posX, 180.0f, 220.0f, 320.0f };

			if (CheckCollisionPointRec(MousePosition, cardrect)) {
				return i + 1;
			}

		}
	}

	if (IsKeyPressed(KEY_ONE) && optionbuff.size() >= 1) {
		return 1;
	}
	if (IsKeyPressed(KEY_TWO) && optionbuff.size() >= 2) {
		return 2;
	}
	if (IsKeyPressed(KEY_THREE) && optionbuff.size()) {
		return 3;
	}

	return 0;
}

void BuffLevel::DrawBuffLevel(int screenwidth, int screenheight, const std::vector<BuffDefinition>& optionbuff, Asset_Manager* AsssetBuff) {

	DrawRectangle(0, 0, screenwidth, screenheight, Fade(BLACK, 0.7));
	DrawText("LEVEL UP! CHOSE BUFF", screenwidth / 2 - 150, 80, 30, GOLD);

	int startposbuffX = screenwidth / 2 - 350;
	int spacingbuff = 250;


	//loping asset buff
	for (int i = 0; i < optionbuff.size(); i++) {
		int postX = startposbuffX + (i * spacingbuff);
		int postY = 180;

		Texture2D CurrentCardAsset = AsssetBuff->GetTexture(optionbuff[i].keyvalue);

		//biar card buff nya gak gepeng
		float cardratio = (float)CurrentCardAsset.width / CurrentCardAsset.height;
		float cardwitdh = 300.0f;
		float cardheight = cardwitdh * cardratio;

		Rectangle destcard = { (float)postX, (float)postY, cardwitdh, cardheight };
		DrawTexturePro(CurrentCardAsset, { 0, 0, (float)CurrentCardAsset.width, (float)CurrentCardAsset.height }, destcard, { 0, 0 }, 0.0f, WHITE);

		//beri judul buff nya le
		int titlewidth = MeasureText(optionbuff[i].namebuff.c_str(), 20);
		int centertext = postX + (cardwitdh / 2) - (titlewidth / 2);
		DrawText(optionbuff[i].namebuff.c_str(), centertext, postY + 30, 20, BLACK);

		/*const char* descText = "Effect Active";
		int descwidth = MeasureText(descText, 15);
		int centerdescX = postX + (cardwitdh / 2) - (descwidth / 2);

		DrawText(descText, centerdescX, postY + 210, 15, DARKGRAY);*/

		//angkan buff
		DrawText(TextFormat("[%d]", i + 1), postX + (cardwitdh / 2) - 10, postY + cardheight  - 30 , 20, BLACK);
	}
}
