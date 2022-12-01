import sys

big5_zhuyin = sys.argv[1] if len(sys.argv) > 1 else 'Big5-ZhuYin.map'
zhuyin_big5 = sys.argv[2] if len(sys.argv) > 2 else 'ZhuYin-Big5.map'

res = {}
with open(big5_zhuyin, 'r', encoding='big5-hkscs') as f:
    data = f.readlines()
    for line in data:
        key, value = line.split(' ')
        value = value.split('/')
        
        res[key] = [key]
        for v in value:
            if v[0] not in res:
                res[v[0]] = [key]
            else:
                if key not in res[v[0]]:
                    res[v[0]].append(key)

with open(zhuyin_big5, 'w', encoding='big5-hkscs') as f:
    for key, value in res.items():
        line = key + '\t' + ' '.join(value) + ' \n'
        f.write(line)