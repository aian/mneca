# coding: utf-8

##
## Make Kanji Cards
##

import re

class CardItem:
    def __init__(self):
        self.label = ""
        self.kana = ""
        self.kanj = ""

class KanjiCard:

    def __init__(self):
        self.LABEL_PAT = '([^:]+):([^:]+)'
        self.RUBY_PAT  = '\(([^{|}]+)\|([^{|}]+)\)'
        self.RUBY_REP  = '<ruby><rb>\\1</rb><rt>\\2</rt></ruby>'

        self.KANJ_PAT  = '{([^{|}]+)\|([^{|}]+)}'
        self.KANJ_REP  = '<span class="q">\\1</span>'
        self.KANA_REP  = '<span class="q">\\2</span>'
        
        self.item_count_per_page = 10

    def parse_item_line(self, line = ""):
        item = CardItem()
        text = ''
        ruby_text = ''

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

    def extract(self, path = ""):
        item_set = []

        with open(path, 'r', encoding='utf-8') as f:
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
        fp.write('<head>')
        fp.write('<link rel="stylesheet" href="print.css" media="print"/>\n')
        fp.write('<link rel="stylesheet" href="style.css"/>\n')
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
    
    def print_card(self, path = "", item_set = []):
        with open(path, "w", encoding='utf-8') as f:
            self.write_html(f, item_set)

    ##
    ## Interface
    ##
            
    def make(self, in_file = '', out_file = 'card.html'):
        #item_set = self.extract(in_file)
        item_set = self.extract('sample\\vocab.txt')

        self.print_card('sample\\bar.html', item_set)
