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
#include <numbers>

const char kWindowTitle[] = "LE2B_29_ヤマトユウヤ_タイトル";
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

struct Spherical {
	Vector3 postion;
	float radius;
	float theta;
	float phi;
};

Vector3 cameraTranslate{0.0f, 1.9f, -6.49f};
Vector3 cameraRotate{0.26f, 0.0f, 0.0f};

Vector3 ToCartesian(const Spherical& s) {
	float rho = s.radius * std::cos(s.theta);
	return {rho * std::cos(s.phi), s.radius * std::sin(s.theta), rho * std::sin(s.phi)};
}

Spherical ToSpherical(const Vector3& p)
{
	float r = std::sqrt(
	p.x*p.x+p.y*p.y+p.z*p.z);
	if (r ==0.0f) {
		return {0.0f, 0.0f, 0.0f};
	}

	float sinTheta = std::clamp(p.y / r, -1.0f, 1.0f);
	float phi = 0.0f;
	if (p.x != 0.0f || p.z != 0.0f) {
		phi = std::atan2(p.z, p.x);
	}
	return { r, std::asin(sinTheta),phi};

}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	Vector3 target{0, 0, 0};

	Vector3 worldUP{0.0f, 1.0f, 0.0f};

	const float halfPi = std::numbers::pi_v<float> / 2.0f;
	Spherical s{6.0f, 0.0f, -halfPi};

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

		s.theta=std::clamp(s.theta, -halfPi+0.01f, halfPi-0.01f);
		s.radius = (std::max)(s.radius, 0.1f);

		Vector3 pos = ToCartesian(s);

		Vector3 eye = target + pos;
		Vector3 forWard = Normalize(target);
		Vector3 right = Normalize(Cross(worldUP, forWard));
		Vector3 up = Cross(forWard, right);

		Matrix4x4 cameraMatrix{
		    {
				{right.x, right.y, right.z, 0.0f},
			{up.x, up.y, up.z, 0.0f},
			{forWard.x, forWard.y, forWard.z, 0.0f},
			{eye.x, eye.y, eye.z, 0.0f}
			}
		};

#ifdef _DEBUG

		ImGui::Begin("Spherical Camera");
		ImGui::DragFloat("Radius", &s.radius, 0.05f, 0.1f, 100.0f);
		ImGui::SliderAngle("Theta", &s.theta, -89.0f, 89.0f);
		ImGui::SliderAngle("Phi", &s.phi, -180.0f, 180.0f);
		ImGui::DragFloat3("Target", &target.x, 0.05f);
		if (ImGui::Button("Reset")) {
			s={6.0f, 0.0f, -halfPi};
		}


		ImGui::Text("Eye: (%.2f, %.2f, %.2f)", eye.x, eye.y, eye.z);
		ImGui::Text("Right: (%.2f, %.2f, %.2f)", right.x, right.y, right.z);
		ImGui::Text("Up: (%.2f, %.2f, %.2f)", up.x, up.y, up.z);
		ImGui::Text("forward: (%.2f, %.2f, %.2f)", forWard.x, forWard.y, forWard.z);
		if (ImGui::BeginTable("cameraMatrix", 4, ImGuiTableFlags_Borders)) {
			for (int row = 0; row < 4; ++row) {
				ImGui::TableNextRow();
				for (int col = 0; col < 4; ++col) {
					ImGui::TableSetColumnIndex(col);
					ImGui::Text("%7.3f", cameraMatrix.m[row][col]);
				}
			}
			ImGui::EndTable();
		}
		ImGui::End();

#endif // _DEBUG


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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