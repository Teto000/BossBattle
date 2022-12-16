//===================================================
//
// 軌跡ヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 剣の軌跡クラス
//--------------------------------
class CMeshOrbit : public CObject
{
public:
	CMeshOrbit();	//コンストラクタ
	~CMeshOrbit();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// セッター
	//------------------
	void SetMtxParent(D3DXMATRIX* pMtx) { m_pMtxParent = pMtx; };	//親のマトリックスの設定

	//------------------
	// ゲッター
	//------------------
	D3DXVECTOR3 GetPosition() override { return m_localpos; }	//位置の取得
	float GetWidth() override { return 0.0f; }				//幅の取得
	float GetHeight() override { return 0.0f; }				//高さの取得

	//------------------
	// 静的メンバ変数
	//------------------
	static CMeshOrbit* Create(D3DXVECTOR3 pos);

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxVtx = 32;		//頂点の最大数

	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_localpos;				//ローカル座標
	D3DXVECTOR3 m_worldPos;				//ワールド座標
	D3DXMATRIX	m_mtxWorld;				//ワールドマトリックス
	D3DXMATRIX* m_pMtxParent;			//軌跡の親モデルのマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;	//インデックスバッファへのポインタ
};

#endif