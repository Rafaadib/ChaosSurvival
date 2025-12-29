
#pragma once

#include <raylib.h>
#include <string>


//event driven untuk buff nya wak
//buat enum di sisni
enum class TypeBuff {
	HEAL_POTION, //
	DAMAGE_UP, // 
	ATTACK_SPEED,  //
	MOVE_SPEED, //
	MULTI_SHOT, //                       
	BOUNCING, //
	CRIT_RATIO, //
	HP_REGEN,
	SHIELD,
	BURN_EFECT,
	MAGNET_RANGE,
	IMMUNITY
};

//buat structure datanya sir
//kartu ambabuff
struct BuffDefinition {
	std::string namebuff;
	std::string Description;
	TypeBuff bufftype;
	float ValueBuff;

	//kunci nama asset
	std::string keyvalue;
};