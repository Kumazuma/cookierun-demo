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
		//�߷¿� ���� �ӵ��� ����Ǵ� ���� �����Ѵ�. ���� ���� ������� true�� ��ȯ�Ѵ�.
		bool GravityMove(CObj* const, float timedalta);
	};
	class SingleJump : public Jump
	{
	public:
		SingleJump();
		void OnLoaded(CObj* const) override;
		virtual IPlayerState* Update(CObj* const, float timedelta)override;
	private:
		bool m_bReachedTop;
		float m_fDelay;
		float m_fPrevSpeed;
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
