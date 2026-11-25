
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

Vector2 Bezier(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {

	Vector2 result{};
	result.x = ((1 - t) * (1 - t)) * p0.x + 2 * t * (1 - t) * p1.x + (t * t) * p2.x;
	result.y = ((1 - t) * (1 - t)) * p0.y + 2 * t * (1 - t) * p1.y + (t * t) * p2.y;
	return result;
}

Vector2 BezierDerivative(const Vector2& p0, const Vector2& p1, const Vector2& p2, float t) {
	Vector2 result;
	result.x = 2 * (p1.x - p0.x) + 2 * t * (p0.x - (2 * p1.x) + p2.x);
	result.y = 2 * (p1.y - p0.y) + 2 * t * (p0.y - (2 * p1.y) + p2.y);
	return result;
}

Vector2 p0{200, 570};
Vector2 p1{800, 70};
Vector2 p2{1080, 570};


Vector2 operator*(float s, const Vector2& v);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int kRoadSegments = 200;
	float t = 0.0f;

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
		Vector2 c = Bezier(p0, p1, p2, t);
		Vector2 derivative =NormalizeToVector2 (BezierDerivative(p0, p1, p2, t).x,BezierDerivative(p0, p1, p2, t).y);

		ImGui::Begin("Bezier");
		ImGui::SliderFloat("t", &t,0,1);
		ImGui::SliderFloat("p1:x", &p1.x,400,800);
		ImGui::SliderFloat("p1:y", &p1.y,20,570);
		ImGui::Text("t:%.2f,%.2f", c.x,c.y);
		ImGui::Text("t':%.2f,%.2f", derivative.x, derivative.y);
		ImGui::End();


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		Novice::DrawEllipse((int)c.x, (int)c.y, 10, 10, 0.0f, 0x181818FF, kFillModeSolid);

	

		Novice::DrawLine(
			(int)c.x, (int)c.y, 
			(int)(c.x+derivative.x*120), (int)(c.y+derivative.y*120),
			0xFF0000FF
			);

		for (int i = 0; i < kRoadSegments; i++) {
			float t0 = static_cast<float>(i) / static_cast<float>(kRoadSegments);
			float t1 = static_cast<float>(i + 1) / static_cast<float>(kRoadSegments);
			Vector2 c0 = Bezier(p0, p1, p2, t0);
			Vector2 c1 = Bezier(p0, p1, p2, t1);
			Novice::DrawLine(
				static_cast<int>(c0.x),
				static_cast<int>(c0.y),
				static_cast<int>(c1.x),
				static_cast<int>(c1.y),
				0xFFFFFFFF
			);
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