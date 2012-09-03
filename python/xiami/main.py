#!/usr/bin/python

# -*- coding: utf-8 -*-

import wx
import wx.lib.mixins.listctrl as listmix

import sys, os
import urllib2, urllib
import re, time, eyeD3
import ConfigParser

import threading

from xml.dom.minidom import parseString

import addrdecoder
from addrdecoder import GetLocation
from mp3info import Mp3Info
from artisthot import ArtistHotParser

# global variables
CONFIG_FILE = 'settings.cfg'

OS_TYPE = ''
PATH_SEP = ''
HOME_DIR = ''
CURR_DIR = ''

global_setting = None

class SettingConfig():

    def __init__(self):
        self.LoadDefaultSettings()

    def LoadDefaultSettings(self):
        self.dpath = HOME_DIR + PATH_SEP + 'Music'

    def LoadConfigFromFile(self):
        config = ConfigParser.ConfigParser(allow_no_value=True)
        config.read(CURR_DIR + PATH_SEP + CONFIG_FILE)

        value = config.get('core', 'download_path')
        if len(value) > 0:
            self.dpath = value

    def WriteConfigToFile(self):
        config = ConfigParser.ConfigParser(allow_no_value=True)
        config_file = CURR_DIR + PATH_SEP + CONFIG_FILE
        config.read(config_file)

        config.set('core', 'download_path', self.dpath)

        with open(config_file, 'wb') as configdata:
            config.write(configdata)


def AttriveData(url):
    try:
        u = urllib2.urlopen(url)
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

''' Self listctrl class for AutoWidth '''
class FlexCtrlList(wx.ListCtrl, listmix.ListCtrlAutoWidthMixin):

    def __init__(self, parent, ID, pos=wx.DefaultPosition,
            size=wx.DefaultSize, style=0):
        wx.ListCtrl.__init__(self, parent, ID, pos, size, style)
        listmix.ListCtrlAutoWidthMixin.__init__(self)
        self.setResizeColumn(0)

# Define notification event for thread completion
EVT_RESULT_ID = wx.NewId()

STATUS_FAILED = 0
STATUS_SUCCEED = 1
STATUS_START = 2

def EVT_RESULT(win, func):
    """Define Result Event."""
    win.Connect(-1, -1, EVT_RESULT_ID, func)

class DownloadEvent(wx.PyEvent):

    def __init__(self, index, status):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_RESULT_ID)
        self.index = index
        self.status = status


''' Downlading thread '''
class DownloadingThread(threading.Thread):

    def __init__(self, window, index, mp3):
        threading.Thread.__init__(self)
        self.notify_window = window
        self.index = index
        self.mp3 = mp3

    def run(self):

        url = self.mp3.url

        wx.PostEvent(self.notify_window,
            DownloadEvent(self.index, STATUS_START))

        try:
            #keepalive_handler = HTTPHandler()
            #opener = urllib2.build_opener(keepalive_handler)
            opener = urllib2.build_opener()
            opener.addheaders = [
                    ('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8'),
                    ('Accept-Charset', 'UTF-8,*;q=0.5'),
                    #('Accept-Encoding', 'gzip,deflate'),
                    ('Accept-Lanaguage', 'en-US,en;q=0.8'),
                    ('User-Agent',
                        'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.81 Safari/537.1')
                    ]
            urllib2.install_opener(opener)
            u = urllib2.urlopen(url)

        except urllib2.URLError, e:
            print 'url open error:', e
            wx.PostEvent(self.notify_window,
                    DownloadEvent(self.index, STATUS_FAILED))
            return

        filename = self.mp3.name + ".mp3"
        filename = filename.replace('/', ' or ')

        if not os.path.exists(global_setting.dpath):
            os.makedirs(global_setting.dpath)

        file_path = global_setting.dpath + PATH_SEP + filename
        try:
            local_file = open(file_path, 'w')
            local_file.write(u.read())
            local_file.close()

            tag = eyeD3.Tag()
            tag.link(file_path)
            tag.header.setVersion(eyeD3.ID3_V2_3)
            tag.encoding = '\x01'
            tag.setArtist(self.mp3.artist)
            tag.setAlbum(self.mp3.album)
            tag.update()
        except IOError, e:
            print 'operate file error:', e
            wx.PostEvent(self.notify_window,
                    DownloadEvent(self.index, STATUS_FAILED))
            return

        wx.PostEvent(self.notify_window, DownloadEvent(self.index, STATUS_SUCCEED))
        return

