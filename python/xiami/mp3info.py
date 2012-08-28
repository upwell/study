# -*- coding: utf-8 -*-

class Mp3Info(object):
    def __init__(self, nm, album, artist, url):
        self.name = nm.encode('utf8')
        self.album = album.encode('utf8')
        self.artist = artist.encode('utf8')
        self.url = url

    def __str__(self):
        str = "[\n" + \
              "'name'     : " + self.name + ",\n" + \
              "'album'    : " + self.album + ",\n" + \
              "'artist'   : " + self.artist + ",\n" + \
              "'url'      : " + self.url + "\n" + \
              "]"
        return str

def test():
    mp3 = Mp3Info(u'高兴', 'up', 'ha', 'http://www.google.com/abc/alright')
    print mp3

if __name__ == '__main__':
    test()
