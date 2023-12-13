import csv
import os


import os

def match_files_recursion(folder_path, file_extension):
    match_files_list = []
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            if file.endswith(file_extension):
                match_files_list.append(os.path.join(root, file))
    return match_files_list





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

folder_path = '/Users/xcx/Downloads/墨墨词库上千本词书大全/打包下载/更多/csv-220301'
# file_extension = '.xlsx'
file_extension = '.csv'
xlsx_path_list=match_files_recursion(folder_path, file_extension)

ok_count = 0
err_count = 0

for item in xlsx_path_list:
    # en_zh_dict_new = get_db_from_excel(item)
    en_zh_dict_new = get_db_from_csv(item)
    for k, v in en_zh_dict_new.items():
        if v  != '找不到解释': ok_count+=1
        else: err_count+=1
    # print(en_zh_dict_new)
    
print(f'ok_count{ok_count}, err_count{err_count}')
