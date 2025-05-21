#ifndef DEFAULTCOMPARATOR_HPP
#define DEFAULTCOMPARATOR_HPP

template<typename T>
struct DefaultComparator {
	bool operator()(const T& a, const T& b) const {
		return a < b;
	}
};

#endif // DEFAULTCOMPARATOR_HPP