class Settings(wx.Frame):

    def __init__(self, parent, title):
        super(Settings, self).__init__(parent, title='Settings',
                size=(400, 200))

        self.InitUI()
        self.Centre()
        self.Show()

    def InitUI(self):
        panel = wx.Panel(self)

        sizer = wx.GridBagSizer(4, 4)

        text_setting = wx.StaticText(panel, label='Settings')
        sizer.Add(text_setting, pos=(0, 0), flag=wx.TOP|wx.LEFT|wx.BOTTOM,
                border=10)

        line = wx.StaticLine(panel)
        sizer.Add(line, pos=(1, 0), span=(1, 4),
                flag=wx.EXPAND|wx.BOTTOM, border=10)

        text_downfolder = wx.StaticText(panel, label='Download Folder')
        sizer.Add(text_downfolder, pos=(2, 0), flag=wx.LEFT, border=10)

        self.tcl_downfolder = wx.TextCtrl(panel)
        sizer.Add(self.tcl_downfolder, pos=(2, 1), span=(1, 2),
                flag=wx.EXPAND)

        btn_downfolder = wx.Button(panel, label='Browse...')
        sizer.Add(btn_downfolder, pos=(2, 3), flag=wx.RIGHT, border=10)

        line = wx.StaticLine(panel)
        sizer.Add(line, pos=(3, 0), span=(1, 4),
                flag=wx.EXPAND|wx.BOTTOM|wx.TOP, border=10)

        btn_ok = wx.Button(panel, label='Ok')
        sizer.Add(btn_ok, pos=(4, 2))
        btn_cancel = wx.Button(panel, label='Cancel')
        sizer.Add(btn_cancel, pos=(4, 3))

        sizer.AddGrowableCol(1)
        panel.SetSizer(sizer)

        btn_ok.Bind(wx.EVT_BUTTON, self.OnOkClicked)
        btn_cancel.Bind(wx.EVT_BUTTON, self.OnCancelClicked)

        # set download folder
        self.tcl_downfolder.SetValue(global_setting.dpath)

    def OnOkClicked(self, e):
        global global_setting

        global_setting.dpath = self.tcl_downfolder.GetValue()
        global_setting.WriteConfigToFile()

        self.Close()

    def OnCancelClicked(self, e):
        self.Close()


class Main(wx.Frame):

    def __init__(self, parent, title):
        super(Main, self).__init__(parent, title='Xiami Downloader',
            size=(600, 500))

        self.InitUI()
        self.Centre()
        self.Show()

    def InitUI(self):

        self.panel = wx.Panel(self)
        font = wx.SystemSettings_GetFont(wx.SYS_SYSTEM_FONT)
        font.SetPointSize(9)

        vbox = wx.BoxSizer(wx.VERTICAL)

        # Toolbar
        toolbar = self.CreateToolBar()
        toolbar.SetToolBitmapSize((16, 16))
        toolbar.AddSeparator()
        setting_tool = toolbar.AddLabelTool(wx.ID_ANY,
                'Settings', wx.Bitmap('settings.png'))
        toolbar.AddSeparator()
        toolbar.Realize()

        self.Bind(wx.EVT_TOOL, self.OnSettings, setting_tool)
        # end of toolbar

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

        # bind dowloading event
        EVT_RESULT(self, self.OnDownloadEvent)

        self.panel.SetSizer(vbox)

    def GetAlbumAddr(self, url):
        albumAddr = ''

        m = re.search('/album/[0-9]+$', url)
        if m is not None:
            m = re.search('[0-9]+$', url)
            albumId = m.group()
            albumAddr = "http://www.xiami.com/song/playlist/id/" + albumId + "/type/1"
            return albumAddr

        m = re.search('/showcollect/id/[0-9]+$', url)
        if m is not None:
            m = re.search('[0-9]+$', url)
            albumId = m.group()
            albumAddr = "http://www.xiami.com/song/playlist/id/" + albumId + "/type/3"
            return albumAddr

        m = re.search('/artist/[0-9]+$', url)
        if m is not None:
            data = AttriveData(url)
            if len(data) > 0:
                parser = ArtistHotParser()
                parser.feed(data)

                baseUrl = 'http://www.xiami.com/song/playlist/id/'
                first = True
                print parser.data
                for songId in parser.data:
                    if first:
                        baseUrl += songId
                        first = False
                    else:
                        baseUrl += ',' + songId
                baseUrl += '/object_name/default/object_id/0'
                return baseUrl

        return albumAddr


    def UrlInfoBtnClicked(self, e):
        url = self.url_text.GetValue()
        url = url.strip()
        if len(url) > 0:
            albumAddr = self.GetAlbumAddr(url)

            if len(albumAddr) > 0 :
                self.mp3s = ParseXMLtoURLs(AttriveData(albumAddr))

                self.song_list.DeleteAllItems()
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
            thread = DownloadingThread(self, index, mp3)
            thread.setDaemon(True)
            thread.start()
            index += 1

    def OnDownloadEvent(self, e):
        index = e.index
        rslt = e.status

        message = ''
        if rslt == STATUS_SUCCEED:
            message = 'Done'
        elif rslt == STATUS_FAILED:
            message = 'Failed'
        elif rslt == STATUS_START:
            message = 'Downloading...'
        else:
            message = 'Unknown'

        self.song_list.SetStringItem(index, 3, message)

    def OnSettings(self, e):
        Settings(None, title='Settings')

def init():
    global HOME_DIR
    global PATH_SEP
    global CURR_DIR
    global global_setting

    HOME_DIR = os.getenv('HOME')
    PATH_SEP = os.sep
    CURR_DIR = os.curdir

    global_setting = SettingConfig()
    global_setting.LoadConfigFromFile()

def main():
    init()
    app = wx.App()
    Main(None, title='Xiami Downloader')
    app.MainLoop()

if __name__ == '__main__':
    main()

