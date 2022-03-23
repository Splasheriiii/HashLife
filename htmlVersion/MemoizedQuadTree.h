#pragma once
#include "QuadTreeManager.h"

#define NW 0
#define NE 1
#define SW 2
#define SE 3

#define TRUE_HASH 13
#define FALSE_HASH 10
#define NOT_IMPLEMENTED_ERROR "MemoizedQuadTree::initialize_by_quads is not implemented"

#define _FNV_offset_basis 14695981039346656037ULL;
#define _FNV_prime 1099511628211ULL;

#include "QuadTree.h"

class MemoizedQuadTree : public QuadTree
{
public:
	static QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	get_memoized_tree(int lvl, bool** values, int start_x = 0, int start_y = 0)
	{
		return (new MemoizedQuadTree(false))->Create(lvl, values, start_x, start_y);
	}
	
	void 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	get_hash_list(std::vector<std::string>* res)
	{
		get_fabric()->get_hash_list(res);
	}
protected:
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	get_quad(int quad)
	{
		if (level == 0)
		{
			return 0;
		}
		return main_quads[quad];
	}
	void 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	initialize_by_quads(QuadTree** new_main_quads)
	{
		throw NOT_IMPLEMENTED_ERROR;
	}
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	Create(int lvl, bool** values, int start_x, int start_y)
	{
		if (lvl == 0)
		{
			return Create((values != nullptr && values[start_y][start_x]));
		}
		int half_size = pow(2, lvl - 1);
		return Create(new QuadTree * [4]{
				Create(lvl - 1, values, start_x, start_y),
				Create(lvl - 1, values, start_x + half_size, start_y),
				Create(lvl - 1, values, start_x, start_y + half_size),
				Create(lvl - 1, values, start_x + half_size, start_y + half_size),
		}, lvl);
	}
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	Create(QuadTree** new_main_quads, int lvl)
	{
		QuadTreeManager* fabric = get_fabric();
		long hash = get_hash(new long[4]{ ((MemoizedQuadTree*)new_main_quads[NW])->hash,
										((MemoizedQuadTree*)new_main_quads[NE])->hash,
										((MemoizedQuadTree*)new_main_quads[SW])->hash,
										((MemoizedQuadTree*)new_main_quads[SE])->hash });
		MemoizedQuadTree* res = (MemoizedQuadTree*)(fabric->get_value(hash));
		if (res == nullptr)
		{
			res = new MemoizedQuadTree((MemoizedQuadTree**)new_main_quads, lvl);
			fabric->set_value(hash, res);
		}
		return res;
	}
	QuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	next_generation()
	{
		QuadTreeManager* fabric = get_fabric();
		if (fabric->has_res_value(hash))
		{
			return fabric->get_value(fabric->get_res_value(hash));
		}
		else
		{
			auto res = (MemoizedQuadTree*)QuadTree::next_generation();
			fabric->set_res_value(hash, res->hash);
			return res;
		}
	}
private:
	MemoizedQuadTree** main_quads;
	long hash;
	inline size_t 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	_Fnv1a_append_bytes(const unsigned char* const _First, const size_t _Count) 
	{ 
		size_t _Val = _FNV_offset_basis;
		for (size_t _Idx = 0; _Idx < _Count; ++_Idx) 
		{
			_Val ^= static_cast<size_t>(_First[_Idx]);
			_Val *= _FNV_prime;
		}
		return _Val;
	}
	// bitwise hashes the representation of an array
	size_t 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	hash_function(long* _First, const size_t _Count)
	{		
		return _Fnv1a_append_bytes(reinterpret_cast<const unsigned char*>(_First), _Count * sizeof(long));
	}

	long 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	get_hash(long* quads_hashes)
	{
		return hash_function(quads_hashes, 4);
	}
	QuadTreeManager* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	get_fabric()
	{
		return QuadTreeManager::get_instance();
	}
	MemoizedQuadTree* 
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
	Create(bool has_value)
	{
		QuadTreeManager* fabric = get_fabric();
		MemoizedQuadTree* res = (MemoizedQuadTree*)(fabric->get_value(has_value ? TRUE_HASH : FALSE_HASH));
		if (res == nullptr)
		{
			res = new MemoizedQuadTree(has_value);
			fabric->set_value(has_value ? TRUE_HASH : FALSE_HASH, res);
		}
		return res;
	}
	MemoizedQuadTree(bool has_value) : MemoizedQuadTree(0)
	{
		this->has_value = has_value;
		hash = has_value ? TRUE_HASH : FALSE_HASH;
	}
	MemoizedQuadTree(int lvl) : QuadTree(lvl) 
	{
		main_quads = new MemoizedQuadTree*[4]{ 0,0,0,0 };
		hash = -99999999;
	}
	MemoizedQuadTree(MemoizedQuadTree** new_main_quads, int lvl) : MemoizedQuadTree(lvl)
	{
		hash = get_hash(new long[4]{ ((MemoizedQuadTree*)new_main_quads[NW])->hash,
									((MemoizedQuadTree*)new_main_quads[NE])->hash,
									((MemoizedQuadTree*)new_main_quads[SW])->hash,
									((MemoizedQuadTree*)new_main_quads[SE])->hash });
		for (size_t i = NW; i <= SE; i++)
		{
			main_quads[i] = new_main_quads[i];
			has_value |= new_main_quads[i]->has_value;
		}
	}
};