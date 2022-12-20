//===================================================
//
// �v���C���[�w�b�_�[
// Author:Teruto Sato
//
//===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_	//��d�C���N���[�h�h�~

//--------------------------------
// �C���N���[�h
//--------------------------------
#include "object.h"

//--------------------------------
// �O���錾
//--------------------------------
class CModel;	//���f��
class CLine;	//���C��
class CHP;		//HP
class CCombo;	//�R���{

//--------------------------------
// �}�N����`
//--------------------------------
#define MAX_PARTS	(7)		//���f���p�[�c�̍ő吔
#define MAX_KEY		(4)		//�L�[�̍ő吔

//================================
// �v���C���[�N���X�̒�`
//================================
class CPlayer : public CObject
{
public:
	//�L�[�v�f
	struct KEY
	{
		D3DXVECTOR3 pos;	//�ʒu
		D3DXVECTOR3 rot;	//����
	};

	//�L�[���
	struct KEY_SET
	{
		int nFrame;	//�t���[����
		KEY aKey[MAX_PARTS];
	};

	//���[�V�������
	struct MOTION_SET
	{
		bool bLoop;					//���[�v���邩�ǂ���
		int nNumKey;				//�L�[�̍ő吔
		KEY_SET aKeySet[MAX_KEY];	//�L�[�Z�b�g���
	};

	//�v���C���[�̃X�e�[�^�X
	struct PLAYER_STATUS
	{
		int nAttackTime;	//�U������
		int nComboValue;	//�R���{�̉��Z�l
		float nAttack;		//�U����
		float fSpeed;		//���x
		float fLife;		//�̗�
		float fRemLife;		//�c��̗�(%)
		float fMaxLife;		//�ő�̗�
	};

	//���[�V�����̎��
	enum MOTION_TYPE
	{
		MOTION_TYPE_IDOL = 0,	//�ҋ@
		MOTION_TYPE_MOVE,		//�ړ�
		MOTION_TYPE_ATTACK,		//�U��
		MOTION_TYPE_MAX
	};

	//�v���C���[�̃��[�h
	enum BATTLEMODE
	{
		BATTLEMODE_NONE = 0,	//�ʏ�
		BATTLEMODE_ATTACK,		//�U��
		BATTLEMODE_SPEED,		//���x
		BATTLEMODE_COMBO,		//�A��
		BATTLEMODE_MAX
	};

	CPlayer();				//�R���X�g���N�^
	~CPlayer() override;	//�f�X�g���N�^

	//----------------
	// �����o�֐�
	//----------------
	HRESULT Init(D3DXVECTOR3 pos) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//----------------
	// �Z�b�^�[
	//----------------
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }						//�ʒu�̐ݒ�
	void SetAttack(float nAttack)	  { m_status.nAttack = nAttack; }		//�U���͂̐ݒ�
	void SetSpeed(float fSpeed)		  { m_status.fSpeed = fSpeed; }			//���x�̐ݒ�
	void SetComboValue(int nValue)    { m_status.nComboValue = nValue; }	//�R���{�̉��Z�l�̐ݒ�
	void AddCombo(int nNumber);	//�R���{���̉��Z

	//----------------
	// �Q�b�^�[
	//----------------
	D3DXVECTOR3 GetPosition() override { return m_pos; }			//�ʒu�̎擾
	D3DXVECTOR3 GetRot()			   { return m_rot; }			//�����̎擾
	float GetWidth() override		   { return 0.0f; }				//���̎擾
	float GetHeight() override		   { return 0.0f; }				//�����̎擾
	D3DXMATRIX GetmtxWorld()		   { return m_mtxWorld; }		//���[���h�}�g���b�N�X�̎擾
	CModel* GetModel(int nNum)		   { return m_pModel[nNum]; }	//���f���̎擾
	PLAYER_STATUS GetStatus()		   { return m_status; }			//�X�e�[�^�X�̎擾
	BATTLEMODE GetMode()			   { return m_battleMode; }		//�o�g�����[�h�̎擾
	MOTION_TYPE GetMotion()			   { return m_type; }			//���[�V�����̎�ނ̎擾
	bool GetCollisionPlayer();	//�����蔻��̎擾

	//----------------
	// �ÓI�����o�֐�
	//----------------
	static CPlayer* Create();

private:
	//---------------------
	// �v���C�x�[�g�֐�
	//---------------------
	void SetModel();											//���f���̐ݒ�
	void SetMotion(MOTION_TYPE type, bool bLoop, int nNumKey);	//���[�V�����̐ݒ�
	void ChangeMotion(MOTION_TYPE type);	//���[�V�����̕ύX
	void GetFileMotion();					//�t�@�C�����g�������[�V�����̎擾
	void MoveKeyboard(int nUpKey, int nDownKey, int nLeftKey, int nRightKey);	//�ړ�
	void MoveJoypad();		//�W���C�p�b�h���g�����ړ�
	void Attack();			//�U������
	void ChangeMode();		//���[�h�`�F���W
	void SetRot();			//�p�x�̐ݒ�
	void SetLine();			//���̐ݒu
	void UpdateLine();		//���̍X�V

private:
	//----------------
	// �萔
	//----------------
	static const int nMaxLine = 12;				//���̍ő吔
	static const int nWheelRotValue = 10;		//�^�C���̉�]��
	static const int nSwordNumber = 4;			//�����f���̔ԍ�
	static const float fDefaultAttack;			//��{�̍U����
	static const float fDefaultSpeed;			//��{�̑��x

	//----------------
	// �����o�ϐ�
	//----------------
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�ړI�̌���
	D3DXVECTOR3 m_vtxMax;		//�傫���̍ő�l
	D3DXVECTOR3 m_vtxMin;		//�傫���̍ŏ��l
	D3DXVECTOR3 m_size;			//�傫��
	D3DXVECTOR3 m_worldMin;		//���[���h��̍ő�l
	D3DXVECTOR3	m_worldMax;		//���[���h��̍ŏ��l
	int m_nNumCombo;			//�R���{��
	float fSizeWidth;			//�T�C�Y(��)
	float fSizeDepth;			//�T�C�Y(���s��)
	float m_rotWheel;			//�^�C���̉�]��
	bool m_bDamage;				//�_���[�W��^����
	PLAYER_STATUS m_status;		//�X�e�[�^�X
	BATTLEMODE m_battleMode;	//�o�g�����[�h
	CHP*  m_pHP;				//HP
	CCombo* m_pCombo;			//�R���{

	/* �� ���f����� �� */
	CModel*  m_pModel[MAX_PARTS];		//���f��
	CLine* m_pLine[nMaxLine];			//���C��

	/* �� ���[�V������� �� */
	int m_nCurrentKey;							//���݂̃L�[�ԍ�
	int m_nCntMotion;							//���[�V�����J�E���^�[
	MOTION_SET m_aMotionSet[MOTION_TYPE_MAX];	//���[�V�������
	MOTION_TYPE m_type;							//���݂̃��[�V����

	/* �� �N�H�[�^�j�I�� �� */
	D3DXMATRIX		m_mtxRot;		//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION	m_quat;			//�N�H�[�^�j�I��
	D3DXVECTOR3		m_vecAxis;		//��]��
	float			m_fValueRot;	//��]�p(���W�A��)
};

#endif