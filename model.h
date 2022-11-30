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
	CModel();	//コンストラクタ
	~CModel();	//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	//----------------
	// セッター
	//----------------
	void SetPos(D3DXVECTOR3 pos);		//位置の設定
	void SetRot(D3DXVECTOR3 rot);		//向きの設定
	void SetParent(CModel* pModel);		//親モデルの設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPos();		//位置の取得
	D3DXVECTOR3 GetRot();		//向きの取得
	D3DXMATRIX GetmtxWorld();	//ワールドマトリックスの取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CModel* Create(LPCTSTR text,D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	void GetModelSize();	//モデルの大きさを取得
	void SetLine();			//線の設置
	void DrawShadow();		//影の描画

private:
	//----------------
	// 定数
	//----------------
	static const int nMaxLine = 32;

	//----------------
	// メンバ変数
	//----------------
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXMATRIX m_mtxParent;		//親のマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_vtxMin;		//最小値
	D3DXVECTOR3 m_vtxMax;		//最大値
	D3DXVECTOR3 m_size;			//大きさ
	CModel* m_pModel;			//親モデルへのポインタ
	LPCTSTR m_modelName;		//モデルのパス
	CLine* m_pLIne[nMaxLine];	//ラインクラス

	/* ↓ モデル情報 ↓ */
	LPD3DXMESH m_pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアル情報へのポインタ
	DWORD m_nNumMat;			//マテリアル情報の数
};

#endif