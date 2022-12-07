//===================================================
//
// モデルの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include "model.h"
#include "application.h"
#include "renderer.h"
#include "light.h"
#include "game.h"
#include "player.h"
#include "texture.h"

//========================
// コンストラクタ
//========================
CModel::CModel()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの設定
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//最小値
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//最大値
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//大きさ
	m_pModel = nullptr;
}

//========================
// デストラクタ
//========================
CModel::~CModel()
{

}

//========================
// 初期化
//========================
HRESULT CModel::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;	//位置

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//-----------------------
	// デバイスの取得
	//-----------------------
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//-----------------------
	// Xファイルの読み込み
	//-----------------------
	D3DXLoadMeshFromX(m_modelName,
					   D3DXMESH_SYSTEMMEM,
					   pDevice,
					   nullptr,
					   &m_pBuffMat,
					   nullptr,
					   &m_nNumMat,
					   &m_pMesh);

	//-----------------------
	// モデルの大きさを取得
	//-----------------------
	GetModelSize();

	return S_OK;
}

//========================
// 終了
//========================
void CModel::Uninit()
{
	//-----------------------
	// メッシュの破棄
	//-----------------------
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//-----------------------
	// マテリアルの破壊
	//-----------------------
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}

//========================
// 更新
//========================
void CModel::Update()
{
}

//========================
// 描画
//========================
void CModel::Draw(D3DMATRIX* mtxPlayer)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//---------------------------------
	// モデルの向きを反映
	//---------------------------------
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//---------------------------------
	// モデルの位置を反映
	//-------------------------------- -
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//---------------------------------
	// 親モデルの設定
	//---------------------------------
	if (m_pModel != nullptr)
	{//親モデルがnullじゃないなら
		//親モデルの情報を設定
		m_mtxParent = m_pModel->GetmtxWorld();

		//親のマトリックスとの掛け算
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);
	}
	else
	{
		//親のマトリックスとの掛け算
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, (const D3DXMATRIX*)mtxPlayer);
	}

	//影の描画
	DrawShadow();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//---------------------------------
	// キャラクターの描画
	//---------------------------------
	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//プレイヤーパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保持しているマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
// 生成
//========================
CModel* CModel::Create(LPCTSTR text, CModel* model, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{//モデルのパス , 親モデルから見た位置 , 親モデルから見た向き
	CModel *pModel = nullptr;

	//----------------------------------
	// 生成と初期化
	//----------------------------------
	pModel = new CModel;	//生成

	if (pModel != nullptr)
	{//NULLチェック
		//メンバ変数に代入
		pModel->m_modelName = text;
		pModel->m_pModel = model;
		pModel->m_rot = rot;

		//初期化
		pModel->Init(pos);
	}

	return pModel;
}

//========================
// モデルの大きさを取得
//========================
void CModel::GetModelSize()
{
	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int i = 0; i < nNumVtx; i++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//---------------------
		// 頂点座標の比較
		//---------------------
		//最小値の比較
		if (vtx.x <= m_vtxMin.x)
		{
			m_vtxMin.x = floorf(vtx.x);
		}
		if (vtx.y <= m_vtxMin.y)
		{
			m_vtxMin.y = floorf(vtx.y);
		}
		if (vtx.z <= m_vtxMin.z)
		{
			m_vtxMin.z = floorf(vtx.z);
		}

		//最大値の比較
		if (vtx.x >= m_vtxMax.x)
		{
			m_vtxMax.x = floorf(vtx.x);
		}
		if (vtx.y >= m_vtxMax.y)
		{
			m_vtxMax.y = floorf(vtx.y);
		}
		if (vtx.z >= m_vtxMax.z)
		{
			m_vtxMax.z = floorf(vtx.z);
		}

		//--------------------------
		// モデルの大きさを設定
		//--------------------------
		m_size.x = m_vtxMax.x - m_vtxMin.x;
		m_size.y = m_vtxMax.y - m_vtxMin.y;
		m_size.z = m_vtxMax.z - m_vtxMin.z;

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}
}

//========================
// 影の描画
//========================
void CModel::DrawShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

	D3DXMATRIX	mtxShadow;	//影のマトリックス
	D3DXPLANE	planeField;	//平面を表す変数
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;

	//シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライト方向の取得
	{
		D3DXVECTOR3 vecDir(CApplication::GetLight()->GetVecDir());
		vecLight = D3DXVECTOR4(-vecDir, 0.0f);	//ライトの逆方向を設定
	}

	pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線と平面の1点から平面を作成
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	//ライトと平面から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//ワールドマトリックスを計算
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//----------------------------
	// マテリアルの色を保存
	//----------------------------
	D3DXCOLOR col[32];
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの色を保存
		col[nCntMat] = pMat[nCntMat].MatD3D.Diffuse;
	}

	//----------------------------
	// キャラクターの描画
	//----------------------------
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの色の設定
		pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//色
		pMat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//発光

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//プレイヤーパーツの描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//----------------------------
	// キャラクターの色を戻す
	//----------------------------
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの色の設定
		pMat[nCntMat].MatD3D.Diffuse = col[nCntMat];
	}

	//保持しているマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================
// 位置の設定
//===========================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// 向きの設定
//===========================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===========================
// 親モデルの設定
//===========================
void CModel::SetParent(CModel* pModel)
{
	m_pModel = pModel;
}

//===========================
// 位置の取得
//===========================
D3DXVECTOR3 CModel::GetPos()
{
	return m_pos;
}

//===========================
// 向きの取得
//===========================
D3DXVECTOR3 CModel::GetRot()
{
	return m_rot;
}

//===========================
// 最大値の取得
//===========================
D3DXVECTOR3 CModel::GetVtxMax()
{
	return m_vtxMax;
}

//===========================
// 最小値の取得
//===========================
D3DXVECTOR3 CModel::GetVtxMin()
{
	return m_vtxMin;
}

//==============================
// ワールドマトリックスの取得
//==============================
D3DXMATRIX CModel::GetmtxWorld()
{
	return m_mtxWorld;
}