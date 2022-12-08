//===================================================
//
// オブジェクトXヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object.h"

//================================
// オブジェクトXクラスの定義
//================================
class CObjectX : public CObject
{
public:
	CObjectX();
	explicit CObjectX(int nPriority);	//コンストラクタ
	virtual ~CObjectX() override;		//デストラクタ

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
	void SetPos(D3DXVECTOR3 pos);		//位置の設定
	void SetRot(D3DXVECTOR3 rot);		//向きの設定
	void SetFireName(LPCTSTR text);		//Xファイルの名前を設定

	//----------------
	// ゲッター
	//----------------
	D3DXVECTOR3 GetPosition() override;	//位置の取得
	float GetWidth() override;			//幅の取得
	float GetHeight() override;			//高さの取得

private:
	void DrawShadow();

private:
	//----------------
	// メンバ変数
	//----------------
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_rot;					//向き
	LPCTSTR m_name;						//Xファイルの名前

	/* ↓ モデル情報 ↓ */
	LPD3DXMESH m_pMesh;					//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuffMat;			//マテリアル情報へのポインタ
	DWORD m_nNumMat;					//マテリアル情報の数
};

#endif