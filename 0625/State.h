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
	class Gravity : public IPlayerState
	{
	public:
		IPlayerState* Update(CObj* const, float timedelta) override;
	protected:
		Gravity();
		//�߷¿� ���� �ӵ��� ����Ǵ� ���� �����Ѵ�. 
		void GravityMove(CObj* const, float timedalta);
		//���� ���� ������� true�� ��ȯ�Ѵ�.
		bool IsReturn(CObj* const);
		float m_fSpeed;
	};
	class Default : public Gravity
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta) override;
	private:
	};
	//���� �߻� Ŭ����
	class Jump : public Gravity
	{
	public:
		void OnLoaded(CObj* const) override;
	protected:
		//�ν��Ͻ�ȭ�Ǹ� �ȵȴ�.
		Jump() = default;

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
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta) override;
	};
	class Slide : public Gravity
	{
	public:
		void OnLoaded(CObj* const) override;
		IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		float m_fPrevHeight;
		float m_fPrevPosY;
		float m_fprevWidth;
	};
}
