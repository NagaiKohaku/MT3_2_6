#include <Novice.h>
#include <Novice.h>
#include <imgui.h>
#include "MakeMatrix.h"
#include "MatrixMath.h"
#include "Vector3Math.h"
#include "Draw.h"
#include "algorithm"

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_MT2_3_6 3次元の衝突判定(AABBと球)";

bool isCollision(const AABB& aabb, const Sphere& sphere);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//ウィンドウサイズ
	float kWindowWidth = 1280.0f;
	float kWindowHeight = 720.0f;

	//カメラ:平行移動
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	//カメラ:回転
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	AABB aabb{
		.min{-0.5f,0.0f,-0.5f},
		.max{0.5f,0.5f,0.5f}
	};

	Sphere sphere{
		.center{0.0f,1.0f,0.0f},
		.radius{0.3f},
	};

	Draw draw;

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

		if (ImGui::TreeNode("Camera")) {
			ImGui::DragFloat3("Translate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("AABB")) {
			ImGui::DragFloat3("min", &aabb.min.x, 0.01f);
			ImGui::DragFloat3("max", &aabb.max.x, 0.01f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sphere")) {
			ImGui::DragFloat3("center", &sphere.center.x, 0.01f);
			ImGui::DragFloat("radius", &sphere.radius, 0.01f);
			ImGui::TreePop();
		}

		aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
		aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
		aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
		aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
		aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
		aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);

		draw.Pipeline(cameraTranslate, cameraRotate, kWindowWidth, kWindowHeight);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		draw.DrawGrid();
		if (isCollision(aabb,sphere)) {
			draw.DrawAABB(aabb, RED);
		} else {
			draw.DrawAABB(aabb, WHITE);
		}

		draw.DrawSphere(sphere, WHITE);

		///
		/// ↑描画処理ここまで
		///

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

bool isCollision(const AABB& aabb, const Sphere& sphere) {

	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z),
	};

	float distance = Length(Subtract(closestPoint,sphere.center));

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}