#pragma once
#include <cstring>
#include <string>
#include <sstream>
#include <memory>
#include <iostream>
#include <fstream>
#include <stdexcept>

template <class T>
class Array {
public:
	const Array<T>& operator=(const Array<T>& right) {
		delete this->dat;
		this->cap = right.cap;
		this->len = right.len;
		this->dat = new T[this->cap];
		std::memset(this->dat, 0, sizeof(T) * this->cap);
		std::copy(right.dat, right.dat + this->len, dat);
		return *this;
	}
	Array() {
		dat = new T[8192];
		std::memset(this->dat, 0, sizeof(T) * 8192);
		len = 0;
		cap = 8192;
	}
	Array(const Array<T>& other) {
		dat = new T[other.cap];
		std::memset(this->dat, 0, sizeof(T) * other.cap);
		len = other.len;
		cap = other.cap;
		if (other.dat != nullptr) {
			std::copy(other.dat, other.dat + len, dat);
		}
	}

	~Array() {
		if (dat != nullptr) delete dat;
	}

	Array<T> clone() {
		auto a = Array<T>();
		a.dat = new T[cap];
		std::memset(a.dat, 0, sizeof(T) * cap);
		a.len = len;
		a.cap = cap;
		if (dat != nullptr) {
			std::copy(dat, dat + len, a.dat);
		}
		return a;
	}
	int getSize() const {
		return len;
	}

	void resize(int size) {
		if (size > getCapacity()) {
			reserve(size);
		}
		len = size;
	}
	void reserve(int size) {
		if (size < len) {
			throw std::invalid_argument("size < len!");
		}
		auto newDat = new T[size];
		std::memset(newDat, 0, size * sizeof(T));
		int my_len = std::min(len, size);
		if (dat != nullptr) {
			std::copy(dat, dat + my_len, newDat);
		}
		cap = size;
		dat = newDat;
	}
	int getCapacity() {
		return cap;
	}

	int shrink_to_fit() {
		resize(len);
	}
	void clear() {
		delete dat;
		dat = nullptr;
		cap = len = 0;
	}
	void push_back(T item) {
		if (dat == nullptr) {
			dat = new T[1];
			std::memset(dat, 0, sizeof(T));
		}
		if (cap - len < 5) {
			auto newDat = new T[(cap + 5) * 2];
			std::memset(newDat, 0, sizeof(T) * ((cap + 5) * 2));
			std::copy(dat, dat + len, newDat);
			dat = newDat;
		}
		dat[len++] = item;
	}
	T pop_back() {
		return dat[--len];
	}

	friend bool operator==(const Array<T>& a, const Array<T>& b) {
		if (a.len != b.len) return false;
		for (int i = 0; i < a.len; i++) {
			if (a[i] != b[i]) return false;
		}
		return true;
	}

	friend bool operator!=(const Array<T>& a, const Array<T>& b) {
		return !(a == b);
	}

	T& operator[](int i) const {
		if (i < 0) {
			i = getSize() + i;
		}
		if (i < 0) {
			throw std::out_of_range("neg index out of range");
		}
		if (i >= len) {
			throw std::out_of_range("index out of range");
		}
		return dat[i];
	}


	friend std::istream& operator>>(std::istream& is, Array<T>& a) {
		std::string line;
		if (!getline(is, line)) {
			return is;
		}
		std::istringstream ss(line);
		int tmp_l = a.len;
		while (ss >> a.dat[a.len]) {
			a.len++;
		}
		if (ss.fail() && !ss.eof()) {
			a.len = tmp_l;
			throw std::bad_cast();
		}
		return is;
	}
	friend std::ostream& operator<<(std::ostream& os, const Array<T>& a) {

		for (int i = 0; i < a.len; i++) {
			os << a[i] << " \n"[i == a.len - 1];
		}
		return os;
	}
private:
	T* dat;
	int len;
	int cap;
};



class BigNUM {
	friend std::ostream& operator<<(std::ostream& os, BigNUM b);
	friend std::istream& operator>>(std::istream& is, BigNUM& b);
public:
	static inline void Swap(BigNUM& a, BigNUM& b) {
		std::swap(a, b);
	}
	inline bool operator<(BigNUM right) {
		strip();
		right.strip();
		if (arr.getSize() < right.arr.getSize()) return true;
		if (arr.getSize() == right.arr.getSize()) {
			for (int i = arr.getSize() - 1; i >= 0; i--) {
				if (arr[i] < right.arr[i]) return true;
				if (arr[i] > right.arr[i]) return false;

			}
		}
		return false;
	}
	inline bool operator==(BigNUM right) {
		strip();
		right.strip();
		if (arr.getSize() != right.arr.getSize()) {
			return false;
		}
		for (int i = 0; i < arr.getSize(); i++) {
			if (arr[i] != right.arr[i]) return false;
		}
		return true;
	}
	bool operator>(BigNUM right) {
		strip();
		right.strip();
		if (arr.getSize() > right.arr.getSize()) return true;
		if (arr.getSize() == right.arr.getSize()) {
			for (int i = arr.getSize() - 1; i >= 0; i--) {
				if (arr[i] > right.arr[i]) return true;
				if (arr[i] < right.arr[i]) return false;
			}
		}
		return false;
	}
	BigNUM& operator=(const BigNUM right) {
		if (&right == this) return *this;
		arr = Array<unsigned int>();
		for (int i = 0; i < right.arr.getSize(); i++) {
			arr.push_back(right.arr[i]);
		}
		return *this;
	}
	BigNUM();
	BigNUM(unsigned int u);
	BigNUM(std::string path);
	void init_from_arr(Array<unsigned int> arr);
	void init_from_str(std::string line);
	void strip();
	BigNUM add(BigNUM& b);
	BigNUM sub(BigNUM& b);
	BigNUM mul(BigNUM& b);
	BigNUM div(BigNUM& b);
	BigNUM mod(BigNUM& b);


	inline BigNUM operator+(BigNUM b) {
		return add(b);
	}


	inline BigNUM operator-(BigNUM b) {
		return sub(b);
	}


	inline BigNUM operator*(BigNUM b) {
		return mul(b);
	}


	friend BigNUM operator/(BigNUM a, BigNUM b) {
		BigNUM ret = a.div(b);
		return ret;
	}


	inline BigNUM operator%(BigNUM b) {
		return mod(b);
	}
	static BigNUM GCD(BigNUM a, BigNUM b) {
		const BigNUM ZERO(0);
		while (!(a == ZERO || b == ZERO)) {
			if (b > a) {
				std::swap(a, b);
			}
			if (b == ZERO) return a;
			a = a % b;
		}
		if (b > a) {
			std::swap(a, b);
		}
		return a;
	}
	static BigNUM LCM(BigNUM m, BigNUM n) {
		return m * n / GCD(m, n);
	}

	inline int getSize() {
		return arr.getSize();
	}
	Array<unsigned int> arr;

private:
	bool push_digit(unsigned int n);

};
