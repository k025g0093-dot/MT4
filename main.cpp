#include "DarwBox.h"
#include "Grid.h"
#include "Lerp.h"
#include "OBB.h"
#include "Segment.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Vector.h"
#include <Novice.h>
#include <cmath>
#include <cstdint>
#ifdef _DEBUG
#include <imgui.h>
#endif
#include "Collision.h"
#include <algorithm> // std::max, std::min 用

#include <chrono>

const char kWindowTitle[] = "LE2B_29_ヤマトユウヤ_タイトル";
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

struct Spher {
	Vector2 pos;
	float radius;

};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Spher spher1 = {
	    {0, 0},
        12
    };
	Spher spher2 = {
	    {640, 360},
        20
    };

	float speed = 3.0f;

	auto prevTime = std::chrono::steady_clock::now();

	int mousePosX, mousePosY = {};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {

		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = now - prevTime;
		float deltaTime = elapsed.count();

		deltaTime = 1.0f / 60.0f;

		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Novice::GetMousePosition(&mousePosX, &mousePosY);

		spher1.pos = {(float)mousePosX, (float)mousePosY};

		spher2.pos.x += (speed * deltaTime) * (spher1.pos.x - spher2.pos.x);
		spher2.pos.y += (speed * deltaTime) * (spher1.pos.y - spher2.pos.y);

#ifdef _DEBUG

		ImGui::Begin("Interpolation");
		ImGui::SliderFloat("speed", &speed, 0.1f, 30.0f);
		ImGui::Text("MousePos:(%f,%f)", (float)mousePosX, (float)mousePosY);
		ImGui::Text("spher1Pos:(%f,%f)", spher1.pos.x, spher1.pos.y);
		ImGui::Text("spher2Pos:(%f,%f)", spher2.pos.x, spher2.pos.y);
		ImGui::Text("now:(%f)", now);
		ImGui::Text("deltaTime:(%f)", deltaTime);
		ImGui::End();

#endif // _DEBUG


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawEllipse((int)spher1.pos.x, (int)spher1.pos.y, (int)spher1.radius, (int)spher1.radius, 0.0f, 0xFF0000FF, kFillModeSolid);
		Novice::DrawLine((int)spher1.pos.x, (int)spher1.pos.y, (int)spher2.pos.x, (int)spher2.pos.y, 0xFF0000FF);
		Novice::DrawEllipse((int)spher2.pos.x, (int)spher2.pos.y, (int)spher2.radius, (int)spher2.radius, 0.0f, 0x00FF00FF, kFillModeSolid);

		///----------------
		/// ↑描画処理ここまで
		///----------------

		// フレームの終了
		Novice::EndFrame();

		prevTime = now;

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}