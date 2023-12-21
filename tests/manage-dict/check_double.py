from curses.ascii import isascii
import json

sys_word = "/Users/xcx/WorkSpaces/LeetCode/test/translate-app/tests/system-word.txt"
en_zh_dict = {}
# 读取现有的 JSON 文件，如果文件不存在，则创建一个空字典
try:
    with open('./dictionary.json', 'r', encoding='UTF-8') as read_file:
        en_zh_dict = json.load(read_file)
except FileNotFoundError:
    en_zh_dict = {}

lineinfo = []
newlineinfo = []

with open(sys_word, 'r', encoding='UTF-8') as file:
    for line in file:
        lineinfo.append(line.strip())

# print(lineinfo)
for item in lineinfo:
    if item not in en_zh_dict:
        newlineinfo.append(item)



with open(sys_word, 'w') as read_file:
    for i in newlineinfo:
        read_file.write(f'{i}\n')
# 写入更新后的字典数据到 JSON 文件
# with open('./dictionary.json', 'w', encoding='UTF-8') as out_file:
#     json.dump(new_en_zh_dict, out_file, ensure_ascii=False)
