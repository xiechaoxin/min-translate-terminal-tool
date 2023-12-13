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
    new_en = 0
    update_zh = 0

    for en, zh in new_dict.items():
        if not en.isascii() or not zh.isascii() or zh == '无':
            continue

        en = en.replace('"', '')
        zh = zh.replace('"', '')

        if en in src_dict:
            if len(zh) > len(src_dict[en]):
                updated_dict[en] = zh
                update_zh += 1
        else:
            updated_dict[en] = zh
            new_en += 1

    return updated_dict, new_en, update_zh

def process_excel_file(file_queue, en_zh_dict, new_en_counter, update_zh_counter):
    new_en = 0
    update_zh = 0

    while not file_queue.empty():
        item = file_queue.get()
        new_dict = get_db_from_excel(item)
        en_zh_dict, new_en_thread, update_zh_thread = filter_and_update_dict(en_zh_dict, new_dict)
        new_en += new_en_thread
        update_zh += update_zh_thread
        file_queue.task_done()

    # Update counters using a lock to avoid race conditions
    with update_counter_lock:
        new_en_counter[0] += new_en
        update_zh_counter[0] += update_zh

def write_updated_dict_to_json(updated_dict):
    with open('./dictionary.json', 'w', encoding='UTF-8') as out_file:
        json.dump(updated_dict, out_file, ensure_ascii=False)

folder_path = '/Users/xcx/Downloads/墨墨词库上千本词书大全/打包下载/精装 2/中文释义'
file_extension = '.xlsx'
xlsx_path_list = match_files_recursion(folder_path, file_extension)

en_zh_dict = read_org_src_dict()

# Create a queue to hold the Excel file paths
file_queue = Queue()

# Fill the queue with file paths
for item in xlsx_path_list:
    file_queue.put(item)

# Number of worker threads (you can adjust this based on your needs)
num_threads = 20

# Create and start worker threads
threads = []
new_en_counter = [0]
update_zh_counter = [0]

# Lock for updating counters
update_counter_lock = threading.Lock()

for _ in range(num_threads):
    thread = threading.Thread(target=process_excel_file, args=(file_queue, en_zh_dict, new_en_counter, update_zh_counter))
    thread.start()
    threads.append(thread)

# Wait for all worker threads to finish
for thread in threads:
    thread.join()

print(f'new_en {new_en_counter[0]}')
print(f'update_zh {update_zh_counter[0]}')

write_updated_dict_to_json(en_zh_dict)
