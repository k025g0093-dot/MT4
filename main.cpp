
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


Vector3 from0 = Normalize(Vector3{1.0f, 0.7f, 0.5f});
Vector3 to0 = -from0;
Vector3 from1 = Normalize(Vector3{-0.6f, 0.9f, 0.2f});
Vector3 to1 = Normalize(Vector3{0.4f, 0.7f, -0.5f});

// 四元数を求める
Matrix4x4 rotateMatrix0 = MakeRotateMatrix(DirectionToDirectuin(
	Normalize(Vector3{1.0f, 0.0f, 0.0f}),
	Normalize(Vector3{-1.0f, 0.0f, 0.0f})));
Matrix4x4 rotateMatrix1 = MakeRotateMatrix(DirectionToDirectuin(from0, to0));
Matrix4x4 rotateMatrix2 = MakeRotateMatrix(DirectionToDirectuin(from1, to1));

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

		printMatrix(0, rotateMatrix0, "rotateMatrix0");
		printMatrix(5, rotateMatrix1, "rotateMatrix1");
		printMatrix(10, rotateMatrix2, "rotateMatrix2");



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