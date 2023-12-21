import json
import string

import jieba


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


def split_txt_to_word_list(line: str):
    # words = jieba.cut_for_search(line)
    words = jieba.cut(line, cut_all=True)
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


process('/Users/xcx/WorkSpaces/LeetCode/test/translate-app/dictionary.json',
        '/Users/xcx/WorkSpaces/LeetCode/test/translate-app/inverted-index.txt')
