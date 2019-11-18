/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include "expire_list.hpp"

template <typename ElemType, typename TimeType, typename IdType>
void ExpireList<ElemType, TimeType, IdType>::insert(const IdType& id, const TimeType& time,
													const ElemType& element)
{
	const auto hret = m_heap.emplace(std::make_tuple(time, id), element);
	const auto tret = m_tmap.emplace(id, time);

	if (!(hret.second && tret.second))
	{
		throw std::logic_error(
				std::string{"inserting duplicated id = "}.append(std::to_string(id)));
	}
}

template <typename ElemType, typename TimeType, typename IdType>
void ExpireList<ElemType, TimeType, IdType>::erase(const IdType& id)
{
	const auto it_id_time = m_tmap.find(id);
	m_heap.erase(std::make_tuple(it_id_time->second, id)); // throw
	m_tmap.erase(it_id_time);
}

template <typename ElemType, typename TimeType, typename IdType>
typename ExpireList<ElemType, TimeType, IdType>::iterator
ExpireList<ElemType, TimeType, IdType>::erase(
		const typename ExpireList<ElemType, TimeType, IdType>::iterator& it)
{
	const auto id = std::get<1>(it->first);
	m_tmap.erase(id); // throw
	return m_heap.erase(it);
}

template <typename ElemType, typename TimeType, typename IdType>
bool ExpireList<ElemType, TimeType, IdType>::clear(const TimeType& time)
{
	for (auto it_ti_elem = m_heap.begin(); it_ti_elem != m_heap.end();)
	{
		const auto time_ = std::get<0>(it_ti_elem->first);
		if (time_ > time)
		{
			return false;
		}

		const auto id = std::get<1>(it_ti_elem->first);
		m_tmap.erase(id);
		it_ti_elem = m_heap.erase(it_ti_elem);
	}

	return true;
}

template <typename ElemType, typename TimeType, typename IdType>
void ExpireList<ElemType, TimeType, IdType>::update(const IdType& id, const TimeType& time)
{
	const auto it_id_time = m_tmap.find(id);
	if (it_id_time == m_tmap.cend())
	{
		throw std::logic_error(
				std::string{"update non-existing id = "}.append(std::to_string(id)));
	}

	// HINT: accept increment only
	if (it_id_time->second < time)
	{
		const auto it_ti_elem = m_heap.find(std::make_tuple(it_id_time->second, id));
		it_id_time->second    = time;
		heap_move(std::make_tuple(time, id), it_ti_elem->second);
		m_heap.erase(it_ti_elem);
	}
}

template <typename ElemType, typename TimeType, typename IdType>
typename ExpireList<ElemType, TimeType, IdType>::iterator
ExpireList<ElemType, TimeType, IdType>::update(
		const ExpireList<ElemType, TimeType, IdType>::iterator& it, const TimeType& time)
{
	const auto id    = std::get<1>(it->first);
	auto&      time_ = m_tmap.at(id);

	// HINT: accept increment only
	if (time_ < time)
	{
		time_ = time;
		heap_move(std::make_tuple(time, id), it->second);
		return m_heap.erase(it);
	}

	return ++typename HeapType::iterator{it};
}

template <typename ElemType, typename TimeType, typename IdType>
bool ExpireList<ElemType, TimeType, IdType>::update(const IdType& id, const TimeType& time,
													const ElemType& element)
{
	const auto it_id_time = m_tmap.find(id);
	if (it_id_time == m_tmap.cend())
	{
		insert(id, time, element);
		return false;
	}

	// HINT: accept increment only, otherwise element is discarded
	if (it_id_time->second < time)
	{
		m_heap.erase(std::make_tuple(it_id_time->second, id));
		it_id_time->second = time;
		m_heap.emplace(std::make_tuple(time, id), element);
	}

	return true;
}
