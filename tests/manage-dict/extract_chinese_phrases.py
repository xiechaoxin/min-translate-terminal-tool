import re


def remove_english_punctuation_space(text):
    # 使用正则表达式移除英文字符、标点和空格
    cleaned_text = re.sub(r'[A-Za-z]', '', text)  # 移除英文字母
    cleaned_text = re.sub(r'[^\w]', '', cleaned_text)  # 移除标点符号和空格
    return cleaned_text


def split_by_english_and_punctuation(text):
    # 使用正则表达式分割字符串
    # \w 匹配任何字母数字字符，\u4e00-\u9fff 是中文字符范围
    # [^\w\u4e00-\u9fff]+ 表示匹配任何非字母数字和非中文字符的序列
    split_list = re.split(r'[^\w\u4e00-\u9fff]+', text)
    return [s for s in split_list if s]  # 移除空字符串


def split_by_english_letters_and_punctuation(text):
    # 使用正则表达式分割字符串，其中英文字母和中英文标点作为分隔符
    split_list = re.split(r'[A-Za-z\u3000-\u303f\uff00-\uffef]', text)
    return [s for s in split_list if s]  # 移除空字符串


'''
正则表达式
正则表达式：正则表达式是一种强大的文本处理工具，它允许我们定义一个模式（pattern），然后搜索或匹配符合该模式的字符串。

中文字符的Unicode范围：在正则表达式中，\u4e00-\u9fff 是中文字符在Unicode编码中的范围。这个范围包括了几乎所有常用的中文字符。

匹配中文字符：正则表达式 [\u4e00-\u9fff]+ 用于匹配一连串的中文字符。方括号 [] 表示匹配方括号内的任意字符，而加号 + 表示匹配一次或多次。因此，这个表达式会匹配任何长度的连续中文字符序列。
'''


def extract_chinese_phrases(text):
    # 使用正则表达式匹配所有的中文短语
    chinese_phrases = re.findall(r'[\u4e00-\u9fff]+', text)
    return chinese_phrases


# 示例使用
# text = 'adj. 净化的；精制的v. 使纯净；去除（purify的过去分词）；精炼'
text = '''n. 【植】铁树 (Cordyline terminalis)abbr. (=titanium)【化】short. it is
而且 就那样，就这样；在那时候；而且还是
abbr. a compte （法语）赊欠买卖; 记账; anne couyante 本年度; ante cibos (Latin=before meals) （拉丁语）饭前
	A股；A股股票；甲类股份
圣诞颂歌；小气财神；圣诞欢歌
第四(fourth)
第四代移动通信；四代技术；四核
第三(third)
abbr. 三维的（three dimensional）
'''
result = extract_chinese_phrases(text)

print(result)
