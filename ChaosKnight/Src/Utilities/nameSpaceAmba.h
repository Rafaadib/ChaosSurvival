
#pragma once

namespace FrameCharacter {

	//framedefinition
	//constan expresion cik
	constexpr int FWidth = 56;
	constexpr int FHeight = 64;
	constexpr int FCount = 8;
	constexpr float FSpeed = 12.0f;

}

namespace Fps{
	constexpr int width = 20;
	constexpr int height = 600;

	/*int fps = 60;*/
}

namespace Atribut {
	//inline mmeberi tahu semua file cik
	//jika file ynag meng include namespace berubah file lain juga tahu perubahanya

	//player atribut
	inline float atkspd = 0.7f;
	inline float damage = 11.0f;
	inline float movepeed = 200.0f;

	//enemy atribut
	inline float enemydamage = 1.0f;

}

