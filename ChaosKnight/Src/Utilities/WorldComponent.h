
#pragma once

#include <vector>

// world ini hanya wadah le
//component nya
#include "RusdiTaboti_Compo.h"

//ini set max ennity wak
//bisa namabah tergantung mood ku:)
const EntityID MAX_ENTITY = 3000;

class WorldComp {
public:
	WorldComp() {
		transforms.reserve(MAX_ENTITY);
		velocity.reserve(MAX_ENTITY);
		sprite.reserve(MAX_ENTITY);
		collider.reserve(MAX_ENTITY);
		health.reserve(MAX_ENTITY);
		lifetime.reserve(MAX_ENTITY);
		enemydata.reserve(MAX_ENTITY);
		Xp.reserve(MAX_ENTITY);
		tdamage.reserve(MAX_ENTITY);

		//isi data kosong wak
		for (int i = 0; i < MAX_ENTITY; i++) {
			activeEntity.push_back(false);

			transforms.push_back({ 0 });
			velocity.push_back({ 0 });
			sprite.push_back({ 0 });
			collider.push_back({ 0 });
			health.push_back({ 0 });
			lifetime.push_back({ 0 });
			enemydata.push_back({ 0 });
			Xp.push_back({ 0 });
			tdamage.push_back({ 0 });
		}

	}

	//buat entity dulu le
	//paek inline juga karna fungsi kecil
	inline int CreateEntity() {
		for (int i = 0; i < MAX_ENTITY; i++) {
			if (!activeEntity[i]) {
				activeEntity[i] = true;

				ResetEntityData(i);

				return i; //balikan le
			}
		}

		return -1; //object dunia
	}

	//hancurkannnn
	inline void DestroyEntity(int id) {
		if (id >= 0 && id < MAX_ENTITY) {
			//timpa data
			activeEntity[id] = false; //kalau nimpa di pikir mas
		}
	}

	//reset kan dulu le 
	inline void Clear() {
		for (int i = 0; i < MAX_ENTITY; i++) {
			activeEntity[i] = false;
		}
	}



private:
	//component resetkan dulu le
	inline void ResetEntityData(int id) {
		transforms[id] = { 0 };
		velocity[id] = { 0 };
		sprite[id] = { 0 };
		collider[id] = { 0 };
		health[id] = { 0 };
		lifetime[id] = { 0 };
		enemydata[id] = { 0 };
		Xp[id] = { 0 };
		tdamage[id] = { 0 };
	}

public:
	//buat array entity vector dulu le
	std::vector<bool> activeEntity;

	std::vector<TransformComponent> transforms;
	std::vector<VelocityComponent> velocity;
	std::vector<Sprite_2d> sprite;
	std::vector<ColliderComponent> collider;
	std::vector<HealthComponent> health;
	std::vector<LifeTimeComponent> lifetime;

	//elite enemy
	std::vector<EnemyDataComponent> enemydata;

	//buat vector untuk xp
	std::vector<XpComponent> Xp;
	//damage juga di buat wak 
	std::vector<TextDamage> tdamage;
	

};