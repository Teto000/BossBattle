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

//--------------------------------
// マクロ定義
//--------------------------------
#define MAX_ENEMY_PARTS	(1)		//モデルパーツの最大数
#define MAX_ENEMY_KEY	(2)		//キーの最大数

//================================
// プレイヤークラスの定義
//================================
class CEnemy : public CObject
{
public:
	//キー要素
	struct KEY
	{
		float fPosX;	//位置
		float fPosY;
		float fPosZ;
		float fRotX;	//向き
		float fRotY;
		float fRotZ;
	};

	//キー情報
	struct KEY_SET
	{
		int nFrame;	//フレーム数
		KEY aKey[MAX_ENEMY_PARTS];
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
	void SetPosition(D3DXVECTOR3 pos);

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override;	//位置の取得
	D3DXVECTOR3 GetRot();				//向きの取得
	float GetWidth() override;			//幅の取得
	float GetHeight() override;			//高さの取得
	D3DXMATRIX GetmtxWorld();			//ワールドマトリックスの取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CEnemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void SetModel();			//モデルの設定
	void SetMotion(bool bLoop);	//モーションの設定

private:
	//----------------
	// メンバ変数
	//----------------
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//目的の向き
	CModel*  m_pModel[MAX_ENEMY_PARTS];	//モデル

	/* ↓ モーション情報 ↓ */
	int m_nCurrentKey;			//現在のキー番号
	int m_nCntMotion;			//モーションカウンター
};

#endif