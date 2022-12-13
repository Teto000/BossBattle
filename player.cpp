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

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
const float CPlayer::fPlayerSpeed = 7.0f;

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
	m_type = MOTION_TYPE_IDOL;					//���݂̃��[�V����

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
	m_bLoop = false;

	//�L�[�Z�b�g���̏�����
	for (int i = 0; i < MOTION_TYPE_MAX; i++)
	{
		for (int j = 0; j < MAX_PARTS; j++)
		{
			m_aKeySet[i].aKey[j].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
			m_aKeySet[i].aKey[j].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		}

		m_aKeySet[i].nFrame = 0;	//�t���[����
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
	m_pos = pos;
	m_nCntMotion = 1;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//��]�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);

	//--------------------
	// ���f���̐���
	//--------------------
	SetModel();

	//-----------------------
	// ���f���̑傫����ݒ�
	//-----------------------
	m_vtxMin = D3DXVECTOR3(-70.0f, 0.0f, -120.0f);
	m_vtxMax = D3DXVECTOR3(70.0f, 100.0f, 30.0f);

	m_size.x = m_vtxMax.x - m_vtxMin.x;
	m_size.y = m_vtxMax.y - m_vtxMin.y;
	m_size.z = m_vtxMax.z - m_vtxMin.z;

	//-----------------------
	// ���̕\��
	//-----------------------
	SetLine();

	//-----------------------
	// ���[�V�����̓ǂݍ���
	//-----------------------
	GetFileMotion();

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
	//�ҋ@���[�V�����ɂ���
	m_type = MOTION_TYPE_IDOL;

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

	//-------------------------
	// �ړ�
	//-------------------------
	// �W���C�p�b�h�ł̑���
	CInputJoypad* joypad = CApplication::GetJoypad();

	if (!CGame::GetFinish())
	{//�I���t���O�������Ă��Ȃ��Ȃ�
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
	// ���[�V����
	//-------------------------
	//�L�[�Z�b�g���̐ݒ�
	//SetKeySet(m_type);

	//���[�V�����̐ݒ�
	SetMotion(m_nNumKey, m_bLoop);

	//-------------------------
	// ���̍X�V
	//-------------------------
	UpdateLine();
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
// �L�[�Z�b�g���̐ݒ�
// �����F���[�V�����̎��
//=====================================
void CPlayer::SetKeySet(int nMotionType)
{
	switch (nMotionType)
	{
	//===================================
	// �ҋ@���[�V����
	//===================================
	case MOTION_TYPE_IDOL:
		m_nNumKey = 3;	//�L�[��

		//----------------------
		// �L�[1
		//----------------------
		m_aKeySet[0].nFrame = 80;	//�t���[����
		m_aKeySet[0].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�^�C��
		m_aKeySet[0].aKey[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//��
		m_aKeySet[0].aKey[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//��
		m_aKeySet[0].aKey[3].rot = D3DXVECTOR3(-0.75f, 0.3f, -0.5f);	//�E�r
		m_aKeySet[0].aKey[4].rot = D3DXVECTOR3(0.5f, 0.5f, 0.4f);		//�E��
		m_aKeySet[0].aKey[5].rot = D3DXVECTOR3(0.7f, -0.5f, -0.06f);	//���r
		m_aKeySet[0].aKey[6].rot = D3DXVECTOR3(1.5f, -0.2f, -1.1f);		//����

		//----------------------
		// �L�[2
		//----------------------
		m_aKeySet[1].nFrame = 80;	//�t���[����
		m_aKeySet[1].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[1].rot = D3DXVECTOR3(-0.1f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[3].rot = D3DXVECTOR3(-0.6f, 0.3f, -0.5f);
		m_aKeySet[1].aKey[4].rot = D3DXVECTOR3(0.8f, 0.5f, 0.4f);
		m_aKeySet[1].aKey[5].rot = D3DXVECTOR3(0.9f, -0.3f, -0.06f);
		m_aKeySet[1].aKey[6].rot = D3DXVECTOR3(0.8f, -0.1f, -0.9f);

		//----------------------
		// �L�[3
		//----------------------
		m_aKeySet[2].nFrame = 80;	//�t���[����
		m_aKeySet[2].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[3].rot = D3DXVECTOR3(-0.75f, 0.3f, -0.5f);
		m_aKeySet[2].aKey[4].rot = D3DXVECTOR3(0.5f, 0.5f, 0.4f);
		m_aKeySet[2].aKey[5].rot = D3DXVECTOR3(0.7f, -0.5f, -0.06f);
		m_aKeySet[2].aKey[6].rot = D3DXVECTOR3(1.5f, -0.2f, -1.1f);
		break;

	//===================================
	// �ړ����[�V����
	//===================================
	case MOTION_TYPE_MOVE:
		m_nNumKey = 3;	//�L�[��

		//----------------------
		// �L�[1
		//----------------------
		m_aKeySet[0].nFrame = 80;	//�t���[����
		m_aKeySet[0].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 //�^�C��
		m_aKeySet[0].aKey[1].rot = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);	 //��
		m_aKeySet[0].aKey[2].rot = D3DXVECTOR3(0.9f, 0.0f, 0.0f);	 //��
		m_aKeySet[0].aKey[3].rot = D3DXVECTOR3(0.0f, 0.0f, -0.5f);	 //�E�r
		m_aKeySet[0].aKey[4].rot = D3DXVECTOR3(0.75f, 0.5f, 0.0f);	 //�E��
		m_aKeySet[0].aKey[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.5f);	 //���r
		m_aKeySet[0].aKey[6].rot = D3DXVECTOR3(0.4f, 0.0f, 0.0f);	 //����

		//----------------------
		// �L�[2
		//----------------------
		m_aKeySet[1].nFrame = 80;	//�t���[����
		m_aKeySet[1].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[1].rot = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[2].rot = D3DXVECTOR3(0.9f, 0.0f, 0.0f);
		m_aKeySet[1].aKey[3].rot = D3DXVECTOR3(0.0f, 0.0f, -0.8f);
		m_aKeySet[1].aKey[4].rot = D3DXVECTOR3(0.75f, 0.5f, 0.0f);
		m_aKeySet[1].aKey[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.8f);
		m_aKeySet[1].aKey[6].rot = D3DXVECTOR3(0.4f, 0.0f, 0.0f);

		//----------------------
		// �L�[2
		//----------------------
		m_aKeySet[2].nFrame = 80;	//�t���[����
		m_aKeySet[2].aKey[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[1].rot = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[2].rot = D3DXVECTOR3(0.9f, 0.0f, 0.0f);
		m_aKeySet[2].aKey[3].rot = D3DXVECTOR3(0.0f, 0.0f, -0.5f);
		m_aKeySet[2].aKey[4].rot = D3DXVECTOR3(0.75f, 0.5f, 0.0f);
		m_aKeySet[2].aKey[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.5f);
		m_aKeySet[2].aKey[6].rot = D3DXVECTOR3(0.4f, 0.0f, 0.0f);
		break;
	}

	//���[�V�����̐ݒ�
	SetMotion(m_nNumKey, true);
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
	int nNumKey = 0;			//�ǂݍ��ݒ��̃L�[��
	int nNumParts = 0;			//�ǂݍ��ݒ��̃p�[�c��

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
						m_bLoop = false;
					}
					else if (nLoop == 1)
					{//�ǂݎ�����l��1�Ȃ�
						m_bLoop = true;
					}
				}
				//------------------------
				// �L�[�̍ő吔
				//------------------------
				else if (strcmp(&cTextHead[0], "NUM_KEY") == 0)
				{//��������NUM_KEY�Ȃ�
					//�����񂩂�L�[�̍ő吔��ǂݎ��
					sscanf(cText, "%s = %d", &cTextHead, &m_nNumKey);
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
							sscanf(cText, "%s = %d", &cTextHead, &m_aKeySet[nNumKey].nFrame);
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
										&m_aKeySet[nNumKey].aKey[nNumParts].pos.x,
										&m_aKeySet[nNumKey].aKey[nNumParts].pos.y,
										&m_aKeySet[nNumKey].aKey[nNumParts].pos.z);
								}
								//------------------------
								// ����
								//------------------------
								else if (strcmp(&cTextHead[0], "ROT") == 0)
								{//��������ROT�Ȃ�
									//�����񂩂������ǂݎ��
									sscanf(cText, "%s = %f %f %f", &cTextHead,
										&m_aKeySet[nNumKey].aKey[nNumParts].rot.x,
										&m_aKeySet[nNumKey].aKey[nNumParts].rot.y,
										&m_aKeySet[nNumKey].aKey[nNumParts].rot.z);
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
							if (nNumKey < m_nNumKey)
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
// �����F�L�[���A���[�v���
//=====================================
void CPlayer::SetMotion(int nNumKey, bool bLoop)
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
		KEY key = m_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = m_aKeySet[m_nCurrentKey + 1].aKey[i];

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
		float fNumRelative = m_nCntMotion / (float)m_aKeySet[m_nCurrentKey].nFrame;

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
	if (m_nCntMotion >= m_aKeySet[m_nCurrentKey].nFrame)
	{//���[�V�����J�E���^�[���Đ��t���[���ɒB������
		m_nCurrentKey++;	//�L�[�ԍ������Z
		m_nCntMotion = 0;	//���[�V�����J�E���^�[��������
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
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;	//�����ړ�
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;	//�����̐؂�ւ�
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//S�L�[�������ꂽ
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;	//���O�ړ�
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//���ړ�
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nRightKey))
	{//D�L�[�������ꂽ
		if (CInputKeyboard::Press(nUpKey))
		{//W�L�[�������ꂽ
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;	//�E���ړ�
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;
		}
		else if (CInputKeyboard::Press(nDownKey))
		{//S�L�[�������ꂽ
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;	//�E�O�ړ�
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;	//�E�ړ�
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.5f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.5f;
		}
	}
	else if (CInputKeyboard::Press(nUpKey))
	{//W�L�[�������ꂽ
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;	//���ړ�
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	else if (CInputKeyboard::Press(nDownKey))
	{//S�L�[�������ꂽ
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;	//�O�ړ�
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
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
		m_type = MOTION_TYPE_MOVE;
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
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.75f;	//�����̐؂�ւ�
		}
		else if (stick.y >= fMoveValue)
		{//�E�O�ړ�
			m_pos.x += sinf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y - D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x += sinf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
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
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.25f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.75f;
		}
		else if (stick.y >= fMoveValue)
		{//���O�ړ�
			m_pos.x += sinf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_pos.z += cosf(cameraRot.y - D3DX_PI * 0.75f) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * 0.25f;
		}
		else
		{
			m_pos.x -= sinf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_pos.z -= cosf(cameraRot.y + D3DX_PI * fMoveValue) * fPlayerSpeed;
			m_rotDest.y = cameraRot.y + D3DX_PI * fMoveValue;
		}
	}
	//-----------------------------------
	// ���ړ�
	//-----------------------------------
	else if (stick.y <= -fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1.0f) * fPlayerSpeed;
		m_rotDest.y = cameraRot.y + D3DX_PI * 1.0f;
	}
	//-----------------------------------
	// �O�ړ�
	//-----------------------------------
	else if (stick.y >= fMoveValue)
	{
		m_pos.x -= sinf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
		m_pos.z -= cosf(cameraRot.y + D3DX_PI * 0.0f) * fPlayerSpeed;
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
	//���̐F
	D3DXCOLOR lineCol(1.0f, 0.0f, 0.0f, 1.0f);

	//���[���h�ϊ��s����g����Min,Max�����߂�
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//�������l���܂Ƃ߂�
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// ����
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLine[0] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLine[1] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[2] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[3] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// ���
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[4] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[5] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[6] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[7] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// �c��
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLine[8] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[9] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[10] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[11] = CLine::Create(m_pos, start, end, lineCol);
}

//========================
// ���̏��̍X�V
//========================
void CPlayer::UpdateLine()
{
	//���[���h�ϊ��s����g����Min,Max�����߂�
	D3DXVec3TransformCoord(&m_worldMin, &m_vtxMin, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldMax, &m_vtxMax, &m_mtxWorld);

	//�ő�ŏ������߂Ȃ���

	//�����܂܂Ȃ��ł������񂷂�

	//�������l���܂Ƃ߂�
	D3DXVECTOR3 min = m_worldMin;
	D3DXVECTOR3 max = m_worldMax;

	//-----------------------------------
	// ����
	//-----------------------------------
	D3DXVECTOR3 start = D3DXVECTOR3(min.x, min.y, min.z);
	D3DXVECTOR3 end = D3DXVECTOR3(max.x, min.y, min.z);
	m_pLine[0]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLine[1]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[2]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLine[3]->SetLine(start, end);

	//-----------------------------------
	// ���
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[4]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[5]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[6]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[7]->SetLine(start, end);

	//-----------------------------------
	// �c��
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLine[8]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLine[9]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLine[10]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLine[11]->SetLine(start, end);
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