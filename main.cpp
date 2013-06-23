#include "rd_core/rd_app.h"
#include "rd_core/rd_astar.h"

const int width = 1206;
const int height = 628;

#define GRID_X_NUM	120
#define GRID_Y_NUM	60

#define GRID_WIDTH	10
#define GRID_HEIGHT	10

#define BLOCK_NUM	1500

class CMap : public ITileJudge
{
public:
	std::vector<Vec2F>	m_vecBackLine;	// 背景线
	std::vector<RectF>	m_vecRect;		// 红色
	std::vector<RectF>	m_vecBlock;		// 黑色

	struct stGrid
	{
		Vec2I pt;
		bool bBlock;
	};
	CAStar	m_Astar;
	std::vector<Vec2F>	m_findPath;

	// 格子
	std::vector<stGrid>	m_vecGrid;
	// 当前位置
	RectF	m_rect;
	Vec2I	m_pos;

	void	Init();
	void	Draw(float fElapsedTime);
	void	Input(Vec2I position);

	stGrid&	GetGrid(int row, int col) { return m_vecGrid[col * GRID_X_NUM + row]; }
	Vec2F	Grid2CenterPt(Vec2I pt)
	{
		Vec2F c;
		c.y = pt.y * GRID_WIDTH + GRID_WIDTH / 2;
		c.x = pt.x * GRID_HEIGHT + GRID_HEIGHT / 2;
		return c;
	}
	bool	IsFree(Vec2I pt)
	{
		if (pt.x >= GRID_X_NUM || pt.y >= GRID_Y_NUM || pt.x < 0 || pt.y < 0)
			return false;
		stGrid& grid = GetGrid(pt.x, pt.y);
		return !grid.bBlock;
	}

};

void CMap::Init()
{
	m_vecBackLine.reserve((GRID_X_NUM + 1) * 2 + (GRID_Y_NUM + 1) * 2);
	//back line  x
	for (int i = 0; i < GRID_X_NUM + 1; ++i)
	{
		Vec2F pt;
		pt.x = i * GRID_WIDTH;
		pt.y = 0;
		m_vecBackLine.push_back(pt);

		pt.x = i * GRID_WIDTH;
		pt.y = GRID_Y_NUM * GRID_HEIGHT;
		m_vecBackLine.push_back(pt);
	}
	//back line  y
	for (int i = 0; i < GRID_Y_NUM + 1; ++i)
	{
		Vec2F pt;
		pt.x = 0;
		pt.y = i * GRID_HEIGHT;
		m_vecBackLine.push_back(pt);

		pt.x = GRID_X_NUM * GRID_WIDTH;
		pt.y = i * GRID_HEIGHT ;
		m_vecBackLine.push_back(pt);
	}

	m_vecGrid.reserve(GRID_X_NUM * GRID_Y_NUM);
	m_vecRect.reserve(GRID_X_NUM * GRID_Y_NUM);
	// grid
	for (int i = 0; i < GRID_X_NUM; ++i)
	{
		for (int j = 0; j < GRID_Y_NUM; ++j)
		{
			stGrid grid;
			grid.pt.Set(i, j);
			grid.bBlock = false;
			m_vecGrid.push_back(grid);

			RectF rect;
			rect.SetRect(i * GRID_WIDTH, j * GRID_HEIGHT, (i + 1) * GRID_WIDTH, (j + 1) * GRID_HEIGHT);
			m_vecRect.push_back(rect);
		}
	}

	// block grid
	m_vecBlock.reserve(BLOCK_NUM);

	// BLOCK_NUM 个随机数，范围 num = [1 - GRID_X_NUM * GRID_Y_NUM)
	// 输出到坐标：row = num / GRID_X_NUM, col = num % GRID_X_NUM
	int grid_num[GRID_X_NUM * GRID_Y_NUM];
	int size = count_of(grid_num);
	for (int i = 0; i < size; ++i)
	{
		grid_num[i] = i;
	}

	// 打乱前BLOCK_NUM数
	for (int i = 0; i < BLOCK_NUM; ++i)
	{
		int index = random<int>(i, size - 1);
		rd_swap(grid_num[i], grid_num[index]);

		int num = grid_num[i];
		int row = num / GRID_X_NUM;
		int col = num % GRID_X_NUM;
		m_vecGrid[num].bBlock = true;

		RectF rect;
		rect.SetRect(col * GRID_WIDTH, row * GRID_HEIGHT, (col + 1) * GRID_WIDTH, (row + 1) * GRID_HEIGHT);
		m_vecBlock.push_back(rect);
	}

	//随机生成人物的位置，在红色格子上
	while (true)
	{
		int index = rand() % m_vecGrid.size();
		if (! m_vecGrid[index].bBlock)
		{
			m_pos = m_vecGrid[index].pt;
			m_rect.SetRect(m_pos.x * GRID_WIDTH, m_pos.y * GRID_HEIGHT, (m_pos.x + 1) * GRID_WIDTH, (m_pos.y + 1) * GRID_HEIGHT);
			break;
		}
	}

	m_Astar.SetTileJudge(this);
}

