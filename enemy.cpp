//===================================================
//
// エネミーの処理
// Author:Teruto Sato
//
//===================================================

//------------------------
// インクルード
//------------------------
#include <assert.h>
#include "enemy.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "model.h"
#include "line.h"
#include "game.h"
#include "player.h"
#include "fade.h"
#include "hp.h"
#include "message.h"
#include "utility.h"
#include "debug_proc.h"

//------------------------
// 静的メンバ変数宣言
//------------------------
const float CEnemy::fDefGravity = 1.0f;	//基本の重力

//========================
// コンストラクタ
//========================
CEnemy::CEnemy() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最大値
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//大きさの最小値
	m_worldMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最大値
	m_worldMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//ワールド上の最小値
	m_nMoveTime = 0;		//移動までの時間
	m_nBreakTime = 0;		//ブレイク状態の時間
	m_fLife = 0.0f;			//体力
	m_fRemLife = 0.0f;		//残り体力(%)
	m_fMaxLife = 0.0f;		//最大体力
	m_fGravity = 0.0f;		//重力の値
	m_fBreak = 0.0f;
	m_fRemBreak = 0.0f;
	m_fMaxBreak = 0.0f;
	m_bNockBack = false;	//ノックバックしたか

	for (int i = 0; i < GAUGE_MAX; i++)
	{
		m_pHP[i] = nullptr;		//HP
	}

	/* ↓ モデル情報 ↓ */
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{
		m_pModel[i] = nullptr;
	}

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = nullptr;
	}

	/* ↓ モーション情報 ↓ */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
	m_type = MOTION_IDOL;

	//-----------------------------------
	// キーセット情報の初期化
	//-----------------------------------
	for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
	{
		for (int i = 0; i < MOTION_MAX; i++)
		{
			for (int j = 0; j < MAX_PARTS; j++)
			{
				m_aMotionSet[nCnt].aKeySet[i].aKey[j].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
				m_aMotionSet[nCnt].aKeySet[i].aKey[j].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
			}

			m_aMotionSet[nCnt].bLoop = false;			//ループするかどうか
			m_aMotionSet[nCnt].aKeySet[i].nFrame = 0;	//フレーム数
		}

		m_aMotionSet[nCnt].nNumKey = 0;
		m_aMotionSet[nCnt].bLoop = false;
	}
}

//========================
// デストラクタ
//========================
CEnemy::~CEnemy()
{

}

//========================
// 初期化
//========================
HRESULT CEnemy::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_pos = pos;
	m_nCntMotion = 1;
	m_fLife = 3000.0f;			//体力
	m_fRemLife = 100.0f;		//残り体力(%)
	m_fMaxLife = m_fLife;		//最大体力
	m_fGravity = fDefGravity;	//重力の値

	m_fBreak = 100.0f;
	m_fRemBreak = 100.0f;
	m_fMaxBreak = m_fBreak;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//--------------------
	// モデルの生成
	//--------------------
	SetModel();

	//-----------------------
	// モデルの大きさを設定
	//-----------------------
	m_vtxMin = D3DXVECTOR3(-80.0f, 0.0f, -150.0f);
	m_vtxMax = D3DXVECTOR3(80.0f, 300.0f, 60.0f);

	m_size.x = m_vtxMax.x - m_vtxMin.x;
	m_size.y = m_vtxMax.y - m_vtxMin.y;
	m_size.z = m_vtxMax.z - m_vtxMin.z;

	//------------------------------
	// モーションの読み込み
	//------------------------------
	GetFileMotion();

	//-----------------------
	// ゲージの生成
	//-----------------------
	{
		//HP
		D3DXVECTOR3 hpPos(SCREEN_WIDTH / 2, 60.0f, 0.0f);
		m_pHP[GAUGE_HP] = CHP::Create(hpPos, 800.0f, 20.0f, CHP::GAUGETYPE_HP_ENEMY);
		m_pHP[GAUGE_HP]->SetLife(m_fLife, m_fRemLife);	//HPの設定

														//ブレイクゲージ
		D3DXVECTOR3 breakpPos(SCREEN_WIDTH / 2, 100.0f, 0.0f);
		m_pHP[GAUGE_BREAK] = CHP::Create(breakpPos, 800.0f, 15.0f, CHP::GAUGETYPE_BREAK);
		m_pHP[GAUGE_BREAK]->SetLife(m_fBreak, m_fRemBreak);	//HPの設定
	}

	return S_OK;
}

