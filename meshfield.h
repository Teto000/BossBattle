//===================================================
//
// メッシュフィールドヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_	//二重インクルード防止

//-------------------------------
// インクルード
//-------------------------------
#include "mesh.h"

//================================
// メッシュフィールドクラス
//================================
class CMeshField : public CMesh
{
public:
	CMeshField();	//コンストラクタ
	~CMeshField();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//------------------
	// 静的メンバ変数
	//------------------
	static CMeshField* Create();
};

#endif