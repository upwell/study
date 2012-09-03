#!/usr/bin/python

import HTMLParser
import sys

reload(sys)
sys.setdefaultencoding('utf8')

class ArtistHotParser(HTMLParser.HTMLParser):
    def __init__(self):
        HTMLParser.HTMLParser.__init__(self)
        self.data = []

    def handle_starttag(self, tag, attrs):
        if tag != 'input':
            return

        is_found = False
        found_value = None
        for name, value in attrs:
            if name == 'name' and value == 'recommendids':
                is_found = True
            elif name == 'value':
                found_value = value

        if is_found:
            self.data.append(found_value)


def test():
    data = open('768', 'r')
    parser = ArtistHotParser()
    parser.feed(data.read())

    print parser.data

if __name__ == '__main__':
    test()
