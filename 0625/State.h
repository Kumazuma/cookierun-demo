#pragma once
class CObj;
struct IPlayerState
{
	virtual void OnLoaded(CObj* const) = 0;
	virtual IPlayerState* Update(CObj* const, float timedelta) = 0;
	virtual ~IPlayerState() = default;
	bool IsNull() const { return this == nullptr; }
};
namespace State
{
	class Default : public IPlayerState
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta) override;
	private:
	};
	//���� �߻� Ŭ����
	class Jump : public IPlayerState
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta)override;
		
	protected:
		//�ν��Ͻ�ȭ�Ǹ� �ȵȴ�.
		Jump() = default;
		//�߷¿� ���� �ӵ��� ����Ǵ� ���� �����Ѵ�. 
		void GravityMove(CObj* const, float timedalta);
		//���� ���� ������� true�� ��ȯ�Ѵ�.
		bool IsReturn(CObj* const);
	};
	class SingleJump : public Jump
	{
	public:
		SingleJump();
		virtual IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		bool m_bReachedTop;
	};
	class DoubleJump : public Jump
	{
	public:
		IPlayerState* Update(CObj* const, float timedelta) override;
	};
	class Slide : public IPlayerState
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		float m_fPrevHeight;
		float m_fPrevPosY;
	};
}
