# -*- coding: utf-8 -*-
"""Помощники оформления пояснительной записки по СТП БГТУ / ГОСТ 2.105-95."""
import copy
import os
import re

from docx.enum.section import WD_ORIENT, WD_SECTION
from docx.enum.table import WD_ALIGN_VERTICAL, WD_TABLE_ALIGNMENT
from docx.enum.text import WD_ALIGN_PARAGRAPH, WD_LINE_SPACING, WD_TAB_ALIGNMENT, WD_TAB_LEADER
from docx.oxml import OxmlElement
from docx.oxml.ns import qn
from docx.shared import Cm, Pt, RGBColor

FONT = 'Times New Roman'
MONO = 'Courier New'
BODY = Pt(14)
SMALL = Pt(12)
TEXT_W = 16.5  # см, ширина полосы набора
IND = Cm(1.25)


# ---------------------------------------------------------------- низкий уровень
def _set_rfonts(rpr, name):
    rf = rpr.find(qn('w:rFonts'))
    if rf is None:
        rf = OxmlElement('w:rFonts')
        rpr.insert(0, rf)
    for attr in ('w:ascii', 'w:hAnsi', 'w:cs', 'w:eastAsia'):
        rf.set(qn(attr), name)
    # убрать тематические шрифты, иначе Word подставит Calibri
    for attr in list(rf.attrib):
        if 'Theme' in attr or 'theme' in attr:
            del rf.attrib[attr]


def _solid_black(rpr):
    color = rpr.find(qn('w:color'))
    if color is not None:
        for attr in list(color.attrib):
            if 'theme' in attr or 'Theme' in attr:
                del color.attrib[attr]


def style_font(style, size=BODY, bold=None, name=FONT, color=RGBColor(0, 0, 0)):
    f = style.font
    f.name = name
    f.size = size
    if bold is not None:
        f.bold = bold
    f.italic = False
    f.color.rgb = color
    rpr = style.element.get_or_add_rPr()
    _set_rfonts(rpr, name)
    _solid_black(rpr)


def _set_lang(style):
    rpr = style.element.get_or_add_rPr()
    for el in rpr.findall(qn('w:lang')):
        rpr.remove(el)
    lang = OxmlElement('w:lang')
    lang.set(qn('w:val'), 'ru-RU')
    lang.set(qn('w:eastAsia'), 'ru-RU')
    lang.set(qn('w:bidi'), 'ar-SA')
    rpr.append(lang)


def setup_styles(doc):
    st = doc.styles

    normal = st['Normal']
    style_font(normal, BODY)
    _set_lang(normal)
    pf = normal.paragraph_format
    pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
    pf.space_before = Pt(0)
    pf.space_after = Pt(0)
    pf.alignment = WD_ALIGN_PARAGRAPH.JUSTIFY
    pf.first_line_indent = IND
    pf.widow_control = True

    h1 = st['Heading 1']
    style_font(h1, BODY, bold=True)
    _set_lang(h1)
    pf = h1.paragraph_format
    pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
    pf.space_before = Pt(0)
    pf.space_after = Pt(18)
    pf.first_line_indent = IND
    pf.left_indent = Cm(0)
    pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
    pf.keep_with_next = True

    h2 = st['Heading 2']
    style_font(h2, BODY, bold=True)
    _set_lang(h2)
    pf = h2.paragraph_format
    pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
    pf.space_before = Pt(6)
    pf.space_after = Pt(18)
    pf.first_line_indent = IND
    pf.left_indent = Cm(0)
    pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
    pf.keep_with_next = True

    for name in ('No Spacing',):
        try:
            s = st[name]
            style_font(s, BODY)
        except KeyError:
            pass

    # переносы, как требует п. 3.1 методических указаний
    settings = doc.settings.element
    for tag in ('w:autoHyphenation', 'w:consecutiveHyphenLimit', 'w:hyphenationZone'):
        for el in settings.findall(qn(tag)):
            settings.remove(el)
    ah = OxmlElement('w:autoHyphenation')
    ah.set(qn('w:val'), 'true')
    settings.append(ah)
    chl = OxmlElement('w:consecutiveHyphenLimit')
    chl.set(qn('w:val'), '3')
    settings.append(chl)


