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
class CModel;	//モデル
class CLine;	//ライン
class CHP;		//HP
class CCombo;	//コンボ

//--------------------------------
// マクロ定義
//--------------------------------
#define MAX_PARTS	(7)		//モデルパーツの最大数
#define MAX_KEY		(4)		//キーの最大数

//================================
// プレイヤークラスの定義
//================================
class CPlayer : public CObject
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
		KEY aKey[MAX_PARTS];
	};

	//モーション情報
	struct MOTION_SET
	{
		bool bLoop;					//ループするかどうか
		int nNumKey;				//キーの最大数
		KEY_SET aKeySet[MAX_KEY];	//キーセット情報
	};

	//プレイヤーのステータス
	struct PLAYER_STATUS
	{
		int nAttackTime;	//攻撃時間
		int nComboValue;	//コンボの加算値
		float nAttack;		//攻撃力
		float fSpeed;		//速度
		float fLife;		//体力
		float fRemLife;		//残り体力(%)
		float fMaxLife;		//最大体力
	};

	//モーションの種類
	enum MOTION_TYPE
	{
		MOTION_TYPE_IDOL = 0,	//待機
		MOTION_TYPE_MOVE,		//移動
		MOTION_TYPE_ATTACK,		//攻撃
		MOTION_TYPE_MAX
	};

	//プレイヤーのモード
	enum BATTLEMODE
	{
		BATTLEMODE_NONE = 0,	//通常
		BATTLEMODE_ATTACK,		//攻撃
		BATTLEMODE_SPEED,		//速度
		BATTLEMODE_COMBO,		//連撃
		BATTLEMODE_MAX
	};

	CPlayer();				//コンストラクタ
	~CPlayer() override;	//デストラクタ

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
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }						//位置の設定
	void SetAttack(float nAttack)	  { m_status.nAttack = nAttack; }		//攻撃力の設定
	void SetSpeed(float fSpeed)		  { m_status.fSpeed = fSpeed; }			//速度の設定
	void SetComboValue(int nValue)    { m_status.nComboValue = nValue; }	//コンボの加算値の設定
	void AddCombo(int nNumber);	//コンボ数の加算

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }			//位置の取得
	D3DXVECTOR3 GetRot()			   { return m_rot; }			//向きの取得
	float GetWidth() override		   { return 0.0f; }				//幅の取得
	float GetHeight() override		   { return 0.0f; }				//高さの取得
	D3DXMATRIX GetmtxWorld()		   { return m_mtxWorld; }		//ワールドマトリックスの取得
	CModel* GetModel(int nNum)		   { return m_pModel[nNum]; }	//モデルの取得
	PLAYER_STATUS GetStatus()		   { return m_status; }			//ステータスの取得
	BATTLEMODE GetMode()			   { return m_battleMode; }		//バトルモードの取得
	MOTION_TYPE GetMotion()			   { return m_type; }			//モーションの種類の取得
	bool GetCollisionPlayer();	//当たり判定の取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CPlayer* Create();

private:
	//---------------------
	// プライベート関数
	//---------------------
	void SetModel();											//モデルの設定
	void SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey);	//モーションの設定
	void ChangeMotion(MOTION_TYPE type);	//モーションの変更
	void GetFileMotion();					//ファイルを使ったモーションの取得
	void MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey);	//移動
	void MoveJoypad();		//ジョイパッドを使った移動
	void Attack();			//攻撃処理
	void ChangeMode();		//モードチェンジ
	void SetRot();			//角度の設定
	void SetLine();			//線の設置
	void UpdateLine();		//線の更新

private:
	//----------------
	// 定数
	//----------------
	static const int nMaxLine = 12;				//線の最大数
	static const int nWheelRotValue = 10;		//タイヤの回転量
	static const int nSwordNumber = 4;			//剣モデルの番号
	static const float fDefaultAttack;			//基本の攻撃力
	static const float fDefaultSpeed;			//基本の速度

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
	D3DXVECTOR3 m_size;			//大きさ
	D3DXVECTOR3 m_worldMin;		//ワールド上の最大値
	D3DXVECTOR3	m_worldMax;		//ワールド上の最小値
	int m_nNumCombo;			//コンボ数
	float fSizeWidth;			//サイズ(幅)
	float fSizeDepth;			//サイズ(奥行き)
	float m_rotWheel;			//タイヤの回転量
	bool m_bDamage;				//ダメージを与えた
	PLAYER_STATUS m_status;		//ステータス
	BATTLEMODE m_battleMode;	//バトルモード
	CHP*  m_pHP;				//HP
	CCombo* m_pCombo;			//コンボ

	/* ↓ モデル情報 ↓ */
	CModel*  m_pModel[MAX_PARTS];		//モデル
	CLine* m_pLine[nMaxLine];			//ライン

	/* ↓ モーション情報 ↓ */
	int m_nCurrentKey;							//現在のキー番号
	int m_nCntMotion;							//モーションカウンター
	MOTION_SET m_aMotionSet[MOTION_TYPE_MAX];	//モーション情報
	MOTION_TYPE m_type;							//現在のモーション

	/* ↓ クォータニオン ↓ */
	D3DXMATRIX		m_mtxRot;		//回転マトリックス(保存用)
	D3DXQUATERNION	m_quat;			//クォータニオン
	D3DXVECTOR3		m_vecAxis;		//回転軸
	float			m_fValueRot;	//回転角(ラジアン)
};

#endif