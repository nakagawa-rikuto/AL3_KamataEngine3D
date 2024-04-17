#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_; 

	/// *************************************
	/// 解放
	/// *************************************
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルの生成
	model_ = Model::Create();

	/// *************************************
	/// 初期化
	/// *************************************
	//  要素数
	const uint32_t kNumBlockHorizontal = 20;

	//  ブロック１個分の横幅
	const float kBlockWidth = 2.0f;

	//  要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	//  キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
	
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}
}

void GameScene::Update() {

	/// *************************************
	/// 更新
	/// *************************************

	//  ブロックの更新
	for (WorldTransform* worldTransformBlock_ : worldTransformBlocks_) {
		
		/// スケーリング行列の作成
		worldTransformBlock_->scale_ = {1, 1, 1};

		/// X軸周り回転行列の作成
		worldTransformBlock_->rotation_ = {0, 0, 0};

		/// Y軸周り回転行列の作成
		worldTransformBlock_->rotation_

		/// Z軸周り回転行列の作成
		
		/// 回転行列の合成(Z回転・X回転・Y回転)
		
		/// 平行移動行列の作成
		worldTransformBlock_->translation_ = {0, 0, 0};
		
		/* worldTransformBlock_->matWorld_ = 
		           スケーリング行列 * 回転行列 * 平行移動行列*/

		// 定数バッファに転送する
		worldTransformBlock_->TransferMatrix();

	}
}

void GameScene::Draw() {

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
