#include<iostream>
#include <cmath>
using namespace std;


long long gcd_Extended(long long a, long long b, long long& x, long long& y);//扩展欧几里得算法
long long modPow(long long base, long long exp, long long mod);//快速幂算法，更快求出
long long calculate_Mod_Inverse(long long e, long long phi);//计算逆元 用于求d
long long modMult(long long a, long long b, long long mod);
//bool isprime(int a);//判断是否输入为素数

class Public_Key {//定义公钥类
public:
	long long n;
	long long e;

	Public_Key(long long n, long long e) : n(n), e(e) {}
};

class Private_Key {//定义私钥类
private:
	long long n;
	long long d;

public:
	Private_Key(long long p, long long q, long long e) {
		n = p * q;
		long long phi = (p - 1) * (q - 1);
		d = calculate_Mod_Inverse(e, phi);
	}

	long long decrypt(long long c) const {//使用快速幂进行求解明文
		return modPow(c, d, n);
	}

};


// 扩展欧几里得算法，求e关于phi的逆元d
long long gcd_Extended(long long a, long long b, long long& x, long long& y) {
	if (b == 0) {
		x = 1;
		y = 0;
		return a;
	}
	long long x1, y1;
	long long gcd = gcd_Extended(b, a % b, x1, y1);
	x = y1;
	y = x1 - (a / b) * y1;
	return gcd;
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


long long calculate_Mod_Inverse(long long e, long long phi) {
	long long x, y;
	gcd_Extended(e, phi, x, y);
	return (x % phi + phi) % phi;
}
/*
bool isprime(int a) {
	bool flag = true;
	if (a < 2) {
		return false;
	}
	else {
		for (int i = 2; i * i <= a; i++) {
			if (a % i == 0) {
				flag = false;
			}

		}
		return flag;
	}
}
*/

int main() {
	long long p, q, e;
	unsigned long long c;
	cin >> p >> q >> e >> c;
	/*
	if ((!isprime(p)) || (!isprime(q))) {//如果p和q有一个不是素数的话就停止计算，并报错
		cout << "p or q is not prime!!!" << endl;
		return 0;
	}
	*/

	// 创建公钥和私钥对象
	Public_Key pubKey(p * q, e);
	Private_Key privKey(p, q, e);

	// 使用私钥解密密文c，得到明文m
	unsigned long long m = privKey.decrypt(c);

	cout << m << endl;

	return 0;
}