from curses.ascii import isascii
import json

# all_word = "/Users/xcx/WorkSpaces/LeetCode/test/translate-app/tests/all_word_chaos.txt"
en_zh_dict = {}
# 读取现有的 JSON 文件，如果文件不存在，则创建一个空字典
try:
    with open('./dictionary.json', 'r', encoding='UTF-8') as read_file:
        en_zh_dict = json.load(read_file)
except FileNotFoundError:
    en_zh_dict = {}

count = 0
new_en_zh_dict = {}
 
for key, value in en_zh_dict.items():
    if value != "找不到解释":
        # 将不满足条件的元素添加到新字典中
        new_en_zh_dict[key] = value
        
print("count", count) #39084 



# 写入更新后的字典数据到 JSON 文件
with open('./dictionary.json', 'w', encoding='UTF-8') as out_file:
    json.dump(new_en_zh_dict, out_file, ensure_ascii=False)
