#ifndef _RD_ASTAR_H
#define _RD_ASTAR_H

#include "rd_core.h"

class ITileJudge
{
public:
	virtual bool IsFree(Vec2I pt) = 0;
};

class CAStar
{
public:
	CAStar() : m_tileJudge(NULL) {}
	
	enum { ChildNum = 8 };

	struct stNode
	{
		Vec2I pt;
		float f;		// f = g + h
		float g;
		float h;
		stNode* parent;
		stNode* child[ChildNum];
	};

	void SetTileJudge(ITileJudge* judge)  {m_tileJudge = judge; }
	
	bool FindPath(Vec2I ptBegin, Vec2I ptEnd);
	
	void GenerateSucc(stNode* best_node, Vec2I pt, Vec2I ptEnd);

	stNode* FindNode(std::vector<stNode*>& vec, Vec2I pt);

	void PropagateDown(stNode* node);

	std::vector<Vec2I>& GetPath() { return m_path; }
private:
	ITileJudge*	m_tileJudge;
	std::vector<stNode*>	m_vecOpen;
	std::vector<stNode*>	m_vecClose;
	std::vector<Vec2I>		m_path;
};


#endif