//===================================================
//
// モーションヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _MOTION_H_
#define _MOTION_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CModel;		//モデル

//--------------------------------
// マクロ定義
//--------------------------------
#define MAX_PARTS	(7)		//モデルパーツの最大数
#define MAX_KEY		(5)		//キーの最大数

//================================
// モーションクラスの定義
//================================
class CMotion
{
public:
	//------------------------
	// キー要素
	//------------------------
	struct KEY
	{
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//向き
	};

	//------------------------
	// キー情報
	//------------------------
	struct KEY_SET
	{
		int nFrame;	//フレーム数
		KEY aKey[MAX_PARTS];
	};

	//------------------------
	// モーション情報
	//------------------------
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
		float fDamageMag;			//ダメージ倍率(magnification)
	};

	//------------------------
	// モーションの種類
	//------------------------
	enum MOTION_TYPE
	{
		MOTION_IDOL = 0,	//待機
		MOTION_MOVE,		//移動
		MOTION_ATTACK_1,	//通常攻撃(1段目)
		MOTION_ATTACK_2,	//通常攻撃(2段目)
		MOTION_MAX
	};

	CMotion();	//コンストラクタ
	~CMotion();	//デストラクタ

	//------------------------
	// メンバ関数
	//------------------------
	void Uninit();
	void Update();
	void Draw(D3DXMATRIX mtxWorld);

	void SetModel();											//モデルの設定
	void SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey);	//モーションの設定
	CMotion::MOTION_TYPE ChangeMotion(MOTION_TYPE type);		//モーションの変更
	void GetFileMotion();										//ファイルを使ったモーションの取得

	//------------------------
	// ゲッター
	//------------------------
	MOTION_SET GetMotionSet() { return motionSet; }

private:
	//------------------------
	// メンバ変数
	//------------------------
	int m_nCurrentKey;						//現在のキー番号
	int m_nCntMotion;						//モーションカウンター
	MOTION_SET m_aMotionSet[MOTION_MAX];	//モーション情報
	MOTION_TYPE m_type;						//現在のモーション
	CModel*  m_pModel[MAX_PARTS];			//モデル
	MOTION_SET motionSet;
};

#endif