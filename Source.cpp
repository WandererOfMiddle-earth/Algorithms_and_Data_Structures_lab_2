#include <cstdio>
#include <iostream>
#include <conio.h>
#include <utility>
#include <string>
#include <vector>
#define epsilon 0.00000001

using namespace std;



int get_key()
{
	int key = _getch();

	if (key == 0 || key == 224)
	{
		int i = 0;
		while (!_kbhit())
		{
			if (i < 1000)
				i++;
			else
				return key;
		}
		key = 1000 + _getch();
	}
	//cout << "[" << key << "]";
	return key;
}


//template <typename V> V string_to_T(string element);
//template <> int string_to_T(string element)
//{
//	return stoi(element);
//}
//template <> double string_to_T(string element)
//{
//	return stod(element);
//}



template <typename T> class set
{
private:
	vector<T> list;

	//template <typename V> friend V string_to_T(string element);

	int operator () (const double element) const
	{
		int size = list.size();
		for (int i = 0; i < size; i++)
			if (abs(list[i] - element) < epsilon)
				return i;
		return -1;
	}
	int operator () (const int element) const
	{
		int size = list.size();
		for (int i = 0; i < size; i++)
			if (list[i] == element)
				return i;
		return -1;
	}

public:
	T operator [] (const int i) const
	{
		if (i < 0 || i >= list.size())
			throw logic_error("Invalid index");
		return list[i];
	}

	set<T>& operator += (const T element)
	{
		if ((*this)(element) == -1)
			list.push_back(element);
		return *this;
	}
	set<T>& operator += (const set<T>& set_)
	{
		int size = set_.list.size();
		for (int i = 0; i < size; i++)
			*this += set_.list[i];
		return *this;
	}

	set<T>& operator -= (const T element)
	{
		int i = (*this)(element);
		if (i != -1)
			list.erase(list.begin() + i);
		return *this;
	}
	set<T>& operator -= (const set<T>& set_)
	{
		int size = set_.list.size();
		for (int i = 0; i < size; i++)
			*this -= set_.list[i];
		return *this;
	}

	set()
	{
	}
	set(T element)
	{
		list.clear();
		list.push_back(element);
	}
	set(const T* array_, int size)
	{
		list.clear();
		for (int i = 0; i < size; i++)
			*this += array_[i];
	}
	set(const vector<T> vector_)
	{
		list.clear();
		int size = vector_.size();
		for (int i = 0; i < size; i++)
			*this += vector_[i];
	}
	set(const set<T>& set_)
	{
		list.clear();
		int size = set_.list.size();
		for (int i = 0; i < size; i++)
			list.push_back(set_.list[i]);
	}

	set<T>& operator = (const set<T>& set_)
	{
		set rhs(set_);
		swap(list, rhs.list);
		return *this;
	}

	friend set<T> operator + (const set<T>& set_1, const set<T>& set_2) // union
	{
		set<T> result(set_1);
		result += set_2;
		return result;
	}
	friend set<T> operator - (const set<T>& set_1, const set<T>& set_2) // difference
	{
		set result(set_1);
		result -= set_2;
		return result;
	}
	friend set<T> operator * (const set<T>& set_1, const set<T>& set_2) // intersection
	{
		return (set_1 + set_2) - ((set_1 - set_2) + (set_2 - set_1));
	}

	friend bool operator == (const set<T>& set_1, const set<T>& set_2)
	{
		if (set_1.list.size() != set_2.list.size())
			return false;

		int size_1 = set_1.list.size();
		for (int i = 0; i < size_1; i++)
			if (set_2(set_1.list[i]) == -1)
				return false;
		return true;
	}
	friend bool operator != (const set<T>& set_1, const set<T>& set_2)
	{
		if (set_1 == set_2)
			return false;
		else
			return true;
	}

	friend istream& operator >> (istream& in, set<T>& set_);
	friend ostream& operator << (ostream& out, const set<T>& set_)
	{
		int size = set_.list.size();
		if (size == 0)
			out << "()";
		else
		{
			out << "(";
			for (int i = 0; i < size - 1; i++)
				out << set_.list[i] << ", ";
			out << set_.list[size - 1] << ")";
		}
		return out;
	}
};

