#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "AABB.h"

class MapChipField;
class Player;
class Enemy {
public:

	Enemy() = default;
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();


	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突応答
	/// </summary>
	void OnCollision(const Player* player);
	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	const Vector3& GetWorldPos() { return worldTransform_.translation_; }

	/// <summary>
	/// AABBの取得
	/// </summary>
	AABB GetAABB();


	void SetPos(Vector3 pos) { worldTransform_.translation_ = pos; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:

	//歩行の速さ
	static inline const float kWalkSpeed = 0.05f;
	//最初の角度[度]
	static inline const float kWalkMotionAngleStart = 30.0f;
	//最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 0.0f;
	//アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;
	//キャラクターの当たり判定のサイズ
	static inline const float kWidth = 1.8f; //横幅
	static inline const float kHeight = 1.8f; //縦幅

	//速度
	Vector3 velocity_;
	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	//経過時間
	float walkTimer_;

};

