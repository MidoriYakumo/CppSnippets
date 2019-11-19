/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <map>
#include <tuple>
#include <utility>
#include <vector>

//// hepler traits

template <bool P, typename T = void>
using enable_if_t = typename std::enable_if<P, T>::type;

//// ExpireList with given id

template <typename ElemType, typename TimeType = float, typename IdType = void>
class ExpireList
{
protected:
	// time, id, element
	using HeapType = std::map<std::tuple<TimeType, IdType>, ElemType>;
	// id, time
	using TimeMapType = std::map<IdType, TimeType>;

	using const_iterator = typename HeapType::const_iterator;
	using iterator       = typename HeapType::iterator;

public:
	ExpireList()  = default;
	~ExpireList() = default;

	ExpireList(const ExpireList&) = default;

	ExpireList& operator=(const ExpireList&) = default;

	inline const ElemType& operator[](const IdType& id) const
	{
		return at(id); // throw
	}

	inline ElemType& operator[](const IdType& id)
	{
		return at(id); // throw
	}

	void insert(const IdType& id, const TimeType& time, const ElemType& element);

	template <typename Iter>
	void insert(Iter&& it_ite_begin, Iter&& it_ite_end)
	{
		std::vector<typename HeapType::value_type>    heap_items;
		std::vector<typename TimeMapType::value_type> tmap_items;
		const auto size = std::distance(it_ite_begin, it_ite_end);
		heap_items.reserve(size);
		tmap_items.reserve(size);
		for (auto it_ite = it_ite_begin; it_ite != it_ite_end; ++it_ite)
		{
			const auto& id      = std::get<0>(*it_ite);
			const auto& time    = std::get<1>(*it_ite);
			const auto& element = std::get<2>(*it_ite);
			heap_items.emplace_back(std::make_tuple(time, id), element);
			tmap_items.emplace_back(id, time);
		}
		insert(heap_items, tmap_items); // HINT: nothrow
	}

	inline const ElemType& at(const IdType& id) const
	{
		return m_heap.at(std::make_tuple(m_tmap.at(id), id)); // throw
	}

	inline ElemType& at(const IdType& id)
	{
		return m_heap.at(std::make_tuple(m_tmap.at(id), id)); // throw
	}

	inline const_iterator begin() const noexcept
	{
		return m_heap.cbegin();
	}

	inline iterator begin() noexcept
	{
		return m_heap.begin();
	}

	inline const_iterator end() const noexcept
	{
		return m_heap.cend();
	}

	inline iterator end() noexcept
	{
		return m_heap.end();
	}

	void     erase(const IdType& id);
	iterator erase(const iterator& it);

	inline void clear() noexcept
	{
		m_heap.clear();
		m_tmap.clear();
	}

	bool clear(const TimeType& time); // return true if early-stopped

	// update with id
	void update(const IdType& id, const TimeType& time);
	// update with iterator
	iterator update(const iterator& it, const TimeType& time);
	// update element as well, return false if (id, time) not found but inserted
	bool update(const IdType& id, const TimeType& time, const ElemType& element);

protected:
	template <typename T>
	inline enable_if_t<std::is_move_constructible<T>::value>
	heap_move(const typename HeapType::key_type& index, T& source)
	{
		m_heap.emplace(index, std::move(source));
	}

	template <typename T>
	inline enable_if_t<!std::is_move_constructible<T>::value &&
					   std::is_default_constructible<T>::value>
	heap_move(const typename HeapType::key_type& index, T& source)
	{
		std::swap(m_heap[index], source);
	}

	template <typename T>
	inline enable_if_t<!std::is_move_constructible<T>::value &&
					   !std::is_default_constructible<T>::value &&
					   std::is_copy_constructible<T>::value>
	heap_move(const typename HeapType::key_type& index, const T& source)
	{
		m_heap.emplace(index, source);
	}

	inline void insert(const std::vector<typename HeapType::value_type>&    heap_items,
					   const std::vector<typename TimeMapType::value_type>& tmap_items)

	{
		m_heap.insert(heap_items.cbegin(), heap_items.cend()); // HINT: nothrow
		m_tmap.insert(tmap_items.cbegin(), tmap_items.cend()); // HINT: nothrow
	}

private:
	HeapType    m_heap;
	TimeMapType m_tmap;
};

//// ExpireList with built-in id

template <typename ElemType, typename TimeType>
class ExpireList<ElemType, TimeType, void> : public ExpireList<ElemType, TimeType, unsigned int>
{
protected:
	using id_t = unsigned int;
	using Base = ExpireList<ElemType, TimeType, id_t>;
	using typename Base::HeapType;
	using typename Base::TimeMapType;

	// methods with IdType = void is no longer callable

public:
	using Base::Base;
	using Base::clear;

	inline void insert(const TimeType& time, const ElemType& element)
	{
		Base::insert(m_id, time, element);
		++m_id;
	}

	template <typename Iter>
	void insert(Iter&& it_te_begin, Iter&& it_te_end)
	{
		std::vector<typename HeapType::value_type>    heap_items;
		std::vector<typename TimeMapType::value_type> tmap_items;
		const auto size = std::distance(it_te_begin, it_te_end);
		heap_items.reserve(size);
		tmap_items.reserve(size);
		for (auto it_it = it_te_begin; it_it != it_te_end; ++it_it)
		{
			const auto& time    = std::get<0>(*it_it);
			const auto& element = std::get<1>(*it_it);
			heap_items.emplace_back(std::make_tuple(time, m_id), element);
			tmap_items.emplace_back(m_id, time);
			++m_id;
		}
		Base::insert(heap_items, tmap_items); // HINT: nothrow
	}

	inline void clear() noexcept
	{
		Base::clear();
		m_id = 0;
	}

private:
	id_t m_id{0};
};
