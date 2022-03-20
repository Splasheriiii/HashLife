#pragma once
#include "Math.h"

#define NW 0
#define NE 1
#define SW 2
#define SE 3

#include "QuadTree.h"
class ArrayQuadTree : public QuadTree
{
public:
	ArrayQuadTree(int lvl, bool** values, int start_x = 0, int start_y = 0) : ArrayQuadTree(lvl)
    {
        if (lvl == 0)
        {
            has_value = values != nullptr && values[start_y][start_x];
        }
        else
        {
            int half_size = pow(2, lvl - 1);
            initialize_by_quads(new QuadTree*[4] {
                Create(lvl - 1, values, start_x, start_y),
                Create(lvl - 1, values, start_x + half_size, start_y),
                Create(lvl - 1, values, start_x, start_y + half_size),
                Create(lvl - 1, values, start_x + half_size, start_y + half_size)
            });
        }
    }
protected:
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 get_quad(int quad)
    {
        return main_quads[quad];
    }
	void 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 initialize_by_quads(QuadTree** new_main_quads)
    {
        for (size_t i = NW; i <= SE; i++)
        {
            main_quads[i] = new_main_quads[i];
            has_value |= (main_quads[i]->does_have_val());
        }
    }
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 Create(int lvl, bool** values, int start_x = 0, int start_y = 0)
    {
        return new ArrayQuadTree(lvl, values, start_x, start_y);
    }
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
 Create(QuadTree** new_main_quads, int lvl)
    {
        return new ArrayQuadTree(new_main_quads, lvl);
    }
private:
	QuadTree** main_quads;
	ArrayQuadTree(QuadTree** new_main_quads, int lvl) : ArrayQuadTree(lvl)
    {
        initialize_by_quads(new_main_quads);
    }
	ArrayQuadTree(int lvl) : QuadTree(lvl) 
    {
        main_quads = new QuadTree*[4]{0,0,0,0};
    }
};