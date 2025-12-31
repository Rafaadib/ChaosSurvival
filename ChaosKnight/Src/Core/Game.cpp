
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "Game.h"

#include "Utilities/MenuGlobal.h"

#include "Entities/Player.h"
#include "Utilities/nameSpaceAmba.h"
#include "Utilities/UIHelper.h"
#include "Game_Camera2d.h"

#include "BuffLevel.h"


Game::Game(int screenWidth, int screenHeight, const char* title)
	: screenWidth(screenWidth), screenHeight(screenHeight) {

	SetTargetFPS(60);
	InitWindow(screenWidth, screenHeight, title);

	//init musik
	InitAudioDevice();

	//rezize window le
	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
	SetWindowMinSize(320, 240);

	ScreenTarget = LoadRenderTexture(VirtualWidth, VirtualHeight);
	SetTextureFilter(ScreenTarget.texture, TEXTURE_FILTER_POINT);

	//set kan camera dulu le 
	CameraChar = std::make_unique<Game_Camera2d>(screenWidth, screenHeight);

	Menu = std::make_unique<BuffLevel>();

	//set kan ECS system yang sudah di buat 
	GameWorld = std::make_unique<WorldComp>();

	//load asset kan wak
	GameAsset = std::make_unique<Asset_Manager>();

	//buff load di isin le
	buffmanager = std::make_unique<Buff_Manager>();

	Wavelevel = 1;
	Explevel = 1;
	CurrentScreen = MAIN_MENU;
	AutoShootTimer = 0.0f;
	SpawnTimer = 0.0f;

	//initialiaze
	LoadAsset();
}

//destructor (ini untuk hancurkan object yang sudah tidak digunakan wak)
Game::~Game(){
	UnloadRenderTexture(ScreenTarget);

	//unlload musik dulu
	//yang seound belum ada hehe
	UnloadMusicStream(Backsound);
	UnloadSound(Buttonclick);

	UnloadAsset();

	CloseAudioDevice();
	CloseWindow();
}

//load game asset (timpa timpa) kalau nimpa di pikir mas
//sosok admin
void Game::LoadAsset() {
	
	//load asset player
	GameAsset->AddTexture("Player", "GameAsset/undedknight.png");
	PlayerKnight = std::make_unique<Character_2d>(
		GameAsset->GetTexture("Player"),
		FrameCharacter::FWidth,
		FrameCharacter::FHeight,
		FrameCharacter::FCount,
		FrameCharacter::FSpeed
	);

	//enemyy texture
	GameAsset->AddTexture("Enemy", "GameAsset/evofirespirit.png");
	GameAsset->AddTexture("Enemy2", "GameAsset/evobats.png");
	GameAsset->AddTexture("Enemy3", "GameAsset/evolarry.png");

	//projectile
	GameAsset->AddTexture("Bullet", "GameAsset/bullet.png");

	//gem
	GameAsset->AddTexture("Gem", "GameAsset/gem.png");

	//untuk kartu
	GameAsset->AddTexture("Card_Heal", "GameAsset/heal.png");
	GameAsset->AddTexture("Card_Crit", "GameAsset/crit.png");
	GameAsset->AddTexture("Card_Damage", "GameAsset/damageup.png");
	GameAsset->AddTexture("Card_Movement", "GameAsset/speedup.png");
	GameAsset->AddTexture("Card_Bounce", "GameAsset/bounce.png");
	GameAsset->AddTexture("Card_Atkspd", "GameAsset/atkspd.png");
	GameAsset->AddTexture("Card_Multishot", "GameAsset/buffmultishot.png");

	//asset mini
	GameAsset->AddTexture("PropA", "GameAsset/atlas3.png");

	//ui
	GameAsset->AddTexture("BarHp", "GameAsset/barhp.png");
	GameAsset->AddTexture("FillHp", "GameAsset/fhp.png");
	GameAsset->AddTexture("BarXp", "GameAsset/bxp.png");
	GameAsset->AddTexture("FillXp", "GameAsset/fxp.png");

	//tobol
	GameAsset->AddTexture("Button", "GameAsset/button.png");
	GameAsset->AddTexture("Pause", "GameAsset/pause2.png");

	//audio
	Backsound = LoadMusicStream("Sound/backsound.wav");
	SetMusicVolume(Backsound, 0.5f);
	PlayMusicStream(Backsound);


}

