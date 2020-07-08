#include "stdafx.h"
#include "CMap.h"
#include "CBlock.h"
#include "MainApp.h"
#include "CGameWorld.h"
#include "item.h"
#include "CObstacle.h"

CMap::CMap(CGameWorld & _rGameWorld, const char * _szDataDirectory)
	:
	m_rGameWorld(_rGameWorld),
	m_fMapX(0.f),					// Map�� ��ü���� X��ǥ // Y ��ǥ�� ����
	m_fMapSpeed(0.f),				// X�� �̵��� ���� Map�� �̵��ӵ�
	m_fFirstBlockX(0.f),			// ù ��° ����� X��ǥ (������ �Ǵ� Block�̴�.)
	m_fFirstBlockY(WINCY >> 1),		// ù ���� ����� Y��ǥ (������ �Ǵ� Block�̴�.)
	m_iBlockWidth(100.f),			// Block�� �ʺ�
	m_iBlockHeight(100.f),			// Block�� ����
	m_pBlockUnderPlayer(nullptr)
{
	FILE* fpIn;
	errno_t err = 0;

	err = fopen_s(&fpIn, _szDataDirectory, "rt");
	if (!err) {
		fscanf_s(fpIn, "%f %f %f %f %d %d",
			&m_fMapX,
			&m_fMapSpeed,
			&m_fFirstBlockX,
			&m_fFirstBlockY,
			&m_iBlockWidth,
			&m_iBlockHeight);

		

		CObj* pBlock = nullptr;
		int iHeightIndex = 0;
		int iBlockNum = 0;
		int bIsObjectsOnBlock = false;
		int iObjectsHeightIndex = 0;
		int eObjectType = OBJ::TYPE_END;
		float fCurrentX = m_fFirstBlockX;
		vector<_object_generation_info> vecObjGenInfos;
		_object_generation_info sObjGenInfos;
		vecObjGenInfos.reserve(20);
		while (!feof(fpIn)) {
			fscanf_s(fpIn, "%d %d %d", &iHeightIndex, &iBlockNum, &bIsObjectsOnBlock);

			vecObjGenInfos.clear();

			// ��� ���� ������Ʈ�� �����Ѵٸ� ������Ʈ ���� ������ ����� �����صд�.
			if (bIsObjectsOnBlock == 1) {
				fscanf_s(fpIn, "%d", &iObjectsHeightIndex);
				iObjectsHeightIndex += 1;
				while (true) {
					fscanf_s(fpIn, "%d", &eObjectType);
					if (eObjectType == -2) break;
					if (eObjectType == -1) {
						// -1�� �ش� ĭ�� �ǳʶٴ� ���� �ǹ��Ѵ�.
						iObjectsHeightIndex++;
						continue;
					}
					// �������� Ÿ�԰� ���̸� �����Ѵ�.
					// X���� �Ʒ� ��� ���� �� ����� X�� ���ӵȴ�.
					sObjGenInfos.eType = static_cast<OBJ::E_TYPE>(eObjectType);
					sObjGenInfos.fHeight = iObjectsHeightIndex * m_iBlockHeight;
					vecObjGenInfos.emplace_back(sObjGenInfos);
					iObjectsHeightIndex++;
				}
			}

			if (iHeightIndex == -1) {
				// iHeightIndex�� == -1�̶�� ��ŵ�Ѵ�.
				for (int i = 0; i < iBlockNum; i++) fCurrentX += m_iBlockWidth;
				continue;
			}
			for (int i = 0; i < iBlockNum; i++) {
				// ����� �����Ѵ�.
				pBlock = new CBlock(_rGameWorld, *this, fCurrentX, m_fFirstBlockY - m_iBlockHeight * iHeightIndex, m_iBlockWidth, m_iBlockHeight);
				m_vecBlocks.emplace_back(pBlock);

				// �������� �����Ѵ�.
				if (!vecObjGenInfos.empty()) {
					for (auto& rObjGenInfo : vecObjGenInfos) {
						switch (rObjGenInfo.eType) {
						case OBJ::TYPE_OBSTACLE:
							m_vecObstacles.emplace_back(new CObstacle(m_rGameWorld, *this, fCurrentX, pBlock->GetY() - rObjGenInfo.fHeight, GetBlockWidth() >> 1, GetBlockHeight()));
							break;
						case OBJ::TYPE_COIN:
							m_vecItems.emplace_back(CItem::CreateItem<Item::CCoin>(m_rGameWorld, *this, fCurrentX, pBlock->GetY() - rObjGenInfo.fHeight));
							break;
						case OBJ::TYPE_HEALTH:
							m_vecItems.emplace_back(CItem::CreateItem<Item::CLife>(m_rGameWorld, *this, fCurrentX, pBlock->GetY() - rObjGenInfo.fHeight));
							break;
						}
					}
				}
				
				fCurrentX += m_iBlockWidth;
			}
		}
		m_fKillX = m_fFirstBlockX - m_iBlockWidth;
		m_fRespawnX = m_vecBlocks.back()->GetX();
	}
	else abort();
}

