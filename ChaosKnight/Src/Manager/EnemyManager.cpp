

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "EnemyManager.h"

Rectangle Enemy::GetRectangle()	{
	return { EnemyPosition.x - EnemyRadius, EnemyPosition.y - EnemyRadius, EnemyRadius * 2, EnemyRadius * 2};
}

EnemyManager::EnemyManager() {
	Spawntimer = 0.0f;
}

//clear enemy in memory
EnemyManager::~EnemyManager() {
	enemy.clear();
}

void EnemyManager::EnemyUpdate(Vector2 PlayerPos, float delta_t) {
	for (auto& e : enemy) {
		//enemy dies continue
		if (!e.EnemyActive) continue;

		//follow player
		Vector2 dir = Vector2Subtract(PlayerPos, e.EnemyPosition);
		dir = Vector2Normalize(dir);
		e.EnemyPosition = Vector2Add(e.EnemyPosition, Vector2Scale(dir, e.EnemySpeed * delta_t));
	}
}

void EnemyManager::EnemyDraw() {
	for (auto& e : enemy) {
		if (e.EnemyActive) {
			Color color = (e.Type == EnemyType::MINIELITE) ? RED : DARKGREEN;
			DrawRectangleRec(e.GetRectangle(), RED);

			float EHpercent = e.EnemyHealth - e.EnemyMaxHealth;
			DrawRectangle(e.EnemyPosition.x - 15, e.EnemyPosition.y - 30, 30 * EHpercent, 5, GREEN);
		}
	}
}

void EnemyManager::SpawnWaves(int Wavelevel, Vector2 PlayerPos) {
	Spawntimer += GetFrameTime();


	float Spawrate = 0.8f / (Wavelevel * 0.5f);
	if (Spawrate < 0.1f) {
		Spawrate = 0.1f;
	}
	//minimum

	//spawn enemies every 1 second / wavelevel
	if (Spawntimer >= Spawrate) {
		Spawntimer = 0.0f;

		Enemy newE;

		//spawn random 400 - 500
		float angle = GetRandomValue(0, 360) * DEG2RAD;
		float distance = GetRandomValue(400, 500);

		//use trigonometry for update enemy  position
		newE.EnemyPosition.x = PlayerPos.x + cos(angle) * distance;
		newE.EnemyPosition.y = PlayerPos.y + sin(angle) * distance;

		int percentEnemyspawn = 10;

		//elite enemy
		if (GetRandomValue(0, 100) <= percentEnemyspawn) {
			newE.Type = EnemyType::MINIELITE;
			newE.EnemyMaxHealth = 25.0f + (Wavelevel * 10);
			newE.EnemySpeed = 80.0f;
			newE.EnemyRadius = 25.0f;
			newE.EnemyDamage = 4.0f;
		}
		else {
			newE.Type = EnemyType::NORMAL;
			newE.EnemyMaxHealth = 15.0f + (Wavelevel * 2);
			newE.EnemySpeed = 100.0f + (Wavelevel * 5);
			newE.EnemyRadius = 15.0f;
			newE.EnemyDamage = 2.0f;
		}

		newE.EnemyHealth = newE.EnemyMaxHealth;
		newE.EnemyActive = true;
		newE.EnemyPosition = {newE.EnemyPosition.x, newE.EnemyPosition.y};

		//add vector member
		enemy.push_back(newE);

	}
}

std::vector<Enemy>& EnemyManager::GetEnemy() {
	return enemy;
}


Enemy* EnemyManager::GetNearestEnemy(Vector2 PlayerPos) {
	Enemy* nearest = nullptr;

	float mindistance = 999999.0f;

	for (auto& e : enemy) {
		if (!e.EnemyActive) continue;

		float dst = Vector2Distance(PlayerPos, e.EnemyPosition);
		if (dst < mindistance) {
			mindistance = dst;
			nearest = &e;
		}
	}

	return nearest;

}

void EnemyManager::ResetEnemy() {
	enemy.clear();
	Spawntimer = 0.0f;
}
