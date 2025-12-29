
#include <raylib.h>
#include <raymath.h>
#include <vector>


#include "Projectile.h"
#include "Manager/EnemyManager.h"

ProjectileManager::ProjectileManager() {

}

ProjectileManager::~ProjectileManager() {
	Bullets.clear();
}

void ProjectileManager::Fire(Vector2 StartPos, Vector2 TargetPos) {
	Projectile PBullet;
	PBullet.ProjectilePos = StartPos;
	PBullet.ProjectileSpeed = 670.0f;
	PBullet.ProjectileRadius = 5.0f;
	PBullet.ProjectileActive = true;

	PBullet.ProjectileLifetime = 0.0f;

	Vector2 Pdir = Vector2Subtract(TargetPos, StartPos);
	PBullet.ProjectileDir = Vector2Normalize(Pdir);

	Bullets.push_back(PBullet);
}

void ProjectileManager::ProjectileUpdate(std::vector<Enemy>& enemies, float delta_t, int& Score) {
	for (auto& p : Bullets) {
		if (!p.ProjectileActive) continue;

		//bullet move
		p.ProjectilePos = Vector2Add(p.ProjectilePos, Vector2Scale(p.ProjectileDir, p.ProjectileSpeed * delta_t));

		p.ProjectileLifetime += delta_t;
		if (p.ProjectileLifetime > 2.0f) {
			p.ProjectileActive = false;
		}

		for (auto& e : enemies) {
			if (!e.EnemyActive) continue;

			//colison detection for enemy
			if (CheckCollisionCircleRec(p.ProjectilePos, p.ProjectileRadius, e.GetRectangle())) {
				p.ProjectileActive = false;

				//player damage
				e.EnemyHealth -= 18.0f;

				if (e.EnemyHealth <= 0) {
					e.EnemyActive = false;
					Score += 1;
				}
				break;
			}
		}
		/*Bullets.clear();*/
	}
}

void ProjectileManager::ProjectileDraw() {
	for (auto& p : Bullets) {
		if (p.ProjectileActive) {
			DrawCircleV(p.ProjectilePos, p.ProjectileRadius, ORANGE);
		}
	}
}

void ProjectileManager::ResetPorjectile() {
	Bullets.clear();
}