void CMap::Draw(float fElapsedTime)
{
	IRender* render = GetRender();
	if (! render)
		return;
//	clear back to red, then do not need draw red grid
//	render->FillRectList(&m_vecRect[0], m_vecRect.size(), 0xffff0000);
	render->FillRectList(&m_vecBlock[0], m_vecBlock.size(), 0xff000000);
	render->FillRect(m_rect, 0xffffff00);
	render->DrawLineList(&m_vecBackLine[0], m_vecBackLine.size(), 0xff000000);

	if (! m_findPath.empty())
	{
		render->DrawLineStrip(&m_findPath[0], m_findPath.size(), 0xffffffff);
	}
}

void CMap::Input(Vec2I position)
{
	Vec2I end_pos(position.x / GRID_WIDTH, position.y / GRID_HEIGHT);
	DEBUG_TRACE("start:%d,%d\tend:%d,%d\n", m_pos.x, m_pos.y, end_pos.x, end_pos.y);

	if (! IsFree(end_pos))
		return;

	QWORD start_time = GetTimer()->GetTime();
	if (m_Astar.FindPath(m_pos, end_pos))
	{
		QWORD end_time = GetTimer()->GetTime();
		DEBUG_TRACE("find path time:%f\n", GetTimer()->GetTimeMillisec(end_time - start_time));

		m_findPath.clear();
		std::vector<Vec2I>& vec = m_Astar.GetPath();
		m_findPath.reserve(vec.size());
		for (int i = 0; i < vec.size(); ++i)
		{
			Vec2I pt = vec[i];
			m_findPath.push_back(Grid2CenterPt(pt));
		}
		m_pos = end_pos;
		m_rect.SetRect(m_pos.x * GRID_WIDTH, m_pos.y * GRID_HEIGHT, (m_pos.x + 1) * GRID_WIDTH, (m_pos.y + 1) * GRID_HEIGHT);
	}
}
//////////////////////////////////////////////////////////////////////////

class CGameApp : public CApp
{
public:
	void Run(float fElapsedTime);
	void Draw(float fElapsedTime);

	void Init();
	bool MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CMap	m_Map;
};

void CGameApp::Init()
{
	m_Map.Init();
}

void CGameApp::Draw(float fElapsedTime)
{
	m_Map.Draw(fElapsedTime);
}

void CGameApp::Run(float fElapsedTime)
{
	CApp::Run(fElapsedTime);
}

bool CGameApp::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CApp::MsgProc(hwnd, uMsg, wParam, lParam);

	if (uMsg == WM_LBUTTONUP)
	{
		Vec2I position = GetAppCursor()->GetPosition();
		m_Map.Input(position);
	}

	return true;
}

int main(int argc, char* argv[])
{
	CGameApp theApp;
	theApp.Create(width, height, "A star");
	theApp.Init();
	theApp.Loop();
	return 0;
}
