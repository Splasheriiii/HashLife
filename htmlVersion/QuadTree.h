#pragma once
#include <math.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define NW 0
#define NE 1
#define SW 2
#define SE 3



class QuadTree
{
public:	
	bool 
#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_KEEPALIVE
#endif
does_have_val()
	{
		return has_value;
	}
	bool** 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 to_array(bool** arr = nullptr, int start_x = 0, int start_y = 0)
	{
		if (arr == nullptr)
		{
			int size = pow(2, level);
			arr = new bool*[size];
			for (size_t i = 0; i < size; i++)
			{
				arr[i] = new bool[size];
			}
		}
		if (level > 1)
		{
			get_quad(NW)->to_array(arr, start_x, start_y);
			get_quad(NE)->to_array(arr, start_x + pow(2, level - 1), start_y);
			get_quad(SW)->to_array(arr, start_x, start_y + pow(2, level - 1));
			get_quad(SE)->to_array(arr, start_x + pow(2, level - 1), start_y + pow(2, level - 1));
		}
		else
		{
			arr[start_y][start_x] = get_quad(NW)->has_value;
			arr[start_y][start_x + 1] = get_quad(NE)->has_value;
			arr[start_y + 1][start_x] = get_quad(SW)->has_value;
			arr[start_y + 1][start_x + 1] = get_quad(SE)->has_value;
		}
		return arr;
	}
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 get_next_generation()
	{
		bool need_expanding = level < 4;
		if (!need_expanding)
		{
			for (size_t i = NW; i <= SE; i++)
			{
				for (size_t j = NW; j <= SE; j++)
				{
					need_expanding |= get_quad(i)->get_quad(j)->has_value && (i != (SE - j));
				}
				for (size_t j = NW; j <= SE; j++)
				{
					need_expanding |= get_quad(i)->get_quad(SE - j)->get_quad(j)->has_value && (i != (SE - j));
				}
			}
		}
		if (need_expanding)
		{
			return expand_tree()->get_next_generation();
		}
		else
		{
			return next_generation();
		}
	}
	 int 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 get_level()
	{
		return level;
	}
protected:
	bool has_value;
	int level;
	QuadTree(int lvl)	
	{
		this->level = lvl;
		this->has_value = false;
	}
	virtual QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 get_quad(int quad) = 0;
	virtual void 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
initialize_by_quads(QuadTree** new_main_quads) = 0;
	virtual QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
Create(int lvl, bool** values, int start_x = 0, int start_y = 0) = 0;
	virtual QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
Create(QuadTree** new_main_quads, int lvl) = 0;
	virtual QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
next_generation()
	{
		if (level > 2)
		{
			QuadTree* top_center_res = merge_two_quads(get_quad(NW), get_quad(NE), false)->next_generation();
			QuadTree* left_center_res = merge_two_quads(get_quad(NW), get_quad(SW), true)->next_generation();
			QuadTree* right_center_res = merge_two_quads(get_quad(NE), get_quad(SE), true)->next_generation();
			QuadTree* bot_center_res = merge_two_quads(get_quad(SW), get_quad(SE), false)->next_generation();
			QuadTree* center_center_res = get_center_quad()->next_generation();

			return Create(new QuadTree * [4]{
				Create(new QuadTree * [4] {
						get_quad(NW)->next_generation()->get_quad(SE),
						top_center_res->get_quad(SW),
						left_center_res->get_quad(NE),
						center_center_res->get_quad(NW)
					},level - 2),
				Create(new QuadTree * [4]{
						top_center_res->get_quad(SE),
						get_quad(NE)->next_generation()->get_quad(SW),
						center_center_res->get_quad(NE),
						right_center_res->get_quad(NW)
					},level - 2),
				Create(new QuadTree * [4]{
						left_center_res->get_quad(SE),
						center_center_res->get_quad(SW),
						get_quad(SW)->next_generation()->get_quad(NE),
						bot_center_res->get_quad(NW)
					},level - 2),
				Create(new QuadTree * [4]{
						center_center_res->get_quad(SE),
						right_center_res->get_quad(SW),
						bot_center_res->get_quad(NE),
						get_quad(SE)->next_generation()->get_quad(NW),
					},level - 2),
				}, level - 1);
		}
		else
		{
			return apply_life_rules();
		}
	}
private:
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
expand_tree()
	{
		return Create(new QuadTree*[4]{
			Create(new QuadTree * [4]{
				Create(level - 1, nullptr),
				Create(level - 1, nullptr),
				Create(level - 1, nullptr),
				get_quad(NW)
			},level),
			Create(new QuadTree * [4]{
				Create(level - 1, nullptr),
				Create(level - 1, nullptr),
				get_quad(NE),
				Create(level - 1, nullptr)
			},level),
			Create(new QuadTree * [4]{
				Create(level - 1, nullptr),
				get_quad(SW),
				Create(level - 1, nullptr),
				Create(level - 1, nullptr)
			},level),
			Create(new QuadTree * [4]{
				get_quad(SE),
				Create(level - 1, nullptr),
				Create(level - 1, nullptr),
				Create(level - 1, nullptr)
			},level),
		}, level + 1);
	}
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
apply_life_rules()
	{
		bool** res = new bool* [3]
		{
			new bool[3]{false,false,false},
			new bool[3]{false,false,false},
			new bool[3]{false,false,false}
		};
		auto arr = to_array();
		for (size_t i = 1; i <= 2; i++)
		{
			for (size_t j = 1; j <= 2; j++)
			{
				int n_count = 0;
				for (size_t y = i - 1; y <= i + 1; y++)
				{
					for (size_t x = j - 1; x <= j + 1; x++)
					{
						n_count += arr[y][x] ? 1 : 0;
					}
				}
				n_count -= arr[i][j] ? 1 : 0;
				res[i][j] = n_count == 3 || (arr[i][j] && n_count == 2);
			}
		}
		for (size_t i = 1; i <= 2; i++)
		{
			for (size_t j = 1; j <= 2; j++)
			{
				arr[i][j] = res[i][j];
			}
		}
		return Create(1, arr, 1, 1);
	}
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
merge_two_quads(QuadTree* first, QuadTree* second, bool is_vert)
	{
		if (is_vert)
		{
			return Create(new QuadTree*[4]{ first->get_quad(SW), first->get_quad(SE), second->get_quad(NW), second->get_quad(NE) }, level - 1);
		}
		else
		{
			return Create(new QuadTree*[4]{ first->get_quad(NE), second->get_quad(NW), first->get_quad(SE), second->get_quad(SW) }, level - 1);
		}
	}
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
get_center_quad()
{
	QuadTree** new_main_quads = new QuadTree*[4];
	for (size_t i = NW; i <= SE; i++)
	{
		int j = SE - i;
		new_main_quads[i] = get_quad(i)->get_quad(j);
	}
	return Create(new_main_quads, level - 1);
}
};