//unload game asset (buang buang)
void Game::UnloadAsset() {
	//clean memory
	/*delete PlayerKnight;
	delete CameraChar;*/
}

//gabungkan le ECS dan OOP hybrid architecture
void Game::UpdateGameplay() {

	float delta_t = GetFrameTime();

	PlayerKnight->AnimUpdate();
	PlayerKnight->ControlCharacter();

	CameraChar->CameraUpdate(PlayerKnight->Position);

	EnemySpawnLogic(delta_t);

	//Ai logic
	GameSystem::UpdateEnemyAI(*GameWorld, PlayerKnight->Position);

	//update mpergerakan
	GameSystem::UpdateMovementSystem(*GameWorld, delta_t);

	//animasi update
	GameSystem::UpdateAnimation(*GameWorld, delta_t);

	//sistem magnet
	GameSystem::UpdateMagnetXP(*GameWorld, PlayerKnight->Position, delta_t);

	//sistem tabrakan
	GameSystem::UpdateCollison(*GameWorld, PlayerKnight.get(), Score, GameAsset->GetTexture("Gem"));

	GameSystem::UpdateDecoration(*GameWorld, PlayerKnight->Position, GameAsset.get());


	AutoShootTimer += delta_t;
	if (AutoShootTimer >= Atribut::atkspd) {

		int TargetID = GameSystem::GetNearestEnemy(*GameWorld, PlayerKnight->Position);

		if (TargetID != -1) {
			int bulletID = GameWorld->CreateEntity();
			if (bulletID != -1) {
				//set bullet component
				GameWorld->transforms[bulletID].PositionComp = PlayerKnight->Position;
				GameWorld->transforms[bulletID].ScaleComp = 0.8f;
				GameWorld->transforms[bulletID].RotationComp = 1.0f;

				GameWorld->velocity[bulletID].SpeedComp = 700.0f;
				GameWorld->velocity[bulletID].IsMovingComp = true;

				//hitung arah msuush
				Vector2 TargetPos = GameWorld->transforms[TargetID].PositionComp;
				Vector2 direction = Vector2Subtract(TargetPos, PlayerKnight->Position);
				GameWorld->velocity[bulletID].DirectionComp = Vector2Normalize(direction);


				GameWorld->collider[bulletID].Active = true;
				GameWorld->collider[bulletID].RadiusComp = 5.0f;
				GameWorld->collider[bulletID].TagComp = TagComponentID::PLAYER_PROJECTILE;

				GameWorld->sprite[bulletID].TextureComp = GameAsset->GetTexture("Bullet");
				Texture2D texBullet = GameAsset->GetTexture("Bullet");

				GameWorld->sprite[bulletID].ColorComp = WHITE;
				GameWorld->sprite[bulletID].ActiveComp = true;
				GameWorld->sprite[bulletID].SourceRecComp = { 0.0f, 0.0f, (float)texBullet.width, (float)texBullet.height };
				GameWorld->sprite[bulletID].OriginCommp = { (float)texBullet.width / 2, (float)texBullet.height  / 2 };

				GameWorld->lifetime[bulletID].MaxLifeTimeComp = 2.0f;

			}

			AutoShootTimer = 0.0f;
		}

	}


	//conecting (blutut devise active:>)
	GameSystem::UpdateLifetime(*GameWorld, delta_t);

	if (PlayerKnight->CurrentHealth <= 0) {
		CurrentScreen = GAME_OVER;
	}

	/*if (PlayerKnight->CurrentHealth >= PlayerKnight->MaxHealth) {
		PlayerKnight->CurrentHealth = PlayerKnight->MaxHealth;
		CurrentScreen = GAMEPLAY;
	}*/

	if (PlayerKnight->CurrentXp >= PlayerKnight->Maxp) {

		PlayerKnight->CurrentXp -= PlayerKnight->Maxp;

		PlayerKnight->Maxp *= 1.2f;

		currentoptionbuff = buffmanager->GetRandomOption(3);

		Explevel++;
		CurrentScreen = LEVEL_UP;
	}


	/*if (IsKeyPressed(KEY_U)) {
		CurrentScreen = LEVEL_UP;
	}*/

}

