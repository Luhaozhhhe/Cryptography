pai_s=[14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7]
l = 4
m = 4
Nr = 4

#define some functions
def solve_reverse_s_box(s_box):
    reverse_box=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    for i in range(len(s_box)):
        reverse_box[s_box[i]]=i
    return reverse_box


#init the s box
reverse_pai_s=solve_reverse_s_box(pai_s)
pairs_num=8000  #用于攻击的随机生成密钥对数量

count_list = [[0 for _ in range(16)] for _ in range(16)] #初始化count数组

original=[0 for _ in range(8000)]
encrypted=[0 for _ in range(8000)]

# 打开文件并读取内容
with open('data/pairs.txt', 'r') as file:
    lines = file.readlines()  # 读取所有行

# 将两行一对分组
for i in range(0, len(lines), 2):
    original[i//2]=lines[i]
    encrypted[i//2]=lines[i+1]


def Linear_Attack_First(original,encrypted): #攻击课本中的八位密钥
    for i in range(8000):
        original_temp=original[i]
        encrypted_temp=encrypted[i]
        original_temp = original_temp.replace('\n', '').replace('\r', '')
        encrypted_temp = encrypted_temp.replace('\n', '').replace('\r', '')
        encrypted_test2=int(encrypted_temp[4:8],2)
        encrypted_test4=int(encrypted_temp[12:],2)
        x=[None]+[int(i) for i in original_temp]
        for i in range(16):
            for j in range(16):
                u2 = [int(i) for i in format(reverse_pai_s[i ^ encrypted_test2], '04b')]
                u4 = [int(i) for i in format(reverse_pai_s[j ^ encrypted_test4], '04b')] 
                u = [None] * 5 + u2 + [None] * 4 + u4
                z = x[5] ^ x[7] ^ x[8] ^ u[6] ^ u[8] ^ u[14] ^ u[16]
                if z&1==0:
                    count_list[i][j]=count_list[i][j]+1;

    for i in range(16):
        for j in range(16):
            count_list[i][j]=count_list[i][j]-4000
            if count_list[i][j]<0:
                count_list[i][j]=-count_list[i][j]   
        
    temp1=0
    temp2=0
    count_max=0
    for i in range(16):
        for j in range(16):
            if count_list[i][j]>count_max:
                count_max=count_list[i][j]
                temp1=i
                temp2=j

    return [temp1,temp2]


count_list_first = [[0 for _ in range(16)] for _ in range(16)] 
count_list_second = [[0 for _ in range(16)] for _ in range(16)] 
count_list_all=[[0 for _ in range(16)] for _ in range(16)] 

def Linear_Attack_Second(original,encrypted):
    result1=Linear_Attack_First(original,encrypted)[0]
    result2=Linear_Attack_First(original,encrypted)[1]
    for i in range(8000):
        original_temp=original[i]
        encrypted_temp=encrypted[i]
        original_temp = original_temp.replace('\n', '').replace('\r', '')
        encrypted_temp = encrypted_temp.replace('\n', '').replace('\r', '')
        encrypted_test1=int(encrypted_temp[:4],2)
        encrypted_test2=int(encrypted_temp[4:8],2)
        encrypted_test3=int(encrypted_temp[8:12],2)
        encrypted_test4=int(encrypted_temp[12:],2)
        x=[None]+[int(i) for i in original_temp]

        for i in range(16):
            for j in range(16):
                u = [None] + [int(i) for i in format(reverse_pai_s[i ^ encrypted_test1], '04b') + \
                                              format(reverse_pai_s[result1 ^ encrypted_test2], '04b') + \
                                              format(reverse_pai_s[j ^ encrypted_test3], '04b') + \
                                              format(reverse_pai_s[result2 ^ encrypted_test4], '04b')]
                z=x[1] ^ x[2] ^ x[4] ^ u[1] ^ u[5] ^ u[9] ^ u[13]
                if z&1==0:
                    count_list_first[i][j]=count_list_first[i][j]+1
                z = x[9] ^ x[10] ^ x[12] ^ u[3] ^ u[7] ^ u[11] ^ u[15]
                if z&1==0:
                    count_list_second[i][j]=count_list_second[i][j]+1
                
    for i in range(16):
        for j in range(16):
            count_list_first[i][j]=count_list_first[i][j]-4000
            if count_list_first[i][j]<0:
                count_list_first[i][j]=-count_list_first[i][j] 
    
    for i in range(16):
        for j in range(16):
            count_list_second[i][j]=count_list_second[i][j]-4000
            if count_list_second[i][j]<0:
                count_list_second[i][j]=-count_list_second[i][j] 


    for i in range(16):
        for j in range(16):
            count_list_all[i][j]=count_list_first[i][j]+count_list_second[i][j]
                
    temp3=0
    temp4=0
    count_max=0
    for i in range(16):
        for j in range(16):
            if count_list_all[i][j]>count_max:
                count_max=count_list_all[i][j]
                temp3=i
                temp4=j
    
    return [temp3,temp4]



result_list1=Linear_Attack_First(original,encrypted)
result_list2=Linear_Attack_Second(original,encrypted)


def Dec_To_Bin(a):
    match a:
        case 0:
            return "0000"
        case 1:
            return "0001"
        case 2:
            return "0010"
        case 3:
            return "0011"
        case 4:
            return "0100"
        case 5:
            return "0101"
        case 6:
            return "0110"
        case 7:
            return "0111"
        case 8:
            return "1000"
        case 9:
            return "1001"
        case 10:
            return "1010"
        case 11:
            return "1011"
        case 12:
            return "1100"
        case 13:
            return "1101"
        case 14:
            return "1110"
        case 15:
            return "1111" 

result1=Dec_To_Bin(result_list1[0])
result2=Dec_To_Bin(result_list1[1])
result3=Dec_To_Bin(result_list2[0])
result4=Dec_To_Bin(result_list2[1])



final_result=result3+result1+result4+result2

print(final_result)


