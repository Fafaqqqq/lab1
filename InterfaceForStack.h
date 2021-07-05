#pragma once
#include "Stack.h"

template<typename T>
class InterfaceForStack
{
private:	

	void Push(Stack<T>& stack, const T& value)
	{
		stack.Push(value);
	}
	
	void Pop(Stack<T>& stack)
	{
		try
		{
			stack.Pop();
			std::cout << "Top element removed!\n\n";
		}
		catch (const stack_empty_error& err)
		{
			std::cerr << err.what() << std::endl;
		}
	}
	
	bool IsEmpty(Stack<T>& stack)
	{
		return stack.IsEmpty();
	}

	const T& CTop(Stack<T>& stack)
	{
		try
		{
			return stack.CTop();
		}
		catch (const stack_empty_error& err)
		{
			std::cerr << err.what() << std::endl;

			return T();
		}
	}

public:

	void run()
	{
		int command_number = INT_MAX;
		T value;

		int size;
		std::cout << "Enter a steck size: ";
		std::cin >> size;
		std::cout << std::endl;

		Stack<T> stack(size);

		std::cout << "Command list:\n";
		std::cout << "1. Add an item to the top.\n";
		std::cout << "2. Delete an item to the top.\n";
		std::cout << "3. Check Stack.\n";
		std::cout << "4. Print the top element.\n\n";
		std::cout << "0. Exit.\n\n";

		while (command_number)
		{
			std::cout << "Enter a command from 0 to 4: ";
			std::cin >> command_number;

			switch (command_number)
			{
			case 0:
				break;

			case 1:
				std::cout << "Enter a value: ";
				std::cin >> value;
				this->Push(stack, value);
				std::cout << std::endl;
				break;

			case 2:
				this->Pop(stack);
				
				break;

			case 3:
				if (this->IsEmpty(stack))
				{
					std::cout << "Your Stack is empty!\n\n";
				}
				else
				{
					std::cout << "Your Stack isn`t empty!\n\n";
				}
				break;

			case 4:

				if (!this->IsEmpty(stack))
				{
					std::cout << "Your the top element: " << this->CTop(stack) << std::endl << std::endl;
				}
				else
				{
					std::cout << "Your Stack is empty!\n\n";
				}
				break;

			default:
				std::cout << "Please enter a valid value again!\n\n";
				break;
			}

		}
	}
};