//========================
// 終了
//========================
void CEnemy::Uninit()
{
	//-------------------------
	// モデルの終了
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Uninit();
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}

	CObject::Release();
}

//========================
// 更新
//========================
void CEnemy::Update()
{
	//モーションのリセット
	//ChangeMotion(MOTION_IDOL);

	//-------------------------
	// モデルの更新
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
		if (m_pModel[i])
		{//モデルがnullじゃないなら
			m_pModel[i]->Update();
		}
	}

	if (m_state == ENEMYSTATE_BREAK)
	{//ブレイク状態なら
		//----------------------------
		// ノックバックする処理
		//----------------------------
		NockBack();

		//------------------------
		// ブレイク状態から復帰
		//------------------------
		m_nBreakTime++;	//時間を数える

		if (m_nBreakTime >= 600)
		{//一定時間経過
			m_state = ENEMYSTATE_NONE;	//ブレイク状態を回復
			m_nBreakTime = 0;			//時間のリセット

										//ブレイクゲージを最大にする
			m_fBreak = m_fMaxBreak;
			m_fRemBreak = m_fBreak * 100 / m_fMaxBreak;

			//ブレイクゲージの再生成
			{
				D3DXVECTOR3 breakpPos(SCREEN_WIDTH / 2, 100.0f, 0.0f);
				m_pHP[GAUGE_BREAK] = CHP::Create(breakpPos, 800.0f, 15.0f, CHP::GAUGETYPE_BREAK);
				m_pHP[GAUGE_BREAK]->SetLife(m_fBreak, m_fRemBreak);	//HPの設定
			}
		}
	}
	else
	{//敵がブレイクしていないなら
		//------------------------
		// 攻撃処理
		//------------------------
		Attack();
	}

	//-------------------------
	// 線の更新
	//-------------------------
	//UpdateLine();

	//---------------------
	// 重力の加算
	//---------------------
	if (m_pos.y >= 0.0f)
	{//飛んでいるなら
		m_pos.y -= m_fGravity;
		m_fGravity += 3.0f;
	}
	else
	{//地面に着いたら
		//重力の値をリセット
		m_fGravity = fDefGravity;

		m_pos.y = 0.0f;		//高さを地面に合わせる
	}

	//--------------------------------
	// モーションの設定
	//--------------------------------
	SetMotion(m_type, m_aMotionSet[m_type].bLoop, m_aMotionSet[m_type].nNumKey);

	//----------------------------
	// 死亡時処理
	//----------------------------
	if (m_fLife <= 0)
	{//体力が尽きたら
		CGame::SetEnemyState();	//敵が死んだ状態

		//HPバーの消去
		for (int i = 0; i < GAUGE_MAX; i++)
		{
			if (m_pHP[i] != nullptr)
			{//nullじゃないなら
				m_pHP[i]->Uninit();
			}
		}

		//消去
		Uninit();
	}
}

//========================
// 描画
//========================
void CEnemy::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//モデルのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// モデルの描画
	//-------------------------
	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
		if (!m_pModel[i])
		{//モデルがnullなら
			return;
		}

		m_pModel[i]->Draw(&m_mtxWorld);
	}
}

//========================
// 生成
//========================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pEnemy = new CEnemy;	//生成

	if (pEnemy != nullptr)
	{//NULLチェック
		//変数に代入
		pEnemy->m_rot = rot;

		//初期化
		pEnemy->Init(pos);
		pEnemy->SetObjType(OBJTYPE_ENEMY);
	}

	return pEnemy;
}

