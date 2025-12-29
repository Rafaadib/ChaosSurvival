
#include <iostream>

#include "Buff_Manager.h"
#include "Utilities/nameSpaceAmba.h"

Buff_Manager::Buff_Manager() {
	InitBuff();
}

Buff_Manager::~Buff_Manager() {

}

void Buff_Manager::InitBuff() {
	//masukan semua buff tadi ke vector wak
	Allbuff.push_back({	"Damage Up +20% ", "Increases Damage 20% ", TypeBuff::DAMAGE_UP, 5.0f, "Card_Damage"});
	Allbuff.push_back({ "Crit +5% ", "Increases Crit 10% ", TypeBuff::CRIT_RATIO, 10.0f, "Card_Crit"});
	Allbuff.push_back({ "Healing +10 ", "Increases Healing 10 ", TypeBuff::HEAL_POTION, 10.0f, "Card_Heal"});
	Allbuff.push_back({ "Movement +20 ", "Increases Movement Speed by 20 ", TypeBuff::MOVE_SPEED, 20.0f, "Card_Movement"});
	Allbuff.push_back({ "Bounce ", "Projectile Bounce to Enemy ", TypeBuff::BOUNCING, 1.0f, "Card_Bounce"});
	Allbuff.push_back({ "Atk Spd ", "Increases Projectile Speed ", TypeBuff::ATTACK_SPEED, 0.1f, "Card_Atkspd"});
	Allbuff.push_back({ "MultiShot ", "increases Multi Projectile ", TypeBuff::MULTI_SHOT, 1.0f, "Card_Multishot"});

}

std::vector<BuffDefinition> Buff_Manager::GetRandomOption(int countbuff) {
	//terus mengocok patrick
	if (Allbuff.empty()) {
		return {};
	}

	//buat vector untuk pilihan
	std::vector<BuffDefinition> shuffled = Allbuff;

	//nag ini adalah algotimanya
	std::random_device Brandom;
	std::mt19937 g(Brandom());
	std::shuffle(shuffled.begin(), shuffled.end(), g);

	//ambil 3 buff stiap naik level king
	std::vector<BuffDefinition> buffresult;
	for (int i = 0; i < countbuff && i < shuffled.size(); i++) {
		buffresult.push_back(shuffled[i]);
	}

	return buffresult;
}

void Buff_Manager::ApplyBuff(Character_2d* Player, TypeBuff type, float valuebuff) {
	//disini untuk setting buffnya
	//pake swirtch karana bisa beratambah banyak
	switch (type) {
		case TypeBuff::DAMAGE_UP :
			Atribut::damage += valuebuff ;
			break;
		case TypeBuff::CRIT_RATIO :
			//belum
			break;
		case TypeBuff::ATTACK_SPEED :
			Atribut::atkspd -= valuebuff;
			if (Atribut::atkspd <= 0.3f) {
				Atribut::atkspd = 0.3f;
			}
			break;
		case TypeBuff::HEAL_POTION :
			Player->CurrentHealth += valuebuff;
			if (Player->CurrentHealth > Player->MaxHealth) {
				Player->CurrentHealth = Player->MaxHealth;
			}
			//max hp up 
			/*Player->MaxHealth += valuebuff;
			Player->CurrentHealth += valuebuff;*/
			break;
		case TypeBuff::MOVE_SPEED :
			Atribut::movepeed += valuebuff;
			break;
		case TypeBuff::BOUNCING :
			Player->BulletBounce += (int)valuebuff;
			break;
		case TypeBuff::MULTI_SHOT :
			Player->BulletCount += (int)valuebuff;
			break;
		
	}
}


