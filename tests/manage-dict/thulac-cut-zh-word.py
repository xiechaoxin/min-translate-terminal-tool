# import thulac
#
# line = "最好是；其后加不带；更好的"
#
# thu = thulac.thulac()
# words = thu.cut(line, text=True)
#
#
# # 分割文本
# words = words.split(";_w ")
#
# # 清洗和提取数据
# vocab_list = []
# for word in words:
#     parts = word.split()
#     # 过滤出含有中文或英文词汇的部分
#     meaningful_parts = [part for part in parts if "_x" in part or "_n" in part or "_v" in part or "_j" in part or "_d" in part]
#     vocab_list.append(" ".join(meaningful_parts))
#
# # 打印词汇表
# for item in vocab_list:
#     print(item)


#
# import thulac
#
# # 初始化 THULAC
# thu = thulac.thulac()
#
# # 待分词的文本
# # line = "我爱北京天安门"
# line = "最好是；其后加不带；更好的"
#
# # 进行分词
# words = thu.cut(line, text=True)
#
# # 输出分词结果
# print(words)
#


# word = '第三(third)'



import json
import string

import thulac


def is_all_chinese_punctuation(word):
    # 定义一个具体的中文标点符号集
    chinese_punctuation = "。，、；：？！“”‘’（）【】—…《》。？！，、；：‘’“”（）【】《》—…－·"
    all_punctuation = string.punctuation + chinese_punctuation

    # 确保字符串不为空
    if not word:
        return False

    # 检查每个字符是否为中文标点
    for char in word:
        if char not in all_punctuation:
            return False
    return True


def is_punctuation_or_space(word):
    return all(char in string.punctuation + ' ' for char in word)


def remove_punctuation(word):
    return ''.join(char for char in word if char not in string.punctuation)


def lower_word(word):
    return word.lower()


thu = thulac.thulac()
def split_txt_to_word_list(line: str):
    # words = jieba.cut_for_search(line)
    # words = jieba.cut(line, cut_all=True)
    global thu

    words = thu.cut(line, text=True)
    print(words)

    li = set()
    item = ''

    # 将分词结果保存到文件
    # with open("words.txt", "w", encoding="utf-8") as file:
    for word in words:
        if len(word) < 2:
            continue
        if word.isascii():
            continue
        if is_punctuation_or_space(word):
            continue
        if is_all_chinese_punctuation(word):
            continue
        li.add(word)
    for i in li:
        item += i + '\t'

    # print(item)
    return item[:-1]


def process(file_path: str, out_file_path: str):
    ans = []

    with open(file_path, 'r', encoding='utf-8') as f:
        dic = json.load(f)
        for k, v in dic.items():
            zh_word_split = split_txt_to_word_list(v)
            line = lower_word(remove_punctuation(k)) + '\t' + zh_word_split
            ans.append(line)
    with open(out_file_path, 'w', encoding='utf-8') as outfile:
        outfile.write('\n'.join(ans))


process('/Users/xcx/WorkSpaces/translate-app/db/dictionary.json',
        '/Users/xcx/WorkSpaces/translate-app/db/inv.txt')
