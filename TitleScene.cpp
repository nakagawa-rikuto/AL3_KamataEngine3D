#include "TitleScene.h"

/* //////////////////////////////////////////
                    コンストラクト
*/ //////////////////////////////////////////
TitleScene::TitleScene() {}

/* //////////////////////////////////////////
                    デストラクト
*/ //////////////////////////////////////////
TitleScene::~TitleScene() {

	delete fade_;
	delete modelText_;
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

	viewProjection_.Initialize();

	text_.Initialize();

	player_.Initialize();

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

/* //////////////////////////////////////////
                    更新
*/ //////////////////////////////////////////
void TitleScene::Update() {

#ifdef _DEBUG

	ImGui::Begin("TitleScene");
	ImGui::DragFloat3("view", &viewProjection_.translation_.x, 1.0f);
	ImGui::DragFloat3("text", &text_.translation_.x, 1.0f);
	ImGui::DragFloat3("player", &player_.translation_.x, 1.0f);
	ImGui::End();

#endif // DEBUG

	switch (phase_) {
	case Phase::kFadeIn:

		fade_->Update();
		if (fade_->IsFinished()) {

			phase_ = Phase::kMain;
		}

		break;
	case Phase::kMain:

		if (Input::GetInstance()->PushKey(DIK_SPACE)) {

			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}

		break;
	case Phase::kFadeOut:

		fade_->Update();
		if (fade_->IsFinished()) {

			finished_ = true;
		}

		break;
	}

	text_.translation_.y += speed_;
	if (text_.translation_.y >= 1.0f || text_.translation_.y <= -1.0f) {
		speed_ *= -1.0f;
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

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画

	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// フェードの描画
	fade_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
