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

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

//Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {

	Quaternion pr{};

	pr.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	pr.x = lhs.y * rhs.z - lhs.z * rhs.y  +rhs.w * lhs.x + lhs.w * rhs.x;
	pr.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;
	pr.z = lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z;

	return pr;
}

//乗法単位元を返す　
Quaternion IdentityQuaternion() {
	Quaternion pr{0,0,0,1};
	return pr;
}

//共役のQuaternionを返す
Quaternion Conjugate(const Quaternion& quaternion) {

	Quaternion pr{};
	pr.w = quaternion.w;
	pr.x = -quaternion.x;
	pr.y = -quaternion.y;
	pr.z = -quaternion.z;
	return pr;
}

//Quaternionのnormを返す
float Norm(const Quaternion& quaternion) {

	float pr{};
	pr = std::sqrt(
		quaternion.w * quaternion.w + 
		quaternion.x * quaternion.x + 
		quaternion.y * quaternion.y + 
		quaternion.z * quaternion.z 
		);
	return pr;
}

float NormSquared(const Quaternion& quaternion) {

	float pr{};
	pr = (
		quaternion.w * quaternion.w + 
		quaternion.x * quaternion.x + 
		quaternion.y * quaternion.y + 
		quaternion.z * quaternion.z 
		);
	return pr;
}

//正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion) {

	Quaternion pr{};
	float norm = Norm(quaternion);
	pr.w = quaternion.w / norm;
	pr.x = quaternion.x / norm;
	pr.y = quaternion.y / norm;
	pr.z = quaternion.z / norm;
	return pr;
}

//逆Quaternionを返す関数
Quaternion Inverse(const Quaternion& quaternion) {

	Quaternion pr{};
	float norm = NormSquared(quaternion);
	Quaternion quat = Conjugate(quaternion);

	pr.w = quat.w / norm;
	pr.x= quat.x / norm;
	pr.y = quat.y / norm;
	pr.z = quat.z / norm;

	return pr;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Quaternion q1{2.0f, 3.0f, 4.0f, 1.0f};
	Quaternion q2{1.0f, 3.0f, 5.0f, 2.0f};

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

		Quaternion identity = IdentityQuaternion();
		Quaternion conj = Conjugate(q1);
		Quaternion inv = Inverse(q1);
		Quaternion normal = Normalize(q1);
		Quaternion mul1 = Multiply(q1, q2);
		Quaternion mul2 = Multiply(q2, q1);
		float norm = Norm(q1);

		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		auto printQuaternion = [](int row, const Quaternion& q, const char* label) {
			int y = row * kRowHeight;
			Novice::ScreenPrintf(kColumnWidth * 0, y, "%6.02f", q.x);
			Novice::ScreenPrintf(kColumnWidth * 1, y, "%6.02f", q.y);
			Novice::ScreenPrintf(kColumnWidth * 2, y, "%6.02f", q.z);
			Novice::ScreenPrintf(kColumnWidth * 3, y, "%6.02f", q.w);
			Novice::ScreenPrintf(kColumnWidth * 4, y, ": %s", label);
		};

		printQuaternion(0, identity, "Identity");
		printQuaternion(1, conj, "Conjugate");
		printQuaternion(2, inv, "Inverse");
		printQuaternion(3, normal, "Normalize");
		printQuaternion(4, mul1, "Multiply(q1, q2)");
		printQuaternion(5, mul2, "Multiply(q2, q1)");

		Novice::ScreenPrintf(kColumnWidth * 0, 6 * kRowHeight, "%6.02f", norm);
		Novice::ScreenPrintf(kColumnWidth * 4, 6 * kRowHeight, ": Norm");

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