# coding: utf-8

##
## Make Kanji Cards
##

import sys
import re

class CardStyle:

    ##
    ## Normal Style
    ##

    NORMAL_STYLE = """
body {
    margin: 0;
    padding: 0;
    font-size: 18pt;
    font-family: "UD デジタル 教科書体 NP-R";
}
.header {
    margin: 0;
    padding: 0.5em;
    border-bottom: 0.5pt solid #ccc;
    font-size: 12pt;
}
.title {
    width: 90%;
    float: left;
}
.page {
    text-align: right;
}
.page:after {
    content: counter(page-count);
}
.card {
    counter-increment: page-count;
}
.card-front {
    page-break-after: always;   
}
.card-back {
    page-break-after: always;   
}
.item-set {
    padding: 1em 2em 0em 8em;
    counter-reset: item-number;
}
.item {
    margin: 0 0 1em 0;
    text-indent: -6em;
    line-height: 150%;
    counter-increment: item-number;
}
.item:before {
    font-family: "BIZ UDゴシック";
    vertical-align: middle;
    content: '□□□　(' counter(item-number, cjk-ideographic) ')　';
}
.card-front .q {
    border-bottom: 0.5pt solid #000;
}
.card-back .q {
    color: #F00;
}
"""
    
    ##
    ## Print Style
    ##

    PRINT_STYLE = """
@page {
    size: 182mm 128mm landscape;
}
"""

#------------------------------------------------------------------------------#

class CardItem:
    def __init__(self):
        self.label = ""
        self.kana = ""
        self.kanj = ""

#------------------------------------------------------------------------------#

class KanjiCard:

    def __init__(self):
        self.COMMENT_PAT = '^\s*#'
        self.BLANK_PAT   = '^\s*$'
        self.LABEL_PAT   = '([^:]+):([^:]+)'
        self.RUBY_PAT    = '\(([^{|}]+)\|([^{|}]+)\)'
        self.RUBY_REP    = '<ruby><rb>\\1</rb><rt>\\2</rt></ruby>'

        self.KANJ_PAT    = '{([^{|}]+)\|([^{|}]+)}'
        self.KANJ_REP    = '<span class="q">\\1</span>'
        self.KANA_REP    = '<span class="q">\\2</span>'
        
        self.item_count_per_page = 10
        self.output = 'out.html'
        self.source = None

    def parse_item_line(self, line = ""):
        item = CardItem()
        text = ''
        ruby_text = ''

        # comment line
        c = re.match(self.COMMENT_PAT, line)
        b = re.match(self.BLANK_PAT, line)
        if not(c is None) or not(b is None):
            return None

        # label
        m = re.match(self.LABEL_PAT, line)
        if not (m is None):
            item.label = m.group(1)
            text = m.group(2)
        else:
            return None

        # ruby
        ruby_text = re.sub(self.RUBY_PAT, self.RUBY_REP, text).strip()
        # kana
        item.kana = re.sub(self.KANJ_PAT, self.KANA_REP, ruby_text).strip()
        # kanj
        item.kanj = re.sub(self.KANJ_PAT, self.KANJ_REP, ruby_text).strip()

        return item

    def extract(self):
        if self.source is None:
            raise(Exception("source file is empty"))

        item_set = []

        with open(self.source, 'r', encoding='utf-8') as f:
            for line in f.readlines():
                line.strip()
                item = self.parse_item_line(line)
                if item :
                    item_set.append(item)

        return item_set

    def write_card(self, fp = None, item_set = [], index = 0):
        end = min(index + self.item_count_per_page, len(item_set))

        ##
        ## Card - OPEN
        ##
        
        fp.write('<div class="card">\n')

        ##
        ## Front
        ##
        
        fp.write('<div class="card-front">\n')
        fp.write('<div class="header">\n')
        fp.write('<div class="title">小学3年生の漢字</div>\n')
        fp.write('<div class="page"></div>\n')
        fp.write('</div>\n')
        fp.write('<div class="item-set">\n')

        for i in range(index, end):
            fp.write('<div class="item">')
            fp.write(item_set[i].kana)
            fp.write('</div>\n')
            
        fp.write('</div>\n')
        fp.write('</div>\n')

        ##
        ## Back
        ##
        
        fp.write('<div class="card-back">\n')
        fp.write('<div class="header">\n')
        fp.write('<div class="title">小学3年生の漢字</div>\n')
        fp.write('<div class="page"></div>\n')
        fp.write('</div>\n')
        fp.write('<div class="item-set">\n')

        for i in range(index, end):
            fp.write('<div class="item">')
            fp.write(item_set[i].kanj)
            fp.write('</div>\n')

        fp.write('</div>\n')
        fp.write('</div>\n')

        ##
        ## Card - CLOSE
        ##
        
        fp.write('</div>\n')

        return end

    ##
    ##
    ##
    ##
    
    def write_html(self, fp = None, item_set = []):
        fp.write('<?xml version="1.0" encoding="UTF-8"?>\n')
        fp.write('<html>\n')
        fp.write('<head>\n')
        fp.write('<style>' + CardStyle.NORMAL_STYLE + '</style>\n')
        fp.write('<style media="print">' + CardStyle.PRINT_STYLE + '</style>\n')
        fp.write('<title>小学3年生の漢字</title>\n')
        fp.write('</head>\n')
        fp.write('<body>\n')

        for i in range(0, len(item_set), self.item_count_per_page):
            i += self.write_card(fp, item_set, i)

        fp.write('</body>\n')
        fp.write('</html>\n')

    ##
    ##
    ##
    ##
    
    def print_card(self, item_set = []):
        if self.output is None:
            raise(Exception("output file is empty"))

        with open(self.output, "w", encoding='utf-8') as f:
            self.write_html(f, item_set)

    ##
    ## Interface
    ##
            
    def make(self):
        self.print_card(self.extract())

