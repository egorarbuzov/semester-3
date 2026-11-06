# -*- coding: utf-8 -*-
"""Определение номеров страниц заголовков по PDF-рендеру."""
import json
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PDFTEXT = '/tmp/kpo/pdftext'


def norm(s):
    s = s.lower().replace('ё', 'е')
    return re.sub(r'[^0-9a-zа-я]', '', s)


def pages_from_pdf(pdf):
    out = subprocess.run([PDFTEXT, pdf], capture_output=True)
    txt = out.stdout.decode('utf-8', 'replace')
    parts = re.split(r'===== PAGE (\d+) =====', txt)
    pages = {}
    for i in range(1, len(parts), 2):
        pages[int(parts[i])] = norm(parts[i + 1])
    return pages


def compute(pdf, toc):
    pages = pages_from_pdf(pdf)
    n = max(pages)

    # первая страница введения в основном тексте (не в содержании)
    start = 1
    for p in range(1, n + 1):
        t = pages[p]
        if 'введение' in t and '11характеристикаязыкапрограммирования' not in t \
                and '1спецификацияязыкапрограммирования' not in t:
            start = p
            break

    res = {}
    prev = start
    for lvl, text in toc:
        key = norm(text)
        found = None
        for p in range(prev, n + 1):
            t = pages[p]
            if lvl == 1:
                if key and key in t[:260]:
                    found = p
                    break
            else:
                if key and key in t:
                    found = p
                    break
        if found is None:
            found = prev
        res[text] = found
        prev = found
    return res, n, start


if __name__ == '__main__':
    pdf = sys.argv[1]
    toc = json.load(open(os.path.join(HERE, 'toc.json'), encoding='utf-8'))
    res, n, start = compute(pdf, [(l, t) for l, t in toc])
    json.dump(res, open(os.path.join(HERE, 'pages.json'), 'w',
                        encoding='utf-8'), ensure_ascii=False, indent=1)
    print(f'pages={n} start={start}')
    prev = None
    for lvl, text in toc:
        p = res[text]
        flag = ''
        if prev is not None and p < prev:
            flag = '  <<< НАРУШЕНИЕ ПОРЯДКА'
        prev = p
        print(f'{"  " * (lvl - 1)}{text} .... {p}{flag}')
