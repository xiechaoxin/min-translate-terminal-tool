import json


lineinfo = []
filepath = './dictionary.json'

dictionary = {}
with open(filepath, 'r', encoding='UTF-8') as file:
    dictionary = json.load(file)


for k, v in dictionary.items():
    lineinfo.append(f'{k.strip()}\t{v.strip()}')
    

with open('dictionary.txt', 'w') as out_file:
    for item in lineinfo:
        out_file.write(item + '\n')
