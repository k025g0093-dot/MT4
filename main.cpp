
#include <Novice.h>
#include <cmath>
#include <cstdint>
#ifdef _DEBUG
#include <imgui.h>
#endif
#include "Collision.h"
#include <algorithm> // std::max, std::min 用
#include <chrono>
#include "Quaternion.h"

const char kWindowTitle[] = "LE2B_29_ヤマトユウヤ_タイトル";
static const int kRowHeight = 20;
static const int kColumnWidth = 60;


Quaternion rotation0 = MakeRotateAxisAngleQuaternion({0.71f, 0.71f, 0.0f}, 0.3f);
Quaternion rotation1 = MakeRotateAxisAngleQuaternion({0.71f, 0.0f, 0.71f}, 3.141592f);

Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};



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



		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

				// x, y, z, w を1行に並べ、最後に名前を表示する
		auto printQuaternion = [](int row, const Quaternion& q, const char* label) {
			int y = row * kRowHeight;
			Novice::ScreenPrintf(kColumnWidth * 0, y, "%6.02f", q.x);
			Novice::ScreenPrintf(kColumnWidth * 1, y, "%6.02f", q.y);
			Novice::ScreenPrintf(kColumnWidth * 2, y, "%6.02f", q.z);
			Novice::ScreenPrintf(kColumnWidth * 3, y, "%6.02f", q.w);
			Novice::ScreenPrintf(kColumnWidth * 4, y, ": %s", label);
		};


		// 名前を1行表示し、その下に4x4を並べる
		auto printMatrix = [](int row, const Matrix4x4& m, const char* label) {
			Novice::ScreenPrintf(0, row * kRowHeight, "%s", label);
			for (int r = 0; r < 4; ++r) {
				for (int c = 0; c < 4; ++c) {
					Novice::ScreenPrintf(kColumnWidth * c, (row + 1 + r) * kRowHeight, "%6.03f", m.m[r][c]);
				}
			}
		};

		printQuaternion(0, interpolate0, "interpolate0, Slerp(q0, q1, 0.0f)");
		printQuaternion(1, interpolate1, "interpolate1, Slerp(q0, q1, 0.3f)");
		printQuaternion(2, interpolate2, "interpolate2, Slerp(q0, q1, 0.5f)");
		printQuaternion(3, interpolate3, "interpolate3, Slerp(q0, q1, 0.7f)");
		printQuaternion(4, interpolate4, "interpolate4, Slerp(q0, q1, 1.0f)");



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