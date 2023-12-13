import os
import openpyxl
import json
import threading
from queue import Queue

def match_files_recursion(folder_path, file_extension):
    match_files_list = []
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            if file.endswith(file_extension):
                match_files_list.append(os.path.join(root, file))
    return match_files_list

def read_org_src_dict() -> dict:
    en_zh_dict = {}

    try:
        with open('./dictionary.json', 'r', encoding='UTF-8') as read_file:
            en_zh_dict = json.load(read_file)
    except FileNotFoundError:
        en_zh_dict = {}

    return en_zh_dict

def get_db_from_excel(file_path) -> dict:
    new_data_dict = {}

    workbook = openpyxl.load_workbook(file_path)
    sheet = workbook.active

    for row in sheet.iter_rows(min_row=2, values_only=True):
        if len(row) >= 2:
            key = str(row[0]).replace('\n', '')
            value = str(row[1]).replace('\n', '')
            new_data_dict[key] = value

    return new_data_dict

def filter_and_update_dict(src_dict, new_dict) -> dict:
    updated_dict = src_dict.copy()

    for en, zh in new_dict.items():
        if not en.isascii() or not zh.isascii() or zh == '无':
            continue

        en = en.replace('"', '')
        zh = zh.replace('"', '')

        if en in src_dict:
            if len(zh) > len(src_dict[en]):
                updated_dict[en] = zh
        else:
            updated_dict[en] = zh

    return updated_dict

def process_excel_file(file_queue, result_queue):
    en_zh_dict = {}

    while not file_queue.empty():
        item = file_queue.get()
        new_dict = get_db_from_excel(item)
        en_zh_dict = filter_and_update_dict(en_zh_dict, new_dict)
        file_queue.task_done()

    result_queue.put(en_zh_dict)

def write_updated_dict_to_json(updated_dict):
    with open('./dictionary.json', 'w', encoding='UTF-8') as out_file:
        json.dump(updated_dict, out_file, ensure_ascii=False)

# 指定文件夹路径和文件扩展名
folder_path = '/Users/xcx/Downloads/墨墨词库上千本词书大全/打包下载/精装 2/中文释义'
file_extension = '.xlsx'

# 匹配文件路径列表
xlsx_path_list = match_files_recursion(folder_path, file_extension)

# 创建队列以保存 Excel 文件路径
file_queue = Queue()

# 填充队列
for item in xlsx_path_list:
    file_queue.put(item)

# 创建结果队列
result_queue = Queue()

# 创建并启动工作线程
threads = []

for _ in range(len(xlsx_path_list)):
    thread = threading.Thread(target=process_excel_file, args=(file_queue, result_queue))
    thread.start()
    threads.append(thread)

# 等待所有工作线程完成
for thread in threads:
    thread.join()

# 合并所有线程的结果
en_zh_dict = {}
while not result_queue.empty():
    partial_dict = result_queue.get()
    en_zh_dict = filter_and_update_dict(en_zh_dict, partial_dict)

# 将更新后的字典写入JSON文件
write_updated_dict_to_json(en_zh_dict)