//========================
// モデルの生成
//========================
void CEnemy::SetModel()
{
	//体
	m_pModel[0] = CModel::Create("data\\MODEL\\Enemy\\body.x", nullptr,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//右ハンマー
	m_pModel[1] = CModel::Create("data\\MODEL\\Enemy\\armR.x", m_pModel[0],
		D3DXVECTOR3(-200.0f, 80.0f, -80.0f), D3DXVECTOR3(0.0f, 0.0f, 0.5f));

	//左ハンマー
	m_pModel[2] = CModel::Create("data\\MODEL\\Enemy\\armL.x", m_pModel[0],
		D3DXVECTOR3(200.0f, 80.0f, -80.0f), D3DXVECTOR3(0.0f, 0.0f, -0.5f));
}

//========================
// 線の設置
//========================
void CEnemy::SetLine()
{
	D3DXMATRIX mtxTrans;	//計算用マトリックス

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = CLine::CreateAll(m_pLine[i], i, m_pos, min, max);
	}
}

//========================
// 線の情報の更新
//========================
void CEnemy::UpdateLine()
{
	//ワールド変換行列を使ってMin,Maxを求める
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//代入する値をまとめる
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		if (m_pLine[i])
		{
			m_pLine[i]->SetLinePos(i, min, max);
		}
	}
}

//========================
// HP減少時の処理
//========================
void CEnemy::SubGauge(float fDamage, GAUGE type)
{
	switch (type)
	{
		//-----------------------
		// HPを減らす処理
		//-----------------------
	case GAUGE_HP:
		m_fLife -= round(fDamage);	//体力の減少

		//残り体力を計算
		m_fRemLife = m_fLife * 100 / m_fMaxLife;
		//HPの設定
		m_pHP[GAUGE_HP]->SetLife(m_fLife, m_fRemLife);

		if (m_fLife < 0 && m_fRemLife < 0
			&& m_pHP[GAUGE_HP])
		{//HPゲージが尽きたら
			m_pHP[GAUGE_HP] = nullptr;
			return;
		}
		break;

		//--------------------------------
		// ブレイクゲージを減らす処理
		//--------------------------------
	case GAUGE_BREAK:
		m_fBreak -= round(fDamage / 3);
		m_fRemBreak = m_fBreak * 100 / m_fMaxBreak;
		m_pHP[GAUGE_BREAK]->SetLife(m_fBreak, m_fRemBreak);

		if (m_fBreak < 0 && m_fRemBreak < 0
			&& m_pHP[GAUGE_BREAK])
		{//ブレイクゲージが尽きたら
			m_pHP[GAUGE_BREAK] = nullptr;

			//ブレイク状態にする
			m_state = ENEMYSTATE_BREAK;
		}
		break;

	default:
		break;
	}
}

//========================
// 攻撃処理
//========================
void CEnemy::Attack()
{
	//変数宣言
	int nMaxAttackTime = 120;				//攻撃時間
	float fAttackArea = 250.0f;				//敵の攻撃範囲
	float fMoveArea = fAttackArea - 30.0f;	//敵の移動範囲

	//プレイヤーの位置を取得
	D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

	//敵とプレイヤー間の距離を計算
	D3DXVECTOR3 vec = playerPos - m_pos;

	//距離の絶対値を計算
	float fDistance = fabs((vec.x + vec.z) / 2);

	//-------------------------
	// プレイヤーまで移動
	//-------------------------
	if (fDistance >= fMoveArea)
	{//プレイヤーが範囲内にいないなら
		//Move();
	}

	//-------------------------
	// 攻撃処理
	//-------------------------
	if (fDistance <= fAttackArea)
	{//プレイヤーが範囲内にいるなら
		//攻撃までの時間を加算
		m_nAttackTime++;

		if (m_nAttackTime >= nMaxAttackTime)
		{//攻撃時間が値に達したら
			//攻撃モーションにする
			ChangeMotion(MOTION_ATTACK);
		}
	}
}

