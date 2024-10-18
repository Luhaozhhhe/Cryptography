import random
import subprocess

counts=8000

# 生成了16位的二进制数
def generate_random_binary_string(length=16, times=1):
    binary_strings = []  # 创建一个空列表来存储生成的二进制字符串
    for _ in range(times):  # 循环指定的次数
        binary_strings.append(''.join(random.choice('01') for _ in range(length)))
    return binary_strings

# 使用函数生成n个长度为16的随机二进制字符串
binary_strings_list = generate_random_binary_string(16, times=counts)

# 规定生成密钥的可执行文件路径
exe_file_path = "./SPN.exe"

def SPN_Process(times=1):
    processing_strings = []
    for i in range(times):
        temp_data = subprocess.run([exe_file_path], input=binary_strings_list[i].encode(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        processing_strings.append(temp_data.stdout.decode().strip())
    return processing_strings

# 确保传递给SPN_Process的times参数与binary_strings_list中的元素数量相匹配
processing_strings_list = SPN_Process(len(binary_strings_list))

# 打印列表的内容
for original, encrypted in zip(binary_strings_list, processing_strings_list):
    print(original)
    print(encrypted)

#将8000对明文密文写入pairs.txt中
with open("./data/pairs.txt", "w") as file:
    for original,encrypted in zip(binary_strings_list,processing_strings_list):
        file.write(original + "\n")
        file.write(encrypted + "\n")

print("Successfully generate the original number and the encrypted number!")

