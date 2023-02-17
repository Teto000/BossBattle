//===================================================
//
// エネミーヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CModel;	//モデル
class CLine;	//ライン
class CHP;		//HP
class CMessage;	//メッセージ

//--------------------------------
// マクロ定義
//--------------------------------
#define MAX_ENEMY_PARTS	(3)		//モデルパーツの最大数
#define MAX_ENEMY_KEY	(24)	//キーの最大数

//================================
// エネミークラスの定義
//================================
class CEnemy : public CObject
{
public:
	//キー要素
	struct KEY
	{
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//向き
	};

	//キー情報
	struct KEY_SET
	{
		int nFrame;	//フレーム数
		KEY aKey[MAX_ENEMY_PARTS];
	};

	//モーション情報
	struct MOTION_SET
	{
		bool bLoop;						//ループするかどうか
		int nNumKey;					//キーの最大数
		KEY_SET aKeySet[MAX_ENEMY_KEY];	//キーセット情報

		int nStartCollision;			//当たり判定の開始時間
		int nDamage;					//ダメージ量
		int nNumHit;					//ヒット数
		int nHitInterval;				//ヒット間隔
	};

	//モーションの種類
	enum MOTION_TYPE
	{
		MOTION_IDOL = 0,	//待機
		MOTION_MOVE,		//移動
		MOTION_ATTACK,		//叩きつけ攻撃
		MOTION_SPIN,		//回転攻撃
		MOTION_MAX
	};

	enum ENEMYSTATE
	{
		ENEMYSTATE_NONE = 0,
		ENEMYSTATE_BREAK,
		ENEMYSTATE_MAX
	};

	enum GAUGE
	{
		GAUGE_HP = 0,
		GAUGE_BREAK,
		GAUGE_MAX
	};

	CEnemy();			//コンストラクタ
	~CEnemy() override;	//デストラクタ

	//----------------
	// メンバ関数
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// セッター
	//----------------
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }			//位置の設定
	void SubGauge(float fDamage, GAUGE type);	//ゲージ減少時の処理

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override	{ return m_pos; }		//位置の取得
	D3DXVECTOR3 GetRot()				{ return m_rot; }		//向きの取得
	float GetWidth() override			{ return 0.0f; }		//幅の取得
	float GetHeight() override			{ return 0.0f; }		//高さの取得
	bool GetHitAtk()				    { return m_bHitAtk; }	//攻撃が当たったかを取得
	D3DXVECTOR3 GetSize()				{ return m_size; }		//大きさの取得
	D3DXMATRIX GetmtxWorld()			{ return m_mtxWorld; }	//ワールドマトリックスの取得
	ENEMYSTATE GetState()				{ return m_state; }		//敵の状態の取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void SetModel();	//モデルの設定
	void SetLine();		//線の設置
	void UpdateLine();	//線の更新
	void EnemyAI();		//敵の動きを管理
	void Attack();		//攻撃処理
	void HitHummer();	//ハンマーの当たり判定
	void NockBack();	//ノックバックする処理
	void Move(D3DXVECTOR3 targetPos);		//移動
	void Rotation(D3DXVECTOR3 targetPos);	//回転

	//モーション
	void GetFileMotion();										//ファイルを使ったモーションの取得
	void SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey);	//モーションの設定
	void ChangeMotion(MOTION_TYPE type);						//モーションの変更

private:
	//----------------
	// 定数
	//----------------
	static const int nMaxLine = 24;	//線の最大数
	static const float fDefGravity;	//基本の重力

	//----------------
	// メンバ変数
	//----------------
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目的の向き
	D3DXVECTOR3 m_vtxMax;		//大きさの最大値
	D3DXVECTOR3 m_vtxMin;		//大きさの最小値
	D3DXVECTOR3 m_worldMin;		//ワールド上の最大値
	D3DXVECTOR3	m_worldMax;		//ワールド上の最小値
	D3DXVECTOR3 m_size;			//大きさ
	int m_nMoveTime;			//移動までの時間
	int m_nAttackTime;			//攻撃までの時間
	int m_nBreakTime;			//ブレイク状態の時間
	int m_nCntFream;			//フレーム数のカウント
	int m_nHitTime;				//ヒットするまでの時間
	int m_nCntHit;				//ヒット数を数える
	float m_fLife;				//体力
	float m_fRemLife;			//残り体力(%)
	float m_fMaxLife;			//最大体力
	float m_fGravity;			//重力の値
	float m_fBreak;
	float m_fRemBreak;
	float m_fMaxBreak;
	bool m_bNockBack;			//ノックバックしたか
	bool m_bHitAtk;				//攻撃が当たったか
	CModel*		m_pModel[MAX_ENEMY_PARTS];	//モデル
	CLine*		m_pLine[nMaxLine];			//ライン
	CHP*		m_pHP[GAUGE_MAX];			//HP
	CMessage*	m_pMessage;					//メッセージ
	ENEMYSTATE m_state;					//敵の状態

	/* ↓ モーション情報 ↓ */
	int m_nCurrentKey;						//現在のキー番号
	int m_nCntMotion;						//モーションカウンター
	MOTION_SET m_aMotionSet[MOTION_MAX];	//モーション情報
	MOTION_TYPE m_type;						//現在のモーション
};

#endif