istream& operator >> (istream& in, set<int>& set_)
{
	string buffer;
	bool flag = true;
	int length = 0;

	while (flag)
	{
		int key = get_key();
		length = buffer.length();

		switch (key)
		{
		case 13: // enter
			if (buffer.empty() || !buffer.empty() && buffer[length - 1] != '-')
				flag = false;
			break;
		case 8: // backspace
			if (!buffer.empty())
			{
				buffer.pop_back();
				cout << "\b \b";
			}
			break;
		case 32: // space
			if (!buffer.empty() && buffer[length - 1] != ' ' && buffer[length - 1] != '-')
			{
				buffer.push_back(' ');
				cout << ' ';
			}
			break;
		case 45: // minus
			if (buffer.empty() || buffer[length - 1] == ' ')
			{
				buffer.push_back('-');
				cout << '-';
			}
			break;
		case 48: // zero
			switch (length)
			{
			case 0:
				buffer.push_back('0');
				cout << '0';
				break;
			case 1:
				if (buffer[0] != '-' && buffer[0] != '0')
				{
					buffer.push_back('0');
					cout << '0';
				}
				break;
			default:
				if (buffer[length - 1] != '-' && buffer.substr(length - 2) != " 0")
				{
					buffer.push_back('0');
					cout << '0';
				}
				break;
			}
			break;
		default: // 1-9 numbers
			if (49 <= key && key <= 57)
			{
				if (length == 0 || length == 1 && buffer[0] != '0' ||
					length >= 2 && buffer.substr(length - 2) != " 0")
				{
					buffer.push_back(key);
					cout << (char)key;
				}
			}
			break;
		}
	}

	int i = 0;
	length = buffer.length();
	while (i < length)
	{
		string buffer_element;
		while (buffer[i] != ' ' && i < length)
		{
			buffer_element += buffer[i];
			i++;
		}
		set_ += stoi(buffer_element);;
		buffer_element.clear();
		i++;
	}
	return in;
}

bool zero_with_minus(string buffer_element)
{
	if (buffer_element[0] == '-' && stod(buffer_element) == 0)
		return true;
	else
		return false;
}
istream& operator >> (istream& in, set<double>& set_)
{
	string buffer;
	bool flag = true;
	int length = 0;

	while (flag)
	{
		int key = get_key();
		length = buffer.length();

		switch (key)
		{
		case 13: // enter
			if (buffer.empty() || !buffer.empty() && buffer[length - 1] != '-' && buffer[length - 1] != '.')
			{
				if (length == 1 || length >= 2 && buffer.substr(length - 2) != "-0" && 
					!zero_with_minus(buffer.substr(buffer.rfind(' ') != -1 ? buffer.rfind(' ') + 1 : 0)))
				{
					flag = false;
				}
			}
			break;
		case 8: // backspace
			if (!buffer.empty())
			{
				buffer.pop_back();
				cout << "\b \b";
			}
			break;
		case 32: // space
			if (!buffer.empty() && buffer[length - 1] != ' ' && buffer[length - 1] != '-' && buffer[length - 1] != '.')
			{
				if (length == 1 || length >= 2 && buffer.substr(length - 2) != "-0" && 
					!zero_with_minus(buffer.substr(buffer.rfind(' ') != -1 ? buffer.rfind(' ') + 1 : 0)))
				{
					buffer.push_back(' ');
					cout << ' ';
				}
			}
			break;
		case 45: // minus
			if (buffer.empty() || buffer[length - 1] == ' ')
			{
				buffer.push_back('-');
				cout << '-';
			}
			break;
		case 46: // dot
			if (!buffer.empty() && buffer[length - 1] != ' ' && buffer[length - 1] != '-' && buffer[length - 1] != '.')
			{
				if (buffer.substr(buffer.rfind(' ') != -1 ? buffer.rfind(' ') + 1 : 0).find('.') == -1)
				{
					buffer.push_back('.');
					cout << '.';
				}
			}
			break;
		default: // 0-9 numbers
			if (48 <= key && key <= 57)
			{
				if (length == 0 || length == 1 && buffer[0] != '0' ||
					length >= 2 && buffer.substr(length - 2) != " 0" && buffer.substr(length - 2) != "-0")
				{
					buffer.push_back(key);
					cout << (char)key;
				}
			}
			break;
		}
	}

	int i = 0;
	length = buffer.length();
	while (i < length)
	{
		string buffer_element;
		while (buffer[i] != ' ' && i < length)
		{
			buffer_element += buffer[i];
			i++;
		}
		set_ += stod(buffer_element);;
		buffer_element.clear();
		i++;
	}
	return in;
}


