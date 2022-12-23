//===================================================
//
// ビルボードヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "object.h"
#include "texture.h"

//================================
// ビルボードクラスの定義
//================================
class CBillBoard : public CObject
{
public:
	CBillBoard();
	explicit CBillBoard(int nPriority);	//コンストラクタ
	virtual ~CBillBoard() override;		//デストラクタ

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
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	//位置の設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	//向きの設定
	D3DXVECTOR3 AddMove(D3DXVECTOR3 move);			//移動量の加算
	void SetSize(float fWidth, float fHeight);		//大きさの設定
	void SetColor(D3DXCOLOR col); 					//色の設定
	void SetVtxCIE_Gauge(float fWidth1
		, float fWidth2, float fHeight1, float fHeight2);	//頂点座標の設定

	/* ↓テクスチャ↓ */
	void SetTexCIE(float left, float fRight);	//テクスチャ座標の設定
	void SetTexture(CTexture::TEXTURE texture);	//テクスチャの設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }	//位置の取得
	float GetWidth() override { return m_fWidth; }			//幅の取得
	float GetHeight() override { return m_fHeight; }		//高さの取得

protected:
	//----------------
	// メンバ変数
	//----------------
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	float m_fRot;		//回転角度
	float m_fScal;		//大きさ
	D3DXCOLOR m_col;	//色
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CTexture::TEXTURE m_texture;		//テクスチャ
};

#endif // !_OBJECT_H_