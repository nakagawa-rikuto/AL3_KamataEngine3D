#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"

#include "ClearScene.h"
#include "GameScene.h"
#include "LoseScene.h"
#include "TitleScene.h"

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
ClearScene* clearScene = nullptr;
LoseScene* loseScene = nullptr;

// シーン（型）
enum class Scene {

	kUnknown = 0,
	kTitle,
	kGame,
	kClera,
	kLose,
};

// 現在のシーン（型）
Scene scene = Scene::kUnknown;

/// <summary>
/// シーンの切り替え処理
/// </summary>
void ChangScene() {

	switch (scene) {
	case Scene::kTitle:

		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;

			// 旧シーン
			delete titleScene;
			titleScene = nullptr;

			// 新シーンの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:

		if (gameScene->isFinished()) {
			if (gameScene->isCheckPhase() == gameScene->GetClearPhase()) {
				// シーン変更
				scene = Scene::kClera;

				// 旧シーン
				delete gameScene;
				gameScene = nullptr;

				// 新シーンの生成と初期化
				clearScene = new ClearScene;
				clearScene->Initialise();

			} else if (gameScene->isCheckPhase() == gameScene->GetLosePhase()) {
			
				// シーン変更
				scene = Scene::kLose;

				// 旧シーン
				delete gameScene;
				gameScene = nullptr;

				// 新シーンの生成と初期化
				loseScene = new LoseScene;
				loseScene->Initialise();
			}
		}

		break;
	case Scene::kLose:

		if (loseScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;

			// 旧シーン
			delete loseScene;
			loseScene = nullptr;

			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialise();
		}

		break;
	case Scene::kClera:

		if (clearScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;

			// 旧シーン
			delete clearScene;
			clearScene = nullptr;

			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialise();
		}

		break;
	}
}

/// <summary>
/// シーンの更新
/// </summary>
void UpdateScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClera:
		clearScene->Update();
		break;
	case Scene::kLose:
		loseScene->Update();
		break;
	}
}

/// <summary>
/// シーンの描画
/// </summary>
void DrawScene() {

	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClera:
		clearScene->Draw();
		break;
	case Scene::kLose:
		loseScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2B_18_ナカガワ_リクト_AL3");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// 最初のシーンの初期化
	scene = Scene::kTitle;

	// タイトルシーンの初期化
	titleScene = new TitleScene();
	titleScene->Initialise();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// シーン切り替え
		ChangScene();
		// 現在シーン更新
		UpdateScene();
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// 現在シーンの描画
		DrawScene();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete titleScene;
	delete gameScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}