#include<iostream>
#include<string>
using namespace std;
int pai_s[16] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };//定义s盒
int pai_p[16] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };//定义p盒

int x[16];//定义明文，16位
int y[16];//定义密文，16位
int k[32];//定义密钥，32位
const int l = 4;
const int m = 4;
const int Nr = 4;

//定义题目中所使用的函数：加密，解密以及SPN

void s_change(int *a,int *b,int *c) {//s盒变化
	int t[4] = {};
	for (int i = 0; i < 4; i++) {
		t[i] = a[4 * i] * 8 + a[4 * i + 1] * 4 + a[4 * i + 2] * 2 + a[4 * i + 3];
		int temp = c[t[i]];
		int j = 4 * i + 3;
		while (temp > 0) {
			b[j--] = temp % 2;
			temp = temp / 2;
		}
		while (j >= i * 4) {
			b[j--] = 0;
		}
		
	}

}

void p_change(int *a,int *b,int *c) {//p盒变化，很简单，就是1-16的一个置换
	for (int i = 0; i < l * m; i++) {
		b[c[i]] = a[i];
	}

}

void SPN(int *x,int *y,int *s,int *p,int *key) {//SPN置换
	int temp1[16], temp2[16], temp3[16];

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 16; j++){
			temp3[j] = key[4 * i + j];

		}

		for (int j = 0; j < 16; j++){
			temp1[j] = x[j] ^ temp3[j];

		}

		s_change(temp1, temp2, s);
		p_change(temp2, x, p);
	}


	for (int j = 0; j < 16; j++){
		temp3[j] = key[4 * 3 + j];

	}

	for (int j = 0; j < 16; j++){
		temp1[j] = x[j] ^ temp3[j];

	}

	s_change(temp1, temp2, s);

	for (int j = 0; j < 16; j++){
		temp3[j] = key[4 * 4 + j];

	}

	for (int j = 0; j < 16; j++){
		y[j] = temp2[j] ^ temp3[j];

	}


}


int main() {
	string plaintext, key;//定义明文和密钥
	cin >> plaintext;//输入明文
	cin >> key;//输入密钥

	//将输入的字符串转化为数字0和1
	for (int i = 0; i < plaintext.length(); i++) {
		if (plaintext[i] == '0') {
			x[i] = 0;
		}
		else if (plaintext[i] == '1') {
			x[i] = 1;
		}
		else {
			cout << "The input is wrong!" << endl;
		}
	}

	//同理，对密钥key也进行同样的操作
	for (int i = 0; i < key.length(); i++) {
		if (key[i] == '1') {
			k[i] = 1;
		}
		else if (key[i] == '0') {
			k[i] = 0;
		}
		else {
			cout << "The input is wrong!" << endl;
		}
	}
	//直接调用函数，做SPN变化
	SPN(x, y, pai_s, pai_p, k);
	
	//输出加密后的密文
	for (int i = 0; i < l * m; i++) {
		cout << y[i];
	}

	return 0;
}