template <> class set<char>
{
private:
	vector<char> list;

	int operator () (const char char_) const
	{
		int size = list.size();
		for (int i = 0; i < size; i++)
			if (list[i] == char_)
				return i;
		return -1;
	}

public:
	char operator [] (const int i) const
	{
		if (i < 0 || i >= (int)list.size())
			throw logic_error("Invalid index");
		return list[i];
	}

	set<char>& operator += (const char char_)
	{
		if ((*this)(char_) == -1)
			list.push_back(char_);
		return *this;
	}
	set<char>& operator += (const set<char>& set_)
	{
		int size = set_.list.size();
		for (int i = 0; i < size; i++)
			*this += set_.list[i];
		return *this;
	}

	set<char>& operator -= (const char char_)
	{
		int i = (*this)(char_);
		if (i != -1)
			list.erase(list.begin() + i);
		return *this;
	}
	set<char>& operator -= (const set<char>& set_)
	{
		int size = set_.list.size();
		for (int i = 0; i < size; i++)
			*this -= set_.list[i];
		return *this;
	}

	set()
	{
	}
	set(const char char_)
	{
		list.clear();
		list.push_back(char_);
	}
	set(const char* c_string_)
	{
		list.clear();
		int len = strlen(c_string_);
		for (int i = 0; i < len; i++)
			*this += c_string_[i];
	}
	set(const string cpp_string_)
	{
		list.clear();
		int length = cpp_string_.length();
		for (int i = 0; i < length; i++)
			*this += cpp_string_[i];
	}
	set(const vector<char> vector_)
	{
		list.clear();
		int size = vector_.size();
		for (int i = 0; i < size; i++)
			*this += vector_[i];
	}
	set(const set<char>& set_)
	{
		list.clear();
		int size = set_.list.size();
		for (int i = 0; i < size; i++)
			list.push_back(set_.list[i]);
	}

	set<char>& operator = (const set<char>& set_)
	{
		set rhs(set_);
		swap(list, rhs.list);
		return *this;
	}

	friend set<char> operator + (const set<char>& set_1, const set<char>& set_2) // union
	{
		set result(set_1);
		result += set_2;
		return result;
	}
	friend set<char> operator - (const set<char>& set_1, const set<char>& set_2) // difference
	{
		set result(set_1);
		result -= set_2;
		return result;
	}
	friend set<char> operator * (const set<char>& set_1, const set<char>& set_2) // intersection
	{
		return (set_1 + set_2) - ((set_1 - set_2) + (set_2 - set_1));
	}

	friend bool operator == (const set<char>& set_1, const set<char>& set_2)
	{
		if (set_1.list.size() != set_2.list.size())
			return false;

		int size_1 = set_1.list.size();
		for (int i = 0; i < size_1; i++)
			if (set_2(set_1.list[i]) == -1)
				return false;
		return true;
	}
	friend bool operator != (const set<char>& set_1, const set<char>& set_2)
	{
		if (set_1 == set_2)
			return false;
		else
			return true;
	}

	friend istream& operator >> (istream& in, set<char>& set_)
	{
		string buffer;
		bool flag = true;

		while (flag)
		{
			int key = get_key();

			switch (key)
			{
			case 13: // enter
				if (!buffer.empty())
					flag = false;
				break;
			case 8: // backspace
				if (!buffer.empty())
				{
					buffer.pop_back();
					cout << "\b \b";
				}
				break;
			default:
				if (key < 1000)
				{
					buffer += key;
					cout << (char)key;
				}
				break;
			}
		}
		set_ = set(buffer); // set = buffer; ???
		return in;
	}
	friend ostream& operator << (ostream& out, const set<char>& set_)
	{
		int size = set_.list.size();
		if (size == 0)
			out << "()";
		else
		{
			out << "(\'";
			for (int i = 0; i < size - 1; i++)
				out << set_.list[i] << "\', \'";
			out << set_.list[set_.list.size() - 1] << "\')";
		}
		return out;
	}
};



template <typename S> void work_with_sets()
{
	system("cls");
	set<S> set_1, set_2;
	
	cout << "\n  Enter numbers to set_1: ";
	cin >> set_1;
	cout << "\n  Enter numbers to set_2: ";
	cin >> set_2;
	cout << endl;
	cout << "\n  set_1 = " << set_1;
	cout << "\n  set_2 = " << set_2;
	cout << endl;
	cout << "\n  set_1 + set_2 = " << set_1 + set_2;
	cout << "\n  set_1 * set_2 = " << set_1 * set_2;
	cout << "\n  set_1 - set_2 = " << set_1 - set_2;
	cout << "\n  set_2 - set_1 = " << set_2 - set_1;

	while (!get_key());
}

void menu_arrow(const int k)
{
	system("cls");
	switch (k % 4)
	{
	case 0:
		cout << "\n > Set of <int>\n   Set of <double>\n   Set of <char>\n   Exit";
		break;
	case 1:
		cout << "\n   Set of <int>\n > Set of <double>\n   Set of <char>\n   Exit";
		break;
	case 2:
		cout << "\n   Set of <int>\n   Set of <double>\n > Set of <char>\n   Exit";
		break;
	case 3:
		cout << "\n   Set of <int>\n   Set of <double>\n   Set of <char>\n > Exit";
		break;
	}
}
int menu()
{
	bool flag = true;
	int k = 10000;

	menu_arrow(k);
	while (flag)
	{
		int key = get_key();

		switch (key)
		{
		case 1072: // up arrow
			k--;
			menu_arrow(k);
			break;
		case 1080: // down arrow
			k++;
			menu_arrow(k);
			break;
		case 13:
			flag = false;
			break;
		}
	}
	return k % 4;

}

int main()
{
	bool flag = true;

	while (flag)
	{
		switch (menu())
		{
		case 0:
			work_with_sets<int>();
			break;
		case 1:
			work_with_sets<double>();
			break;
		case 2:
			work_with_sets<char>();
			break;
		case 3:
			flag = false;
			break;
		}
	}

	return 0;
}