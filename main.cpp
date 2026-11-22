
#include <Novice.h>
#include <cmath>
#include <cstdint>
#ifdef _DEBUG
#include <imgui.h>
#endif
#include "Collision.h"
#include "Quaternion.h"
#include <algorithm> // std::max, std::min 用
#include <chrono>

const char kWindowTitle[] = "LE2B_29_ヤマトユウヤ_タイトル";

/*台の形*/
float f(float x) { return 0.125f * x * x * x + 0.25f * x * x; }
/*導関数*/
float fPraime(float x) { return 0.75f * x * x + 0.0f; }

Vector2 Normalized(const Vector2& v) {
	float length = std::sqrtf(v.x * v.x + v.y * v.y);
	return {v.x / length, v.y / length};
}

float x = 0.0f;

// ステージに使う横線
struct Lines {
	Vector2 pos;
	Vector2 world;
};

struct Front {
	Vector2 pos;
	Vector2 world;
};

struct Back {
	Vector2 pos;
	Vector2 world;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Front front{};
	front.world.x = 0.0f;
	front.world.y = 0.0f;
	front.pos.x = front.world.x;
	front.pos.y = front.world.y;

	Back back{};
	back.world.x = 0.0f;
	back.world.y = 0.0f;
	back.pos.x = back.world.x;
	back.pos.y = back.world.y;

	Vector2 kBodyHalfLenght = {0.15f, 0.15f};

	/*地面の線*/
	Lines lineB{};
	lineB.world.x = 0.0f;
	lineB.world.y = 0.0f;
	lineB.pos.x = lineB.world.x;
	lineB.pos.y = lineB.world.y;

	Lines lineA{};
	lineA.world.x = 0.0f;
	lineA.world.y = 0.0f;
	lineA.pos.x = lineA.world.x;
	lineA.pos.y = lineA.world.y;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {

		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Vector2 position{x, f(x)};
		float slope = fPraime(x);
		Vector2 forward = NormalizeToVector2(1.0f, slope);

		forward = Normalized({1.0f, slope});
		back.world.x = position.x - forward.x * kBodyHalfLenght.x;
		back.world.y = position.y - forward.y * kBodyHalfLenght.y;

		front.world.x = position.x + forward.x * kBodyHalfLenght.x;
		front.world.y = position.y + forward.y * kBodyHalfLenght.y;

		if (preKeys[DIK_A]) {
			if (x <= 0) {
				x = 0.0f;
			}
			x -= 0.0167f;
		}
		if (preKeys[DIK_D]) {
			if (x >= 2) {
				x = 2.0f;
			}
			x += 0.0167f;
		}

		ImGui::Begin("jump Ramp");
		ImGui::DragFloat("x:%d", &x);

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		front.pos.x = 400 + 240 * front.world.x;
		front.pos.y = 640 - 240 * front.world.y;
		Novice::DrawEllipse((int)front.pos.x, (int)front.pos.y, 10, 10, 0.0f, 0xFFFF00FF, kFillModeSolid);

		back.pos.x = 400 + 240 * back.world.x;
		back.pos.y = 640 - 240 * back.world.y;
		Novice::DrawEllipse((int)back.pos.x, (int)back.pos.y, 10, 10, 0.0f, 0xFFFF00FF, kFillModeSolid);

		Novice::DrawLine((int)front.pos.x, (int)front.pos.y, (int)back.pos.x, (int)back.pos.y, 0xFFFF00FF);

		lineB.pos.x = lineB.world.x;
		lineB.pos.y = 640 - 240 * lineB.world.y;
		Novice::DrawLine((int)lineB.pos.x, (int)(lineB.pos.y), (int)lineB.pos.x + 1280, (int)(lineB.pos.y), 0xffffffff);

		for (int i = 0; i < 200; i++) {

			lineA.pos.x = (float)(i * (2.0f / 200.0f));
			lineA.pos.y = f(lineA.pos.x);
			float nextX = (float)((i + 1) * (2.0f / 200.0f));
			float nextY = f(nextX);

			lineA.pos.x = 400 + 240 * lineA.pos.x;
			lineA.pos.y = 640 - 240 * lineA.pos.y;
			nextX = 400 + 240 * nextX;
			nextY = 640 - 240 * nextY;

			Novice::DrawLine(
				(int)lineA.pos.x, 
				(int)lineA.pos.y,
				(int)nextX,
				(int)nextY,
				0xFFFFFFFF);
		}

		///----------------
		/// ↑描画処理ここまで
		///----------------

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}