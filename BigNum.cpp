#include "BigNum.h"
#include <stack>
#include <iostream>
#include <algorithm>
#include <cctype>

bool BigNUM::push_digit(unsigned int n) {
	if (n > 9) {
		throw std::invalid_argument("digit must be 0 ~ 9");
	}
	arr.push_back(n);
	return true;
}

void BigNUM::init_from_arr(Array<unsigned int> n) {
	for (int i = n.getSize() - 1; i >= 0; i--) {
		int t = n[i];
		while (t) {
			push_digit(t % 10);
			t /= 10;
		}
	}
	strip();
}

void BigNUM::init_from_str(const std::string str) {
	for (int i = str.length() - 1; i >= 0; i--) {
		if (!isdigit(str[i])) continue;
		push_digit((unsigned int)(str[i] - '0'));
	}
	strip();
}

BigNUM::BigNUM(unsigned int u) {
	Array<unsigned int> arr;
	arr.push_back(u);
	init_from_arr(arr);
}
BigNUM::BigNUM() {}
BigNUM::BigNUM(std::string path) {
	std::string line;
	std::ifstream ifs(path, std::ifstream::in);
	ifs >> line;
	init_from_str(line);
	strip();
}

void BigNUM::strip() {
	int l = arr.getSize();
	for (int i = arr.getSize() - 1; i > 0; i--) {
		if (arr[i] != 0) break;
		l--;
	}
	if (l < 1) l = 1;
	arr.resize(l);
}

std::ostream& operator<<(std::ostream& os, BigNUM b) {
	b.strip();
	for (int i = b.arr.getSize() - 1; i >= 0; i--) {
		os << b.arr[i];
		int lef = b.arr.getSize() % 3;
		if ((b.arr.getSize() - i) % 3 == lef && i != 0) {
			os << " ";
		}
	}
	return os;
}

std::istream& operator>>(std::istream& is, BigNUM& b) {
	Array<unsigned int> arr;
	is >> arr;
	b.init_from_arr(arr);
	return is;
}

BigNUM BigNUM::add(BigNUM& bigB) {
	const int LEN = std::max(getSize(), bigB.getSize());
	auto bigC = BigNUM();
	arr.resize(LEN + 50);
	bigB.arr.resize(LEN + 50);
	bigC.arr.resize(LEN + 50);
	Array<unsigned int>& a = arr;
	Array<unsigned int>& b = bigB.arr;
	Array<unsigned int>& c = bigC.arr;
	for (int i = 0; i < c.getSize(); i++) {
		c[i] = 0;
	}
	for (int i = 0; i < LEN; i++)
		c[i] = a[i] + b[i];

	for (int i = 0; i < LEN - 1; i++) {
		if (c[i] >= 10) {
			c[i + 1] += c[i] / 10;
			c[i] = c[i] % 10;
		}
	}
	bigC.strip();
	return bigC;
}
BigNUM BigNUM::mul(BigNUM& bigB) {
	const int LEN = std::max(getSize(), bigB.getSize());
	auto bigC = BigNUM();
	arr.resize(LEN * 2 + 50);
	bigB.arr.resize(LEN * 2 + 50);
	bigC.arr.resize(LEN * 2 + 50);
	auto& a = arr;
	auto& b = bigB.arr;
	auto& c = bigC.arr;
	int i, j;

	for (i = 0; i < LEN; i++) {
		for (j = 0; j < LEN; j++) {
			c[i + j] += a[j] * b[i];
			if (c[i + j] >= 10) {
				c[i + j + 1] += c[i + j] / 10;
				c[i + j] = c[i + j] % 10;
			}
		}
	}
	strip();
	bigB.strip();
	bigC.strip();
	return bigC;
}
BigNUM BigNUM::sub(BigNUM& bigB) {
	const int LEN = std::max(getSize(), bigB.getSize());
	auto bigC = BigNUM();
	arr.resize(LEN + 50);
	bigB.arr.resize(LEN + 50);
	bigC.arr.resize(LEN + 50);
	Array<unsigned int>& a = arr;
	Array<unsigned int>& b = bigB.arr;
	Array<unsigned int>& c = bigC.arr;

	int i, borrow = 0;
	for (i = 0; i < LEN; i++) {
		c[i] = a[i] - b[i] - borrow;
		if (c[i] < 10) {
			borrow = 0;
		}
		else {
			c[i] = c[i] + 10;
			borrow = 1;
		}
	}
	strip();
	bigB.strip();
	bigC.strip();
	return bigC;
}
static inline BigNUM getMid(BigNUM& bot, BigNUM& top) {
	int len = std::max(bot.getSize(), top.getSize());
	top.arr.resize(len);
	bot.arr.resize(len);
	Array<unsigned int> arr;
	
	int lef = 0;
	BigNUM diff = top - bot;
	for (int i = -1; i > -diff.getSize(); i--) {
		diff.arr[i] += lef * 10;
		lef = diff.arr[i] % 2;
		diff.arr[i] /= 2;
	}
	diff = diff + bot;
	diff.strip();
	top.strip();
	bot.strip();
	return diff;
}
BigNUM BigNUM::div(BigNUM& bigB) {
	const int LEN = std::max(getSize(), bigB.getSize());
	BigNUM target = *this;
	BigNUM top = target;
	BigNUM bot(0);
	bot.strip();
	BigNUM mid = getMid(bot, top);
	mid.strip();
	BigNUM calc = mid * bigB;
	BigNUM diff = calc - mid;
	calc.strip();
	BigNUM one(1);
	while (diff > bigB || calc > target) {
		bot.strip();
		top.strip();
		mid = getMid(bot, top);

		calc = mid * bigB;

		if (calc > target) {
			diff = calc.sub(target);

			top = mid.sub(one);
		}
		else if (calc < target) {
			diff = target.sub(calc);
			bot = mid.add(one);
		}
		else {
			break;
		}
		bot.strip();

	}
	mid.strip();
	return mid;
}

BigNUM BigNUM::mod(BigNUM& bigB) {
	BigNUM ans = div(bigB);
	ans = ans.mul(bigB);
	ans = sub(ans);
	return ans;
}

