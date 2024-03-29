//===================================================
//
// プレイヤーヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CModel;		//モデル
class CLine;		//ライン
class CHP;			//HP
class CCombo;		//コンボ
class CDamage;		//ダメージ
class COrbit;		//軌跡
class CBullet;		//弾
class CMessage;		//メッセージ
class CBg;			//背景

//--------------------------------
// マクロ定義
//--------------------------------
#define MAX_PARTS	(7)		//モデルパーツの最大数
#define MAX_KEY		(24)	//キーの最大数

//================================
// プレイヤークラスの定義
//================================
class CPlayer : public CObject
{
public:
	//-------------------------
	// キー要素
	//-------------------------
	struct KEY
	{
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//向き
	};

	//-------------------------
	// キー情報
	//-------------------------
	struct KEY_SET
	{
		int nFrame;	//フレーム数
		KEY aKey[MAX_PARTS];
	};

	//-------------------------
	// モーション情報
	//-------------------------
	struct MOTION_SET
	{
		bool bLoop;					//ループするかどうか
		int nNumKey;				//キーの最大数
		KEY_SET aKeySet[MAX_KEY];	//キーセット情報

		int nStartCollision;		//当たり判定の開始時間
		int nNextAtkTime;			//次の攻撃の入力開始時間
		int nNumPoint;				//消費ポイント数
		int nCritical;				//クリティカル率
		int nNumHit;				//ヒット数
		int nHitInterval;			//ヒット間隔
		int nBreakDamage;			//ブレイクゲージに与えるダメージ
		float fDamageMag;			//ダメージ倍率(magnification)
	};

	//-------------------------
	// プレイヤーのステータス
	//-------------------------
	struct PLAYER_STATUS
	{
		int nAttackTime;	//攻撃時間
		int nComboValue;	//コンボの加算値
		float nAttack;		//攻撃力
		float fSpeed;		//速度
		float fLife;		//体力
		float fRemLife;		//残り体力(%)
		float fMaxLife;		//最大体力
		bool bAvoidance;	//回避状態
	};

	//-------------------------
	// モーションの種類
	//-------------------------
	enum MOTION_TYPE
	{
		MOTION_IDOL = 0,	//待機
		MOTION_MOVE,		//移動
		MOTION_ATTACK_1,	//通常攻撃(1段目)
		MOTION_ATTACK_2,	//通常攻撃(2段目)
		MOTION_ATTACK_SPIN,	//回転切り
		MOTION_MAX
	};

	CPlayer();				//コンストラクタ
	~CPlayer() override;	//デストラクタ

	//-------------------------
	// メンバ関数
	//-------------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//-------------------------
	// セッター
	//-------------------------
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }						//位置の設定
	void SetAttack(float nAttack)	  { m_status.nAttack = nAttack; }		//攻撃力の設定
	void SetSpeed(float fSpeed)		  { m_status.fSpeed = fSpeed; }			//速度の設定
	void SetComboValue(int nValue)    { m_status.nComboValue = nValue; }	//コンボの加算値の設定
	void AddCombo(int nNumber);		//コンボ数の加算
	void AddLife(float fDamage);	//HP減少時の処理
	void NockBack(float fHeight);	//ノックバック

	//-------------------------
	// ゲッター
	//-------------------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }				//位置の取得
	D3DXVECTOR3 GetRot()			   { return m_rot; }				//向きの取得
	D3DXVECTOR3 GetSize()			   { return m_size; }				//大きさの取得
	D3DXMATRIX GetmtxWorld()		   { return m_mtxWorld; }			//ワールドマトリックスの取得
	const int GetSwordNumber()		   { return nSwordNumber; }			//剣モデルの番号を取得
	float GetWidth() override		   { return 0.0f; }					//幅の取得
	float GetHeight() override		   { return 0.0f; }					//高さの取得
	bool GetHitAttack()				   { return m_bFinishAttack; }		//攻撃を当てたかを取得
	bool GetAvoidance()				   { return m_status.bAvoidance; }	//回避状態の取得
	CModel* GetModel(int nNum)		   { return m_pModel[nNum]; }		//モデルの取得
	CCombo* GetCombo()				   { return m_pCombo; }				//コンボの取得
	PLAYER_STATUS GetStatus()		   { return m_status; }				//ステータスの取得
	MOTION_TYPE GetMotion()			   { return m_type; }				//モーションの種類の取得

	//-------------------------
	// 静的メンバ関数
	//-------------------------
	static CPlayer* Create();