#------------------------------------------------------------------------------#

class WordItem:
    def __init__(self, line_number, line):
        self.line_number = line_number
        self.line = line

class DupCheck:

    def __init__(self):
        self.word_stat = {}
        self.KANJ_PAT = r'[{]([^{|}]+)\|([^{|}]+)[}]'

    def add_word_stat(self, line = '', line_number = 0):
        lst = re.findall(self.KANJ_PAT, line)
        if len(lst) <= 0:
            print('Ignored: %05d: %s' % (line_number, line))
        else:
            for m in lst:
                w = WordItem(line_number, line)
                if m[0] in self.word_stat:
                    self.word_stat[m[0]].append(w)
                else:
                    self.word_stat[m[0]] = [w]

    def print_word_stat(self):
        total = 0
        dup = 0
        for w in self.word_stat:
            lst = self.word_stat[w]
            
            if len(lst) > 1:
                print('%s: ---- DUPRICATED' % w)
                dup += 1
            else:
                # print("%s: " % w)
                pass

            if len(lst) > 1:
                for item in lst:
                    total += 1
                    print("    %05d: %s" % (item.line_number, item.line))

        print('------------------------------------------------------------')
        print('%d example(s) are registered for each word' % total)
        print('%d word(s) are stored' % len(self.word_stat))
        print('%d word(s) are duplicated.' % dup)
    
    def read_list(self, fp = None):
        num = 0
        line = fp.readline()
        while line:
            num += 1
            line = line.strip()
            self.add_word_stat(line, num)
            line = fp.readline()
        
    def check(self, path = ''):
        print('Check the file: %s' % path)
        with open(path, 'r', encoding='utf-8') as f:
            self.read_list(f)
        
        self.print_word_stat()
        
#------------------------------------------------------------------------------#

def dup_check(path = ''):
    dc = DupCheck()
    dc.check(path)

if __name__ == '__main__':
    if len(sys.argv) > 1:
        dup_check(sys.argv[1])
    else:
        print('usage: %s <path>', sys.argv[0])
