
#pragma once

#include <memory>

#include "Manager/System_Ambalabu.h"
#include "Manager/Asset_Manager.h"
#include "Manager/Buff_Manager.h"

#define GROWN_COLOR  CLITERAL(Color){ 201, 181, 156, 255 }

//forward declaration
typedef enum GameScreen;

class Character_2d;
class Game_Camera2d;
class BuffLevel;


class Game {
public:
	Game(int screenWidth, int screenHeight, const char* title);
	~Game();

	void Run();
	
	Vector2 GetVirtualMousePotition();

	bool IsGameRunning = true;

private:
	void Update();
	void Draw();
	void LoadAsset();
	void UnloadAsset();

	void UpdateGameplay();
	void GameReset();

	void EnemySpawnLogic(float Delta_t);

	int screenWidth;
	int screenHeight;

	//resollusi wibu (virtual)
	//for screen
	RenderTexture2D ScreenTarget;
	Rectangle ScreenSourceRec;
	Rectangle ScreenDestRec;
	Vector2 ScreenoOrigin;

	float ScreenScale;

	const int VirtualWidth = 1126;
	const int VirtualHeight = 650;

	////musik kang
	Music Backsound = { 0 };
	/*Sound Buttonclick = { 0 };*/
	bool DrawButton(const char* text, Texture2D buttontexture, Rectangle recbutton, Vector2 mousebutton);


	//smart pointer (pointer cerdas)
	//hybrid architecture

	std::unique_ptr<Character_2d> PlayerKnight;
	std::unique_ptr<Game_Camera2d> CameraChar;
	/*Character_2d* PlayerKnight;*/
	/*Game_Camera2d* CameraChar;*/
	std::unique_ptr<BuffLevel> Menu;
	std::unique_ptr<WorldComp> GameWorld;

	//load asset 
	std::unique_ptr<Asset_Manager> GameAsset;

	//menu buff
	std::unique_ptr<Buff_Manager> buffmanager;
	std::vector<BuffDefinition> currentoptionbuff;

	GameScreen CurrentScreen;

	int Wavelevel;
	int Explevel;
	float AutoShootTimer;
	float SpawnTimer;

	int buffcount = 0;

	int Score = 0;

	float delta_t;
	const float PScale = 1.0f;

};