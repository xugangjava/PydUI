#pragma once
#include "stdafx.h"

class XLock
{
public:
	inline XLock()
	{
		m_thread_state = PyGILState_Ensure();
	}

	inline ~XLock()
	{
		PyGILState_Release(m_thread_state);
	}

private:
	PyGILState_STATE m_thread_state;
};

