import csv
from curses.ascii import isascii
import os
import openpyxl
import json


import os

def match_files_recursion(folder_path, file_extension):
    match_files_list = []
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            if file.endswith(file_extension):
                match_files_list.append(os.path.join(root, file))
    return match_files_list


def read_org_src_dict()->dict:
    en_zh_dict = {}

    # 读取现有的 JSON 文件，如果文件不存在，则创建一个空字典
    try:
        with open('./dictionary.json', 'r', encoding='UTF-8') as read_file:
            en_zh_dict = json.load(read_file)
    except FileNotFoundError:
        en_zh_dict = {}

    return en_zh_dict

def get_db_from_csv(file_path)->dict:
    # 创建一个空字典来存储数据
    data_dict = {}

    # 打开CSV文件并读取
    with open(file_path, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) >= 2:  # 确保行中至少有两个元素
                key = row[0].replace('\n', '')   # 第一列的值作为键
                value = row[1].replace('\n', '') # 第二列的值作为值
                data_dict[key] = value
    return data_dict

def get_db_from_excel(file_path)->dict:
    # 定义一个空字典来存储A和B列的数据
    new_data_dict = {}

    # 打开Excel文件
    workbook = openpyxl.load_workbook(file_path)
    # 选择第一个工作表
    sheet = workbook.active

    # 从A列和B列读取数据并存储到字典中
    for row in sheet.iter_rows(min_row=2, values_only=True):
        if len(row) >= 2:
            key = str(row[0]).replace('\n', '')
            value = str(row[1]).replace('\n', '')
            new_data_dict[key] = value
    return new_data_dict


def fillter(src_dict:dict, new_dict: dict) -> dict:
    dict_fillter = src_dict.copy()
    
    for en, zh in new_dict.items():
        if not str(en).isascii() or str(zh).isascii(): continue
        if en is None or zh is None: continue
        if zh  == '无': continue
        if zh  == '找不到解释': continue

        if en in src_dict:
            if len(zh)>len(src_dict[en]):
                # print("need update zh")
                global update_zh
                update_zh += 1
                dict_fillter[en] = zh
                # print(f'{en} {zh}')
                # print(f'{en} {en_zh_dict[en]}')
        else:
            dict_fillter[en] = zh
            # print(en,">>>>",  zh)
            # print('---------------')
            global new_en
            new_en+=1    
            
    return dict_fillter
            
new_en = 0
update_zh = 0

# folder_path = '/Users/xcx/Downloads/墨墨词库上千本词书大全/打包下载/精装 2/中文释义'
folder_path = '/Users/xcx/Downloads/墨墨词库上千本词书大全/打包下载/更多/csv-220301'
# file_extension = '.xlsx'
file_extension = '.csv'
xlsx_path_list=match_files_recursion(folder_path, file_extension)

en_zh_dict = read_org_src_dict()
print("before enzhdict", len(en_zh_dict))
for item in xlsx_path_list:
    # en_zh_dict_new = get_db_from_excel(item)
    en_zh_dict_new = get_db_from_csv(item)
    en_zh_dict = fillter(en_zh_dict, en_zh_dict_new)

    print(f'new_en {new_en}')
    print(f'update_zh {update_zh}')
    print("after enzhdict", len(en_zh_dict))
    print('================')

new = {}

print("-----开始写json")
for k, v in en_zh_dict.items():
    k = str(k).replace('"', '')
    v = str(v).replace('"', '')
    new[k] = v

# 写入更新后的字典数据到 JSON 文件
with open('./dictionary.json', 'w', encoding='UTF-8') as out_file:
    json.dump(new, out_file, ensure_ascii=False)
