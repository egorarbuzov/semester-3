# -*- coding: utf-8 -*-
"""Генерация структурных схем и графов для пояснительной записки (PIL)."""
import math
import os

from PIL import Image, ImageDraw, ImageFont

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, 'fig')
os.makedirs(OUT, exist_ok=True)

REG = '/System/Library/Fonts/Supplemental/Arial.ttf'
BOLD = '/System/Library/Fonts/Supplemental/Arial Bold.ttf'
S = 3


def f(sz, bold=False):
    return ImageFont.truetype(BOLD if bold else REG, sz * S)


class Canvas:
    def __init__(self, w, h, s=S):
        self.w, self.h, self.s = w, h, s
        self.im = Image.new('RGB', (w * s, h * s), 'white')
        self.d = ImageDraw.Draw(self.im)

    def rect(self, x, y, w, h, text='', sz=13, bold=False, r=0):
        box = [x * self.s, y * self.s, (x + w) * self.s, (y + h) * self.s]
        if r:
            self.d.rounded_rectangle(box, radius=r * self.s, fill='white',
                                     outline='black',
                                     width=max(1, int(1.1 * self.s)))
        else:
            self.d.rectangle(box, fill='white', outline='black',
                             width=max(1, int(1.1 * self.s)))
        if text:
            self.text(x + w / 2, y + h / 2, text, sz, bold)

    def circle(self, cx, cy, r, text='', sz=12, double=False):
        bb = [(cx - r) * self.s, (cy - r) * self.s,
              (cx + r) * self.s, (cy + r) * self.s]
        self.d.ellipse(bb, fill='white', outline='black',
                       width=max(1, int(1.1 * self.s)))
        if double:
            r2 = r - 4
            bb = [(cx - r2) * self.s, (cy - r2) * self.s,
                  (cx + r2) * self.s, (cy + r2) * self.s]
            self.d.ellipse(bb, outline='black', width=max(1, int(1.1 * self.s)))
        if text:
            self.text(cx, cy, text, sz, True)

    def text(self, cx, cy, text, sz=13, bold=False):
        lines = str(text).split('\n')
        font = f(sz, bold)
        lh = sz * self.s * 1.26
        total = lh * len(lines)
        for i, line in enumerate(lines):
            yy = cy * self.s - total / 2 + lh * (i + 0.5)
            self.d.text((cx * self.s, yy), line, font=font, fill='black',
                        anchor='mm')

    def line(self, x1, y1, x2, y2, w=1.1):
        self.d.line([x1 * self.s, y1 * self.s, x2 * self.s, y2 * self.s],
                    fill='black', width=max(1, int(w * self.s)))

    def head(self, x1, y1, x2, y2, size=6):
        ang = math.atan2(y2 - y1, x2 - x1)
        for sg in (+1, -1):
            a = ang + sg * 0.42
            self.line(x2, y2, x2 - size * math.cos(a), y2 - size * math.sin(a))

    def arrow(self, x1, y1, x2, y2, size=6):
        self.line(x1, y1, x2, y2)
        self.head(x1, y1, x2, y2, size)

    def orth(self, pts, arrow=True, size=6, both=False):
        """Ломаная с ортогональными сегментами."""
        for i in range(len(pts) - 1):
            self.line(pts[i][0], pts[i][1], pts[i + 1][0], pts[i + 1][1])
        if arrow:
            self.head(pts[-2][0], pts[-2][1], pts[-1][0], pts[-1][1], size)
        if both:
            self.head(pts[1][0], pts[1][1], pts[0][0], pts[0][1], size)

    def save(self, name, dpi=200):
        path = os.path.join(OUT, name)
        im = self.im.resize((int(self.w * self.s * dpi / 72),
                             int(self.h * self.s * dpi / 72)), Image.LANCZOS)
        im.save(path, dpi=(dpi, dpi))
        return path


# ================================================================== 2.1
def fig_translator():
    c = Canvas(660, 500)
    x, w = 100, 280
    ys = [8, 78, 148, 218, 288, 358, 428]
    labels = ['Исходный текст (.aea)', 'Лексический анализатор',
              'Синтаксический анализатор', 'Дерево разбора',
              'Семантический анализатор', 'Генератор кода',
              'Ассемблерный текст (.asm)']
    for y, t in zip(ys, labels):
        c.rect(x, y, w, 46, t, 12)
    for i in range(len(ys) - 1):
        c.arrow(x + w / 2, ys[i] + 46, x + w / 2, ys[i + 1])

    tx, tw = 505, 145
    c.rect(tx, 140, tw, 130, '')
    c.text(tx + tw / 2, 158, 'Таблица', 11, True)
    c.text(tx + tw / 2, 175, 'идентификаторов', 11, True)
    for i, t in enumerate(['имя', 'тип', 'область видимости', 'адрес']):
        c.text(tx + tw / 2, 198 + i * 17, t, 10)

    bus = 420
    for y in (101, 171, 311):
        c.orth([(x + w, y), (bus, y)], arrow=False)
    c.line(bus, 101, bus, 311)
    c.arrow(bus, 206, tx, 206)
    c.save('fig_2_1.png')


