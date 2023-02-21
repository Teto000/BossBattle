//===================================================
//
// 軌跡ヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//--------------------------------
// 剣の軌跡クラス
//--------------------------------
class COrbit : public CObject
{
public:
	COrbit();	//コンストラクタ
	~COrbit();	//デストラクタ

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
	void SetMtxParent(D3DXMATRIX pMtx) { m_pMtxParent = pMtx; };	//親のマトリックスの設定
	void SetOffset(D3DXVECTOR3 offset) { m_offsetPos = offset; }	//オフセット座標の設定
	void SetColor(D3DXCOLOR col);			//色の設定

	//------------------
	// ゲッター
	//------------------
	D3DXVECTOR3 GetPosition() override { return m_offsetPos; }	//位置の取得
	float GetWidth() override { return 0.0f; }					//幅の取得
	float GetHeight() override { return 0.0f; }					//高さの取得

	//------------------
	// 静的メンバ変数
	//------------------
	static COrbit* Create(D3DXVECTOR3 offset, D3DXMATRIX mtx);

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxVtx = 2048;	//頂点の最大数

	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_offsetPos;			//オフセット座標
	D3DXVECTOR3 m_worldPos;				//ワールド座標
	D3DXMATRIX	m_mtxWorld;				//ワールドマトリックス
	D3DXMATRIX m_pMtxParent;			//軌跡の親モデルのマトリックス
	D3DXCOLOR m_initCol;				//色の初期値
	int m_nNumVtx;						//頂点数

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
};

#endif