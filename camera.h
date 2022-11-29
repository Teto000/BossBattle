//===================================================
//
// カメラヘッダー
// Author : Sato Teruto
//
//===================================================
#ifndef _CAMERA_H_	//このマクロが定義されてなかったら
#define _CAMERA_H_	//2重インクルード防止のマクロ定義

//--------------------------------
// インクルード
//--------------------------------
#include <d3dx9.h>
#include "main.h"

//================================
// カメラクラスの定義
//================================
class CCamera
{
public:
	CCamera();	//コンストラクタ
	~CCamera();	//デストラクタ

	//------------------
	// メンバ関数
	//------------------
	void Init();
	void Uninit();
	void Update();
	void SetCamera(LPDIRECT3DDEVICE9 pDevice);

	//------------------
	// ゲッター
	//------------------
	D3DXVECTOR3 GetPosV();
	D3DXVECTOR3 GetRot();

private:
	void Turn();

private:
	//------------------
	// メンバ変数
	//------------------
	D3DXVECTOR3 m_posV;			//現在の視点
	D3DXVECTOR3 m_posR;			//現在の注視点
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_worldPosV;	//ワールド座標(視点)
	D3DXVECTOR3 m_worldPosR;	//ワールド座標(注視点)
	D3DXVECTOR3 m_posVDest;		//目的の視点
	D3DXVECTOR3 m_posRDest;		//目的の注視点
	D3DXVECTOR3 m_rotDest;		//目的の角度
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXMATRIX  m_mtxWorld;		//ワールドマトリックス
	float m_TSPEED;
	bool m_bLockOn;
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	D3DVIEWPORT9 m_viewport;
};

#endif