//========================
// ノックバックする処理
//========================
void CEnemy::NockBack()
{
	if (CGame::GetPlayer()->GetHitAttack())
	{//プレイヤーが攻撃を当てた状態なら
		//リセット
		m_nAttackTime = 0;			//攻撃までの時間
		m_fGravity = fDefGravity;	//重力の値

		//--------------------------------
		// ノックバックする処理
		//--------------------------------
		//プレイヤーの位置を取得
		D3DXVECTOR3 playerPos = CGame::GetPlayer()->GetPosition();

		//敵とプレイヤー間のベクトルを計算
		D3DXVECTOR3 vec = playerPos - m_pos;

		D3DXVec3Normalize(&vec, &vec);	//ベクトルの正規化

		if (!m_bNockBack)
		{//ノックバックしていないなら
			m_pos += -vec * 2.0f;	//逆ベクトル方向に移動
			m_pos.y += 100.0f;		//上昇
			m_bNockBack = true;
		}
	}
	else
	{
		m_bNockBack = false;
	}
}

//========================
// 移動処理
//========================
void CEnemy::Move()
{
	//------------------------------
	// プレイヤーの方を向く
	//------------------------------
	//プレイヤーの位置を取得
	D3DXVECTOR3 playerPos(CGame::GetPlayer()->GetPosition());

	//2点間の距離を求める
	float X = m_pos.x - playerPos.x;
	float Z = m_pos.z - playerPos.z;

	//角度の設定
	float angle = atan2f(X, Z);

	//向きの設定
	m_rotDest = D3DXVECTOR3(0.0f, angle, 0.0f);

	//-------------------------------
	// 目的の角度の正規化
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//回転したい角度が180以上なら
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//回転したい角度が-180以下なら
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// 目的の角度まで回転する
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.03f;	//減衰処理

	//-------------------------------
	// 角度の正規化
	//-------------------------------
	m_rot.y = CUtility::GetNorRot(m_rot.y);

	//------------------------------
	// プレイヤーに向かって移動
	//------------------------------
	//プレイヤーと敵のベクトルを求める
	D3DXVECTOR3 vec(playerPos - m_pos);

	//ベクトルの正規化
	D3DXVec3Normalize(&vec, &vec);

	//プレイヤーに向かって移動
	m_move = vec * 1.5f;
	m_pos += m_move;

	//移動モーションにする
	ChangeMotion(MOTION_MOVE);
}

