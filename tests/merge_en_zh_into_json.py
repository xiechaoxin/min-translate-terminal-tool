import json

en_zh_dict = {}

with open('/Users/xcx/Downloads/墨墨词库上千本词书大全/enwordfeq_final.txt', 'r') as read_file:
    for line in read_file:
        first_word_end = line.find(':')
        en: str = line[0:first_word_end].strip()
        zh: str = line[first_word_end + 1:].strip()
        
        if len(zh) == 0 or len(en) == 0:
            continue
        en_zh_dict[en] = zh

# 检查字典内容
print(en_zh_dict)


with open('/Users/xcx/WorkSpaces/LeetCode/test/translate-app/tests/dictionary.json', 'w', encoding='UTF-8') as out_file:
    json.dump(en_zh_dict, out_file, ensure_ascii=False)
