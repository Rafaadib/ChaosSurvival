
#include "System_Ambalabu.h"
#include "Entities/Player.h"
#include "Utilities/nameSpaceAmba.h"
#include "Manager/Asset_Manager.h"

#include <raymath.h>

//ini adalah systemnya atau logika nya

//logika kejar player
//walah cik
void GameSystem::UpdateEnemyAI(WorldComp& World, Vector2 PlayerPos) {
	//loping semua entity
	for (int i = 0; i < MAX_ENTITY; i++) {
		//skip skip
		if (!World.activeEntity[i]) {
			continue;
		}

		if (World.collider[i].TagComp != TagComponentID::ENEMY) {
			continue;
		}

		//akses kan alamat le&
		TransformComponent& transformcomp = World.transforms[i];
		VelocityComponent& velocitycomp = World.velocity[i];

		//lojik arah ke player
		//mikir kids
		Vector2 direction = Vector2Subtract(PlayerPos, transformcomp.PositionComp);

		//normalisasaikan dulu le
		velocitycomp.DirectionComp = Vector2Normalize(direction);

		//jalan le
		velocitycomp.IsMovingComp = true;

	}
}

//system matematika dan fisika
void GameSystem::UpdateMovementSystem(WorldComp& World, float Delta_t) {
	//loping lagi le
	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		if (!World.velocity[i].IsMovingComp) {
			continue;
		}
		
		//akses alamat&
		TransformComponent& tcomp = World.transforms[i];
		VelocityComponent& vcomp = World.velocity[i];

		// formula nya le: posisi baru = posisi lama + (Arah, Speed * Waktu)
		tcomp.PositionComp = Vector2Add(tcomp.PositionComp, Vector2Scale(vcomp.DirectionComp, vcomp.SpeedComp * Delta_t));
	}
}


void GameSystem::UpdateLifetime(WorldComp& World, float Delta_t) {
	//loping lagi wak
	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		
		//cek apakah entitas punya batas hidup (lifetime kurang dari  0)
		if (World.lifetime[i].MaxLifeTimeComp <= 0.0f) {
			continue;
		}

		//akses alamat lagi&
		LifeTimeComponent& lcomp = World.lifetime[i];
		lcomp.LifeTimeComp += Delta_t;

		if (lcomp.LifeTimeComp >= lcomp.MaxLifeTimeComp) {
			//hapus entity
			World.DestroyEntity(i); 
		}

	}
}

void GameSystem::UpdateCollison(WorldComp& World, Character_2d* Player, int& kill_ref, Texture2D gemTexture) {
	//loping cari projectile / deteksi proectile
	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		if (!World.collider[i].Active) {
			continue;
		}

		TagComponentID tagA = World.collider[i].TagComp;

		//loating terus
		if (tagA == TagComponentID::XP) {
			//cek jarak gem ke player
			float playerRadius = 20.0f;
			if (CheckCollisionCircles(World.transforms[i].PositionComp, World.collider[i].RadiusComp, Player->Position, playerRadius)) {
				//tambah xp player
				int xpAmount = 7;
				Player->CurrentXp += xpAmount;

				//hapus dulu gem nya biar ga numpuk di memory
				World.DestroyEntity(i);
			}
			continue;
		}

		//enemy vs player
		if (tagA == TagComponentID::ENEMY) {
			float radiusenemy = 20.0f;
			if (CheckCollisionCircles(World.transforms[i].PositionComp, World.collider[i].RadiusComp, Player->Position, radiusenemy)) {
				Player->TakeDamage(Atribut::enemydamage);
			}
			//jangan continue
			//continue terus
		}

		//bullet vs enemmy
		if (tagA == TagComponentID::PLAYER_PROJECTILE) {
			//nested loop
			for (int j = 0; j < MAX_ENTITY; j++) {
				if (!World.activeEntity[j]) {
					continue;
				}
				if (World.collider[j].TagComp != TagComponentID::ENEMY) {
					continue;
				}

				//cek tbrakan 
				//playeer i musuh j
				float distance = Vector2Distance(World.transforms[i].PositionComp, World.transforms[j].PositionComp);
				float radiusSum = World.collider[i].RadiusComp + World.collider[j].RadiusComp;

				if (distance < radiusSum) {
					//kena hit wak

					//hapus porjecktile
					World.DestroyEntity(i);

					World.health[j].CurrenHealthComp -= Atribut::damage;
					World.health[j].IFrameTimer = 0.15f;
					World.sprite[j].ColorComp = RED;

					SpawnDamageText(World, World.transforms[j].PositionComp, (float)Atribut::damage, RED );


					//cek muush nya wak
					if (World.health[j].CurrenHealthComp <= 0) {
						Vector2 deathPos = World.transforms[j].PositionComp;

						World.DestroyEntity(j);

						kill_ref += 1;
						SpawnXp(World, deathPos, 3, gemTexture);
					}
					//break dulu wak capek
					break;
				}

			}
		}
	}
}

