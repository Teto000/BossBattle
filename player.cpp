//===================================================
//
// �v���C���[�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
#include <stdio.h>
#include <assert.h>
#include "player.h"
#include "object.h"
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "camera.h"
#include "debug_proc.h"
#include "model.h"
#include "game.h"
#include "line.h"
#include "enemy.h"
#include "hp.h"
#include "combo.h"

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
const float CPlayer::fDefaultAttack = 20.0f;	//��{�̍U����
const float CPlayer::fDefaultSpeed = 7.0f;		//��{�̑��x

//========================
// �R���X�g���N�^
//========================
CPlayer::CPlayer() : CObject(0)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�傫���̍ő�l
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�傫���̍ŏ��l
	m_worldMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���[���h��̍ő�l
	m_worldMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���[���h��̍ŏ��l
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�傫��
	m_nNumCombo = 0;							//�R���{��
	m_status.nComboValue = 0;					//�R���{�̉��Z�l
	fSizeWidth = 0.0f;							//�T�C�Y(��)
	fSizeDepth = 0.0f;							//�T�C�Y(���s��)
	m_bDamage = false;							//�_���[�W��^����
	m_type = MOTION_TYPE_IDOL;					//���݂̃��[�V����
	m_battleMode = BATTLEMODE_NONE;				//�o�g�����[�h
	m_pHP = nullptr;							//HP
	m_pCombo = nullptr;							//�R���{

	//�X�e�[�^�X
	m_status.nAttack = 0;			//�U����
	m_status.nAttackTime = 0;		//�U������
	m_status.fSpeed = 0.0f;			//���x
	m_status.fLife = 0.0f;			//�̗�
	m_status.fRemLife = 0.0f;		//�c��̗�(%)
	m_status.fMaxLife = 0.0f;		//�ő�̗�

	//���f��
	for (int i = 0; i < MAX_PARTS; i++)
	{
		m_pModel[i] = nullptr;
	}

	//��
	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = nullptr;
	}

	/* �� ���[�V������� �� */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;

	//----------------------------
	// �L�[�Z�b�g���̏�����
	//----------------------------
	for (int nCnt = 0; nCnt < MOTION_TYPE_MAX; nCnt++)
	{
		for (int i = 0; i < MOTION_TYPE_MAX; i++)
		{
			for (int j = 0; j < MAX_PARTS; j++)
			{
				m_aMotionSet[nCnt].aKeySet[i].aKey[j].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
				m_aMotionSet[nCnt].aKeySet[i].aKey[j].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
			}

			m_aMotionSet[nCnt].bLoop = false;
			m_aMotionSet[nCnt].aKeySet[i].nFrame = 0;	//�t���[����
		}


		m_aMotionSet[nCnt].nNumKey = 0;
		m_aMotionSet[nCnt].bLoop = false;
	}
}

//========================
// �f�X�g���N�^
//========================
CPlayer::~CPlayer()
{

}

