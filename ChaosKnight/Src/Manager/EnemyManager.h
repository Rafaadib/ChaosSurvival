
#pragma once

enum class EnemyType {
	NORMAL,
	MINIELITE
};

struct Enemy {
	Vector2 EnemyPosition;
	float EnemySpeed;
	bool EnemyActive;

	float EnemyHealth;
	float EnemyMaxHealth;
	float EnemyDamage;

	EnemyType Type;
	float EnemyRadius;

	Rectangle GetRectangle();
};

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void EnemyUpdate(Vector2 PlayerPos, float delta_t);
	void EnemyDraw();

	void SpawnWaves(int Wavelevel, Vector2 PlayerPos);

	std::vector<Enemy>& GetEnemy();
	Enemy* GetNearestEnemy(Vector2 PlayerPos);

	void ResetEnemy();

private:
	std::vector<Enemy> enemy;
	float Spawntimer;

};