//===================================================
//
// ラインヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _LINE_H_
#define _LINE_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "object.h"

//--------------------------------
// 前方宣言
//--------------------------------
class CObject3D;

//================================
// ラインクラスの定義
//================================
class CLine : public CObject
{
public:
	CLine();			//コンストラクタ
	~CLine() override;	//デストラクタ

	//--------------------
	// メンバ関数
	//--------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//-------------------
	// セッター
	//-------------------
	void SetLine(D3DXVECTOR3 start, D3DXVECTOR3 end);

	//-------------------
	// ゲッター
	//-------------------
	D3DXVECTOR3 GetPosition() override { return m_pos; };	//位置の取得
	float GetWidth() override { return 0.0f; };				//幅の取得
	float GetHeight() override { return 0.0f; };			//高さの取得

	//--------------------
	// 静的メンバ関数
	//--------------------
	static CLine* Create(D3DXVECTOR3 pos, D3DXVECTOR3 start
						, D3DXVECTOR3 end, D3DXCOLOR col);

private:
	//--------------------
	// 定数
	//--------------------
	static const int nMaxLine = 128;	//線の最大数

	//--------------------
	// メンバ変数
	//--------------------
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_start;	//始点
	D3DXVECTOR3 m_end;		//終点
	D3DXCOLOR	m_col;		//色
	D3DXMATRIX	m_mtxWorld;	//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	CObject3D*	m_pObject3D;//オブジェクト3D
};

#endif // !_LINE_H_