# ================================================================== 2.2
def fig_interaction():
    c = Canvas(660, 235)
    w = 140
    xs = [10, 175, 340, 505]
    r1 = ['Исходный текст', 'Лексический\nанализатор', 'Таблица лексем',
          'Синтаксический\nанализатор']
    r2 = ['Генератор кода', 'Размеченное\nдерево', 'Семантический\nанализатор',
          'Дерево разбора']
    for x, t in zip(xs, r1):
        c.rect(x, 25, w, 50, t, 11)
    for x, t in zip(xs, r2):
        c.rect(x, 140, w, 50, t, 11)
    for i in range(3):
        c.arrow(xs[i] + w, 50, xs[i + 1], 50)
    c.arrow(xs[3] + w / 2, 75, xs[3] + w / 2, 140)
    for i in range(3, 0, -1):
        c.arrow(xs[i], 165, xs[i - 1] + w, 165)
    c.arrow(xs[0] + w / 2, 190, xs[0] + w / 2, 208)
    c.text(xs[0] + w / 2, 222, 'ассемблерный текст', 9)
    c.save('fig_2_2.png')


# ================================================================== 3.1
def fig_lexer():
    c = Canvas(640, 290)
    c.rect(10, 118, 100, 48, 'Исходный\nтекст', 12)
    c.rect(150, 118, 120, 48, 'Модуль ввода\nсимволов', 11)
    c.rect(310, 118, 130, 48, 'Распознаватель\nлексем', 11)
    c.rect(485, 118, 110, 48, 'Поток\nлексем', 12)
    c.arrow(110, 142, 150, 142)
    c.arrow(270, 142, 310, 142)
    c.arrow(440, 142, 485, 142)
    c.rect(150, 25, 120, 44, 'Кодовая\nтаблица', 11)
    c.arrow(210, 69, 210, 118)
    c.rect(310, 218, 130, 44, 'Таблица\nлексем', 11)
    c.orth([(375, 166), (375, 218)], arrow=False, both=True)
    c.rect(485, 218, 110, 44, 'Сообщения\nоб ошибках', 11)
    c.orth([(440, 166), (440, 190), (540, 190), (540, 218)])
    c.save('fig_3_1.png')


# ================================================================== 3.2
def fig_fsm():
    c = Canvas(620, 120)
    c.circle(50, 65, 19, 'q0')
    c.circle(230, 65, 19, 'q1')
    c.circle(430, 65, 19, 'q2', double=True)
    c.arrow(6, 65, 31, 65)
    c.text(26, 40, 'начало', 9)
    c.orth([(69, 50), (211, 50)])
    c.text(140, 38, 'a..z | A..Z | _', 10)
    c.orth([(230, 84), (70, 84)])
    c.text(150, 96, '0..9 | _', 10)
    c.orth([(249, 50), (411, 50)])
    c.text(330, 38, 'a..z | A..Z | 0..9 | _', 10)
    c.orth([(430, 84), (250, 84)])
    c.text(340, 96, 'прочее', 10)
    c.rect(540, 43, 62, 44, 'лексема', 10)
    c.arrow(449, 65, 540, 65)
    c.save('fig_3_2.png')


# ================================================================== 4.1
def fig_parser():
    c = Canvas(640, 300)
    c.rect(10, 105, 95, 50, 'Поток\nлексем', 12)
    c.rect(145, 105, 115, 50, 'Буфер\nвходных\nлексем', 10)
    c.rect(300, 105, 125, 50, 'Блок управления\n(магазин)', 10)
    c.rect(465, 105, 120, 50, 'Дерево\nразбора', 12)
    c.arrow(105, 130, 145, 130)
    c.arrow(260, 130, 300, 130)
    c.arrow(425, 130, 465, 130)
    c.rect(300, 15, 125, 44, 'Управляющая\nтаблица', 11)
    c.arrow(362, 59, 362, 105)
    c.rect(300, 205, 125, 44, 'Подсистема\nсообщений', 11)
    c.orth([(362, 155), (362, 205)], arrow=False, both=True)
    c.rect(465, 205, 120, 44, 'Протокол\nразбора', 11)
    c.orth([(425, 140), (445, 140), (445, 227), (465, 227)])
    c.save('fig_4_1.png')