private:
	//-------------------------
	// プライベート関数
	//-------------------------
	void MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey);	//移動
	void MoveJoypad();			//ジョイパッドを使った移動
	void SetRot();				//角度の設定

	void AttackManager();		//攻撃の管理
	void EnhanceAttack();		//攻撃を強化する処理
	void Attack();				//攻撃処理
	void HitSword();			//剣が当たった処理
	bool GetOutAttack(bool equal, bool and);	//攻撃状態かどうかを返す

	void SetModel();											//モデルの設定
	void GetFileMotion();										//ファイルを使ったモーションの取得
	void SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey);	//モーションの設定
	void ChangeMotion(MOTION_TYPE type);						//モーションの変更

private:
	//-------------------------
	// 定数
	//-------------------------
	static const int nMaxLine = 12;				//線の最大数
	static const int nSwordNumber = 4;			//剣モデルの番号
	static const int nResetModeTime = 600;		//モードチェンジ終了までの時間
	static const int nNeedEnhanceCombo = 30;	//攻撃の強化に必要なコンボ数
	static const float fDefaultAttack;			//基本の攻撃力
	static const float fDefaultSpeed;			//基本の速度

	//-------------------------
	// メンバ変数
	//-------------------------
	D3DXMATRIX	m_mtxWorld;		//ワールドマトリックス
	D3DXMATRIX	m_mtxRot;		//回転マトリックス(保存用)
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//前の位置
	D3DXVECTOR3 m_Offset;		//剣先へのオフセット座標
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目的の向き
	D3DXVECTOR3 m_vtxMax;		//大きさの最大値
	D3DXVECTOR3 m_vtxMin;		//大きさの最小値
	D3DXVECTOR3 m_size;			//大きさ
	D3DXVECTOR3 m_worldMin;		//ワールド上の最大値
	D3DXVECTOR3	m_worldMax;		//ワールド上の最小値
	int m_nNumCombo;			//コンボ数
	int m_nCntHit;				//ヒット数を数える
	int m_nHitTime;				//ヒットまでの時間を数える
	int m_nCntModeTime;			//モード終了までの時間を数える
	int m_nAvoidTime;			//回避時間
	int m_nAvoidStan;			//回避硬直
	int m_nWheelRotValue;		//タイヤの回転量
	int m_nBulletTime;			//弾の発射時間
	int m_nCntAttack;			//連撃数を数える
	float m_fSizeWidth;			//サイズ(幅)
	float m_fSizeDepth;			//サイズ(奥行き)
	float m_fGravity;			//重力の値
	float m_rotWheel;			//タイヤの回転量
	bool m_bFinishAttack;		//攻撃が終わった状態
	bool m_bHit;				//1ヒットした状態
	bool m_bNockBack;			//ノックバックしている状態
	bool m_bEnhance;			//技が強化される状態
	bool m_bCntAttack;			//連撃数を数える状態
	CHP*		m_pHP;			//HP
	CCombo*		m_pCombo;		//コンボ
	CDamage*	m_pDamage;		//ダメージ
	COrbit*		m_pOrbit;		//軌跡
	CBullet*	m_pBullet;		//弾
	CMessage*	m_pMessage;		//メッセージ
	CBg*		m_pBg;			//背景

	//列挙型
	PLAYER_STATUS m_status;		//ステータス

	/* ↓ モデル情報 ↓ */
	CModel*  m_pModel[MAX_PARTS];		//モデル
	CLine* m_pLine[nMaxLine];			//ライン

	/* ↓ モーション情報 ↓ */
	int m_nCurrentKey;						//現在のキー番号
	int m_nCntMotion;						//モーションカウンター
	MOTION_SET m_aMotionSet[MOTION_MAX];	//モーション情報
	MOTION_TYPE m_type;						//現在のモーション
	MOTION_TYPE m_typeOld;					//1つ前のモーション
};

#endif