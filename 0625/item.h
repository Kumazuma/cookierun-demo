#pragma once
#include"CObj.h"

class CItem : public CObj
{
public:
	enum class Type{NONE, COIN, LIFE, END};
	template<typename _T> 
	static CObj* CreateItem(CGameWorld& world, float fX, float fY);
	int Update(void) override;
	CItem::Type GetType()const { return m_itemType; }
protected:
	CItem(CGameWorld& world, float fX, float fY, CItem::Type type = Type::NONE);
	Type m_itemType;
	float m_realScale;/*[0, 1]*/
	char m_sign;/* -1, 1*/
};
namespace Item
{
	class CCoin : public CItem
	{
		friend class CItem;
	public:
		~CCoin();
		
		void Render(const HDC& hDC) override;
	private:
		CCoin(CGameWorld& world, float fX, float fY);
	};
	class CLife : public CItem
	{
		friend class CItem;
	public:
		~CLife();
		void Render(const HDC& hDC) override;
	private:
		CLife(CGameWorld& world, float fX, float fY);
	};
}

