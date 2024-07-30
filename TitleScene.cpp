#include "TitleScene.h"

/* //////////////////////////////////////////
                    コンストラクト
*/ //////////////////////////////////////////
TitleScene::TitleScene() {}

/* //////////////////////////////////////////
                    デストラクト
*/ //////////////////////////////////////////
TitleScene::~TitleScene() {

	delete modelText_;
	delete modelPlayer_;
}

// Getter
bool TitleScene::IsFinished() { return finished_; }

/* //////////////////////////////////////////
                    初期化
*/ //////////////////////////////////////////
void TitleScene::Initialise() {

	/// *************************************
	/// 初期化
	/// *************************************
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	modelText_ = Model::CreateFromOBJ("Text", true);
	modelPlayer_ = Model::CreateFromOBJ("Player", true);

	viewProjection_.Initialize();
	viewProjection_.translation_ = {0.0f, 0.0f, -10.0f};

	text_.Initialize();
	text_.translation_ = {0.0f, 50.0f, 0.0f};

	player_.Initialize();
	player_.translation_ = {0.0f, -30.0f, 0.0f};
}

/* //////////////////////////////////////////
                    更新
*/ //////////////////////////////////////////
void TitleScene::Update() {

#ifdef _DEBUG

	ImGui::Begin("TitleScene");
	ImGui::DragFloat3("view", &viewProjection_.translation_.x, 10.0f);
	ImGui::DragFloat3("text", &text_.translation_.x, 10.0f);
	ImGui::DragFloat3("player", &player_.translation_.x, 10.0f);
	ImGui::End();

#endif // DEBUG

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {

		finished_ = true;
	}

	float speed = 0.1f;
	text_.translation_.y += speed;
	if (text_.translation_.y >= 100.0f || text_.translation_.y <= -50.0f) {
		speed *= -1.0f;
	}

	viewProjection_.UpdateMatrix();
	text_.UpdateMatrix();
	player_.UpdateMatrix();
}

/* //////////////////////////////////////////
                    描画
*/ //////////////////////////////////////////
void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	/// *************************************
	/// 描画
	/// *************************************
	modelText_->Draw(text_, viewProjection_);
	modelText_->Draw(player_, viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
