import json
import re
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

def extract_chinese_phrases(text):
    # 使用正则表达式匹配所有的中文短语
    chinese_phrases = re.findall(r'[\u4e00-\u9fff]+', text)
    return chinese_phrases


'''
正则表达式
正则表达式：正则表达式是一种强大的文本处理工具，它允许我们定义一个模式（pattern），然后搜索或匹配符合该模式的字符串。

中文字符的Unicode范围：在正则表达式中，\u4e00-\u9fff 是中文字符在Unicode编码中的范围。这个范围包括了几乎所有常用的中文字符。

匹配中文字符：正则表达式 [\u4e00-\u9fff]+ 用于匹配一连串的中文字符。方括号 [] 表示匹配方括号内的任意字符，而加号 + 表示匹配一次或多次。因此，这个表达式会匹配任何长度的连续中文字符序列。
'''
def split_txt_to_word_list(line: str):

    seen = set()
    res = ''
    li = []

    extract_ch_words = extract_chinese_phrases(line)
    for word in extract_ch_words:
        if len(word) < 2:
            continue
        if word not in seen:
            seen.add(word)
            li.append(word)


    # words_split_from_jieba = jieba.cut_for_search(line)
    words_split_from_jieba = jieba.cut(line, cut_all=True)


    # 将分词结果保存到文件
    # with open("words.txt", "w", encoding="utf-8") as file:
    for word in words_split_from_jieba:
        if len(word) < 2:
            continue
        if word.isascii():
            continue
        if is_punctuation_or_space(word):
            continue
        if is_all_chinese_punctuation(word):
            continue
        if word not in seen:
            seen.add(word)
            li.append(word)
    for i in li:
        res += (i + '\t')

    return res[:-1]



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
        '/Users/xcx/WorkSpaces/translate-app/db/inverted-index.txt')
