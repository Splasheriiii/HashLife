#pragma once
#include <unordered_map>
#include <vector>
#include "QuadTree.h"


class QuadTreeManager
{
	static QuadTreeManager* instance;	
public:
	static QuadTreeManager* get_instance()
	{
		return instance == nullptr ? (instance = new QuadTreeManager()) : instance;
	}

	QuadTree* get_value(long hash)
	{
		return hash_map.find(hash) != hash_map.end()
				? hash_map[hash]
				: nullptr;
	}
	void set_value(long hash, QuadTree* tree)
	{
		hash_map[hash] = tree;
	}

	bool has_res_value(long hash)
	{
		return hash_res_map.find(hash) != hash_res_map.end();
	}
	long get_res_value(long hash)
	{
		return hash_res_map[hash];
	}
	void set_res_value(long hash, long res_hash)
	{
		hash_res_map[hash] = res_hash;
	}
private:
	QuadTreeManager(){}
	std::unordered_map<long, QuadTree*> hash_map;
	std::unordered_map<long, long> hash_res_map;	
};
QuadTreeManager* QuadTreeManager::instance = 0;