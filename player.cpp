//===================================================
//
// �v���C���[�̏���
// Author:Teruto Sato
//
//===================================================

//------------------------
// �C���N���[�h
//------------------------
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

//------------------------
// �ÓI�����o�ϐ��錾
//------------------------
const float CPlayer::fPlayerSpeed = 7.0f;

//------------------------
// �O���[�o���ϐ�
//------------------------
CPlayer::KEY_SET g_aKeySet[] =	//�L�[�Z�b�g���
{
	//----------------------
	// �L�[1
	//----------------------
	{ 40,	//�t���[����
		//		Pos				Rot
		{{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//�^�C��
		{ 0.0f,0.0f,0.0f , -1.0f,0.0f,0.0f },	//��
		{ 0.0f,0.0f,0.0f , 0.9f,0.0f,0.0f },	//��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,-0.56f },	//�E�r
		{ 0.0f,0.0f,0.0f , 0.75f,0.5f,0.0f },	//�E��
		{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.5f },	//���r
		{ 0.0f,0.0f,0.0f , 0.4f,0.0f,0.0f }},	//����
	},

	//----------------------
	// �L�[2
	//----------------------
	{ 40,	//�t���[����
			//		Pos				Rot
	{ { 0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f },	//�^�C��
	{ 0.0f,0.0f,0.0f , -1.0f,0.0f,0.0f },	//��
	{ 0.0f,0.0f,0.0f , 0.9f,0.0f,0.0f },	//��
	{ 0.0f,0.0f,0.0f , 0.0f,0.0f,-0.56f },	//�E�r
	{ 0.0f,0.0f,0.0f , 0.75f,0.5f,0.0f },	//�E��
	{ 0.0f,0.0f,0.0f , 0.0f,0.0f,0.5f },	//���r
	{ 0.0f,0.0f,0.0f , 0.4f,0.0f,0.0f } },	//����
	},
};

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

	//���f��
	for (int i = 0; i < MAX_PARTS; i++)
	{
		m_pModel[i] = nullptr;
	}

	//��
	for (int i = 0; i < nMaxLine; i++)
	{
		m_pLIne[i] = nullptr;
	}

	/* �� ���[�V������� �� */
	m_nCurrentKey = 0;
	m_nCntMotion = 0;
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
	// ���f���̑傫�����擾
	//-----------------------
	GetSize();

	//-----------------------
	// ���̕\��
	//-----------------------
	//SetLine();

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
	MoveKeyboard(DIK_W, DIK_S, DIK_A, DIK_D);	//�L�[�{�[�h
	MoveJoypad();	//�W���C�p�b�h

	//�^�C���̉�]
	m_pModel[0]->SetRotX(m_rotWheel);

	if (CInputKeyboard::Press(DIK_Z))
	{
		m_pModel[1]->SetRotX(m_rotWheel / 2);
	}

	//D3DXMatrix

	//-------------------------
	// ��]
	//-------------------------
	//��]���̐ݒ�
	/*m_vecAxis.x = sinf(D3DXToRadian(90));
	m_vecAxis.z = sinf(D3DXToRadian(90));

	//��]�p�̐ݒ�
	if (CInputKeyboard::Press(DIK_A))
	{//A�L�[�������ꂽ
		m_fValueRot = D3DXToRadian(fPlayerSpeed);
	}
	else if (CInputKeyboard::Press(DIK_D))
	{//D�L�[�������ꂽ
		m_fValueRot = -D3DXToRadian(fPlayerSpeed);
	}

	if (CInputKeyboard::Press(DIK_W))
	{//W�L�[�������ꂽ
		m_fValueRot = D3DXToRadian(fPlayerSpeed);
	}
	else if (CInputKeyboard::Press(DIK_S))
	{//S�L�[�������ꂽ
		m_fValueRot = -D3DXToRadian(fPlayerSpeed);
	}

	if (!CInputKeyboard::Press(DIK_A) && !CInputKeyboard::Press(DIK_D)
		&& !CInputKeyboard::Press(DIK_W) && !CInputKeyboard::Press(DIK_S))
	{//�ړ����Ă��Ȃ��Ȃ�
		m_fValueRot = 0.0f;
	}*/

	//-------------------------
	// ���[�V����
	//-------------------------
	SetMotion(true);

	//-------------------------
	// ���̍X�V
	//-------------------------
	//UpdateLine();
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
		D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//���f��1�F��
	m_pModel[1] = CModel::Create("data\\MODEL\\body.x", nullptr,
		D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

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

//========================
// ���[�V�����̐ݒ�
//========================
void CPlayer::SetMotion(bool bLoop)
{
	if (m_nCurrentKey + 1 >= MAX_KEY)
	{//�L�[���ő吔�ɒB������
		if (bLoop)
		{
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
		KEY key = g_aKeySet[m_nCurrentKey].aKey[i];
		KEY keyNext = g_aKeySet[m_nCurrentKey + 1].aKey[i];

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
		float fDifPosX = keyNext.fPosX - key.fPosX;
		float fDifPosY = keyNext.fPosY - key.fPosY;
		float fDifPosZ = keyNext.fPosZ - key.fPosZ;

		//����
		float fDifRotX = keyNext.fRotX - key.fRotX;
		float fDifRotY = keyNext.fRotY - key.fRotY;
		float fDifRotZ = keyNext.fRotZ - key.fRotZ;

		//-----------------------------------------
		// ���Βl�̌v�Z
		// (���[�V�����J�E���^�[ / �t���[����)
		//-----------------------------------------
		float fNumRelative = m_nCntMotion / (float)g_aKeySet[m_nCurrentKey].nFrame;

		//-----------------------------------------
		// ���ݒl�̌v�Z
		// (�J�n�l + (���� * ���Βl))
		//-----------------------------------------
		//�ʒu
		fPosX += key.fPosX + (fDifPosX * fNumRelative);
		fPosY += key.fPosY + (fDifPosY * fNumRelative);
		fPosZ += key.fPosZ + (fDifPosZ * fNumRelative);

		//����
		fRotX = key.fRotX + (fDifRotX * fNumRelative);
		fRotY = key.fRotY + (fDifRotY * fNumRelative);
		fRotZ = key.fRotZ + (fDifRotZ * fNumRelative);

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
	if (m_nCntMotion >= g_aKeySet[m_nCurrentKey].nFrame)
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
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.04f;	//��������

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
	m_pLIne[0] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLIne[1] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[2] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[3] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// ���
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[4] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[5] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[6] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[7] = CLine::Create(m_pos, start, end, lineCol);

	//-----------------------------------
	// �c��
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLIne[8] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[9] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[10] = CLine::Create(m_pos, start, end, lineCol);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[11] = CLine::Create(m_pos, start, end, lineCol);
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
	m_pLIne[0]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, min.y, max.z);
	m_pLIne[1]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[2]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, min.y, max.z);
	m_pLIne[3]->SetLine(start, end);

	//-----------------------------------
	// ���
	//-----------------------------------
	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[4]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[5]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, max.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[6]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, max.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[7]->SetLine(start, end);

	//-----------------------------------
	// �c��
	//-----------------------------------
	start = D3DXVECTOR3(min.x, min.y, min.z);
	end = D3DXVECTOR3(min.x, max.y, min.z);
	m_pLIne[8]->SetLine(start, end);

	start = D3DXVECTOR3(min.x, min.y, max.z);
	end = D3DXVECTOR3(min.x, max.y, max.z);
	m_pLIne[9]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, min.z);
	end = D3DXVECTOR3(max.x, max.y, min.z);
	m_pLIne[10]->SetLine(start, end);

	start = D3DXVECTOR3(max.x, min.y, max.z);
	end = D3DXVECTOR3(max.x, max.y, max.z);
	m_pLIne[11]->SetLine(start, end);
}

//===========================
// �傫���̎擾
//===========================
void CPlayer::GetSize()
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		//-------------------------------
		// �ŏ��l�̔�r
		//-------------------------------
		D3DXVECTOR3 vtxMin = m_pModel[i]->GetVtxMin();

		if (vtxMin.x <= m_vtxMin.x)
		{
			m_vtxMin.x = floorf(m_vtxMin.x);
		}
		if (vtxMin.y <= m_vtxMin.y)
		{
			m_vtxMin.y = floorf(m_vtxMin.y);
		}
		if (vtxMin.z <= m_vtxMin.z)
		{
			m_vtxMin.z = floorf(m_vtxMin.z);
		}

		//-------------------------------
		// �ő�l�̔�r
		//-------------------------------
		D3DXVECTOR3 vtxMax = m_pModel[i]->GetVtxMax();

		if (vtxMax.x >= m_vtxMax.x)
		{
			m_vtxMax.x = floorf(vtxMax.x);
		}
		if (vtxMax.y >= m_vtxMax.y)
		{
			m_vtxMax.y = floorf(vtxMax.y);
		}
		if (vtxMax.z >= m_vtxMax.z)
		{
			m_vtxMax.z = floorf(vtxMax.z);
		}
	}
}

//===========================
// �ʒu�̐ݒ�
//===========================
void CPlayer::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
// �ʒu�̎擾
//===========================
D3DXVECTOR3 CPlayer::GetPosition()
{
	return m_pos;
}

//========================
// �����̎擾
//========================
D3DXVECTOR3 CPlayer::GetRot()
{
	return m_rot;
}

//===========================
// ���̎擾
//===========================
float CPlayer::GetWidth()
{
	return 0.0f;
}

//===========================
// �����̎擾
//===========================
float CPlayer::GetHeight()
{
	return 0.0f;
}

//===========================
// ���[���h�}�g���b�N�X�̎擾
//===========================
D3DXMATRIX CPlayer::GetmtxWorld()
{
	return m_mtxWorld;
}