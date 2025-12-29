
#pragma once

#include <vector>
#include <algorithm>
#include <random>

#include "Utilities/AmbaBuff.h"
#include "Entities/Player.h"

class Buff_Manager {
public:
	Buff_Manager();
	~Buff_Manager();

	//fungsiu ambil buff acak sebnayak 3 gunain vector
	std::vector<BuffDefinition> GetRandomOption(int countbuff);

	//fungsi terapkan efek ke player / pemain
	//gunain pointer king
	void ApplyBuff(Character_2d* Player, TypeBuff type, float valuebuff);

private:
	std::vector<BuffDefinition> Allbuff;
	void InitBuff();
};