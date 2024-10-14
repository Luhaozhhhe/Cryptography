import vigenerecipher
def keyword(Ciphertext, keylength):
    ListCiphertext = list(Ciphertext)
    # 标准数据来源于课本
    Standard = {'A': 0.082, 'B': 0.015, 'C': 0.028, 'D': 0.043, 'E': 0.127, 'F': 0.022, 'G': 0.020, 'H': 0.061,
                'I': 0.070, 'J': 0.002, 'K': 0.008, 'L': 0.040, 'M': 0.024, 'N': 0.067, 'O': 0.075, 'P': 0.019,
                'Q': 0.001, 'R': 0.060, 'S': 0.063, 'T': 0.091, 'U': 0.028, 'V': 0.010, 'W': 0.023, 'X': 0.001,
                'Y': 0.020, 'Z': 0.001}

    while True:
        KeyResult = []

        for i in range(keylength):
            # 使用切片分组
            PresentCipherList = ListCiphertext[i::keylength]

            # 初始化重合指数最大值为０，检验移动位数对应字符以＊代替
            QuCoincidenceMax = 0
            KeyLetter = "*"

            # 遍历移动的位数
            # m是密钥对应的英文字母
            for m in range(26):
                # 初始化当前移动位数的重合互指数为０
                QuCoincidencePresent = 0

                # 遍历计算重合指数：各个字符的频率＊对应英文字符出现的标准频率－－－的和
                for Letter in set(PresentCipherList):
                    # fi/n
                    LetterFrequency = PresentCipherList.count(Letter) / len(PresentCipherList)

                    # 标准频率
                    # ord(Letter) - 65是将letter对应的字母化为26内的数值，然后与m运算，得到的k是对应的明文字母
                    k = chr((ord(Letter) - 65 - m) % 26 + 65)
                    StandardFrequency = Standard[k]

                    # 计算重合互指数，累加遍历26个英文字母
                    QuCoincidencePresent = QuCoincidencePresent + LetterFrequency * StandardFrequency

                # 保存遍历过程中重合指数的最大值，同时保存对应应对的位数，即对应key的字符
                if QuCoincidencePresent > QuCoincidenceMax:
                    QuCoincidenceMax = QuCoincidencePresent
                    # m是26个英文对应的位置，从0开始，+65是因为A在ascii中是65
                    KeyLetter = chr(m + 65)
            print("第", i + 1, "个密钥字母为:", KeyLetter, "对应的重合互指数为:", QuCoincidenceMax)
            # 保存当前位置key的值，退出循环，进行下一组子密文移动位数的尝试
            KeyResult.append(KeyLetter)
        # 列表转为字符串
        Key = "".join(KeyResult)
        break
    return Key


if __name__ == '__main__':

    Ciphertext = input("输入密文：").upper()
    KeyResult = keyword(Ciphertext, 5)

    print("密钥最可能为：", KeyResult, "\n")



