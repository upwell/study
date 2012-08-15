#!/usr/bin/python

import wx

class Example(wx.Frame):
	def __init__(self, *args, **kwargs):
		super(Example, self).__init__(*args, **kwargs)

		self.InitUI()

	def InitUI(self):
		menubar = wx.MenuBar()
		fileMenu = wx.Menu()
		fitem = fileMenu.Append(wx.ID_EXIT, 'Quit', 'Quit Application')
		menubar.Append(fileMenu, '&File')
		self.SetMenuBar(menubar)

		self.Bind(wx.EVT_MENU, self.OnQuit, fitem)

		self.SetSize((300, 200))
		self.SetTitle('Simple menu')
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
