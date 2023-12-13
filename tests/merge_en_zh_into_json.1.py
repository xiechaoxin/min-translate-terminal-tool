from curses.ascii import isascii
import json

all_word = "/Users/xcx/WorkSpaces/LeetCode/test/translate-app/tests/all_word_chaos.txt"
en_zh_dict = {}

# 读取现有的 JSON 文件，如果文件不存在，则创建一个空字典
try:
    with open('./dictionary.json', 'r', encoding='UTF-8') as read_file:
        en_zh_dict = json.load(read_file)
except FileNotFoundError:
    en_zh_dict = {}

print("before enzhdict", len(en_zh_dict))
new_en = 0
update_zh = 0
with open(all_word, 'r', encoding='UTF-8') as file:
    for line in file:
        split_idx = line.find('\t')
        en = line[0:split_idx]
        zh = line[split_idx + 1:len(line)].strip()

        if not str(en).isascii() or str(zh).isascii(): continue
        if en is None or zh is None: continue
        if zh  == '无': continue

        if en in en_zh_dict:
            if len(zh)>len(en_zh_dict[en]):
                # print("need update zh")
                update_zh +=1
                en_zh_dict[en] = zh
                # print(f'{en} {zh}')
                # print(f'{en} {en_zh_dict[en]}')
        else:
            en_zh_dict[en] = zh
            # print(en,">>>>",  zh)
            # print('---------------')
            new_en+=1
            
        
print(f'new_en {new_en}')
print(f'update_zh {update_zh}')
print("after enzhdict", len(en_zh_dict))

new = {}

for k, v in en_zh_dict.items():
    k = str(k).replace('"', '')
    v = str(v).replace('"', '')
    new[k] = v

# 写入更新后的字典数据到 JSON 文件
# with open('./dictionary.json', 'w', encoding='UTF-8') as out_file:
    # json.dump(new, out_file, ensure_ascii=False)
# 