//========================
// ������
//========================
HRESULT CPlayer::Init(D3DXVECTOR3 pos)
{
	//--------------------
	// �����l�̐ݒ�
	//--------------------
	m_pos = pos;						//�ʒu
	fSizeWidth = 30.0f;					//���f���̕�
	fSizeDepth = 30.0f;					//���f���̉��s��
	m_status.fLife = 300.0f;			//�̗�
	m_status.fRemLife = 100.0f;			//�c��̗�(%)
	m_status.fMaxLife = m_status.fLife;	//�ő�̗�
	m_status.nAttack = 20;				//�U����
	m_nNumCombo = 0;					//�R���{��
	m_status.nComboValue = 1;			//�R���{�̉��Z�l
	m_status.fSpeed = 7.0f;				//���x

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//��]�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);

	//--------------------
	// HP�̐���
	//--------------------
	{
		D3DXVECTOR3 hpPos(300.0f, 50.0f, 0.0f);
		m_pHP = CHP::Create(hpPos, 500.0f, 30.0f, CHP::HPTYPE_PLAYER);
		m_pHP->SetLife(m_status.fLife, m_status.fRemLife);	//HP�̐ݒ�
	}

	//--------------------
	// ���f���̐���
	//--------------------
	SetModel();

	//-----------------------
	// ���f���̑傫����ݒ�
	//-----------------------
	m_vtxMin = D3DXVECTOR3(-fSizeWidth, 10.0f, -fSizeDepth);
	m_vtxMax = D3DXVECTOR3(fSizeWidth, 150.0f, fSizeDepth);

	m_size.x = m_vtxMax.x - m_vtxMin.x;
	m_size.y = m_vtxMax.y - m_vtxMin.y;
	m_size.z = m_vtxMax.z - m_vtxMin.z;

	//-----------------------
	// ���̕\��
	//-----------------------
	//SetLine();

	//-----------------------
	// ���[�V�����̓ǂݍ���
	//-----------------------
	GetFileMotion();

	//-----------------------
	// �R���{���\��
	//-----------------------
	m_pCombo = CCombo::Create(D3DXVECTOR3(1000.0f, 360.0f, 0.0f), m_nNumCombo);

	return S_OK;
}

//========================
// �I��
//========================
void CPlayer::Uninit()
{
	//-------------------------
	// ���f���̏I��
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		if (m_pModel[i])
		{//���f����null����Ȃ��Ȃ�
			m_pModel[i]->Uninit();
			delete m_pModel[i];
			m_pModel[i] = nullptr;
		}
	}

	CObject::Release();
}

//========================
// �X�V
//========================
void CPlayer::Update()
{
	//-------------------------
	// ���[�V�����̃��Z�b�g
	//-------------------------
	if (m_type != MOTION_TYPE_ATTACK)
	{//�U�����[�V��������Ȃ��Ȃ�
		//�ҋ@���[�V�����ɂ���
		ChangeMotion(MOTION_TYPE_IDOL);
	}

	//-------------------------
	// ���f���̍X�V
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		if (m_pModel[i])
		{//���f����null����Ȃ��Ȃ�
			m_pModel[i]->Update();
		}
	}

	if (!CGame::GetFinish())
	{//�I���t���O�������Ă��Ȃ��Ȃ�
		//-------------------------
		// �ړ�
		//-------------------------
		// �W���C�p�b�h�ł̑���
		CInputJoypad* joypad = CApplication::GetJoypad();

		if (m_type != MOTION_TYPE_ATTACK)
		{//�U��������Ȃ��Ȃ�
			if (!joypad->IsJoyPadUse(0))
			{//�W���C�p�b�h���g���Ă��Ȃ��Ȃ�
				MoveKeyboard(DIK_W, DIK_S, DIK_A, DIK_D);	//�L�[�{�[�h
			}
			else
			{
				MoveJoypad();	//�W���C�p�b�h
			}
		}

		//�^�C���̉�]
		m_pModel[0]->SetRotX(m_rotWheel);

		//-------------------------
		// �U������
		//-------------------------
		Attack();

		//-------------------------
		// ���[�h�`�F���W
		//-------------------------
		ChangeMode();
	}

	//-------------------------
	// ���[�V�����̐ݒ�
	//-------------------------
	SetMotion(m_type, m_aMotionSet[m_type].bLoop, m_aMotionSet[m_type].nNumKey);

	//-------------------------
	// ���̍X�V
	//-------------------------
	//UpdateLine();

	//-------------------------
	// �̗͂̌���
	//-------------------------
	if (CInputKeyboard::Press(DIK_0))
	{
		m_status.fLife--;	//�̗͂̌���

		//�c��̗͂��v�Z
		m_status.fRemLife = m_status.fLife * 100 / m_status.fMaxLife;

		//HP�̐ݒ�
		m_pHP->SetLife(m_status.fLife, m_status.fRemLife);
	}
}

