#include <vector>
#include <set>
#include <ostream>

template<typename T>
std::ostream & operator<<(std::ostream &out, std::vector<T> &vec)
{
	out << "[";
	bool first = true;
	for (const auto &elem: vec) {
		if (first) {
			out << elem;
			first = false;
		} else {
			out << ", " << elem;
		}
	}
	out << "]";
	return out;
}

template<typename T>
std::ostream & operator<<(std::ostream &out, std::set<T> &vec)
{
	out << "[";
	bool first = true;
	for (const auto &elem: vec) {
		if (first) {
			out << elem;
			first = false;
		} else {
			out << ", " << elem;
		}
	}
	out << "]";
	return out;
}