def setup_section(sec, a3=False):
    if a3:
        sec.orientation = WD_ORIENT.LANDSCAPE
        sec.page_width = Cm(42.0)
        sec.page_height = Cm(29.7)
        sec.left_margin = Cm(2.0)
        sec.right_margin = Cm(1.0)
        sec.top_margin = Cm(1.0)
        sec.bottom_margin = Cm(1.0)
    else:
        sec.orientation = WD_ORIENT.PORTRAIT
        sec.page_width = Cm(21.0)
        sec.page_height = Cm(29.7)
        sec.left_margin = Cm(3.0)
        sec.right_margin = Cm(1.5)
        sec.top_margin = Cm(2.0)
        sec.bottom_margin = Cm(2.0)
    sec.header_distance = Cm(1.25)
    sec.footer_distance = Cm(1.0)


def add_page_field(sec):
    """Номер страницы: правый верхний угол, Times New Roman 12 пт."""
    sec.different_first_page_header_footer = True
    hdr = sec.header
    hdr.is_linked_to_previous = False
    p = hdr.paragraphs[0]
    p.text = ''
    p.alignment = WD_ALIGN_PARAGRAPH.RIGHT
    pf = p.paragraph_format
    pf.first_line_indent = Cm(0)
    pf.left_indent = Cm(0)
    pf.space_before = Pt(0)
    pf.space_after = Pt(0)
    pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
    fld = OxmlElement('w:fldSimple')
    fld.set(qn('w:instr'), ' PAGE ')
    p._p.append(fld)


# ---------------------------------------------------------------- абзацы
_TOKEN = re.compile(r'(\*\*.+?\*\*|`.+?`)')


def _fill(p, text, size=BODY, name=FONT, bold=False):
    for chunk in _TOKEN.split(text):
        if not chunk:
            continue
        if chunk.startswith('**') and chunk.endswith('**') and len(chunk) > 4:
            run = p.add_run(chunk[2:-2])
            run.bold = True
            run.font.size = size
            run.font.name = name
        elif chunk.startswith('`') and chunk.endswith('`') and len(chunk) > 2:
            run = p.add_run(chunk[1:-1])
            run.font.size = size
            run.font.name = MONO
        else:
            run = p.add_run(chunk)
            run.bold = bold or None
            run.font.size = size
            run.font.name = name
        if run.font.name:
            _set_rfonts(run._element.get_or_add_rPr(), run.font.name)


def para(doc, text='', align=WD_ALIGN_PARAGRAPH.JUSTIFY, indent=IND, size=BODY,
         name=FONT, bold=False, before=0, after=0, page_break=False,
         keep_next=False, left=0, ls=WD_LINE_SPACING.SINGLE, caps=False):
    p = doc.add_paragraph()
    pf = p.paragraph_format
    pf.alignment = align
    pf.first_line_indent = indent
    pf.left_indent = left
    pf.space_before = Pt(before)
    pf.space_after = Pt(after)
    pf.line_spacing_rule = ls
    pf.keep_with_next = keep_next
    if page_break:
        pf.page_break_before = True
    if name == MONO:
        for line in (text.split('\n') if text else ['']):
            r = p.add_run(line)
            r.font.size = size
            r.font.name = MONO
            _set_rfonts(r._element.get_or_add_rPr(), MONO)
            if line != text.split('\n')[-1] if text else False:
                r.add_break()
        return p
    if text:
        _fill(p, text.upper() if caps else text, size=size, name=name, bold=bold)
    return p


def heading1(doc, text, structural=False, page_break=True):
    """Раздел или структурный элемент записки."""
    p = doc.add_paragraph(style='Heading 1')
    pf = p.paragraph_format
    pf.page_break_before = page_break
    pf.space_before = Pt(0)
    pf.space_after = Pt(18)
    if structural:
        pf.alignment = WD_ALIGN_PARAGRAPH.CENTER
        pf.first_line_indent = Cm(0)
    else:
        pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
        pf.first_line_indent = IND
    r = p.add_run(text)
    r.bold = True
    r.font.size = BODY
    r.font.name = FONT
    _set_rfonts(r._element.get_or_add_rPr(), FONT)
    return p


