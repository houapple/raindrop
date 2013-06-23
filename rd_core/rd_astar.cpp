#include "rd_astar.h"

/*
Best_First_Search()
{
	Open = [��ʼ�ڵ�];
	Closed = [];
	while (Open��ǿ�)
	{
		��Open��ȡ��һ���ڵ�X������OPEN����ɾ����
		if (X��Ŀ��ڵ�)
		{
			���·��PATH��
			����·��PATH��
		}
		for (ÿһ��X���ӽڵ�Y)
		{
			if (Y����OPEN���CLOSE����)
			{
				��Y�Ĺ���ֵ��
				����Y����OPEN���У�
			}
			//��û������
			else if (Y��OPEN����)
			{
				if (Y�Ĺ���ֵС��OPEN��Ĺ���ֵ)
					����OPEN���еĹ���ֵ��
			}
			else //Y��CLOSE����
			{
				if (Y�Ĺ���ֵС��CLOSE��Ĺ���ֵ)
				{
					����CLOSE���еĹ���ֵ��
					��CLOSE�����Ƴ��ڵ㣬������OPEN���У�
				}
			}
			��X�ڵ����CLOSE���У�
			���չ���ֵ��OPEN���еĽڵ�����
		}//end for
	}//end while
}//end func
*/

static bool SortNode(CAStar::stNode* node1, CAStar::stNode* node2)
{
	if (! node1 || ! node2)
		return false;

	return node1->f < node2->f;
}

int heuristic(Vec2I start, Vec2I end)
{
/*	int dx = abs(start.x - end.x);
	int dy = abs(start.y - end.y);
	int D = 5;
	if (dx > dy)
		return D * dx;
	else
		return D * dy;
		*/
	return square_distance<int>(start, end);
}

bool CAStar::FindPath(Vec2I ptBegin, Vec2I ptEnd)
{
	if (! m_tileJudge || ptBegin == ptEnd)
		return false;
	
	//������ʼ�ڵ㣬������Open����
	stNode* node = (stNode*)rd_malloc(sizeof(stNode));
	node->pt = ptBegin;
	node->g = 0.0f;
	node->h = heuristic(ptBegin, ptEnd);
	node->f = node->g + node->h;
	node->parent = NULL;
	memset(node->child, 0, sizeof(node->child));
	m_vecOpen.push_back(node);

	m_path.clear();
	stNode* best_node = NULL;
	for (;;)
	{
		CFuncTime func_time("for", 1);
		if (m_vecOpen.empty())
			break;
		std::sort(m_vecOpen.begin(), m_vecOpen.end(), SortNode);
		//��Open����ȡ��һ������ֵ��õĽڵ�
		best_node = m_vecOpen.at(0);
		
		DebugTrace("get node: %d %d %f\n", best_node->pt.x, best_node->pt.y, best_node->f);

		//�ҵ��ˣ�����
		if (best_node && best_node->pt == ptEnd)
			break;
		//���������ӽڵ�		
		// �I
		Vec2I pt;
		pt.Set(best_node->pt.x - 1, best_node->pt.y - 1);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);
		// ��
		pt.Set(best_node->pt.x, best_node->pt.y - 1);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);
		// �J
		pt.Set(best_node->pt.x + 1, best_node->pt.y -1);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);
		// ��
		pt.Set(best_node->pt.x - 1, best_node->pt.y);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);
		// ��
		pt.Set(best_node->pt.x + 1, best_node->pt.y);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);
		// �L
		pt.Set(best_node->pt.x - 1, best_node->pt.y + 1);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);
		// ��
		pt.Set(best_node->pt.x, best_node->pt.y + 1);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);
		// �K
		pt.Set(best_node->pt.x + 1, best_node->pt.y + 1);
		if (m_tileJudge->IsFree(pt))
			GenerateSucc(best_node, pt, ptEnd);

		
		m_vecOpen.erase(m_vecOpen.begin());
		m_vecClose.push_back(best_node);
	}

	while (best_node)
	{
		m_path.push_back(best_node->pt);
		best_node = best_node->parent;
	}
	std::reverse(m_path.begin(), m_path.end());
	
	free_list<stNode>(m_vecOpen);
	free_list<stNode>(m_vecClose);

	return true;
}

void CAStar::GenerateSucc(stNode* best_node, Vec2I pt, Vec2I ptEnd)
{
	CFuncTime func_time("GenerateSucc", 1);
	stNode* old_node = NULL;
	int g = best_node->g + 1;
	int c = 0;

	// �ӽڵ���open����
	if ((old_node = FindNode(m_vecOpen, pt)) != NULL)
	{
		for (c = 0; c < ChildNum; c++)
		{
			if (best_node->child[c] == NULL)
				break;
		}
		best_node->child[c] = old_node;
		
		//�Ƚ�Open���еĹ���ֵ�͵�ǰ�Ĺ���ֵ��ֻҪ�Ƚ�gֵ�Ϳ����ˣ�
		if (g < old_node->g)
		{
			old_node->parent = best_node;
			old_node->g = g;
			old_node->f = g + old_node->h;
		}
	}
	// �ӽڵ���close����
	else if ((old_node = FindNode(m_vecClose, pt)) != NULL)
	{
		for (c = 0; c < ChildNum; c++)
		{
			if (best_node->child[c] == NULL)
				break;
		}
		best_node->child[c] = old_node;

		if (g < old_node->g)
		{
			old_node->parent = best_node;
			old_node->g = g;
			old_node->f = g + old_node->h;
			
			//�����θ���Old�������ӽڵ�Ĺ���ֵ
			PropagateDown(old_node);
		}
	}
	// �ӽڵ㲻��Open����Ҳ����Close����
	else 
	{
		//�����½ڵ�
		stNode* new_node = (stNode*)rd_malloc(sizeof(stNode));
		new_node->pt = pt;
		new_node->g = g;
		new_node->h = heuristic(pt, ptEnd);
		new_node->f = new_node->g + new_node->h;
		new_node->parent = best_node;
		memset(new_node->child, 0, sizeof(new_node->child));
		//����Open����
		m_vecOpen.push_back(new_node);

		for (c = 0; c < ChildNum; c++)
		{
			if (best_node->child[c] == NULL)
				break;
		}
		best_node->child[c] = new_node;

		DebugTrace("new node: %d %d %f\n", pt.x, pt.y, new_node->f);
	}
}

CAStar::stNode* CAStar::FindNode(std::vector<stNode*>& vec, Vec2I pt)
{
	CFuncTime func_time("FindNode", 1);

	for (int i = 0; i < vec.size(); ++i)
	{
		if (vec[i]->pt == pt)
			return vec[i];
	}
	return NULL;
}

void CAStar::PropagateDown(stNode* node)
{
	CFuncTime func_time("PropagateDown", 1);
	std::stack<stNode*>  stack_node;
	stack_node.push(node);

	while (! stack_node.empty())
	{
		stNode* tp = stack_node.top();
		stack_node.pop();

		for (int i = 0; i < ChildNum; ++i)
		{
			stNode* c = tp->child[i];
			if (c)
			{
				c->g = tp->g + 1;
				c->f = c->g + c->h;
				stack_node.push(c);
			}
		}
	}
}