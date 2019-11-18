
#include "expire_list.inl.hpp"

#include <iostream>

template <typename T>
void print_list(const T& list)
{
	for (const auto& ti_elem : list)
	{
		std::cout << ti_elem.second << " ";
	}
	std::cout << std::endl;
}

int main()
{
	try
	{
		{
			ExpireList<std::string, float, int> list;
			std::cout << "insert: ";
			list.insert(1, 3, "1@3");
			list.insert(2, 1, "2@1");
			list.insert(3, 4, "3@4");
			list.insert(4, 1, "4@1");
			print_list(list);
			std::cout << "batch insert: ";
			std::vector<std::tuple<int, float, std::string>> insert_list{
					{5, 5, "5@5"},
					{6, 9, "6@9"},
					{7, 2, "7@2"},
					{8, 6, "8@6"},
			};
			list.insert(insert_list.begin(), insert_list.end());
			print_list(list);
			{
				std::cout << "erase: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				std::cout << "remain ";
				list_.erase(2);
				list_.erase(7);
				print_list(list_);
			}
			{
				std::cout << "erase iter: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				auto iter = list_.begin();
				std::cout << "erase " << iter->second << std::endl;
				iter = list_.erase(iter);
				std::cout << "erase " << iter->second << std::endl;
				iter = list_.erase(iter);
				std::cout << "remain ";
				print_list(list_);
			}
			{
				std::cout << "clear: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				list_.clear();
				std::cout << "remain ";
				print_list(list_);
			}
			{
				std::cout << "clear expire = 4: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				list_.clear(4);
				std::cout << "remain ";
				print_list(list_);
			}
			{
				std::cout << "at: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				std::cout << "get 2, " << list_.at(2) << std::endl;
				std::cout << "get 8, " << list_.at(8) << std::endl;
				list_.at(8) = "888";
				std::cout << "set 8, " << list_.at(8) << std::endl;
			}
			//		{
			//			std::cout << "foreach: " << std::endl;
			//			std::cout << "clone, clear, ";
			//			auto list_ = list;
			//			list_.clear(3);
			//			print_list(list_);
			//			list_.foreach ([](const std::string& s) { std::cout << "for " << s << "
			//";
			//}); 			std::cout << std::endl;
			//		}
			{
				std::cout << "iterator compare: " << std::endl;
				auto list_ = list;
				std::cout << "same " << !(list.begin() != list.begin()) << std::endl;
				std::cout << "different " << !(list.begin() != list_.begin()) << std::endl;
				std::cout << "different end " << !(list.end() != list_.end()) << std::endl;
			}
			{
				std::cout << "update: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				std::cout << "update time of 1 to 7, ";
				list_.update(1, 7);
				print_list(list_);
				std::cout << "clear 4, ";
				list_.clear(4);
				print_list(list_);
			}
			{
				std::cout << "update element: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				std::cout << "update 1 to 1@7, ";
				list_.update(1, 7, "1@7");
				print_list(list_);
				std::cout << "add 9@5, ";
				list_.update(9, 5, "9@5");
				print_list(list_);
				std::cout << "find 9@5, update time to 8, ";
				for (auto it = list_.begin(); it != list_.end();)
				{
					if (it->second == "9@5")
					{
						it = list_.update(it, 8);
					}
					else
					{
						++it;
					}
				}
				print_list(list_);
				std::cout << "clear 4, ";
				list_.clear(4);
				print_list(list_);
			}
		}
		std::cout << std::endl << "IdType = void" << std::endl << std::endl;
		{
			ExpireList<std::string> list;
			std::cout << "insert: ";
			list.insert(3, "1@3");
			list.insert(3, "1@3");
			list.insert(1, "2@1");
			list.insert(4, "3@4");
			list.insert(1, "4@1");
			print_list(list);
			std::cout << "batch insert: ";
			std::vector<std::tuple<float, std::string>> insert_list{
					{5, "5@5"},
					{9, "6@9"},
					{2, "7@2"},
					{6, "8@6"},
			};
			list.insert(insert_list.begin(), insert_list.end());
			print_list(list);
			{
				std::cout << "update: " << std::endl;
				std::cout << "clone ";
				auto list_ = list;
				print_list(list_);
				std::cout << "updated ";
				for (auto it = list_.begin(); it != list_.end();)
				{
					if (it->second == "9@5")
					{
						it = list_.update(it, 8);
					}
					else
					{
						++it;
					}
				}
				print_list(list_);
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
