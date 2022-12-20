//=====================================
//
// オブジェクト2Dヘッダー
// Author : Sato Teruto
//
//=====================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "object.h"
#include "texture.h"

//--------------------------------
// オブジェクト2Dクラスの定義
//--------------------------------
class CObject2D : public CObject
{
public:
	CObject2D();
	explicit CObject2D(int nPriority);	//コンストラクタ
	virtual ~CObject2D() override;		//デストラクタ

	//----------------
	// メンバ関数
	//----------------
	virtual HRESULT Init(D3DXVECTOR3 pos) override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	
	D3DXVECTOR3 AddMove(D3DXVECTOR3 move);		//移動量の加算

	//----------------
	// セッター
	//----------------
	void SetPosition(D3DXVECTOR3 pos);						//位置の設定
	void SetSize(float fWidth, float fHeight);				//大きさの設定
	void SetColor(D3DXCOLOR col);							//色の設定
	void SetScreenX(float posX, float fLeft, float fRight);	//左右画面端の設定
	void SetScreenY(float posY, float fUp, float fDown);	//上下画面端の設定
	void SetAnimation(float fNumber, int nPattern);			//アニメーションの設定

	bool OutScreen(D3DXVECTOR3 pos);	//画面外に出た処理
	D3DXVECTOR3 MoveCircle(D3DXVECTOR3 CenterPos, float fAngle, float fLength);	//位置を円形に動かす

	/* ↓テクスチャ↓ */
	void SetTexCIE(float left, float fRight);	//テクスチャ座標の設定
	void SetTexture(CTexture::TEXTURE texture);	//テクスチャの設定

	/* ↓頂点座標↓ */
	void SetVtxCIE(D3DXVECTOR3 pos, float fWidth, float fHeight);						//頂点座標の設定
	void SetVtxCIE_Rot(D3DXVECTOR3 pos, float rotX, float fWidth, float fHeight);		//頂点座標の設定(回転)
	void SetVtxCIE_Gauge(D3DXVECTOR3 pos, float fWidth1,
						float fWidth2, float fHeight1, float fHeight2);					//頂点座標の設定(ゲージ)
	void SetVtxCIE_Score(D3DXVECTOR3 pos, float fWidth, float fHeight, float fSpace);	//頂点座標の設定(スコア)

	/* ↓描画↓ */
	void SetAddALPHA();		//αブレンディングを加算合成に設定
	void ResAddALPHA();		//αブレンディングを元に戻す
	
	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override;					//位置の取得
	D3DXVECTOR3 GetTargetPos();							//相手の位置の取得
	float GetWidth() override;							//幅の取得
	float GetHeight() override;							//高さの取得
	CObject* GetCollision(CObject::EObjType TirgetType);//当たり判定の取得

protected:
	//----------------
	// メンバ変数
	//----------------
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_TargetPos;	//相手の位置
	D3DXVECTOR3 m_move;			//移動量
	float m_fWidth;				//幅
	float m_fHeight;			//高さ
	float m_fRot;				//回転角度
	float m_fScal;				//大きさ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CTexture::TEXTURE m_texture;		//テクスチャ
};

#endif // !_OBJECT_H_