import collections
import string
from collections import Counter

s = 'IYMYSILONRFNCQXQJEDSHBUIBCJUZBOLFQYSCHATPEQGQJEJNGNXZWHHGWFSUKULJQACZKKJOAAHGKEMTAFGMKVRDOPXNEHEKZNKFSKIFRQVHHOVXINPHMRTJPYWQGJWPUUVKFPOAWPMRKKQZWLQDYAZDRMLPBJKJOBWIWPSEPVVQMBCRYVCRUZAAOUMBCHDAGDIEMSZFZHALIGKEMJJFPCIWKRMLMPINAYOFIREAOLDTHITDVRMSE'

def create_ascii_to_num():
    return {c: i for i, c in enumerate(sorted(set(string.ascii_uppercase)))}

def create_num_to_ascii():
    return {i: c for i, c in enumerate(sorted(set(string.ascii_uppercase)))}

def create_ascii_back(num_to_ascii):
    return {b: num_to_ascii[(a + 25) % 26] for b, a in create_ascii_to_num().items()}

ascii_to_num = create_ascii_to_num()
num_to_ascii = create_num_to_ascii()
ascii_back = create_ascii_back(num_to_ascii)

def calculate_ic(text):
    n = len(text)
    frequency = collections.Counter(text)
    ic = sum(f * (f - 1) for f in frequency.values()) / (n * (n - 1))
    return ic

def back_cipher(ciphertext, l):
    after = ciphertext
    for i in range(l, len(ciphertext), l):
        after = after[:i] + ''.join([ascii_back[asc] for asc in after[i:]])
    return after

def find_key_length(ciphertext, max_len=15):
    average_ics = []
    for m in range(1, max_len + 1):
        a = back_cipher(ciphertext, m)
        ics = []
        for i in range(m):
            subsequence = a[i::m]
            ic = calculate_ic(subsequence)
            ics.append(ic)
        average_ics.append((m, sum(ics) / len(ics)))
    print(average_ics)
    # 找出最可能的密钥长度
    likely_length = max(average_ics, key=lambda x: (x[1] - 0.065))[0]
    return likely_length

# 调用 find_key_length 函数并传入密文
key_length = find_key_length(s)
print("最可能的密钥长度是:", key_length)


def recover(ciphertext):
    answer=''
    for i in range(49):
        for j in range(5):
            answer=answer+chr(((ord(ciphertext[i*5+j])-ord('A'))+26-i%26)%26+ord('A'))
    #补上剩下的最后一位，直接手动添加就行
    answer=answer+chr(((ord(ciphertext[49*5+0])-ord('A'))+3)%26+ord('A'))

    return answer




ciphertext='IYMYSILONRFNCQXQJEDSHBUIBCJUZBOLFQYSCHATPEQGQJEJNGNXZWHHGWFSUKULJQACZKKJOAAHGKEMTAFGMKVRDOPXNEHEKZNKFSKIFRQVHHOVXINPHMRTJPYWQGJWPUUVKFPOAWPMRKKQZWLQDYAZDRMLPBJKJOBWIWPSEPVVQMBCRYVCRUZAAOUMBCHDAGDIEMSZFZHALIGKEMJJFPCIWKRMLMPINAYOFIREAOLDTHITDVRMSE'
print(recover(ciphertext))

#IYMYSHKNMQDLAOVNGBAPDXQEXXEPUWIFZKSLVATMHWIYIAVAEXDNPMXWVLUHIYIZXDNPMXWVAMMSRVPXDKPQWTEAMXXFVMPLRGURLYQOLWVAMMSZBMRSKPUWLRAYSHKXQVUVKFPNZVOLPIIOXTINAVWVZNIGKWEFDIVQCPILXINNIETTIPMTHKPQQDJBQRVROURVRZFMRLTMXTRVPXTTPZMRFTAVTUXQVHFVMPXKGURIYMNYHZVQWH


