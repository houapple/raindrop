#include "rd_core/rd_app.h"

const int width = 800;
const int height = 600;

class CGameApp : public CApp
{
public:
	void Run(float fElapsedTime) { }
	void Draw(float fElapsedTime);
};

void CGameApp::Draw(float fElapsedTime)
{
	IRender* render = GetRender();
	if (! render)
		return;

	render->DrawLine(Vec2F(100, 0), Vec2F(0, 100), 0xffffffff);

	render->DrawLine(Vec2F(10, 10), Vec2F(100, 100), 0xffffffff);

	for (int i = 0; i < 1; ++i)
	{
		int l = rand() % width;
		int t = rand() % height;
		int r = rand() % width;
		int b = rand() % height;

		render->DrawRect(RectF(min(l,r), min(t, b), max(r, l) + 1, max(b, t) + 1), 0xffffffff);
	}

	render->FillRect(RectF(10, 10, 50, 50), 0xffff0000);

	std::vector<Vec2F> vec;
	vec.push_back(Vec2F(30, 32));
	vec.push_back(Vec2F(80, 67));
	vec.push_back(Vec2F(70, 93));
	vec.push_back(Vec2F(120, 170));
	vec.push_back(Vec2F(45, 220));
	vec.push_back(Vec2F(20, 376));

	render->DrawLineStrip(&vec[0], vec.size(), 0xff00ff00);
}

int main(int argc, char* argv[])
{
	CGameApp theApp;
	theApp.Create(width, height, "Rain drop");
	theApp.Loop();
	return 0;
}
