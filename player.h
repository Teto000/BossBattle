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

	//モーションの種類
	enum MOTION_TYPE
	{
		MOTION_TYPE_IDOL = 0,	//待機
		MOTION_TYPE_MOVE,		//移動
		MOTION_TYPE_ATTACK,		//攻撃
		MOTION_TYPE_MAX
	};

	//モーション情報
	struct MOTION_SET
	{
		bool bLoop;					//ループするかどうか
		int nNumKey;				//キーの最大数
		KEY_SET aKeySet[MAX_KEY];	//キーセット情報
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
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	//位置の設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }			//位置の取得
	D3DXVECTOR3 GetRot()			   { return m_rot; }			//向きの取得
	float GetWidth() override		   { return 0.0f; }				//幅の取得
	float GetHeight() override		   { return 0.0f; }				//高さの取得
	D3DXMATRIX GetmtxWorld()		   { return m_mtxWorld; }		//ワールドマトリックスの取得
	CModel* GetModel(int nNum)		   { return m_pModel[nNum]; }	//モデルの取得
	BATTLEMODE GetMode()			   { return m_battleMode; }		//バトルモードの取得
	bool GetCollisionPlayer();	//当たり判定の取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CPlayer* Create();

private:
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
	static const int nMaxLine = 128;		//最大体力
	static const int nWheelRotValue = 10;	//タイヤの回転量
	static const float fSpeed;				//移動速度

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
	int m_nCntAttackTime;		//攻撃時間
	float m_fLife;				//体力
	float m_fRemLife;			//残り体力
	float fSizeWidth;			//サイズ(幅)
	float fSizeDepth;			//サイズ(奥行き)
	float m_rotWheel;			//タイヤの回転量
	BATTLEMODE m_battleMode;	//バトルモード
	CHP*  m_pHP;				//HP

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