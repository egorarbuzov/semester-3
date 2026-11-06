# -*- coding: utf-8 -*-
"""Сборка пояснительной записки."""
import json
import os
import sys

from docx import Document
from docx.enum.section import WD_SECTION
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt

import dochelp as H

HERE = os.path.dirname(os.path.abspath(__file__))
FIG = os.path.join(HERE, 'fig')
PAGES = os.path.join(HERE, 'pages.json')
TARGET = os.path.join(os.path.dirname(HERE),
                      'Пояснительная записка КП_КПО_Арбузов.docx')
SOURCE = os.path.join(HERE, 'original_backup.docx')


class Builder:
    def __init__(self, doc, pages=None):
        self.doc = doc
        self.pages = pages or {}
        self.toc = []          # [(level, text, page)]
        self._toc_anchor = None

    # ------------------------------------------------------------ заголовки
    def h1(self, text, structural=False, page=True):
        self.doc_h1(text, structural, page)
        self.toc.append((1, text))

    def doc_h1(self, text, structural=False, page=True):
        H.heading1(self.doc, text, structural=structural, page_break=page)

    def h2(self, text):
        H.heading2(self.doc, text)
        self.toc.append((2, text))

    def h2n(self, text):
        """подраздел без записи в содержание"""
        H.heading2(self.doc, text)

    # ------------------------------------------------------------ текст
    def p(self, text, **kw):
        return H.para(self.doc, text, **kw)

    def ul(self, items):
        H.bullets(self.doc, items)

    def table(self, caption, headers, rows, widths, align=None):
        return H.table(self.doc, caption, headers, rows, widths, align)

    def fig(self, name, caption, width=H.TEXT_W):
        H.figure(self.doc, os.path.join(FIG, name), caption, width)

    def listing(self, code, caption=None):
        H.listing(self.doc, code, caption)

    def formula(self, text, number, legend=None):
        H.formula(self.doc, text, number, legend)

    def note(self, text):
        H.note(self.doc, text)

    def empty(self, n=1):
        for _ in range(n):
            H.para(self.doc, '', indent=Cm(0))

    def new_section(self, a3=False):
        sec = self.doc.add_section(WD_SECTION.NEW_PAGE)
        H.setup_section(sec, a3=a3)
        sec.different_first_page_header_footer = False
        if a3:
            pg = OxmlElement('w:pgBorders')
            pg.set(qn('w:offsetFrom'), 'page')
            for edge, val in (('top', '284'), ('left', '1134'),
                              ('bottom', '284'), ('right', '284')):
                el = OxmlElement('w:' + edge)
                el.set(qn('w:val'), 'single')
                el.set(qn('w:sz'), '18')
                el.set(qn('w:space'), '0')
                el.set(qn('w:color'), '000000')
                pg.append(el)
            sec._sectPr.append(pg)
        return sec

    # ------------------------------------------------------------ содержание
    def toc_anchor(self):
        p = self.doc.add_paragraph()
        pf = p.paragraph_format
        pf.space_before = Pt(0)
        pf.space_after = Pt(0)
        self._toc_anchor = p
        return p

    def build_toc(self):
        entries = [(lvl, txt, str(self.pages.get(txt, '00')))
                   for lvl, txt in self.toc]
        holder = Document()
        tmp = Builder(holder)
        H.toc_field(holder, entries)
        anchor = self._toc_anchor._p
        for el in list(holder.element.body.iterchildren()):
            if el.tag == qn('w:p'):
                anchor.addprevious(el)
        anchor.getparent().remove(anchor)
        self._toc_anchor = None


def load_pages():
    if os.path.exists(PAGES):
        with open(PAGES, encoding='utf-8') as fh:
            return json.load(fh)
    return {}


def build(out_path, pages):
    doc = Document(SOURCE)
    H.setup_styles(doc)
    H.setup_section(doc.sections[0])
    H.add_page_field(doc.sections[0])

    # оставляем только титульный лист
    body = doc.paragraphs
    for p in body[43:]:
        p._element.getparent().remove(p._element)

    d = Builder(doc, pages)

    import content1, content2, content3, content4
    content1.build(d)
    H.para(doc, 'Содержание', align=WD_ALIGN_PARAGRAPH.CENTER, indent=Cm(0),
           bold=True, before=0, after=18, page_break=True)
    d.toc_anchor()          # placeholder, заполняется позже
    content1.build_intro(d)
    content1.build_s1(d)
    content2.build(d)

    d.build_toc()
    doc.save(out_path)
    with open(os.path.join(HERE, 'toc.json'), 'w', encoding='utf-8') as fh:
        json.dump([[lvl, txt] for lvl, txt in d.toc], fh, ensure_ascii=False,
                  indent=1)
    return d


if __name__ == '__main__':
    pages = load_pages()
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join(HERE, 'out.docx')
    b = build(out, pages)
    print('saved', out)
    print('headings:', len(b.toc))