//==========================================
// ファイルからモーション情報を取得
//==========================================
void CEnemy::GetFileMotion()
{
	const int nMaxText = 256;	//文字の最大数

	FILE *pFile;				//ファイルポインタを宣言する
	char cText[nMaxText];		//1行分の文字読み取り用変数
	char cTextHead[nMaxText];	//頭文字を取るようの変数
	int nNumMotion = 0;			//読み込み中のモーション番号
	int nNumKey = 0;			//読み込み中のキー番号
	int nNumParts = 0;			//読み込み中のパーツ番号

	//--------------------------------
	// ファイルの読み込み
	//--------------------------------
	//ファイルを開く
	pFile = fopen("data/MOTION/enemy.txt", "r");

	if (pFile == nullptr)
	{//開けなかったら
		assert(false);
	}

	//文字の読み取り処理
	while (fgets(cText, nMaxText, pFile) != nullptr)
	{
		//文字列の分析
		sscanf(cText, "%s", &cTextHead);

		//========================================
		// モーション情報
		//========================================
		if (strcmp(&cTextHead[0], "MOTIONSET") == 0)
		{//頭文字がMOTIONSETなら
		 //文字の読み取り処理
			while (fgets(cText, nMaxText, pFile) != nullptr)
			{
				//文字列の分析
				sscanf(cText, "%s", &cTextHead);

				//-------------------------------
				// ループするかどうか
				//-------------------------------
				if (strcmp(&cTextHead[0], "LOOP") == 0)
				{//頭文字がLOOPなら
					int nLoop = 0;

					//文字列からループの値を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &nLoop);

					if (nLoop == 0)
					{//読み取った値が0なら
						m_aMotionSet[nNumMotion].bLoop = false;
					}
					else if (nLoop == 1)
					{//読み取った値が1なら
						m_aMotionSet[nNumMotion].bLoop = true;
					}
				}
				//-------------------------------
				// キーの最大数
				//-------------------------------
				else if (strcmp(&cTextHead[0], "NUM_KEY") == 0)
				{//頭文字がNUM_KEYなら
					//文字列からキーの最大数を読み取る
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNumKey);
				}
				//========================================
				// キーセット情報
				//========================================
				else if (strcmp(&cTextHead[0], "KEYSET") == 0)
				{//頭文字がKEYSETなら
					//文字の読み取り処理
					while (fgets(cText, nMaxText, pFile) != nullptr)
					{
						//文字列の分析
						sscanf(cText, "%s", &cTextHead);

						//-------------------------------
						// フレーム数
						//-------------------------------
						if (strcmp(&cTextHead[0], "FRAME") == 0)
						{//頭文字がFRAMEなら
							//文字列からキーの最大数を読み取る
							sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].aKeySet[nNumKey].nFrame);
						}
						//========================================
						// キー情報
						//========================================
						else if (strcmp(&cTextHead[0], "KEY") == 0)
						{//頭文字がKEYなら
							//文字の読み取り処理
							while (fgets(cText, nMaxText, pFile) != nullptr)
							{
								//文字列の分析
								sscanf(cText, "%s", &cTextHead);

								//-------------------------------
								// 位置
								//-------------------------------
								if (strcmp(&cTextHead[0], "POS") == 0)
								{//頭文字がPOSなら
									//文字列から位置を読み取る
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.x,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.y,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.z);
								}
								//-------------------------------
								// 向き
								//-------------------------------
								else if (strcmp(&cTextHead[0], "ROT") == 0)
								{//頭文字がROTなら
									//文字列から向きを読み取る
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.x,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.y,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.z);
								}
								else if (strcmp(&cTextHead[0], "END_KEY") == 0)
								{//キーの読み取りが終了したら
									if (nNumParts + 1 < MAX_ENEMY_PARTS)
									{//パーツ数を超えないなら
										//パーツ番号の加算
										nNumParts++;
									}
									else
									{//パーツ数分読み込んだら
										//パーツ番号をリセット
										nNumParts = 0;
									}
									break;
								}
							}
						}
						else if (strcmp(&cTextHead[0], "END_KEYSET") == 0)
						{//キーセットの読み取りが終了したら
							if (nNumKey < m_aMotionSet[nNumMotion].nNumKey)
							{//キー数が最大じゃないなら
								//キー番号の加算
								nNumKey++;
							}
							break;
						}
					}
				}
				else if (strcmp(&cTextHead[0], "END_MOTIONSET") == 0)
				{//モーションの読み取りが終了したら
					//キー番号をリセット
					nNumKey = 0;
					//モーション番号の加算
					nNumMotion++;
					break;
				}
			}
		}

		//----------------------------------
		// 保存中の文字列の初期化
		//----------------------------------
		ZeroMemory(&cText, sizeof(cText));
		ZeroMemory(&cTextHead, sizeof(cTextHead));
	}

	//ファイルを閉じる
	fclose(pFile);
}

