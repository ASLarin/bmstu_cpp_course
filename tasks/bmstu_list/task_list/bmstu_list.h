#include <cstddef>
#include <iterator>
#include <ostream>
#include "abstract_iterator.h"

namespace bmstu
{
template <typename T>
class list
{
	struct node
	{
		node() = default;

		node(node* prev, const T& value, node* next)
			: value_(value), next_node_(next), prev_node_(prev)
		{
		}

		T value_;
		node* next_node_ = nullptr;
		node* prev_node_ = nullptr;
	};

   public:
	struct iterator
		: public abstract_iterator<iterator, T, std::bidirectional_iterator_tag>
	{
		node* current;
		iterator() : current(nullptr) {}
		iterator(node* node) : current(node) {}

		::bmstu::list<T>::iterator& operator++() override
		{
			if (current)
			{
				current = current->next_node_;
			}
			return *this;
		}

		::bmstu::list<T>::iterator& operator--() override
		{
			if (current)
			{
				current = current->prev_node_;
			}
			return *this;
		}

		::bmstu::list<T>::iterator operator++(int) override
		{
			iterator tmp = *this;
			++(*this);
			return tmp;
		}

		::bmstu::list<T>::iterator operator--(int) override
		{
			iterator tmp = *this;
			--(*this);
			return tmp;
		}

		::bmstu::list<T>::iterator& operator+=(
			const typename abstract_iterator<
				typename ::bmstu::list<T>::iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n) override
		{
			if (n > 0)
			{
				for (typename abstract_iterator<
						 typename ::bmstu::list<T>::iterator, T,
						 std::bidirectional_iterator_tag>::difference_type i =
						 0;
					 i < n && current; ++i)
				{
					++(*this);
				}
			}
			else if (n < 0)
			{
				for (typename abstract_iterator<
						 typename ::bmstu::list<T>::iterator, T,
						 std::bidirectional_iterator_tag>::difference_type i =
						 0;
					 i < -n && current; ++i)
				{
					--(*this);
				}
			}
			return *this;
		}

		::bmstu::list<T>::iterator& operator-=(
			const typename abstract_iterator<
				iterator,
				T,
				std::bidirectional_iterator_tag>::difference_type& n) override
		{
			return (*this) += -n;
		}

		::bmstu::list<T>::iterator operator+(
			const typename abstract_iterator<iterator,
											 T,
											 std::bidirectional_iterator_tag>::
				difference_type& n) const override
		{
			iterator tmp = *this;
			return tmp += n;
		}

		::bmstu::list<T>::iterator operator-(
			const typename abstract_iterator<iterator,
											 T,
											 std::bidirectional_iterator_tag>::
				difference_type& n) const override
		{
			iterator tmp = *this;
			return tmp -= n;
		}

		typename abstract_iterator<iterator,
								   T,
								   std::bidirectional_iterator_tag>::reference
		operator*() const override
		{
			return current->value_;
		}

		typename abstract_iterator<iterator,
								   T,
								   std::bidirectional_iterator_tag>::pointer
		operator->() const override
		{
			return &(current->value_);
		}

		bool operator==(const ::bmstu::list<T>::iterator& other) const override
		{
			return current == other.current;
		}

		bool operator!=(const ::bmstu::list<T>::iterator& other) const override
		{
			return !(*this == other);
		}

		explicit operator bool() const override { return current != nullptr; }

		// Исправить на возврат значения вместо ссылки
		typename abstract_iterator<
			iterator,
			T,
			std::bidirectional_iterator_tag>::difference_type
		operator-(const ::bmstu::list<T>::iterator& other) const override
		{
			// Если итераторы равны, расстояние 0
			if (*this == other)
			{
				return 0;
			}

			// Проверяем направление от other к this
			typename abstract_iterator<
				iterator, T, std::bidirectional_iterator_tag>::difference_type
				dist = 0;
			iterator temp = other;
			while (temp.current != nullptr && temp != *this)
			{
				++temp;
				++dist;
			}

			// Если нашли this, возвращаем положительное расстояние
			if (temp == *this)
			{
				return dist;
			}

			// Если не нашли, пробуем в другом направлении
			dist = 0;
			temp = *this;
			while (temp.current != nullptr && temp != other)
			{
				++temp;
				--dist;	 // Отрицательное расстояние, так как идём от this к
						 // other
			}

			// Если нашли other, возвращаем отрицательное расстояние
			if (temp == other)
			{
				return dist;
			}

			// Если не нашли в обоих направлениях, итераторы из разных
			// контейнеров
			return 0;
		}
	};
	using const_iterator = iterator;

