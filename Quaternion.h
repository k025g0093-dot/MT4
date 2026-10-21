#pragma once
#include <algorithm>
#include <iostream>

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

// Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {

	Quaternion pr{};

	pr.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	pr.x = lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x;
	pr.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;
	pr.z = lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z;

	return pr;
}

// 乗法単位元を返す　
Quaternion IdentityQuaternion() {
	Quaternion pr{0, 0, 0, 1};
	return pr;
}

// 共役のQuaternionを返す
Quaternion Conjugate(const Quaternion& quaternion) {

	Quaternion pr{};
	pr.w = quaternion.w;
	pr.x = -quaternion.x;
	pr.y = -quaternion.y;
	pr.z = -quaternion.z;
	return pr;
}

// Quaternionのnormを返す
float Norm(const Quaternion& quaternion) {

	float pr{};
	pr = std::sqrt(quaternion.w * quaternion.w + quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	return pr;
}

float NormSquared(const Quaternion& quaternion) {

	float pr{};
	pr = (quaternion.w * quaternion.w + quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	return pr;
}

// 正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion) {

	Quaternion pr{};
	float norm = Norm(quaternion);
	pr.w = quaternion.w / norm;
	pr.x = quaternion.x / norm;
	pr.y = quaternion.y / norm;
	pr.z = quaternion.z / norm;
	return pr;
}

// 逆Quaternionを返す関数
Quaternion Inverse(const Quaternion& quaternion) {

	Quaternion pr{};
	float norm = NormSquared(quaternion);
	Quaternion quat = Conjugate(quaternion);

	pr.w = quat.w / norm;
	pr.x = quat.x / norm;
	pr.y = quat.y / norm;
	pr.z = quat.z / norm;

	return pr;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, const float angle) {

	Quaternion q{};

	q.w = std::cos(angle / 2);
	q.x = std::sin(angle / 2) * axis.x;
	q.y = std::sin(angle / 2) * axis.y;
	q.z =  std::sin(angle / 2) * axis.z;

	return q;
}

Vector3 RotateVector(
	const Vector3& vector, 
	const Quaternion& quaternion) {

	Vector3 q{};

	Quaternion qm = {vector.x, vector.y, vector.z,0};
	Quaternion quat = Multiply(quaternion, Multiply(qm,Conjugate(quaternion)));

	q.x = quat.x;
	q.y = quat.y;
	q.z = quat.z;

	return q;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {

	Matrix4x4 mRotate{};

	mRotate.m[0][0] = {(quaternion.w * quaternion.w) + (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z)};
	mRotate.m[0][1] = {2 * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z))};
	mRotate.m[0][2] = {2 * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y))};
	mRotate.m[0][3] = 0;

	mRotate.m[1][0] = {2 * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z))};
	mRotate.m[1][1] = {(quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z)};
	mRotate.m[1][2] = {2 * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x))};
	mRotate.m[1][3] = 0;

	mRotate.m[2][0] = {2 * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y))};
	mRotate.m[2][1] = {2 * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x))};
	mRotate.m[2][2] = {(quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z)};
	mRotate.m[2][3] = 0;

	mRotate.m[3][0] = 0;
	mRotate.m[3][1] = 0;
	mRotate.m[3][2] = 0;
	mRotate.m[3][3] = 1;

	return mRotate;
}
