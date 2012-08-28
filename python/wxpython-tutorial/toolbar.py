#!/usr/bin/python

import wx

class Example(wx.Frame):

	def __init__(self, *args, **kwargs):
		super(Example, self).__init__(*args, **kwargs)

		self.InitUI()

	def InitUI(self):

		toolbar = self.CreateToolBar()
		qtool = toolbar.AddLabelTool(wx.ID_ANY,
			'QUIT', wx.Bitmap('exit.png'))
		toolbar.Realize()

		self.Bind(wx.EVT_TOOL, self.OnQuit, qtool)

		self.SetSize((200, 250))
		self.SetTitle('Toolbar')
		self.Centre()
		self.Show(True)

	def OnQuit(self, e):
		self.Close()

def main():
	ex = wx.App()
	Example(None)
	ex.MainLoop()

if __name__ == '__main__':
	main()