//game update
void Game::Update() {

	UpdateMusicStream(Backsound);

	if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {
		int DisplayVirtual = GetCurrentMonitor();

		if (IsWindowFullscreen()) {
			ToggleFullscreen();
			SetWindowSize(screenWidth, screenHeight);
		}
		else {
			ToggleFullscreen();
			SetWindowSize(GetMonitorWidth(DisplayVirtual), GetMonitorHeight(DisplayVirtual));
		}
	}

	Vector2 virtualmouse = GetVirtualMousePotition();

	//gamesatatenya
	
	if (CurrentScreen == GAMEPLAY) {
		UpdateGameplay();
	}
	else if (CurrentScreen == LEVEL_UP) {

		Vector2 MousePos = GetVirtualMousePotition();

		int choice = Menu->UpdateAndChoice(MousePos, currentoptionbuff);
		if (choice != 0) {
			BuffDefinition Selected = currentoptionbuff[choice - 1];

			buffmanager->ApplyBuff(PlayerKnight.get(), Selected.bufftype, Selected.ValueBuff);
			
			buffcount++;

			if (buffcount >= 5) {
				Wavelevel++;
				buffcount = 0;
			}

			CurrentScreen = GAMEPLAY;
			
		}

	}
	else if (CurrentScreen == CREDITS) {
		if (IsKeyDown(KEY_ESCAPE) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			CurrentScreen = MAIN_MENU;
		}
	}
	/*else if (CurrentScreen == PAUSE) {
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			cur
		}
	}*/

	//if (CurrentScreen == GAME_OVER) {
	//	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
	//		CurrentScreen = MAIN_MENU;
	//	}
	//}

}

void Game::GameReset() {
	PlayerKnight->Position = { (float)screenWidth / 2, (float)screenHeight / 2 };
	PlayerKnight->CurrentHealth = PlayerKnight->MaxHealth;

	//hapus ribuan entity
	GameWorld->Clear();

	Wavelevel = 1;
	Explevel = 1;
	Score = 0;
	CurrentScreen = GAMEPLAY;

}

//logika musuh
void Game::EnemySpawnLogic(float Delta_t) {
	SpawnTimer += Delta_t;

	//setting spawn musuh
	float Spawrate = 0.8f / (Wavelevel * 0.5f);
	//batasi spawn
	if (Spawrate < 0.1f) {
		Spawrate = 0.1f;

	}

	//apaakah sudah spawn wak?
	if (SpawnTimer >= Spawrate) {
		SpawnTimer = 0.0f;

		//buat entity barul wak
		int enemy = GameWorld->CreateEntity();
		if (enemy == -1) {
			return;
		}

		//spawn random 400 - 500
		float angle = GetRandomValue(0, 360) * DEG2RAD;
		float distance = GetRandomValue(400, 500);

		//fungsi trigonometri le(sin untuk posisi y dan cos untuk posisi x)
		Vector2 SpawnPos = {
			PlayerKnight->Position.x + cos(angle) * distance,
			PlayerKnight->Position.y + sin(angle) * distance
		};

		//1 persen hingga 100persen
		int GachaEnemy = GetRandomValue(1, 50);

		//normal enemy 
		std::string tkey = "Enemy";
		float Escale = 1.2f;
		float Espeed = 80.0f;
		float Ehp = 15.0f;
		float Eradius = 15.0f;
		Color Etinit = WHITE;
		bool isElite = false;

		if (GachaEnemy <=  15) {
			tkey = "Enemy";
			Escale = 1.2f;
			Espeed = 80.0f;
			Ehp = 15.0f;

		}
		else if (GachaEnemy <= 30) {
			tkey = "Enemy2";
			Escale = 1.2f;
			Espeed = 100.0f;
			Ehp = 13.0f;
		}
		else if (GachaEnemy <= 45 ) {
			tkey = "Enemy3";
			Escale = 1.0f;
			Espeed = 60.0f;
			Ehp = 14.0f;
		}
		else {
			tkey = "Enemy3";
			isElite = true;

			Escale = 1.7f;
			Espeed = 50.0f;
			Ehp = 42.0f;
			Eradius = 15.0f * 2.0f;

		}

		//buat component transform le
		GameWorld->transforms[enemy].PositionComp = SpawnPos;
		GameWorld->transforms[enemy].ScaleComp = Escale;
		GameWorld->transforms[enemy].RotationComp = 0.0f;

		GameWorld->velocity[enemy].SpeedComp = Espeed;
		GameWorld->velocity[enemy].IsMovingComp = false;


		//buat component aniamnsi
		Texture2D texEnemy = GameAsset->GetTexture(tkey);
		GameWorld->sprite[enemy].ActiveComp = true;
		GameWorld->sprite[enemy].TextureComp = texEnemy;
		GameWorld->sprite[enemy].ColorComp = Etinit;

		int framecount = 4;
		float framewidth = (float)texEnemy.width / (float)framecount;
		GameWorld->sprite[enemy].isAnimated = true;
		GameWorld->sprite[enemy].MaxFrameComp = framecount;
		GameWorld->sprite[enemy].FrameWidthComp = framewidth;
		GameWorld->sprite[enemy].FrameSpeedComp = 0.15f;
		GameWorld->sprite[enemy].SourceRecComp = { 0, 0, framewidth, (float)texEnemy.height };
		GameWorld->sprite[enemy].OriginCommp = { framewidth / 2, (float)texEnemy.height / 2.0f };

		//pysyic and logik
		//hit box cik
		GameWorld->collider[enemy].Active = true;
		GameWorld->collider[enemy].TagComp = TagComponentID::ENEMY;
		GameWorld->collider[enemy].RadiusComp = Eradius;

		GameWorld->health[enemy].HealthComp = Ehp + (Wavelevel * 2);
		GameWorld->health[enemy].CurrenHealthComp = GameWorld->health[enemy].HealthComp;

		//musuh eleit lebih banyak exp drop
		if (isElite) {
			GameWorld->enemydata[enemy].Xp_die = 15;
		}
	}


}