//untuk mendetkesi musuh terdekat wak
int GameSystem::GetNearestEnemy(WorldComp& World, Vector2 PlayerPos) {
	int nearestID = -1;
	float MidDistance = 999999.0f;

	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		if (World.collider[i].TagComp != TagComponentID::ENEMY) {
			continue;
		}

		float dst = Vector2Distance(PlayerPos, World.transforms[i].PositionComp);
		if (dst < MidDistance) {
			MidDistance = dst;
			nearestID = i;
		}
	}

	return nearestID;
}



void GameSystem::UpdateAnimation(WorldComp& World, float Delta_t) {
	//loop
	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		if (!World.sprite[i].ActiveComp) {
			continue;
		}

		if (World.velocity[i].IsMovingComp) {
			float directionx = World.velocity[i].DirectionComp.x;
			float currentwidth = World.sprite[i].SourceRecComp.width;

			//flip ke kanan
			if (directionx < 0 && currentwidth < 0) {
				World.sprite[i].SourceRecComp.width *= -1;
			}//normalakan le
			else if (directionx > 0 && currentwidth > 0) {
				World.sprite[i].SourceRecComp.width *= -1;
			}
		}


		if (!World.sprite[i].isAnimated) {
			continue;
		}

		//akses alamt lagi&
		Sprite_2d& s = World.sprite[i];

		//timer logika wak
		s.FrameTimerComp += Delta_t;
		if (s.FrameTimerComp >= s.FrameSpeedComp) {
			s.FrameTimerComp = 0.0f;
			s.CurrentFrameComp++;

			//loop animasi musuh
			if (s.CurrentFrameComp >= s.MaxFrameComp) {
				s.CurrentFrameComp = 0;
			}

			s.SourceRecComp.x = (float)s.CurrentFrameComp * fabsf(s.FrameWidthComp);
		}


	}
}

void GameSystem::UpdateMagnetXP(WorldComp& World, Vector2 PlayerPos, float Delta_t) {
	float magnetradius = 150.0f;
	float magnetspeed = 400.0f;

	//loop lagi (jir lah)
	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		if (World.collider[i].TagComp != TagComponentID::XP) {
			continue;
		}

		//akses alamat lagi wak& woilah cik
		TransformComponent& t = World.transforms[i];
		VelocityComponent& v = World.velocity[i];

		//cek jarak ke player
		float distance = Vector2Distance(PlayerPos, t.PositionComp);

		if (distance < magnetradius) {
			//hiutng arah ke player
			Vector2 direction = Vector2Subtract(PlayerPos, t.PositionComp);
			v.DirectionComp = Vector2Normalize(direction);
			v.SpeedComp = magnetspeed;
			
			v.IsMovingComp = true;
		}
		else {
			//gem/xp diam kalau jauh dari player
			v.IsMovingComp = false;
		}

	}
}

