import string


def vigenere_decrypt(text, key):

    lower_tab = string.ascii_lowercase  # 小写字母
    upper_tab = string.ascii_uppercase  # 大写字母
    digit_tab = string.digits

    plain_text = ''
    key_index = 0
    for char in text:
        if char.isupper():
            offset = ord(key[key_index % len(key)].upper()) - ord('A')
            plain_text += upper_tab[(upper_tab.index(char) - offset) % 26]
            key_index += 1
        elif char.islower():
            offset = ord(key[key_index % len(key)].lower()) - ord('a')
            plain_text += lower_tab[(lower_tab.index(char) - offset) % 26]
            key_index += 1
        elif char.isdigit():
            offset = ord(key[key_index % len(key)].upper()) - ord('A')
            plain_text += digit_tab[(digit_tab.index(char) - offset) % 10]
            key_index += 1
        else:
            plain_text += char

    return plain_text


if __name__ == '__main__':
    secret_key = 'PRIME'
    cipher_text = 'IYMYSHKNMQDLAOVNGBAPDXQEXXEPUWIFZKSLVATMHWIYIAVAEXDNPMXWVLUHIYIZXDNPMXWVAMMSRVPXDKPQWTEAMXXFVMPLRGURLYQOLWVAMMSZBMRSKPUWLRAYSHKXQVUVKFPNZVOLPIIOXTINAVWVZNIGKWEFDIVQCPILXINNIETTIPMTHKPQQDJBQRVROURVRZFMRLTMXTRVPXTTPZMRFTAVTUXQVHFVMPXKGURIYMNYHZVQWH'
    plain_text = vigenere_decrypt(cipher_text, secret_key)
    print(f'解密后得到的明文是{plain_text}')

#解密后得到的明文是THEMOSTFAMOUSCRYPTOLOGISTINHISTORYOWESHISFAMELESSTOWHATHEDIDTHANTOWHATHESAIDANDTOTHESENSATIONALWAYINWHICHHESAIDITANDTHISWASMOSTPERFECTLYINCHARACTERFORHERBERTOSBORNEYARDLEYWASPERHAPSTHEMOSTENGAGINGARTICULATEANDTECHNICOLOREDPERSONALITYINTHEBUSINESS