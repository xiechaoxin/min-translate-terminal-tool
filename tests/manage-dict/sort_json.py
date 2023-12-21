import json
from collections import OrderedDict


data = {}
with open('./dictionary.json', 'r', encoding='UTF-8') as in_file:
    # 将 JSON 字符串转换为字典
    data = json.load(in_file)

count = 0

for k, v in data.items():
    if str(k).find('\t') != -1 or str(v).find('\t') != -1:
        print(k)
        print(v)
        print('--------------------------------------------------------')
        count +=1
print(count)
# # 按键的英文字典序排序
# sorted_keys = sorted(data.keys())

# # 根据排序后的键重建字典
# sorted_data = {key: data[key] for key in sorted_keys}

# # 将排序后的字典转换为 JSON 字符串
# sorted_json = json.dumps(sorted_data, indent=4, ensure_ascii=False)

# 将排序后的 JSON 写入文件
# with open('./dictionary.json', 'w', encoding='UTF-8') as out_file:
    # out_file.write(sorted_json)
