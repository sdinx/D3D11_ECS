//----------------------------------------------------------------------------------
// D3D11_ECS
// create date: 2018.05.12 ( Prototype_version )
//----------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
// Component
//----------------------------------------------------------------------------------
// namespace D3D11Utility
// �����K�� ��{�I�ɂ͎��R ������Component��t����ꍇ�͕K�����ꂷ�� ( ��: WeaponComponent ).


/* �R���|�[�l���g�ɂ��� */
// Entity.h ��D3D11Utility::Entity �N���X�͈�ɑ΂��ďd�����Ȃ������̃R���|�[�l���g����������.
// �R���|�[�l���g�̎��̂� ComponentManager.h �� D3D11Utility::Systems::ComponentManager �N���X�������Ă���.
// �R���|�[�l���g���g�̐e�̃G���e�B�e�BID���瑼�̃R���|�[�l���g���Q�Ƃ��邱�Ƃ��ł���.
// ������, �R���|�[�l���g�̂��O�����p�ɂɍs���邽��, �K��null�`�F�b�N�����ăR���|�[�l���g�����݂��邩�̊m�F������K�v������.


/* �R���|�[�l���g�̈������Ȃ� */
// Entity::AddComponent�֐��ŃG���e�B�e�B�ɑ΂��ăR���|�[�l���g��ǉ����� ( ��: entity->AddComponent<Camera>() ).
// AddComponent�֐��̈����͒ǉ�����R���|�[�l���g�̃R���X�g���N�^����.
// Entity::GetComponent�֐��ŃG���e�B�e�B�̃R���|�[�l���g���擾�ł��� ( ��: entity->GetComponent<Transform>() ).
// GetComponent�֐��͎��s����� nullptr ���Ԃ�׃R���|�[�l���g�̑��݂̊m�F���ł���.


/* �R���|�[�l���g�̎������@�ɂ��� */
// Component.h ��include����D3D11Utility::Component �N���X���p��.
// �K��static ComponentId�^�� STATIC_COMPONENT_ID �ϐ���錾����.
// Component.h �Ő錾���Ă��� �������z�֐��̎�����static�֐��Ƃ���STATIC_COMPONENT_ID �̃Q�b�^�[�ƃZ�b�^�[���K�v.
// Update�֐��ɖ����[�v�s���������L�q����.


/* �������z�֐� */
// virtual  void  HandleMessage( const  GameUtility::Message&  msg ) = 0;
// ����̃��b�Z�[�W�𐶐����ĕ��򏈗���������.
// virtual  void  HandleMessage( const  GameUtility::Message&  msg, Value  var ) = 0;
// ��L�Ɉ����Ƃ��ē���̒l���K�v�ȏꍇ ( void* �^����L���X�g����K�v������ ).
// virtual  void  Update() = 0;
// �X�V�����Ƃ��ăV�X�e���������[�v�Ăяo��.


/* �Q�l�R���|�[�l���g */
// Renderable.h
// Camera.h

////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
// ISystem �� SystemManager
//----------------------------------------------------------------------------------
// namespace D3D11Utility::Systems
// �����K�� ������System��t���� ( ��: PlayerSystem )


/* ���������R���|�[�l���g���Ǘ�����N���X�ɂ��� */
// �����܂ł��V�X�e�����g���Ǘ�����R���|�[�l���g�����������悤�ɂ���.
// �e���v���[�g�֐���ComponentManager::GetComponents�֐����g���ăR���|�[�l���g���擾����.
// �V�X�e���N���X�̃C���X�^���X�̍쐬�͕K�� SystemManager::AddSystem�֐� ����s��.
// ����V�X�e�����d���ǉ����Ă͋֎~.


/* �R���|�[�l���g�Ǘ��N���X�̎����ɂ��� */
// ISystem.h ��include����D3D11Utility::Systems::ISystem �N���X���p������.
// Update�֐��ɖ����[�v�s�������ƊǗ����Ă���R���|�[�l���g��Update�֐��Ăяo��.


/* �Q�l�V�X�e�� */
// IDirect3DRenderer.h
// IDirect3DRenderer::Rendering�֐� ���Q�l�ɃR���|�[�l���g�}�l�[�W���[����R���|�[�l���g���擾����.