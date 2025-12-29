
#pragma once

#include <raylib.h>
#include <memory>

#include "Utilities/WorldComponent.h"


class Character_2d;
class Asset_Manager;

//buat class game system
//class static loh ya
//mengapa static? kareana kita bisa panggill fungsi tanpa membuat objeknya dulu cik
//ini adalah systemnya atau logika nya

class GameSystem {
public:
	//system AI
	static void UpdateEnemyAI(WorldComp& World, Vector2 PlayerPos);

	//system gerak
	static void UpdateMovementSystem(WorldComp& World, float Delta_t);

	//system lifeteme (update gerak le) hapus peluru
	//aku utbuh prlelur
	static void UpdateLifetime(WorldComp& World, float Delta_t);

	//update tabarakan le
	static void UpdateCollison(WorldComp& World, Character_2d* Player, int& Score_ref, Texture2D gemTexture);

	//system mencari musuh terdekat untuk di temnbk
	static int GetNearestEnemy(WorldComp& World, Vector2 PlayerPos);

	//animasi wak
	static void UpdateAnimation(WorldComp& World, float Delta_t);

	//update player dapat xp atau buat magnet 
	static void UpdateMagnetXP(WorldComp& World, Vector2 PlayerPos, float Delta_t);

	//gamabrkan dulu le
	static void UpdateRenderSystem(WorldComp& World);

	//decoration
	static void UpdateDecoration(WorldComp& World, Vector2 PlayerPos, Asset_Manager* AssetDecoration);
	static void RenderDecoration(WorldComp& Wolrd);

private:
	//helper function
	//tuloongg
	static void SpawnXp(WorldComp& World, Vector2 PosiitonXp, int XpValue, Texture2D gemTexture);

	static void SpawnDamageText(WorldComp& World, Vector2 PositionDmg, int dmg, Color dcolor);


};