bool Game::DrawButton(const char* text,Texture2D buttontexture , Rectangle recbutton, Vector2 mousebutton) { //add 2d texture
	bool click = false;

	//cek apakah diatas tommbol mosuenya
	bool isUpbutton = CheckCollisionPointRec(mousebutton, recbutton);

	Color Btint = WHITE;
	if (isUpbutton) {
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			Btint = GRAY; //seusuaiakan wak
		}
		else {
			Btint = { 33, 52, 72, 255 };
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			//belum ada sound clik nya hehe
			/*PlaySound(Buttonclick);*/
			click = true;
		}
	}

	Rectangle Sourecbutton = {
		0,
		0,
		(float)buttontexture.width,
		(float)buttontexture.height
	};
	Rectangle destecbutton = recbutton;

	DrawTexturePro(buttontexture, Sourecbutton, destecbutton, { 0, 0 }, 0.0f, Btint);

	int fontsize = 20;
	int widthtext = MeasureText(text, fontsize);

	float textX = recbutton.x + (recbutton.width / 2) - (widthtext / 2);
	float textY = recbutton.y + (recbutton.height / 2) - (fontsize / 2);

	DrawText(text, textX + 2, textY + 2, fontsize, BLACK);
	DrawText(text, textX, textY, fontsize, WHITE);


	return click;
}

//virtual layar(resolusi wibu)
Vector2 Game::GetVirtualMousePotition() {

	Vector2 Mmouse = GetMousePosition();

	float VirtualMouseScaleX = (float)GetScreenWidth() / VirtualWidth;
	float VirtuaMouseScaleY = (float)GetScreenHeight() / VirtualHeight;
	float VirtualMouseCurrentScale = (VirtualMouseScaleX < VirtuaMouseScaleY) ? VirtualMouseScaleX : VirtuaMouseScaleY;

	float NewVirtualMouseWidth = (float)VirtualWidth * VirtualMouseCurrentScale;
	float NewVirtualmouseHeight = (float)VirtualHeight * VirtualMouseCurrentScale;
	float VirtualMouseOffsetX = (GetScreenWidth() - NewVirtualMouseWidth) * 0.5f;
	float VirtualMouseOffsetY = (GetScreenHeight() - NewVirtualmouseHeight) * 0.5f;

	Vector2 VirtualMouse = {} ;
	VirtualMouse.x = (Mmouse.x - VirtualMouseOffsetX) / VirtualMouseCurrentScale;
	VirtualMouse.y = (Mmouse.y - VirtualMouseOffsetY) / VirtualMouseCurrentScale;

	//jangan over layar le
	if (VirtualMouse.x < 0) {
		VirtualMouse.x = 0;
	}
	if (VirtualMouse.y < 0) {
		VirtualMouse.y = 0;
	}
	if (VirtualMouse.x > VirtualWidth) {
		VirtualMouse.x = VirtualWidth;
	}
	if (VirtualMouse.y > VirtualHeight) {
		VirtualMouse.y = VirtualHeight;
	}


	return VirtualMouse;
}



