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
#include "object3d.h"

//================================
// ラインクラスの定義
//================================
class CLine : public CObject3D
{
public:
	CLine();	//コンストラクタ
	~CLine();	//デストラクタ

	//--------------------
	// メンバ関数
	//--------------------
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	//--------------------
	// 静的メンバ関数
	//--------------------
	static CLine* Create(D3DXVECTOR3 pos, D3DXVECTOR3 start
						, D3DXVECTOR3 end, D3DXCOLOR col);

private:
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
};

#endif // !_LINE_H_