void GameSystem::UpdateDecoration(WorldComp& World, Vector2 PlayerPos, Asset_Manager* AssetDecoration) {
	const int max_decoration = 15; //max decoration
	const float spawn_radiusmin = 500.0f;
	const float spawn_radiusmax = 800.0f;
	const float despawn = 1000.0f;

	int currentdeco = 0;

	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		if (World.collider[i].TagComp != TagComponentID::GROWN_DECORATION) {
			continue;
		}

		currentdeco++;

		//hitung jarak ke player
		float distdeco = Vector2Distance(PlayerPos, World.transforms[i].PositionComp);

		//hapus jika player menjauh cik
		if (distdeco > despawn) {
			World.DestroyEntity(i);
			currentdeco--;
		}
	}

	//spawn decorasi baru jika kurang dari plaeyr
	if (currentdeco < max_decoration) {
		int decoration = World.CreateEntity();
		if (decoration == -1) {
			return;
		}

		//gacha pososiii (aku menugnakan donut renderring)
		float d_angle = GetRandomValue(0, 360); // lingkaran
		float d_dist = GetRandomValue((int)spawn_radiusmin, (int)spawn_radiusmax);

		Vector2 spawnpos = {
			PlayerPos.x + cos(d_angle) * d_dist,
			PlayerPos.y + sin(d_angle) * d_dist
		};

		World.transforms[decoration].PositionComp = spawnpos;

		//bair random
		World.transforms[decoration].ScaleComp = (float)GetRandomValue(2, 3);
		/*World.transforms[decoration].RotationComp = (float)GetRandomValue(0, 360);*/

		//gacha lagi
		Texture2D atlasT = AssetDecoration->GetTexture("PropA");

		//tentukan sesuian jumalamh asset decorasinya
		float atlwidth = atlasT.width / 7.0f;
		float atlheight = (float)atlasT.height;

		int frameindext = GetRandomValue(0, 6);

		//setup component
		World.sprite[decoration].TextureComp = atlasT;
		World.sprite[decoration].ActiveComp = true;
		
		//variasi warna
		/*int shadeco = GetRandomValue(200, 500);*/
		World.sprite[decoration].ColorComp = WHITE;

		//jawa adalah koenci
		//ini memtong aset atlas
		World.sprite[decoration].SourceRecComp = {
			(float)frameindext * atlwidth,
			0.0f,
			atlwidth,
			atlheight
		};

		World.sprite[decoration].OriginCommp = {
			atlwidth / 2,
			atlheight / 2
		};

		World.collider[decoration].TagComp = TagComponentID::GROWN_DECORATION;
		World.collider[decoration].Active = false;



	}
}

void GameSystem::SpawnXp(WorldComp& World, Vector2 PositionXp, int XpValue, Texture2D gemTexture) {
	int id = World.CreateEntity();
	if (id == -1) {
		return;
	}

	//set posisii msuush mati 
	World.transforms[id].PositionComp = PositionXp;
	World.transforms[id].ScaleComp = 1.5f;
	World.transforms[id].RotationComp = 0.0f;

	//set gem
	World.sprite[id].ActiveComp = true;
	World.sprite[id].ColorComp = WHITE;

	World.sprite[id].TextureComp = gemTexture;
	World.sprite[id].SourceRecComp = { 0, 0, (float)gemTexture.width, (float)gemTexture.height};
	World.sprite[id].OriginCommp = { (float)gemTexture.width / 2, (float)gemTexture.height / 2};

	//fisika wak tag id
	World.collider[id].Active = true;
	World.collider[id].RadiusComp = 10.0f;
	World.collider[id].TagComp = TagComponentID::XP;

	//set xp
	World.Xp[id].ValueXP = XpValue;

	//seting velocity suapa bisa kesedot wak
	World.velocity[id].SpeedComp = 0;
	World.velocity[id].IsMovingComp = false;

}