//========================
// �`��
//========================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���f���̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//---------------------------------------
	// �N�H�[�^�j�I�����g������]�̔��f
	//---------------------------------------
	/*//�N�H�[�^�j�I���쐬
	//�����F�N�H�[�^�j�I���A��]���A��]�p
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	//�N�H�[�^�j�I�������]�}�g���b�N�X���쐬
	//�����F��]�}�g���b�N�X�A�N�H�[�^�j�I��
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	//��]�}�g���b�N�X(�ۑ��p)�ɔ��f
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);

	//���[���h�}�g���b�N�X�ɔ��f
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);*/

	//���f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-------------------------
	// ���f���̕`��
	//-------------------------
	for (int i = 0; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		if (!m_pModel[i])
		{//���f����null�Ȃ�
			return;
		}

		m_pModel[i]->Draw(&m_mtxWorld);
	}
}

//========================
// ����
//========================
CPlayer* CPlayer::Create()
{
	CPlayer *pPlayer = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pPlayer = new CPlayer;	//����

	if (pPlayer != nullptr)
	{//NULL�`�F�b�N
		//������
		pPlayer->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pPlayer->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer;
}

//========================
// ���f���̐���
//========================
void CPlayer::SetModel()
{
	//���f��0�F�^�C��
	m_pModel[0] = CModel::Create("data\\MODEL\\wheel.x", nullptr,
		D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f��1�F��
	m_pModel[1] = CModel::Create("data\\MODEL\\body.x", nullptr,
		D3DXVECTOR3(0.0f, 30.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f��2�F��
	m_pModel[2] = CModel::Create("data\\MODEL\\head.x", m_pModel[1],
		D3DXVECTOR3(0.0f, 105.0f, 12.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f��3�F�E�r
	m_pModel[3] = CModel::Create("data\\MODEL\\armR.x", m_pModel[1],
		D3DXVECTOR3(-25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f��4�F�E��
	m_pModel[4] = CModel::Create("data\\MODEL\\handR.x", m_pModel[3],
		D3DXVECTOR3(-4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f��5�F���r
	m_pModel[5] = CModel::Create("data\\MODEL\\armL.x", m_pModel[1],
		D3DXVECTOR3(25.0f, 85.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f��6�F����
	m_pModel[6] = CModel::Create("data\\MODEL\\handL.x", m_pModel[5],
		D3DXVECTOR3(4.0f, -40.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================
// �t�@�C�����烂�[�V���������擾
//=====================================
void CPlayer::GetFileMotion()
{
	const int nMaxText = 256;	//�����̍ő吔

	FILE *pFile;				//�t�@�C���|�C���^��錾����
	char cText[nMaxText];		//1�s���̕����ǂݎ��p�ϐ�
	char cTextHead[nMaxText];	//�����������悤�̕ϐ�
	int nNumMotion = 0;			//�ǂݍ��ݒ��̃��[�V�����ԍ�
	int nNumKey = 0;			//�ǂݍ��ݒ��̃L�[�ԍ�
	int nNumParts = 0;			//�ǂݍ��ݒ��̃p�[�c�ԍ�

	//-------------------------
	// �t�@�C���̓ǂݍ���
	//-------------------------
	//�t�@�C�����J��
	pFile = fopen("data/MOTION/player.txt", "r");

	if (pFile == nullptr)
	{//�J���Ȃ�������
		assert(false);
	}

	//�����̓ǂݎ�菈��
	while (fgets(cText, nMaxText, pFile) != nullptr)
	{
		//������̕���
		sscanf(cText, "%s", &cTextHead);

		//===================================
		// ���[�V�������
		//===================================
		if (strcmp(&cTextHead[0], "MOTIONSET") == 0)
		{//��������MOTIONSET�Ȃ�
			//�����̓ǂݎ�菈��
			while (fgets(cText, nMaxText, pFile) != nullptr)
			{
				//������̕���
				sscanf(cText, "%s", &cTextHead);

				//------------------------
				// ���[�v���邩�ǂ���
				//------------------------
				if (strcmp(&cTextHead[0], "LOOP") == 0)
				{//��������LOOP�Ȃ�
					int nLoop = 0;

					//�����񂩂烋�[�v�̒l��ǂݎ��
					sscanf(cText, "%s = %d", &cTextHead, &nLoop);

					if (nLoop == 0)
					{//�ǂݎ�����l��0�Ȃ�
						m_aMotionSet[nNumMotion].bLoop = false;
					}
					else if (nLoop == 1)
					{//�ǂݎ�����l��1�Ȃ�
						m_aMotionSet[nNumMotion].bLoop = true;
					}
				}
				//------------------------
				// �L�[�̍ő吔
				//------------------------
				else if (strcmp(&cTextHead[0], "NUM_KEY") == 0)
				{//��������NUM_KEY�Ȃ�
					//�����񂩂�L�[�̍ő吔��ǂݎ��
					sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].nNumKey);
				}
				//===================================
				// �L�[�Z�b�g���
				//===================================
				else if (strcmp(&cTextHead[0], "KEYSET") == 0)
				{//��������KEYSET�Ȃ�
					//�����̓ǂݎ�菈��
					while (fgets(cText, nMaxText, pFile) != nullptr)
					{
						//������̕���
						sscanf(cText, "%s", &cTextHead);

						//------------------------
						// �t���[����
						//------------------------
						if (strcmp(&cTextHead[0], "FRAME") == 0)
						{//��������FRAME�Ȃ�
							//�����񂩂�L�[�̍ő吔��ǂݎ��
							sscanf(cText, "%s = %d", &cTextHead, &m_aMotionSet[nNumMotion].aKeySet[nNumKey].nFrame);
						}
						//===================================
						// �L�[���
						//===================================
						else if (strcmp(&cTextHead[0], "KEY") == 0)
						{//��������KEY�Ȃ�
							//�����̓ǂݎ�菈��
							while (fgets(cText, nMaxText, pFile) != nullptr)
							{
								//������̕���
								sscanf(cText, "%s", &cTextHead);

								//------------------------
								// �ʒu
								//------------------------
								if (strcmp(&cTextHead[0], "POS") == 0)
								{//��������POS�Ȃ�
									//�����񂩂�ʒu��ǂݎ��
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.x,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.y,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].pos.z);
								}
								//------------------------
								// ����
								//------------------------
								else if (strcmp(&cTextHead[0], "ROT") == 0)
								{//��������ROT�Ȃ�
									//�����񂩂������ǂݎ��
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.x,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.y,
										&m_aMotionSet[nNumMotion].aKeySet[nNumKey].aKey[nNumParts].rot.z);
								}
								else if (strcmp(&cTextHead[0], "END_KEY") == 0)
								{//�L�[�̓ǂݎ�肪�I��������
									if (nNumParts + 1 < MAX_PARTS)
									{//�p�[�c���𒴂��Ȃ��Ȃ�
										//�p�[�c�ԍ��̉��Z
										nNumParts++;
									}
									else
									{//�p�[�c�����ǂݍ��񂾂�
										//�p�[�c�ԍ������Z�b�g
										nNumParts = 0;
									}
									break;
								}
							}
						}
						else if (strcmp(&cTextHead[0], "END_KEYSET") == 0)
						{//�L�[�Z�b�g�̓ǂݎ�肪�I��������
							if (nNumKey < m_aMotionSet[nNumMotion].nNumKey)
							{//�L�[�����ő傶��Ȃ��Ȃ�
								//�L�[�ԍ��̉��Z
								nNumKey++;
							}
							break;
						}
					}
				}
				else if (strcmp(&cTextHead[0], "END_MOTIONSET") == 0)
				{//���[�V�����̓ǂݎ�肪�I��������
					//�L�[�ԍ������Z�b�g
					nNumKey = 0;
					//���[�V�����ԍ��̉��Z
					nNumMotion++;
					break;
				}
			}
		}

		//---------------------------
		// �ۑ����̕�����̏�����
		//---------------------------
		ZeroMemory(&cText, sizeof(cText));
		ZeroMemory(&cTextHead, sizeof(cTextHead));
	}

	//�t�@�C�������
	fclose(pFile);
}

//=====================================
// ���[�V�����̐ݒ�
// �����F��ށA���[�v��ԁA�L�[��
//=====================================
void CPlayer::SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey)
{
	if (m_nCurrentKey + 1 >= nNumKey)
	{//�L�[���ő吔�ɒB������
		if (bLoop)
		{//���[�v����Ȃ�
			m_nCurrentKey = 0;	//�L�[�ԍ��̏�����
		}
		else
		{
			return;
		}
	}

	for (int i = 1; i < MAX_PARTS; i++)
	{//���f���p�[�c������
		//-----------------------------------------
		// NULL�`�F�b�N
		//-----------------------------------------
		if (!m_pModel[i])
		{//���f���p�[�c��null�Ȃ�
			return;
		}

		//�L�[�����������ϐ�
		KEY key = m_aMotionSet[type].aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = m_aMotionSet[type].aKeySet[m_nCurrentKey + 1].aKey[i];

		//-----------------------------------------
		// ���ݒl���擾
		//-----------------------------------------
		//�ʒu���擾
		float fPosX = m_pModel[i]->GetPos().x;
		float fPosY = m_pModel[i]->GetPos().y;
		float fPosZ = m_pModel[i]->GetPos().z;

		//�������擾
		float fRotX = m_pModel[i]->GetRot().x;
		float fRotY = m_pModel[i]->GetRot().y;
		float fRotZ = m_pModel[i]->GetRot().z;

		//-----------------------------------------
		// �����̌v�Z
		// (�I���l - �J�n�l)
		//-----------------------------------------
		//�ʒu
		float fDifPosX = keyNext.pos.x - key.pos.x;
		float fDifPosY = keyNext.pos.y - key.pos.y;
		float fDifPosZ = keyNext.pos.z - key.pos.z;

		//����
		float fDifRotX = keyNext.rot.x - key.rot.x;
		float fDifRotY = keyNext.rot.y - key.rot.y;
		float fDifRotZ = keyNext.rot.z - key.rot.z;

		//-----------------------------------------
		// ���Βl�̌v�Z
		// (���[�V�����J�E���^�[ / �t���[����)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)m_aMotionSet[type].aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// ���ݒl�̌v�Z
		// (�J�n�l + (���� * ���Βl))
		//-----------------------------------------
		//�ʒu
		fPosX += key.pos.x + (fDifPosX * fNumRelative);
		fPosY += key.pos.y + (fDifPosY * fNumRelative);
		fPosZ += key.pos.z + (fDifPosZ * fNumRelative);

		//����
		fRotX = key.rot.x + (fDifRotX * fNumRelative);
		fRotY = key.rot.y + (fDifRotY * fNumRelative);
		fRotZ = key.rot.z + (fDifRotZ * fNumRelative);

		//-----------------------------------------
		// ���f�����̐ݒ�
		//-----------------------------------------
		//�ʒu�̐ݒ�
		m_pModel[i]->SetPos(D3DXVECTOR3(fPosX, fPosY, fPosZ));

		//�����̐ݒ�
		m_pModel[i]->SetRot(D3DXVECTOR3(fRotX, fRotY, fRotZ));
	}

	//���[�V�����J�E���^�[��i�߂�
	m_nCntMotion++;

	//-------------------------
	// ������
	//-------------------------
	if (m_nCntMotion >= m_aMotionSet[type].aKeySet[m_nCurrentKey].nFrame)
	{//���[�V�����J�E���^�[���Đ��t���[���ɒB������
		m_nCurrentKey++;	//�L�[�ԍ������Z
		m_nCntMotion = 0;	//���[�V�����J�E���^�[��������
	}
}

//=====================================
// ���[�V�����̕ύX
// �����F�ύX���������[�V�����̗�
//=====================================
void CPlayer::ChangeMotion(MOTION_TYPE type)
{
	//���[�V�����̕ύX
	m_type = type;

	//���[�V�������̏�����
	if (m_type == MOTION_TYPE_ATTACK)
	{
		m_nCurrentKey = 0;
		m_nCntMotion = 0;
	}
}

//=====================================
// �ړ�
// �����F��L�[,���L�[,���L�[,�E�L�[
//=====================================
void CPlayer::MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey)
{
	//�J�����̏��擾
	D3DXVECTOR3 cameraRot = CGame::GetCamera()->GetRot();

	//-------------------------------
	// �v���C���[�̑���
	//-------------------------------
	if (CInputKeyboard::Press(nLeftKey))
	{//A�L�[�������ꂽ
		if (CInputKeyboard::Press(nUpKey))
		{//W�L�[�������ꂽ
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;	//�����ړ�
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;	//�����̐؂�ւ�
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//S�L�[�������ꂽ
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;	//���O�ړ�
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;	//���ړ�
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nRightKey))
	{//D�L�[�������ꂽ
		if (CInputKeyboard::Press(nUpKey))
		{//W�L�[�������ꂽ
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;	//�E���ړ�
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//S�L�[�������ꂽ
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;	//�E�O�ړ�
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;	//�E�ړ�
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.5f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nUpKey))
	{//W�L�[�������ꂽ
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;	//���ړ�
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	else if (CInputKeyboard::Press(nDownKey))
	{//S�L�[�������ꂽ
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;	//�O�ړ�
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 0.0f;
	}

	//������ړI�̊p�x�ɍ��킹��
	SetRot();

	//�^�C���̉�]�ʂ̉��Z
	m_rotWheel += D3DXToRadian(-nWheelRotValue);

	if (!CInputKeyboard::Press(nUpKey) && !CInputKeyboard::Press(nDownKey)
		&& !CInputKeyboard::Press(nRightKey) && !CInputKeyboard::Press(nLeftKey))
	{//�ړ��L�[��������Ă��Ȃ��Ȃ�
		//�^�C���̉�]�ʂ�0�ɂ���
		m_rotWheel = 0;
	}
	else
	{//�ǂꂩ��������Ă���Ȃ�
		//�ړ����[�V�����ɂ���
		ChangeMotion(MOTION_TYPE_MOVE);
	}
}

//===========================
// �W���C�p�b�h���g�����ړ�
//===========================
void CPlayer::MoveJoypad()
{
	// �W���C�p�b�h�ł̑���
	CInputJoypad* joypad = CApplication::GetJoypad();
	D3DXVECTOR3 stick = joypad->Stick(CInputJoypad::JOYKEY_LEFT_STICK, 0);

	if (joypad->IsJoyPadUse(0) == false)
	{//�W���C�p�b�h���g���Ă��Ȃ��Ȃ�
		return;
	}

	//�X�e�B�b�N�𓮂����l�̐ݒ�
	float fMoveValue = 0.5f;

	//�J�����̏��擾
	D3DXVECTOR3 cameraRot = CGame::GetCamera()->GetRot();

	//-----------------------------------
	// �E�ړ�
	//-----------------------------------
	if (stick.x >= fMoveValue)
	{
		// �X�e�B�b�N��|���������ֈړ�����
		if (stick.y <= -fMoveValue)
		{//�E���ړ�
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;	//�����̐؂�ւ�
		}
		else if (stick.y >= fMoveValue)
		{//�E�O�ړ�
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * fMoveValue;
		}
	}
	//-----------------------------------
	// ���ړ�
	//-----------------------------------
	else if (stick.x <= -fMoveValue)
	{
		if (stick.y <= -fMoveValue)
		{//�����ړ�
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;
		}
		else if (stick.y >= fMoveValue)
		{//���O�ړ�
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * fMoveValue) * m_status.fSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * fMoveValue;
		}
	}
	//-----------------------------------
	// ���ړ�
	//-----------------------------------
	else if (stick.y <= -fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * m_status.fSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	//-----------------------------------
	// �O�ړ�
	//-----------------------------------
	else if (stick.y >= fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * m_status.fSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 0.0f;
	}

	//������ړI�̊p�x�ɍ��킹��
	SetRot();

	//�^�C���̉�]�ʂ̉��Z
	m_rotWheel += D3DXToRadian(-nWheelRotValue);

	if (stick.x < fMoveValue && stick.x > -fMoveValue 
		&& stick.y < fMoveValue && stick.y > -fMoveValue)
	{//�X�e�B�b�N����������Ă��Ȃ��Ȃ�
		//�^�C���̉�]�ʂ�0�ɂ���
		m_rotWheel = 0;
	}
}

//===========================
// �U������
//===========================
void CPlayer::Attack()
{
	//-----------------------------------
	// �U�����[�V�����ֈڍs
	//-----------------------------------
	if (CInputKeyboard::Trigger(DIK_RETURN) && m_type != MOTION_TYPE_ATTACK)
	{//ENTER�L�[�������ꂽ & �U�����[�V����������Ȃ��Ȃ�
		//�U�����[�V�����ɂ���
		ChangeMotion(MOTION_TYPE_ATTACK);
	}

	//-----------------------------------
	// ���[�V�����ƍU�����Ԃ����킹��
	//-----------------------------------
	if (m_type == MOTION_TYPE_ATTACK)
	{//�U�����[�V�������Ȃ�
		int nAttackFream = 0;
		for (int i = 0; i < m_aMotionSet[m_type].nNumKey; i++)
		{//�L�[��-1�񕪉�
			//�U�����[�V�����̃t���[���������v����
			nAttackFream += m_aMotionSet[m_type].aKeySet[i].nFrame;
		}

		if (nAttackFream <= m_status.nAttackTime)
		{//�U�����Ԃ��U�����[�V�����̃t���[�����̍��v�𒴂�����
			//�ҋ@���[�V�����ɂ���
			ChangeMotion(MOTION_TYPE_IDOL);
			m_status.nAttackTime = 0;	//�U�����Ԃ̃��Z�b�g
			m_bDamage = false;			//�_���[�W��^���Ă��Ȃ���Ԃɂ���
		}
		else
		{
			//�U�����Ԃ����Z
			m_status.nAttackTime++;
		}
	}

	//-----------------------------------
	// ���Ƃ̓����蔻��
	//-----------------------------------
	if (m_type == CPlayer::MOTION_TYPE_ATTACK && m_pModel[nSwordNumber]->GetCollisionAttack()
		&& !m_bDamage)
	{//�v���C���[���U���� & ���Ɠ������Ă��� & �_���[�W��^���Ă��Ȃ��Ȃ�
		//�U���͕��G�̗̑͂�����
		CGame::GetEnemy()->SubLife(m_status.nAttack);

		//�R���{���̉��Z
		CGame::GetPlayer()->AddCombo(m_status.nComboValue);

		//�_���[�W��^������Ԃɂ���
		m_bDamage = true;
	}
}

//===========================
// ���[�h�`�F���W
//===========================
void CPlayer::ChangeMode()
{
	//-------------------------------
	// ���[�h�̐؂�ւ�
	//-------------------------------
	if (CInputKeyboard::Trigger(DIK_1))
	{
		m_battleMode = BATTLEMODE_ATTACK;
	}
	else if (CInputKeyboard::Trigger(DIK_2))
	{
		m_battleMode = BATTLEMODE_SPEED;
	}
	else if (CInputKeyboard::Trigger(DIK_3))
	{
		m_battleMode = BATTLEMODE_COMBO;
	}

	//-------------------------------
	// ���[�h�ʃX�e�[�^�X�̃��Z�b�g
	//-------------------------------
	SetAttack(fDefaultAttack);		//�U����
	SetSpeed(fDefaultSpeed);		//���x
	SetComboValue(1);				//�R���{�̉��Z�l

	//-------------------------------
	// ���[�h���Ƃ̏���
	//-------------------------------
	switch (m_battleMode)
	{
	case BATTLEMODE_ATTACK:
		SetAttack(fDefaultAttack * 5.0f);			//�U���͂̋���
		break;
	case BATTLEMODE_SPEED:
		SetSpeed(fDefaultSpeed * 2.0f);				//���x�̋���
		break;
	case BATTLEMODE_COMBO:
		SetComboValue(m_status.nComboValue * 2);	//�R���{���Z�̋���
		break;
	default:
		break;
	}
}

//===========================
// �p�x�̐ݒ�
//===========================
void CPlayer::SetRot()
{
	//-------------------------------
	// �ړI�̊p�x�̐��K��
	//-------------------------------
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{//��]�������p�x��180�ȏ�Ȃ�
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{//��]�������p�x��-180�ȉ��Ȃ�
		m_rotDest.y += D3DX_PI * 2;
	}

	//-------------------------------
	// �ړI�̊p�x�܂ŉ�]����
	//-------------------------------
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.05f;	//��������

	//-------------------------------
	// �p�x�̐��K��
	//-------------------------------
	if (m_rot.y > D3DX_PI)
	{//�p�x��180�ȏ�Ȃ�
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{//�p�x��-180�ȉ��Ȃ�
		m_rot.y += D3DX_PI * 2;
	}
}

//========================
// ���̐ݒu
//========================
void CPlayer::SetLine()
{
	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�ϊ��s����g����Min,Max�����߂�
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//�������l���܂Ƃ߂�
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLine[i] = CLine::CreateAll(m_pLine[i], i, m_pos, min, max);
	}
}

//========================
// ���̏��̍X�V
//========================
void CPlayer::UpdateLine()
{
	//���[���h�ϊ��s����g����Min,Max�����߂�
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//�������l���܂Ƃ߂�
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

//===========================
// �v���C���[�̓����蔻��
//===========================
bool CPlayer::GetCollisionPlayer()
{
	//-----------------------------
	// �v���C���[�̒[�̐ݒ�
	//-----------------------------
	float fLeft = m_pos.x - (m_size.x / 2);		//�v���C���[�̍��[
	float fRight = m_pos.x + (m_size.x / 2);	//�v���C���[�̉E�[
	float fTop = m_pos.y + (m_size.y / 2);		//�v���C���[�̏�[
	float fBottom = m_pos.y - (m_size.y / 2);	//�v���C���[�̉��[
	float fFront = m_pos.z - (m_size.z / 2);	//�v���C���[�̑O�[
	float fBack = m_pos.z + (m_size.z / 2);		//�v���C���[�̌�[

	//-----------------------------
	// �G�̏����擾
	//-----------------------------
	D3DXVECTOR3 enemyPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	D3DXVECTOR3 enemySize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (CGame::GetEnemy() != nullptr)
	{
		enemyPos = CGame::GetEnemy()->GetPosition();	//�ʒu
		enemySize = CGame::GetEnemy()->GetSize();		//�傫��
	}

	//-----------------------------
	// �����蔻��
	//-----------------------------
	if (enemyPos.x + enemySize.x >= fLeft
		&& enemyPos.x - enemySize.x <= fRight
		&& enemyPos.y + enemySize.y >= fTop
		&& enemyPos.y - enemySize.y <= fBottom
		&& enemyPos.z + enemySize.z >= fFront
		&& enemyPos.z - enemySize.z <= fBack)
	{//�G�ƃv���C���[����������
		return true;
	}

	return false;
}

//===========================
// �R���{���̉��Z
//===========================
void CPlayer::AddCombo(int nNumber)
{
	m_nNumCombo = m_pCombo->AddNumber(nNumber);
}