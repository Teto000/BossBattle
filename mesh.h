//===================================================
//
// メッシュヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _MESH_H_
#define _MESH_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "object3d.h"
#include "renderer.h"

//================================
// メッシュクラス
//================================
class CMesh : public CObject
{
public:
	CMesh();
	explicit CMesh(int nPriority);	//コンストラクタ
	~CMesh();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void CollisionMesh();	//メッシュフィールドの当たり判定

	void SetMeshSize(int nDivision, float fWidth);	//最大幅の設定
	void SetTexture(CTexture::TEXTURE texture);		//テクスチャの設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override;	//位置の取得
	float GetWidth() override;			//幅の取得
	float GetHeight() override;			//高さの取得

private:
	void SetVtxNor(VERTEX_3D* pVtx, WORD* pIdx);	//頂点の法線を設定

private:
	//------------------
	// 定数
	//------------------
	static const int nMaxVtx = 512;	//頂点の最大数

	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_VtxPos[nMaxVtx];	//頂点の位置
	D3DXVECTOR3 m_Normal;			//面法線ベクトル格納用
	D3DXVECTOR3 m_VtxNor[nMaxVtx];	//頂点の法線ベクトル格納用
	D3DXVECTOR3 m_rot;				//向き
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス

	/* ↓ メッシュを構成する情報 */
	int m_nNumDivision ;	//ポリゴンの分割数
	int m_nCntVtx ;			//頂点の分割数
	int m_nNumVtx;			//頂点数
	int m_nNumPolygon;		//ポリゴン数
	int m_nNumIndex;		//インデックス数
	float m_fMaxWidth;		//メッシュフィールドの最大幅
	float m_fMeshWidth;		//メッシュフィールドの幅
	float m_fTexSize;		//テクスチャの分割サイズ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CTexture::TEXTURE m_texture;		//テクスチャ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//インデックスバッファへのポインタ
};

#endif