void GameSystem::SpawnDamageText(WorldComp& World, Vector2 Positiondmg, int dmg, Color dcolor) {
	int damageid = World.CreateEntity();
	if (damageid == -1) {
		return;
	}

	//20 ini menyesualikan ya wak
	//damage di atas musuh
	World.transforms[damageid].PositionComp = { Positiondmg.x, Positiondmg.y - 20 };

	//efek muncul
	World.velocity[damageid].IsMovingComp = true;
	World.velocity[damageid].DirectionComp = { 0.0f, -1.0f };
	World.velocity[damageid].SpeedComp = 60.0f; //pelan pelan pak supir

	//logik
	World.tdamage[damageid].Damage = dmg;
	World.tdamage[damageid].ColorDamage = dcolor;
	World.tdamage[damageid].FontText = 18.0f;

	//lifetime
	World.collider[damageid].TagComp = TagComponentID::DMG_TEXT;
	World.collider[damageid].Active = false;

	World.lifetime[damageid].MaxLifeTimeComp = 0.8f;


}

//render decorasi
void GameSystem::RenderDecoration(WorldComp& World) {
	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}
		if (World.collider[i].TagComp != TagComponentID::GROWN_DECORATION) {
			continue;
		}

		//akses alamt ddari transform struct
		TransformComponent& deco_tranf = World.transforms[i];
		Sprite_2d& deco_asset = World.sprite[i];

		//jarak
		Rectangle deco_desrect = {
			deco_tranf.PositionComp.x, 
			deco_tranf.PositionComp.y,
			deco_asset.SourceRecComp.width * deco_tranf.ScaleComp,
			deco_asset.SourceRecComp.height * deco_tranf.ScaleComp

		};

		DrawTexturePro(deco_asset.TextureComp, deco_asset.SourceRecComp, deco_desrect, deco_asset.OriginCommp, deco_tranf.RotationComp, deco_asset.ColorComp);
	}
}

//render entity
void GameSystem::UpdateRenderSystem(WorldComp& World) {
	//loping texture musush
	for (int i = 0; i < MAX_ENTITY; i++) {
		if (!World.activeEntity[i]) {
			continue;
		}

		//skip dekorasi
		if (World.collider[i].TagComp == TagComponentID::GROWN_DECORATION) {
			continue;
		}

		//render text damage
		if (World.collider[i].TagComp == TagComponentID::DMG_TEXT) {
			Vector2 Posd = World.transforms[i].PositionComp;
			const char* txt = TextFormat("-%d", World.tdamage[i].Damage);
			Color colord = World.tdamage[i].ColorDamage;

			//tranparan
			// ini manusia tembus pandang
			float lifetext = World.lifetime[i].LifeTimeComp;
			if (lifetext > 0.5f) {
				colord = Fade(colord, 0.5f);
			}

			DrawText(txt, Posd.x, Posd.y, 18.0f, colord);
			continue;
		}

		//cek lagi entitinya
		if (World.collider[i].TagComp == TagComponentID::ENEMY) {
			float delta_t = GetFrameTime();
			if (World.health[i].IFrameTimer > 0.0f) {
				World.health[i].IFrameTimer -= delta_t;
				World.sprite[i].ColorComp = RED;
			}
			else {
				World.sprite[i].ColorComp = WHITE;
			}

		}

		if (!World.sprite[i].ActiveComp) {
			continue;
		}



		//akses alamt lagi le&
		TransformComponent& t = World.transforms[i];
		Sprite_2d& s = World.sprite[i];

		//destinasi gamabr di layar window
		Rectangle DestRec = {
			t.PositionComp.x,
			t.PositionComp.y,
			(float)s.SourceRecComp.width * t.ScaleComp,
			(float)s.SourceRecComp.height * t.ScaleComp
		};

		////reset warna musuh
		////pelan pelan pak supir
		//if (s.ColorComp.r != 255 || s.ColorComp.g != 255 || s.ColorComp.b != 255) {
		//	s.ColorComp = WHITE;
		//}

		////buat bayangan dulu le
		//float ShadowScale = 1.0f;
		//Color ShadowColor = ColorAlpha(BLACK, 0.3f);

		//DrawEllipse(
		//	t.PositionComp.x,
		//	t.PositionComp.y + 30,
		//	15.0f * ShadowScale,
		//	8.0f * ShadowScale,
		//	ShadowColor
		//);

		DrawTexturePro(s.TextureComp, s.SourceRecComp, DestRec, s.OriginCommp, t.RotationComp, s.ColorComp);

	}
}