//Amba draw
void Game::Draw() {

	float ScreenScaleX = (float)GetScreenWidth() / VirtualWidth;
	float ScreemScaleY = (float)GetScreenHeight() / VirtualHeight;

	ScreenScale = (ScreenScaleX < ScreemScaleY) ? ScreenScaleX : ScreemScaleY;

	float NewScreenWidth = (float)VirtualWidth * ScreenScale;
	float NewScreenHeight = (float)VirtualHeight * ScreenScale;
	float ScreenOffsetX = (GetScreenWidth() - NewScreenWidth) * 0.5f;
	float ScreemnOffsetY = (GetScreenHeight() - NewScreenHeight) * 0.5f;

	ScreenSourceRec = { 0.0f, 0.0f, (float)ScreenTarget.texture.width, -(float)ScreenTarget.texture.height };
	ScreenDestRec = {ScreenOffsetX, ScreemnOffsetY, NewScreenWidth, NewScreenHeight};
	ScreenoOrigin = { 0.0f, 0.0f };

	BeginTextureMode(ScreenTarget);
		ClearBackground(GROWN_COLOR); //clear buffer

		Vector2 mouseclik = GetVirtualMousePotition();
		Texture2D btntexture = GameAsset->GetTexture("Button");
		Texture2D btnpause = GameAsset->GetTexture("Pause");

		if (CurrentScreen == MAIN_MENU) {
			DrawText("Chaos", VirtualWidth / 2 - 150, 100, 50, RED);
			DrawText("Survival", VirtualWidth / 2 - 60, 150, 50, DARKGRAY);

			//tombol
			if (DrawButton("Play",  btntexture,  { (float)VirtualWidth / 2 - 100, 250, 200, 50 }, mouseclik)) {
				GameReset();
				CurrentScreen = GAMEPLAY;
			}
			if (DrawButton("Credit", btntexture,  { (float)VirtualWidth / 2 - 100, 320, 200, 50 }, mouseclik)) {
				CurrentScreen = CREDITS;
			}
			if (DrawButton("Exit", btntexture, { (float)VirtualWidth / 2 - 100, 390, 200, 50 }, mouseclik)) {
				IsGameRunning = false;
			}
		}
		else if (CurrentScreen == CREDITS) {
			DrawText("Creadits wak", VirtualWidth / 2 - 130, 50, 40, BLACK);

			DrawText("Mas Rusdi", VirtualWidth / 2 - 100, 150, 20, DARKGRAY);
			DrawText("Mas AMBA", VirtualWidth / 2 - 100, 190, 20, DARKGRAY);
			DrawText("Mas Asep Sentrifugal", VirtualWidth / 2 - 100, 230, 20, DARKGRAY);
			DrawText("Mas Ambatron", VirtualWidth / 2 - 100, 270, 20, DARKGRAY);
			DrawText("Mas Ambalabu", VirtualWidth / 2 - 100, 310, 20, DARKGRAY);

			DrawText("click untuk kembali", VirtualWidth / 2 - 225, 500, 50, RED);
		}
		else if (CurrentScreen == GAMEPLAY || CurrentScreen == LEVEL_UP || CurrentScreen == PAUSE) {
			CameraChar->CameraBeginMode();

				GameSystem::RenderDecoration(*GameWorld);
				
				//buat bayangan dulu le
				float ShadowScale = 1.0f;
				Color ShadowColor = ColorAlpha(BLACK, 0.3f);

				DrawEllipse(
					PlayerKnight->Position.x,
					PlayerKnight->Position.y + 35,
					20.0f * ShadowScale,
					8.0f * ShadowScale,
					ShadowColor
					);

				//draw palyer
				PlayerKnight->AnimDraw(PlayerKnight->Position, PScale, WHITE);

				//render semua entity wak
				GameSystem::UpdateRenderSystem(*GameWorld);

				DrawCircle(VirtualWidth / 2, VirtualHeight / 2, 10, BLUE);

				

			CameraChar->CameraEndMode();


			//ui hp
			UI::DrawBarUI(
				{ 10.0f, 10.0f },
				PlayerKnight->CurrentHealth,
				PlayerKnight->MaxHealth,
				4.0f,
				GameAsset->GetTexture("FillHp"),
				GameAsset->GetTexture("BarHp")
			);
			/*float PHpercent = PlayerKnight->CurrentHealth / PlayerKnight->MaxHealth;
			DrawRectangle(20, 40, 200, 30, LIGHTGRAY);
			DrawRectangle(20, 40, 200 * PHpercent, 30, RED);
			DrawRectangleLines(20, 40, 200, 30, BLACK);*/
			DrawText(TextFormat("HP : %.0f", PlayerKnight->CurrentHealth), 83, 60, 20, WHITE);

			//kill count
			const char* ScoreKill = TextFormat("Kill : %d", Score);
			int Textwidth = MeasureText(ScoreKill, 30);
			DrawText(ScoreKill, VirtualWidth - Textwidth - 30, 30, 25, RED);


			//ui exp
			UI::DrawBarUI(
				{ 10.0f, 80.0f},
				PlayerKnight->CurrentXp,
				PlayerKnight->Maxp,
				3.0f,
				GameAsset->GetTexture("FillXp"),
				GameAsset->GetTexture("BarXp")
			);
			/*float Expercent = (float)PlayerKnight->CurrentXp / PlayerKnight->Maxp;
			DrawRectangle(20, 80, 200, 15, DARKGRAY);
			DrawRectangle(20, 80, 200 * Expercent, 15, BLUE);
			DrawRectangleLines(20, 80, 200, 15, BLACK);*/
			DrawText(TextFormat("Level  %d", Explevel), 200, 100, 20, BLACK);


			//waveinfo
			DrawText(TextFormat("Wave %d", Wavelevel), VirtualWidth / 2 - 10, 40, 30, BLACK);
			DrawText(TextFormat("Nextwave %d/5 buffs", buffcount), VirtualWidth / 2 - 40, 75, 15, BLACK);

			float pausize = 40.0f;
			Rectangle pauserect = {
				VirtualWidth - pausize - 20,
				600,
				pausize,
				pausize
			};

			if (CurrentScreen == GAMEPLAY) {
				if (DrawButton("", btnpause, pauserect, mouseclik)) {
					CurrentScreen = PAUSE;
				}
			}
			else if (CurrentScreen == PAUSE) {
				DrawTexturePro(btnpause, { (float)btnpause.width, (float)btnpause.height }, pauserect, { 0, 0 }, 0.0f, WHITE);
			}

			if (CurrentScreen == PAUSE) {
				DrawRectangle(0, 0, VirtualWidth, VirtualHeight, Fade(BLACK, 0.6f));

				DrawText("Game Pause", VirtualWidth / 2 - 130, 130, 50, RED);

				float btnpausewidth = 200;
				float btnpauseheight = 60;
				float btnpauseX = VirtualWidth / 2 - btnpausewidth / 2;

				if (DrawButton("Resume", btntexture, { btnpauseX, 200, btnpausewidth, btnpauseheight }, mouseclik)) {
					CurrentScreen = GAMEPLAY;
				}
				if (DrawButton("Main Menu", btntexture, { btnpauseX, 360, btnpausewidth, btnpauseheight }, mouseclik)) {
					GameReset();
					CurrentScreen = MAIN_MENU;
				}
			}

		}
		else if (CurrentScreen == GAME_OVER) {
			DrawRectangle(0, 0, VirtualWidth, VirtualHeight, Fade(BLACK, 0.8f));

			DrawText("Game Over", VirtualWidth / 2 - 120, 150, 50, RED);
			DrawText(TextFormat("Total Score : %d", Score), VirtualWidth / 2 - 80, 220, 30, WHITE);

			//play again
			if (DrawButton("Play Again", btntexture, { (float)VirtualWidth / 2 - 100, 350, 200, 50 }, mouseclik)) {
				GameReset();
				CurrentScreen = GAMEPLAY;
			}
			if (DrawButton("Main Menu", btntexture, { (float)VirtualWidth / 2 - 100, 420, 200, 50 }, mouseclik)) {
				CurrentScreen = MAIN_MENU;
			}
			
		}

		if (CurrentScreen == LEVEL_UP) {
			Menu->DrawBuffLevel(VirtualWidth, VirtualHeight, currentoptionbuff, GameAsset.get());
		}

	EndTextureMode();
		 
	BeginDrawing();
		ClearBackground(GROWN_COLOR);
	
		DrawTexturePro(ScreenTarget.texture, ScreenSourceRec, ScreenDestRec, ScreenoOrigin, 0.0f, WHITE);

		DrawFPS(Fps::width, Fps::height);
	EndDrawing();
}

//game loop 
void Game::Run() {
	while (!WindowShouldClose() && IsGameRunning) {
		Update();
		Draw();
	}
}
