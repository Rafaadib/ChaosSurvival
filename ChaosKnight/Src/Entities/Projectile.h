
#pragma once

//forward decalraction
class EnemyManager;
struct Enemy;

struct Projectile {
	Vector2 ProjectilePos;
	Vector2 ProjectileDir;

	float ProjectileSpeed;
	bool ProjectileActive;
	float ProjectileRadius;

	float ProjectileLifetime;
};


class ProjectileManager {
public:

	ProjectileManager();
	~ProjectileManager();

	void Fire(Vector2 StartPos, Vector2 TargetPos);

	void ProjectileUpdate(std::vector<Enemy>& enemies, float delta_t, int& Score);
	void ProjectileDraw();

	void ResetPorjectile();


private:
	std::vector<Projectile> Bullets;

};