#include<iostream>
using namespace std;

// 模乘法，避免大数溢出
long long modMult(long long a, long long b, long long mod) {
	long long result = 0;
	a %= mod;
	while (b > 0) {
		if (b % 2 == 1) {
			result = (result + a) % mod;
		}
		a = (a * 2) % mod;
		b /= 2;
	}
	return result;
}

// 快速幂算法，避免大数溢出
long long modPow(long long base, long long exp, long long mod) {
	long long result = 1;
	base %= mod;

	while (exp > 0) {
		if (exp % 2 == 1) {
			result = modMult(result, base, mod); // 使用模乘法，避免溢出
		}
		base = modMult(base, base, mod); // 使用模乘法，避免溢出
		exp /= 2;
	}
	return result;
}

//Miller_Rabin素性检测实现
bool Miller_Rabin(long long n,int times) {
	if (n <= 1) {
		return false;
	}
	if (n == 2) {
		return true;
	}
	if (n % 2 == 0) {
		return false;
	}
	//计算n-1对2取阶数
	int fac_num = 0;
	long long temp = n - 1;
	while (temp % 2 == 0) {
		temp = temp / 2;
		fac_num++;
	}

	//开始验证，times为我们的计算轮次
	for (int i = 0; i < times; i++) {
		long long a = rand() % (n - 1) + 1;//随机取一个范围内的整数
		long long b = modPow(a, temp, n);//计算对应的模幂结果
		if (b == 1 || b == n - 1) {
			continue;
		}
		bool is_composite = true;
		for (int j = 0; j < fac_num - 1; j++) {
			b = modMult(b, b, n);  // b = b^2 % n，防止溢出
			if (b == n - 1) {
				is_composite = false;  // 如果 b == n-1，则 n 是素数
				break;
			}
			if (b == 1) {
				return false;  // 如果 b == 1，则 n 不是素数
			}
		}
		return !is_composite;
	}
	return true;
	
}

int main() {
	long long n;
	cin >> n;
	int times = 100;
	if (Miller_Rabin(n, times) == true) {
		cout << "Yes" << endl;
	}
	else {
		cout << "No" << endl;
	}
	return 0;

}