def heading2(doc, text):
    p = doc.add_paragraph(style='Heading 2')
    pf = p.paragraph_format
    pf.space_before = Pt(18)
    pf.space_after = Pt(18)
    pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
    pf.first_line_indent = IND
    r = p.add_run(text)
    r.bold = True
    r.font.size = BODY
    r.font.name = FONT
    _set_rfonts(r._element.get_or_add_rPr(), FONT)
    return p


def listing(doc, code, caption=None):
    lines = code.rstrip('\n').split('\n')
    for i, line in enumerate(lines):
        p = doc.add_paragraph()
        pf = p.paragraph_format
        pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
        pf.first_line_indent = Cm(0)
        pf.left_indent = Cm(0)
        pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
        pf.space_before = Pt(14) if i == 0 else Pt(0)
        pf.space_after = Pt(0)
        pf.keep_with_next = i < len(lines) - 1
        r = p.add_run(line if line else ' ')
        r.font.size = SMALL
        r.font.name = MONO
        _set_rfonts(r._element.get_or_add_rPr(), MONO)
    if caption:
        para(doc, caption, align=WD_ALIGN_PARAGRAPH.LEFT, indent=Cm(0),
             size=SMALL, before=6, after=14)


def figure(doc, path, caption, width_cm=TEXT_W):
    p = doc.add_paragraph()
    pf = p.paragraph_format
    pf.alignment = WD_ALIGN_PARAGRAPH.CENTER
    pf.first_line_indent = Cm(0)
    pf.left_indent = Cm(0)
    pf.space_before = Pt(14)
    pf.space_after = Pt(6)
    pf.keep_with_next = True
    run = p.add_run()
    run.add_picture(path, width=Cm(width_cm))
    para(doc, caption, align=WD_ALIGN_PARAGRAPH.CENTER, indent=Cm(0),
         before=0, after=14)


def formula(doc, text, number, legend=None):
    p = doc.add_paragraph()
    pf = p.paragraph_format
    pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
    pf.first_line_indent = Cm(0)
    pf.left_indent = Cm(0)
    pf.space_before = Pt(14)
    pf.space_after = Pt(0)
    pf.tab_stops.add_tab_stop(Cm(TEXT_W / 2), WD_TAB_ALIGNMENT.CENTER)
    pf.tab_stops.add_tab_stop(Cm(TEXT_W), WD_TAB_ALIGNMENT.RIGHT)
    r = p.add_run('\t' + text + '\t(' + number + ')')
    r.font.size = BODY
    r.font.name = FONT
    _set_rfonts(r._element.get_or_add_rPr(), FONT)
    if legend:
        first = True
        for item in legend:
            para(doc, ('где ' if first else '') + item,
                 indent=IND, align=WD_ALIGN_PARAGRAPH.JUSTIFY,
                 before=0, after=0)
            first = False
        para(doc, '', indent=Cm(0), after=0)


def note(doc, text):
    para(doc, 'Примечание – ' + text, align=WD_ALIGN_PARAGRAPH.JUSTIFY,
         indent=IND, size=SMALL, before=6, after=6)


def bullets(doc, items, marker='\u2013'):
    for it in items:
        p = doc.add_paragraph()
        pf = p.paragraph_format
        pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
        pf.first_line_indent = Cm(0)
        pf.left_indent = IND
        pf.space_before = Pt(0)
        pf.space_after = Pt(0)
        pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
        _fill(p, marker + ' ' + it)


# ---------------------------------------------------------------- таблицы
def _cell_text(cell, text, size=SMALL, bold=False, align=WD_ALIGN_PARAGRAPH.LEFT):
    cell.vertical_alignment = WD_ALIGN_VERTICAL.CENTER
    p = cell.paragraphs[0]
    p.text = ''
    p.style = cell.part.document.styles['Normal']
    pf = p.paragraph_format
    pf.alignment = align
    pf.first_line_indent = Cm(0)
    pf.left_indent = Cm(0)
    pf.space_before = Pt(0)
    pf.space_after = Pt(0)
    pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
    parts = str(text).split('\n')
    for i, part in enumerate(parts):
        if i:
            p.add_run().add_break()
        _fill(p, part, size=size, bold=bold) if part else None