CMap::~CMap()
{
}

void CMap::Update(void)
{
	m_fMapX += m_fMapSpeed;
	if (m_fMapX >= GetMapDistance()) m_fMapX -= GetMapDistance();
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->Update(); } }
	for (auto& pObstacle : m_vecObstacles) { DO_IF_IS_VALID_OBJ(pObstacle) { pObstacle->Update(); } }
	for (auto& pItem : m_vecItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Update(); } }
}

void CMap::LateUpdate(void)
{
	m_pBlockUnderPlayer = nullptr;	// ���� �÷��̾� �Ʒ��� �ִ� ����� �÷��̾� �Ʒ��� ���� ���� �� �����Ƿ� �簻���� ���� nullptr�� ����.
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->LateUpdate(); } }
	for (auto& pObstacle : m_vecObstacles) { DO_IF_IS_VALID_OBJ(pObstacle) { pObstacle->LateUpdate(); } }
	// �������� �÷��̾ �Ծ ������� �ٽ� ��ȸ�Ҷ� �����ȴ�. => ���� �ݺ�
	for (auto& pItem : m_vecItems) {  pItem->LateUpdate(); }
}

void CMap::Render(const HDC& _hdc)
{
	for (auto& pBlock : m_vecBlocks) { DO_IF_IS_VALID_OBJ(pBlock) { pBlock->Render(_hdc); } }
	for (auto& pObstacle : m_vecObstacles) { DO_IF_IS_VALID_OBJ(pObstacle) { pObstacle->Render(_hdc); } }
	for (auto& pItem : m_vecItems) { DO_IF_IS_VALID_OBJ(pItem) { pItem->Render(_hdc); } }
}

void CMap::Release(void)
{
}

float CMap::GetConvX(float _fX) const
{
	if (_fX - m_fMapX < GetKillX()) {
		return GetRespawnX() + (_fX - m_fMapX - GetKillX());
	}
	return _fX - m_fMapX;
}

float CMap::GetConvLeft(float _fX) const
{
	return GetConvX(_fX) - (m_iBlockWidth >> 1);
}

float CMap::GetConvRight(float _fX) const
{
	return GetConvX(_fX) + (m_iBlockWidth >> 1);
}

CBlock * CMap::GetBlockUnderPlayer(void) const
{
	return m_pBlockUnderPlayer;
}

void CMap::SetBlockUnderPlayer(CBlock * _pBlock)
{
	if (m_pBlockUnderPlayer) {
		// ����� �ִٸ� ���� �ֻ�� ������� �����Ѵ�.
		if (_pBlock->GetY() < m_pBlockUnderPlayer->GetY())
			m_pBlockUnderPlayer = _pBlock;
	}
	m_pBlockUnderPlayer = _pBlock;
}