	list()
	{
		// Создаем пустой список с фиктивными узлами
		head_ = new node();
		tail_ = new node();
		head_->next_node_ = tail_;
		tail_->prev_node_ = head_;
	}

	template <typename it>
	list(it begin, it end) : list()
	{
		for (auto iter = begin; iter != end; ++iter)
		{
			push_back(*iter);
		}
	}

	list(std::initializer_list<T> values) : list()
	{
		for (const auto& val : values)
		{
			push_back(val);
		}
	}

	list(const list& other) : list()
	{
		for (const auto& item : other)
		{
			push_back(item);
		}
	}

	list(list&& other) : size_(other.size_)
	{
		// Перемещение указателей
		head_ = other.head_;
		tail_ = other.tail_;

		// Обнуление источника
		other.head_ = nullptr;
		other.tail_ = nullptr;
		other.size_ = 0;
	}

#pragma endregion
#pragma region pushs

	template <typename Type>
	void push_back(const Type& value)
	{
		node* last = tail_->prev_node_;
		node* new_node = new node(last, value, tail_);
		last->next_node_ = new_node;
		tail_->prev_node_ = new_node;
		++size_;
	}

	template <typename Type>
	void push_front(const Type& value)
	{
		node* first = head_->next_node_;
		node* new_node = new node(head_, value, first);
		head_->next_node_ = new_node;
		first->prev_node_ = new_node;
		++size_;
	}

#pragma endregion

	bool empty() const noexcept { return size_ == 0; }

	~list()
	{
		clear();
		delete head_;
		delete tail_;
	}

	void clear()
	{
		while (!empty())
		{
			node* to_delete = head_->next_node_;
			head_->next_node_ = to_delete->next_node_;
			to_delete->next_node_->prev_node_ = head_;
			delete to_delete;
			--size_;
		}
	}

	size_t size() const { return size_; }

	void swap(list& other) noexcept
	{
		std::swap(head_, other.head_);
		std::swap(tail_, other.tail_);
		std::swap(size_, other.size_);
	}

	friend void swap(list& l, list& r) { l.swap(r); }

#pragma region iterators

	iterator begin() noexcept { return iterator{head_->next_node_}; }

	iterator end() noexcept { return iterator{tail_}; }

	const_iterator begin() const noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator end() const noexcept { return const_iterator{tail_}; }

	const_iterator cbegin() const noexcept
	{
		return const_iterator{head_->next_node_};
	}

	const_iterator cend() const noexcept { return const_iterator{tail_}; }

#pragma endregion

	T operator[](size_t pos) const
	{
		auto it = begin();
		for (size_t i = 0; i < pos; ++i)
		{
			++it;
		}
		return *it;
	}

	T& operator[](size_t pos)
	{
		auto it = begin();
		for (size_t i = 0; i < pos; ++i)
		{
			++it;
		}
		return *it;
	}

	friend bool operator==(const list& l, const list& r)
	{
		if (l.size() != r.size())
		{
			return false;
		}

		auto l_it = l.begin();
		auto r_it = r.begin();

		while (l_it != l.end())
		{
			if (*l_it != *r_it)
			{
				return false;
			}
			++l_it;
			++r_it;
		}

		return true;
	}

	friend bool operator!=(const list& l, const list& r) { return !(l == r); }

	friend auto operator<=>(const list& lhs, const list& rhs)
	{
		return lexicographical_compare_(lhs, rhs) ? std::strong_ordering::less
			   : lexicographical_compare_(rhs, lhs)
				   ? std::strong_ordering::greater
				   : std::strong_ordering::equal;
	}

	friend std::ostream& operator<<(std::ostream& os, const list& other)
	{
		os << "{";
		bool first = true;
		for (const auto& item : other)
		{
			if (!first)
			{
				os << ", ";
			}
			os << item;
			first = false;
		}
		os << "}";
		return os;
	}

	iterator insert(const_iterator pos, const T& value)
	{
		node* current = pos.current;
		node* prev = current->prev_node_;

		node* new_node = new node(prev, value, current);
		prev->next_node_ = new_node;
		current->prev_node_ = new_node;

		++size_;
		return iterator{new_node};
	}

   private:
	static bool lexicographical_compare_(const list<T>& l, const list<T>& r)
	{
		auto l_it = l.begin();
		auto r_it = r.begin();

		while (l_it != l.end() && r_it != r.end())
		{
			if (*l_it < *r_it)
				return true;
			if (*r_it < *l_it)
				return false;
			++l_it;
			++r_it;
		}

		return l_it == l.end() && r_it != r.end();
	}

	size_t size_ = 0;
	node* tail_ = nullptr;
	node* head_ = nullptr;
};
// namespace bmstu
}  // namespace bmstu