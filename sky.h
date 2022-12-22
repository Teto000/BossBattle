//===================================================
//
// スカイヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _SKY_H_
#define _SKY_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"
#include "texture.h"
#include "object3d.h"

//================================
// スカイヘッダーの定義
//================================
class CSky : public CObject
{
public:
	CSky();				//コンストラクタ
	~CSky() override;	//デストラクタ

	//----------------
	// メンバ関数
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition()override { return m_pos; }	//位置の取得
	float GetWidth() override { return 0; };			//幅の取得
	float GetHeight() override { return 0; };			//高さの取得

	//----------------
	// 静的メンバ関数
	//----------------
	static CSky* Create(CTexture::TEXTURE texture);

private:
	void SetVtxNor(VERTEX_3D* pVtx, WORD* pIdx);	//頂点の法線を設定

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxVtx = 512;			//頂点の最大数

	//----------------
	// メンバ変数
	//----------------
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_VtxPos[nMaxVtx];	//頂点の位置
	D3DXVECTOR3 m_Normal;			//面法線ベクトル格納用
	D3DXVECTOR3 m_VtxNor[nMaxVtx];	//頂点の法線ベクトル格納用
	D3DXVECTOR3 m_rot;				//向き
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス

	/* ↓ メッシュを構成する情報 */
	int m_nNumVtx;			//頂点数
	int m_nNumPolygon;		//ポリゴン数
	int m_nNumIndex;		//インデックス数
	int m_vertical;			//垂直方向の分割数

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CTexture::TEXTURE m_texture;		//テクスチャ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
};

#endif#pragma once