//==========================================
// モーションの設定
// 引数：種類、ループ状態、キー数
//==========================================
void CEnemy::SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey)
{
	if (m_nCurrentKey + 1 >= nNumKey)
	{//キーが最大数に達したら
		if (bLoop)
		{//ループするなら
			m_nCurrentKey = 0;	//キー番号の初期化
		}
		else
		{
			return;
		}
	}

	for (int i = 0; i < MAX_ENEMY_PARTS; i++)
	{//モデルパーツ数分回す
		//-------------------------------------------------------
		// NULLチェック
		//-------------------------------------------------------
		if (!m_pModel[i])
		{//モデルパーツがnullなら
			return;
		}

		//キー情報を持った変数
		KEY key = m_aMotionSet[type].aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = m_aMotionSet[type].aKeySet[m_nCurrentKey + 1].aKey[i];

		//-------------------------------------------------------
		// 現在値を取得
		//-------------------------------------------------------
		//位置を取得
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//向きを取得
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-------------------------------------------------------
		// 差分の計算
		// (終了値 - 開始値)
		//-------------------------------------------------------
		//位置
		float fDifPosX = keyNext.pos.x - key.pos.x;
		float fDifPosY = keyNext.pos.y - key.pos.y;
		float fDifPosZ = keyNext.pos.z - key.pos.z;

		//向き
		float fDifRotX = keyNext.rot.x - key.rot.x;
		float fDifRotY = keyNext.rot.y - key.rot.y;
		float fDifRotZ = keyNext.rot.z - key.rot.z;

		//-------------------------------------------------------
		// 差分の角度の正規化
		//-------------------------------------------------------
		fDifRotX = CUtility::GetNorRot(fDifRotX);	//Xの値
		fDifRotY = CUtility::GetNorRot(fDifRotY);	//Yの値
		fDifRotZ = CUtility::GetNorRot(fDifRotZ);	//Zの値

		//-------------------------------------------------------
		// 相対値の計算
		// (モーションカウンター / フレーム数)
		//-------------------------------------------------------
		float fNumRelative = m_nCntMotion / (float)m_aMotionSet[type].aKeySet[m_nCurrentKey].nFrame;

		//-------------------------------------------------------
		// 現在値の計算
		// (開始値 + (差分 * 相対値))
		//-------------------------------------------------------
		//初期値の取得
		D3DXVECTOR3 initPos = m_pModel[i]->GetInitPos();
		D3DXVECTOR3 initRot = m_pModel[i]->GetInitRot();

		//位置
		fPosX = initPos.x + key.pos.x + (fDifPosX * fNumRelative);
		fPosY = initPos.y + key.pos.y + (fDifPosY * fNumRelative);
		fPosZ = initPos.z + key.pos.z + (fDifPosZ * fNumRelative);

		//向き
		fRotX = initRot.x + key.rot.x + (fDifRotX * fNumRelative);
		fRotY = initRot.y + key.rot.y + (fDifRotY * fNumRelative);
		fRotZ = initRot.z + key.rot.z + (fDifRotZ * fNumRelative);

		//-------------------------------------------------------
		// モデル情報の設定
		//-------------------------------------------------------
		//位置の設定
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//向きの設定
		m_pModel[i]->SetRot(D3DXVECTOR3(fRotX, fRotY, fRotZ));
	}

	//モーションカウンターを進める
	m_nCntMotion++;

	//--------------------------------
	// 初期化
	//--------------------------------
	if (m_nCntMotion >= m_aMotionSet[type].aKeySet[m_nCurrentKey].nFrame)
	{//モーションカウンターが再生フレームに達したら
		m_nCurrentKey++;	//キー番号を加算
		m_nCntMotion = 0;	//モーションカウンターを初期化
	}
}

//==========================================
// モーションの変更
// 引数：変更したいモーションの列挙
//==========================================
void CEnemy::ChangeMotion(MOTION_TYPE type)
{
	//モーションの変更
	m_type = type;

	//モーション情報の初期化
	/*if (m_type == MOTION_ATTACK)
	{
		m_nCurrentKey = 0;
		m_nCntMotion = 0;
		m_nAttackTime = 0;
	}*/
}