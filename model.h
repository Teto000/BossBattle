//===================================================
//
// モデルヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _MODEL_H_
#define _MODEL_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CLine;	//ライン

//================================
// モデルクラスの定義
//================================
class CModel
{
public:
	//----------------
	// モデルの種類
	//----------------
	enum MODELTYPE
	{
		NONE = 0,
		PLAYER,
		ENEMY,
		MAX
	};

	CModel();	//コンストラクタ
	~CModel();	//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw(D3DMATRIX* mtxPlayer);

	//----------------
	// セッター
	//----------------
	void SetPos(D3DXVECTOR3 pos)	{ m_pos = pos; }		//位置の設定
	void SetRot(D3DXVECTOR3 rot)	{ m_rot = rot; }		//向きの設定
	void SetRotX(float rotX)		{ m_rot.x = rotX; };	//X軸の回転量を設定
	void SetParent(CModel* pModel)	{ m_pModel = pModel; }	//親モデルの設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPos()		{ return m_pos; }		//位置の取得
	D3DXVECTOR3 GetRot()		{ return m_rot; }		//向きの取得
	D3DXVECTOR3 GetInitPos()	{ return m_initPos; }	//位置の初期値の取得
	D3DXVECTOR3 GetInitRot()	{ return m_initRot; }	//向きの初期値の取得
	D3DXMATRIX GetmtxWorld()	{ return m_mtxWorld; }	//ワールドマトリックスの取得
	LPD3DXMESH GetMesh()		{ return m_pMesh; };	//メッシュ情報の取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CModel* Create(
		LPCTSTR text, CModel* model, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void DrawShadow();	//影の描画

private:
	//----------------
	// 定数
	//----------------
	static const int nMaxLine = 12;			//線の最大数

	//----------------
	// メンバ変数
	//----------------
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXMATRIX m_mtxParent;		//親のマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_initPos;		//位置の初期値
	D3DXVECTOR3 m_initRot;		//向きの初期値
	CModel* m_pModel;			//親モデルへのポインタ
	LPCTSTR m_modelName;		//モデルのパス
	MODELTYPE m_type;			//種類
	CLine* m_pLine[nMaxLine];	//ライン
	D3DXVECTOR3 m_vtxMax;		//大きさの最大値
	D3DXVECTOR3 m_vtxMin;		//大きさの最小値
	D3DXVECTOR3 m_worldMin;		//ワールド上の最大値
	D3DXVECTOR3	m_worldMax;		//ワールド上の最小値

	/* ↓ モデル情報 ↓ */
	LPD3DXMESH m_pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアル情報へのポインタ
	DWORD m_nNumMat;			//マテリアル情報の数
};

#endif