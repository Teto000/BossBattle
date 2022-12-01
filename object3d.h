//===================================================
//
// オブジェクト3Dヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "object.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"

//================================
// オブジェクト3Dクラスの定義
//================================
class CObject3D : public CObject
{
public:
	CObject3D();
	explicit CObject3D(int nPriority);	//コンストラクタ
	virtual ~CObject3D() override;		//デストラクタ

	//----------------
	// メンバ関数
	//----------------
	virtual HRESULT Init(D3DXVECTOR3 pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//----------------
	// セッター
	//----------------
	void SetPos(D3DXVECTOR3 pos);	//位置の設定
	void SetRot(D3DXVECTOR3 rot);
	void SetColor(D3DXCOLOR col);	//色の設定
	void SetSize(float fWidth, float fHeight, float fDepth);
	void SetVtx(VERTEX_3D* vtx);	//頂点情報の設定

	/* ↓テクスチャ↓ */
	void SetTexCIE(float left, float fRight);	//テクスチャ座標の設定
	void SetTexture(CTexture::TEXTURE texture);	//テクスチャの設定

	/* ↓描画↓ */
	void SetSubALPHA();		//αブレンディングを減算合成に設定
	void ResetSubALPHA();	//αブレンディングを元に戻す

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override;	//位置の取得
	float GetWidth() override;			//幅の取得
	float GetHeight() override;			//高さの取得

private:
	//----------------
	// メンバ変数
	//----------------
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	float m_Depth;		//奥行き
	float m_fRot;		//回転角度
	float m_fScal;		//大きさ
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CTexture::TEXTURE m_texture;		//テクスチャ
};

#endif // !_OBJECT_H_