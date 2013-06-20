#ifndef _RD_FPS_H
#define _RD_FPS_H

class CFps
{
public:
	CFps() : m_dwCurFrameCnt(0), m_fTime(0.0f), m_fps(0.0f)  { }

	DWORD GetFrameCount() const { return m_dwCurFrameCnt; }
	float GetFps() const { return m_fps; }

	void Run(float fElapsedTime)
	{
		++m_dwCurFrameCnt;
		m_fTime += fElapsedTime;
		if (m_fTime >= 1.0f)
		{
			m_fps = m_dwCurFrameCnt / m_fTime;
			m_dwCurFrameCnt = 0;
			m_fTime = 0.0f;
		}
	}

private:
	DWORD m_dwCurFrameCnt;
	float m_fTime;
	float m_fps;
};


#endif