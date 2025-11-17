#pragma once
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
struct ForwardListNode
{
	T data;
	ForwardListNode* next = nullptr;
	ForwardListNode() = default;
	ForwardListNode(const T& data_) : data(data_) {}
	ForwardListNode(T&& data_) : data(std::move(data_)) {}
	ForwardListNode(const ForwardListNode& other) : data(other.data)
	{
		if (other.next)
		{
			next = new ForwardListNode(*other.next);
		}
	}
	ForwardListNode(ForwardListNode&& other) noexcept
		: data(std::move(other.data))
	{
		if (other.next)
		{
			next = other.next;
			other.next = nullptr;
		}
	}
	ForwardListNode(const T& data_, ForwardListNode* next_)
		: data(data_), next(next_)
	{
	}
	ForwardListNode(T&& data_, ForwardListNode* next_)
		: data(std::move(data_)), next(next_)
	{
	}
};

template <typename T>
void create_list(ForwardListNode<T>*& head, const std::vector<T>& data)
{
	if (data.empty())
	{
		head = nullptr;
		return;
	}

	head = new ForwardListNode<T>(data[0]);

	auto current = head;

	for (size_t i = 1; i < data.size(); ++i)
	{
		current->next = new ForwardListNode<T>(data[i]);
		current = current->next;
	}
}

template <typename T>
void delete_list(ForwardListNode<T>*& head)
{
	ForwardListNode<T>* current = head;

	while (current != nullptr)
	{
		ForwardListNode<T>* next = current->next;
		delete current;
		current = next;
	}

	head = nullptr;
}

template <typename T>
void print_list(const ForwardListNode<T>* head, std::ostream& os)
{
	if (head == nullptr)
	{
		os << "[]";
		return;
	}

	os << "[";

	const ForwardListNode<T>* current = head;
	while (current != nullptr)
	{
		os << current->data;

		if (current->next != nullptr)
		{
			os << ", ";
		}

		current = current->next;
	}

	os << "]";
}

template <typename T>
void reverse_list(ForwardListNode<T>*& head)
{
	if (head == nullptr || head->next == nullptr)
	{
		return;
	}

	ForwardListNode<T>* prev = nullptr;
	ForwardListNode<T>* current = head;
	ForwardListNode<T>* next = nullptr;

	while (current != nullptr)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}

	head = prev;
}

template <typename T>
void remove_duplicates(ForwardListNode<T>*& head)
{
	if (head == nullptr || head->next == nullptr)
	{
		return;
	}

	ForwardListNode<T>* current = head;

	while (current != nullptr)
	{
		ForwardListNode<T>* runner = current;

		while (runner->next != nullptr)
		{
			if (runner->next->data == current->data)
			{
				ForwardListNode<T>* to_delete = runner->next;
				runner->next = runner->next->next;
				delete to_delete;
			}
			else
			{
				runner = runner->next;
			}
		}

		current = current->next;
	}
}
