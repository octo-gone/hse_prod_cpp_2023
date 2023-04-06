#include "inline_vector/inline_vector.hpp"
#include <iostream>
#include <vector>

int main()
{
    InlineVector<int, 5> myVector;
    int choice, value;

    while (true)
    {
        std::cout << "Menu:\n";
        std::cout << "1. Add value\n";
        std::cout << "2. Remove last value\n";
        std::cout << "3. Print size and capacity\n";
        std::cout << "4. Print vector\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice:\n";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                std::cout << "Enter value to add:\n";
                std::cin >> value;
                myVector.push_back(value);
                std::cout << "Value added.\n";
                break;

            case 2:
                if (myVector.empty())
                {
                    std::cout << "Vector is empty.\n";
                }
                else
                {
                    myVector.pop_back();
                    std::cout << "Last value removed.\n";
                }
                break;

            case 3:
                std::cout << "Size: " << myVector.size() << std::endl;
                std::cout << "Capacity: " << myVector.capacity() << std::endl;
                break;

            case 4:
                if (myVector.empty())
                {
                    std::cout << "Vector is empty.\n";
                }
                else
                {
                    std::cout << "Vector contents:\n";
                    for (size_t i = 0; i < myVector.size(); i++)
                    {
                        std::cout << myVector[i] << " ";
                    }
                    std::cout << std::endl;
                }
                break;

            case 5:
                std::cout << "Exiting...\n";
                return 0;

            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
}