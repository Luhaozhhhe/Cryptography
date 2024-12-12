#include <iostream>
using namespace std;

// 定义节点类表示椭圆曲线上的点
class Point {
public:
    long long x;
    long long y;
    bool isInfinity;

    Point() : x(0), y(0), isInfinity(false) {}
    Point(long long x, long long y, bool isInfinity) : x(x), y(y), isInfinity(isInfinity) {}
};

// 模乘运算函数
long long modMul(long long a, long long b, long long n) {
    long long result = 0;
    while (b != 0) {
        if (b & 1) {
            result = (result + a) % n;
        }
        a = (a * 2) % n;
        b = b / 2;
    }
    return result;
}

// 扩展欧几里得算法求乘法逆元
long long Solve_Inverse(long long a, long long p) {
    long long s, t;
    long long a0 = a;
    long long r0 = a0;
    long long r1 = p;
    long long s0 = 1;
    long long t0 = 0;
    long long s1 = 0;
    long long t1 = 1;
    long long r = r0 % r1;
    long long q = (r0 - r) / r1;
    while (r != 0) {
        s = s0 - q * s1;
        t = t0 - q * t1;
        s0 = s1;
        t0 = t1;
        s1 = s;
        t1 = t;

        r0 = r1;
        r1 = r;
        r = r0 % r1;
        q = (r0 - r) / r1;
    }

    if (s > 0) {
        return s;
    }
    else {
        return (s + p) % p;
    }
}

// 快速幂取模运算（自定义哈希相关操作）
long long Solve_Hash(long long x, long long base, long long q) {
    long long result = 1;
    while (x != 0) {
        if (x & 1) {
            result = modMul(result, base, q);
        }
        base = modMul(base, base, q);
        x = x / 2;
    }
    return result;
}

// 判断两个节点是否互为逆元（基于给定模数）
bool Is_Inverse(Point p1, Point p2, long long p) {
    return ((p1.x - p2.x) % p == 0) && ((p1.y + p2.y) % p == 0);
}

// 当两点横坐标相等时计算斜率
long long Calculate_k_for_same_x(Point point1, long long curveParamA, long long modulus) {
    long long term1 = modMul(point1.x, point1.x, modulus);
    term1 = modMul(3, term1, modulus);
    term1 = (term1 + curveParamA) % modulus;

    long long term2 = modMul(point1.y, 2, modulus);
    term2 = Solve_Inverse(term2, modulus);
    return modMul(term1, term2, modulus);
}

// 当两点横坐标不相等时计算斜率
long long Calculate_k_for_different_x(Point point1, Point point2, long long modulus) {
    long long diffY = point2.y - point1.y;
    if (diffY < 0) {
        diffY += modulus;
    }
    long long diffX = point2.x - point1.x;
    if (diffX < 0) {
        diffX += modulus;
    }
    diffX = Solve_Inverse(diffX, modulus);
    return modMul(diffY, diffX, modulus);
}

// 根据斜率计算新的点坐标
Point Calculate_Point(long long slope, Point point1, Point point2, long long modulus) {
    Point temp;
    long long x3 = modMul(slope, slope, modulus);
    x3 -= point1.x;
    if (x3 < 0) {
        x3 += modulus;
    }
    x3 -= point2.x;
    if (x3 < 0) {
        x3 += modulus;
    }
    temp.x = x3;

    long long diffX = point1.x - x3;
    if (diffX < 0) {
        diffX += modulus;
    }
    long long y3 = modMul(slope, diffX, modulus);
    y3 -= point1.y;
    if (y3 < 0) {
        y3 += modulus;
    }
    temp.y = y3;
    return temp;
}

// 椭圆曲线上两点相加操作
Point point_add(Point p1, Point p2, long long curveParamA, long long modulus) {
    if (p1.isInfinity) {
        return p2;
    }
    if (p2.isInfinity) {
        return p1;
    }
    if (Is_Inverse(p1, p2, modulus)) {
        Point temp;
        temp.x = -1;
        temp.y = -1;
        temp.isInfinity = false;
        return temp;
    }

    Point temp;
    if ((p1.x - p2.x) % modulus == 0) {
        long long slope = Calculate_k_for_same_x(p1, curveParamA, modulus);
        temp = Calculate_Point(slope, p1, p2, modulus);
    }
    else {
        long long slope = Calculate_k_for_different_x(p1, p2, modulus);
        temp = Calculate_Point(slope, p1, p2, modulus);
    }
    return temp;
}

// 椭圆曲线上一个点乘以整数k次（点的倍数相加）
Point mutiply_add(Point pp, long long k, long long curveParamA, long long modulus) {
    Point result(-1, -1, true);
    while (k) {
        if (k & 1) {
            result = point_add(result, pp, curveParamA, modulus);
        }
        pp = point_add(pp, pp, curveParamA, modulus);
        k >>= 1;
    }
    return result;
}

int main() {
    long long x0, y0;
    cin >> x0 >> y0;
    long long message, x, k;
    cin >> message >> x >> k;
    Point initialPoint(x0, y0, false);

    long long curveParamA = 1, curveParamB = 6, prime = 11;

    Point kTimesPoint = mutiply_add(initialPoint, k, curveParamA, prime);
    long long hashModulus = 13;
    long long r = kTimesPoint.x % hashModulus;
    long long kInverse = Solve_Inverse(k, hashModulus);
    long long tmp = modMul(r, message, hashModulus);
    tmp += Solve_Hash(x, 2, hashModulus);
    long long s = modMul(kInverse, tmp % hashModulus, hashModulus);
    cout << r << s << endl;

    return 0;
}