def _borders(table):
    tblPr = table._tbl.tblPr
    borders = OxmlElement('w:tblBorders')
    for edge in ('top', 'left', 'bottom', 'right', 'insideH', 'insideV'):
        el = OxmlElement('w:' + edge)
        el.set(qn('w:val'), 'single')
        el.set(qn('w:sz'), '4')
        el.set(qn('w:space'), '0')
        el.set(qn('w:color'), '000000')
        borders.append(el)
    tblPr.append(borders)


def table(doc, caption, headers, rows, widths, align=None, font_size=SMALL):
    if caption:
        para(doc, caption, align=WD_ALIGN_PARAGRAPH.LEFT, indent=Cm(0),
             before=14, after=6, keep_next=True)
    t = doc.add_table(rows=1, cols=len(headers))
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    t.autofit = False
    tblPr = t._tbl.tblPr
    layout = OxmlElement('w:tblLayout')
    layout.set(qn('w:type'), 'fixed')
    tblPr.append(layout)
    _borders(t)

    hdr = t.rows[0]
    trPr = hdr._tr.get_or_add_trPr()
    th = OxmlElement('w:tblHeader')
    trPr.append(th)
    for i, h in enumerate(headers):
        _cell_text(hdr.cells[i], h, size=font_size, bold=True,
                   align=WD_ALIGN_PARAGRAPH.CENTER)
    for row in rows:
        cells = t.add_row().cells
        for i, val in enumerate(row):
            a = WD_ALIGN_PARAGRAPH.LEFT
            if align and i < len(align) and align[i] == 'c':
                a = WD_ALIGN_PARAGRAPH.CENTER
            _cell_text(cells[i], val, size=font_size, align=a)

    if len(t.rows) <= 9:
        for row in t.rows[:-1]:
            for cell in row.cells:
                for cp in cell.paragraphs:
                    cp.paragraph_format.keep_with_next = True

    scale = sum(widths)
    for i, w in enumerate(widths):
        cm = TEXT_W * (w / scale)
        for row in t.rows:
            row.cells[i].width = Cm(cm)
        t.columns[i].width = Cm(cm)
    # отступ после таблицы
    para(doc, '', indent=Cm(0), after=0)
    return t


# ---------------------------------------------------------------- оглавление
def toc_field(doc, entries, widths_cm=TEXT_W):
    """Поле TOC с закешированным результатом: Word сможет обновить номера страниц."""
    def make_entry(text, page, level, first, last):
        p = doc.add_paragraph()
        pf = p.paragraph_format
        pf.alignment = WD_ALIGN_PARAGRAPH.LEFT
        pf.first_line_indent = Cm(0)
        pf.left_indent = Cm(0 if level == 1 else 0.75)
        pf.space_before = Pt(0)
        pf.space_after = Pt(0)
        pf.line_spacing_rule = WD_LINE_SPACING.SINGLE
        pf.tab_stops.add_tab_stop(Cm(widths_cm), WD_TAB_ALIGNMENT.RIGHT,
                                  WD_TAB_LEADER.DOTS)
        if first:
            r = p.add_run()
            fc = OxmlElement('w:fldChar')
            fc.set(qn('w:fldCharType'), 'begin')
            r._element.append(fc)
            r2 = p.add_run()
            it = OxmlElement('w:instrText')
            it.set(qn('xml:space'), 'preserve')
            it.text = ' TOC \\o "1-2" \\h \\z \\u '
            r2._element.append(it)
            r3 = p.add_run()
            sep = OxmlElement('w:fldChar')
            sep.set(qn('w:fldCharType'), 'separate')
            r3._element.append(sep)
        _fill(p, text, size=BODY, bold=(level == 1))
        tab = p.add_run('\t' + str(page))
        tab.font.size = BODY
        tab.font.name = FONT
        _set_rfonts(tab._element.get_or_add_rPr(), FONT)
        if last:
            r4 = p.add_run()
            end = OxmlElement('w:fldChar')
            end.set(qn('w:fldCharType'), 'end')
            r4._element.append(end)
        return p

    n = len(entries)
    for i, (lvl, text, page) in enumerate(entries):
        make_entry(text, page, lvl, i == 0, i == n - 1)