# ================================================================== 5.1
def fig_semantic():
    c = Canvas(560, 265)
    c.rect(10, 100, 140, 52, 'Дерево\nразбора', 12)
    c.rect(185, 100, 155, 52, 'Модуль\nсемантических\nпроверок', 10)
    c.rect(375, 100, 150, 52, 'Размеченное\nдерево', 11)
    c.arrow(150, 126, 185, 126)
    c.arrow(340, 126, 375, 126)
    c.rect(185, 15, 155, 44, 'Правила\nсемантики', 11)
    c.arrow(262, 59, 262, 100)
    c.rect(185, 200, 155, 44, 'Таблица\nсимволов', 11)
    c.orth([(262, 152), (262, 200)], arrow=False, both=True)
    c.rect(375, 200, 150, 44, 'Сообщения\nоб ошибках', 10)
    c.orth([(340, 152), (340, 175), (450, 175), (450, 200)])
    c.save('fig_5_1.png')


# ================================================================== 7.1
def fig_codegen():
    c = Canvas(640, 260)
    c.rect(10, 95, 105, 52, 'Дерево\nразбора', 12)
    c.rect(150, 95, 125, 52, 'Генератор кода', 12)
    c.rect(310, 95, 120, 52, 'Ассемблерный\nтекст', 11)
    c.rect(465, 95, 120, 52, 'Транслятор\nNASM', 11)
    c.arrow(115, 121, 150, 121)
    c.arrow(275, 121, 310, 121)
    c.arrow(430, 121, 465, 121)
    c.rect(150, 195, 125, 44, 'Таблица\nсимволов', 11)
    c.arrow(212, 195, 212, 147)
    c.rect(310, 15, 120, 44, 'Статическая\nбиблиотека', 10)
    c.arrow(370, 59, 370, 95)
    c.rect(465, 195, 120, 44, 'Объектный\nмодуль', 11)
    c.orth([(525, 147), (525, 195)])
    c.save('fig_7_1.png')


# ================================================================== деревья
def tree_metrics(root, bw, gap):
    def width(node):
        if not node[1]:
            return bw + gap
        return sum(width(ch) for ch in node[1])

    def depth(node):
        if not node[1]:
            return 0
        return 1 + max(depth(ch) for ch in node[1])

    return width(root), depth(root)


def draw_tree(c, root, dy, bw, bh, sz=11, gap=6, pad=20):
    def width(node):
        if not node[1]:
            return bw + gap
        return sum(width(ch) for ch in node[1])

    w, d = tree_metrics(root, bw, gap)
    left0 = pad + max(0, (c.w - 2 * pad - w) / 2)

    def place(node, left, level):
        node_w = width(node)
        cx = left + node_w / 2
        y = pad + level * dy
        c.rect(cx - bw / 2, y, bw, bh, node[0], sz, r=3)
        if node[1]:
            cl = left
            for ch in node[1]:
                cw = width(ch)
                c.line(cx, y + bh, cl + cw / 2, y + dy)
                place(ch, cl, level + 1)
                cl += cw

    place(root, left0, 0)
    return w, d


def fig_ast():
    tree = ('main', [
        ('Decl\nuint8 c', [
            ('выражение', [
                ('call bitOr', [('var a', []), ('var b', [])]),
            ]),
        ]),
        ('Print', [
            ('concat', [
                ('str "c = "', []),
                ('call intToStr', [('var c', [])]),
            ]),
        ]),
    ])
    bw, gap, dy, bh = 110, 8, 60, 38
    w, d = tree_metrics(tree, bw, gap)
    c = Canvas(int(w + 40), int(d * dy + bh + 44))
    draw_tree(c, tree, dy, bw, bh, 10, gap)
    c.save('fig_4_2.png')


def fig_ast_a3():
    tree = ('Program', [
        ('Decl\nuint8 a', []),
        ('Decl\nbool flag', []),
        ('Func\nbitOr', [
            ('Param\nuint8 x', []),
            ('Param\nuint8 y', []),
            ('Return', [('выражение\nx | y', [])]),
        ]),
        ('Main', [
            ('Decl\nuint8 c', [('init\ncall bitOr(a, b)', [])]),
            ('DoWhile', [
                ('Block', [
                    ('Print', [('call intToStr(i)', [])]),
                    ('Assign\ni = i + 1', []),
                ]),
                ('условие\ni < 3', []),
            ]),
            ('If', [
                ('условие\nflag', []),
                ('Print\n"flag is true"', []),
                ('Else', [('Print\n"flag is false"', [])]),
            ]),
        ]),
    ])
    bw, gap, dy, bh = 150, 10, 105, 58
    w, d = tree_metrics(tree, bw, gap)
    c = Canvas(int(w + 40), int(d * dy + bh + 40), s=2)
    draw_tree(c, tree, dy, bw, bh, 11, gap)
    c.save('fig_a3.png', dpi=130)


if __name__ == '__main__':
    fig_translator()
    fig_interaction()
    fig_lexer()
    fig_fsm()
    fig_parser()
    fig_semantic()
    fig_codegen()
    fig_ast()
    fig_ast_a3()
    for n in sorted(os.listdir(OUT)):
        print(n, os.path.getsize(os.path.join(OUT, n)))
