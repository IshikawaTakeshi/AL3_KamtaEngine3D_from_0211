#pragma once

#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "AABB.h"

class MapChipField;
class Enemy;
enum class LRDirection {
	kRight,
	kLeft,
};

/// <summary>
/// プレイヤー
/// </summary>
class Player {
public:

	//マップとの当たり判定情報
	struct CollisionMapInfo {
		bool touchCeiling = false; //天井衝突フラグ
		bool landing = false; //着地状態フラグ
		bool touchWall = false; //壁接触フラグ
		Vector3 move; //移動量
	};

	//角
	enum Corner {
		kRightBottom, //右下
		kLeftBottom, //左下
		kRightTop, //右上
		kLeftTop, //左上

		kNumCorner //要素数
	};

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
	/// 移動入力処理
	/// </summary>
	void Move();

	/// <summary>
	/// 上方向衝突判定
	/// </summary>
	void IsCollisionTop(CollisionMapInfo& info);

	/// <summary>
	/// 下方向衝突判定
	/// </summary>
	void IsCollisionBottom(CollisionMapInfo& info);

	/// <summary>
	/// 右方向衝突判定
	/// </summary>
	void IsCollisionRight(CollisionMapInfo& info);

	/// <summary>
	/// 左方向衝突判定
	/// </summary>
	void IsCollisionLeft(CollisionMapInfo& info);

	/// <summary>
	/// 衝突判定結果を反映した移動処理
	/// </summary>
	void MovementByCollision(const CollisionMapInfo& info);

	/// <summary>
	/// 天井に接触している場合の処理
	/// </summary>
	void ProcessWhenTouchCeiling(CollisionMapInfo& info);

	/// <summary>
	/// 壁に接触している場合の処理
	/// </summary>
	void ProcessWhenTouchWall(CollisionMapInfo& info);

	/// <summary>
	/// 接地状態の切り替え処理
	/// </summary>
	void SwitchGroundState(CollisionMapInfo& info);
	/// <summary>
	/// 指定した角の座標計算
	/// </summary>
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	/// <summary>
	/// worldTransformの取得
	/// </summary>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// velocityの取得
	/// </summary>
	const Vector3& GetVelocity() { return velocity_; }

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	const Vector3& GetWorldPos() { return worldTransform_.translation_; }

	/// <summary>
	/// 衝突応答
	/// </summary>
	void OnCollision(const Enemy* enemy);

	/// <summary>
	/// AABBの取得
	/// </summary>
	AABB GetAABB();

	void SetPos(Vector3 pos) { worldTransform_.translation_ = pos; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }



private:

	//加速率
	static inline const float kAcceleration = 0.01f;
	//加速減衰率
	static inline const float kAttenuation = 0.18f;
	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.01f;
	//壁接触時の速度減衰率
	static inline const float kAttenuationWall = 0.5f;
	//最大速度制限
	static inline const float kLimitRunSpeed = 0.5f;
	//旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	//重力加速度(下方向}
	static inline const float kGravityAcceleration = 0.01f;
	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.6f;
	//ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 0.3f;
	//キャラクターの当たり判定のサイズ
	static inline const float kWidth = 1.8f; //横幅
	static inline const float kHeight = 1.8f; //縦幅
	
	static inline const float kBlank = 0.02f;
	static inline const float kBottomBlank = 0.05f;

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	//速度
	Vector3 velocity_ = {};
	//加速度
	Vector3 acceleration_ = {};
	//プレイヤーの向き
	LRDirection lrDirection_ = LRDirection::kRight;
	//衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;
	//接地状態フラグ
	bool onGround_ = true;

};

