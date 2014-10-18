#pragma once

class CriticalSection
{
private:
	CRITICAL_SECTION m_Section;

public:
	CriticalSection()
	{
		InitializeCriticalSection(&m_Section);
	}

	~CriticalSection()
	{
		DeleteCriticalSection(&m_Section);
	}

	void Enter()
	{
		EnterCriticalSection(&m_Section);
	}

	void Leave()
	{
		LeaveCriticalSection(&m_Section);
	}

	class Lock
	{
	private:
		CriticalSection& m_CriticalSection;

	public:
		Lock(CriticalSection& criticalSection) :
			m_CriticalSection(criticalSection)
		{
			m_CriticalSection.Enter();
		}

		~Lock()
		{
			m_CriticalSection.Leave();
		}
	};
};