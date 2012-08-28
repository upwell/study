#!/usr/bin/python

# -*- coding: utf-8 -*-

import wx
import wx.lib.mixins.listctrl as listmix

import sys
import urllib2, urllib
import re, time, eyeD3

import threading

from xml.dom.minidom import parseString
#from urlgrabber.keepalive import HTTPHandler

import addrdecoder
from addrdecoder import GetLocation
from mp3info import Mp3Info

def AttriveXML(albumAddr):
    try:
        u = urllib2.urlopen(albumAddr)
    except urllib2.URLError, e:
        print "urlopen error:", e
        return ""

    data = u.read()
    return data

def ParseXMLtoURLs(data):
    mp3s = []

    dom = parseString(data)
    tracks = dom.getElementsByTagName("track")
    for track in tracks:
        rawData = track.getElementsByTagName("location")[0].firstChild.wholeText
        url = GetLocation(rawData)

        name = track.getElementsByTagName("title")[0].firstChild.wholeText
        album = track.getElementsByTagName("album_name")[0].firstChild.wholeText
        artist = track.getElementsByTagName("artist")[0].firstChild.wholeText

        mp3 = Mp3Info(name, album, artist, url)
        mp3s.append(mp3)

    return mp3s

class FlexCtrlList(wx.ListCtrl, listmix.ListCtrlAutoWidthMixin):

    def __init__(self, parent, ID, pos=wx.DefaultPosition,
            size=wx.DefaultSize, style=0):
        wx.ListCtrl.__init__(self, parent, ID, pos, size, style)
        listmix.ListCtrlAutoWidthMixin.__init__(self)
        self.setResizeColumn(0)


class ThreadDownloading(threading.Thread):

    def __init__(self, index, mp3):
        threading.Thread.__init__(self)
        self.index = index
        self.mp3 = mp3

    def run(self):

        url = self.mp3.url

        try:
            #keepalive_handler = HTTPHandler()
            #opener = urllib2.build_opener(keepalive_handler)
            opener = urllib2.build_opener()
            opener.addheaders = [
                    ('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'),
                    ('Accept-Charset', 'UTF-8,*;q=0.5'),
                    ('Accept-Encoding', 'gzip,deflate'),
                    ('Accept-Lanaguage', 'en-US,en;q=0.8'),
                    ('User-Agent', 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.81 Safari/537.1')
                    ]
            urllib2.install_opener(opener)
            u = urllib2.urlopen(url)

        except urllib2.URLError, e:
            print 'url open error:', e
            return

        filename = self.mp3.name + ".mp3"
        filename = filename.replace('/', ' or ')
        try:
            local_file = open(filename, 'w')
            local_file.write(u.read())
            local_file.close()

            tag = eyeD3.Tag()
            tag.link(filename)
            tag.header.setVersion(eyeD3.ID3_V2_3)
            tag.encoding = '\x01'
            tag.setArtist(self.mp3.artist)
            tag.setAlbum(self.mp3.album)
            tag.update()
        except IOError, e:
            print 'operate file error:', e
            return

        return


class Main(wx.Frame):

    def __init__(self, parent, title):
        super(Main, self).__init__(parent, title='Xiami Downloader',
            size=(500, 300))

        self.InitUI()
        self.Centre()
        self.Show()

    def InitUI(self):

        self.panel = wx.Panel(self)
        font = wx.SystemSettings_GetFont(wx.SYS_SYSTEM_FONT)
        font.SetPointSize(9)

        vbox = wx.BoxSizer(wx.VERTICAL)

        # URL row
        hbox_url = wx.BoxSizer(wx.HORIZONTAL)

        url_str = wx.StaticText(self.panel, label='Album URL:')
        url_str.SetFont(font)
        self.url_text = wx.TextCtrl(self.panel)
        url_info_btn = wx.Button(self.panel, label='Retrive Album')

        hbox_url.Add(url_str, flag=wx.RIGHT, border=8)
        hbox_url.Add(self.url_text, proportion=1)
        hbox_url.Add(url_info_btn, flag=wx.LEFT, border=8)

        vbox.Add(hbox_url, flag=wx.EXPAND|wx.LEFT|wx.RIGHT|wx.TOP, border=10)

        url_info_btn.Bind(wx.EVT_BUTTON, self.UrlInfoBtnClicked)
        # end of URL row

        # Album info
        hbox_album = wx.BoxSizer(wx.VERTICAL)

        self.song_list = FlexCtrlList(self.panel, 100, style=wx.LC_REPORT|wx.BORDER_SUNKEN)
        self.song_list.InsertColumn(0, 'Song')
        self.song_list.InsertColumn(1, 'Artist')
        self.song_list.InsertColumn(2, 'Album')
        self.song_list.InsertColumn(3, 'Status')


        hbox_album.Add(self.song_list, 1, wx.ALL|wx.EXPAND, 5)

        vbox.Add(hbox_album, proportion=1,
                flag=wx.EXPAND|wx.LEFT|wx.RIGHT|wx.TOP|wx.Bottom, border=10)
        # end of album info

        # Download button
        hbox_down = wx.BoxSizer(wx.HORIZONTAL)

        down_btn = wx.Button(self.panel, label='Download')
        hbox_down.Add(down_btn, flag=wx.RIGHT, border=8)
        vbox.Add(hbox_down, flag=wx.TOP, border=10)

        down_btn.Bind(wx.EVT_BUTTON, self.DownBtnClicked)
        # end of download button

        self.panel.SetSizer(vbox)

    def UrlInfoBtnClicked(self, e):
        url = self.url_text.GetValue()
        if len(url) > 0:
            url = url.strip()
            m = re.search('/album/[0-9]+$', url)
            if m is not None:
                m = re.search('[0-9]+$', url)
                albumId = m.group()

                albumAddr = "http://www.xiami.com/song/playlist/id/" + albumId + "/type/1"
                self.mp3s = ParseXMLtoURLs(AttriveXML(albumAddr))

                index = 0
                for mp3 in self.mp3s:
                    self.song_list.InsertStringItem(index, mp3.name)
                    self.song_list.SetStringItem(index, 1, mp3.artist)
                    self.song_list.SetStringItem(index, 2, mp3.album)
                    self.song_list.SetStringItem(index, 3, 'Not Start')

                    index += 1

    def DownBtnClicked(self, e):
        if len(self.mp3s) <= 0:
            return

        index = 0
        for mp3 in self.mp3s:
            thread = ThreadDownloading(index, mp3)
            thread.setDaemon(True)
            thread.start()
            index += 1



def main():
    app = wx.App()
    Main(None, title='Xiami Downloader')
    app.MainLoop()

if __name__ == '__main__':
    main()

