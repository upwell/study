#!/usr/bin/env python

import wx
from IcapCli import IcapReq


# Create a new frame class, derived from the wxPython Frame.
class IcapFrame(wx.Frame):

    def __init__(self, parent, title):
        super(IcapFrame, self).__init__(parent, title=title,
                size=(780, 700))


        # Add a panel and some controls to display the size and position
        panel = wx.Panel(self)
        border = wx.BoxSizer(wx.VERTICAL)

        label_icap_host = wx.StaticText(panel, label="ICAP Host:")
        self.icap_host_ctrl = wx.TextCtrl(panel, size=(200,25))
        label_icap_port = wx.StaticText(panel, label="ICAP Port:")
        self.icap_port_ctrl = wx.TextCtrl(panel, size=(50,25), value='1344')

        label_session_id = wx.StaticText(panel, label="Session ID:")
        self.session_id_ctrl = wx.TextCtrl(panel, value='11111111111111111')
        label_http_url = wx.StaticText(panel, label="HTTP URL:")
        self.http_url_ctrl = wx.TextCtrl(panel, value='http://')

        label_account_type = wx.StaticText(panel, label="Account Type:")
        self.account_type_ctrl = wx.TextCtrl(panel)
        label_account = wx.StaticText(panel, label="Account:")
        self.account_ctrl = wx.TextCtrl(panel)

        self.inc_account_cb = wx.CheckBox(panel, label="Send with Account")

        self.panel = panel

        gs = wx.FlexGridSizer(5, 4, 5, 5)
        gs.Add(label_icap_host, flag=wx.EXPAND)
        gs.Add(self.icap_host_ctrl, flag=wx.EXPAND)
        gs.Add(label_icap_port, flag=wx.EXPAND)
        gs.Add(self.icap_port_ctrl, flag=wx.EXPAND)
        gs.Add(label_session_id, flag=wx.EXPAND)
        gs.Add(self.session_id_ctrl, flag=wx.EXPAND)
        gs.Add(wx.StaticText(panel, label=''), flag=wx.EXPAND)
        gs.Add(wx.StaticText(panel, label=''), flag=wx.EXPAND)
        gs.Add(label_http_url, flag=wx.EXPAND)
        gs.Add(self.http_url_ctrl, flag=wx.EXPAND)
        gs.Add(wx.StaticText(panel, label=''), flag=wx.EXPAND)
        gs.Add(wx.StaticText(panel, label=''), flag=wx.EXPAND)
        gs.Add(wx.StaticText(panel, label=''), flag=wx.EXPAND)
        gs.Add(self.inc_account_cb, flag=wx.EXPAND)
        gs.Add(wx.StaticText(panel, label=''), flag=wx.EXPAND)
        gs.Add(wx.StaticText(panel, label=''), flag=wx.EXPAND)
        gs.Add(label_account, flag=wx.EXPAND)
        gs.Add(self.account_ctrl, flag=wx.EXPAND)
        gs.Add(label_account_type, flag=wx.EXPAND)
        gs.Add(self.account_type_ctrl, flag=wx.EXPAND)
        border.Add(gs, flag=wx.EXPAND)
        border.Add((-1,10))

        self.go_button = wx.Button(panel, -1, label='Go')
        self.go_button.Bind(wx.EVT_BUTTON, self.click_go)

        self.reset_button = wx.Button(panel, -1, label='Reset')
        self.reset_button.Bind(wx.EVT_BUTTON, self.click_reset)

        self.clear_rslt_btn = wx.Button(panel, -1, label='Clear Result')
        self.clear_rslt_btn.Bind(wx.EVT_BUTTON, self.clear_rslt)

        self.result_ctrl = wx.TextCtrl(panel, -1, "",
                style=wx.TE_READONLY|wx.TE_MULTILINE)

        box_button = wx.BoxSizer(wx.HORIZONTAL)
        box_button.Add(self.go_button, wx.LEFT, border=20)
        box_button.Add(self.reset_button, wx.LEFT, border=20)
        box_button.Add(self.clear_rslt_btn, wx.LEFT, border=20)
        border.Add(box_button, flag=wx.ALL, border=10)

        box_result = wx.BoxSizer(wx.VERTICAL)
        box_result.Add(self.result_ctrl, proportion=1, flag=wx.EXPAND, border=10)
        border.Add(box_result, proportion=1, flag=wx.LEFT|wx.RIGHT|wx.EXPAND, border=10)
        border.Add((-1,25))

        panel.SetSizer(border)

    def click_go(self, event):
        self.go_button.Hide()

        is_inc_acc = self.inc_account_cb.IsChecked()
        is_ok = False

        if self.icap_host_ctrl.IsEmpty() or \
                self.icap_port_ctrl.IsEmpty() or \
                self.http_url_ctrl.IsEmpty() or \
                self.session_id_ctrl.IsEmpty() :
            pass
        else:
            req = IcapReq(self.icap_host_ctrl.Value,
                    int(self.icap_port_ctrl.Value))
            if is_inc_acc:
                if self.account_type_ctrl.IsEmpty() or \
                        self.account_ctrl.IsEmpty() :
                    pass
                else:
                    req.set_url_acc(self.http_url_ctrl.Value,
                            self.session_id_ctrl.Value,
                            self.account_type_ctrl.Value,
                            self.account_ctrl.Value)
                    is_ok = True
            else:
                req.set_url(self.http_url_ctrl.Value,
                        self.session_id_ctrl.Value)
                is_ok = True

        if not is_ok:
            self.result_ctrl.WriteText('oops, please check, there is at least one empty setting.\n')
        else:
            data = req.issue_req()
            self.result_ctrl.WriteText(data)
            req.close_conn()

        self.go_button.Show()

    def click_reset(self, event):
        self.icap_host_ctrl.Clear()
        self.icap_port_ctrl.SetValue('1344')
        self.http_url_ctrl.SetValue('http://')
        self.session_id_ctrl.SetValue('11111111111111111')
        self.account_type_ctrl.Clear()
        self.account_ctrl.Clear()
        self.inc_account_cb.SetValue((False))

    def clear_rslt(self, event):
        self.result_ctrl.Clear()


# Every wxWidgets application must have a class derived from wx.App
class IcapApp(wx.App):

    # wxWindows calls this method to initialize the application
    def OnInit(self):

        # Create an instance of our customized Frame class
        frame = IcapFrame(None, "Icap Request Tool")
        frame.Centre()
        frame.Show()

        # Tell wxWindows that this is our main window
        self.SetTopWindow(frame)

        # Return a success flag
        return True



app = IcapApp()     # Create an instance of the application class
app.MainLoop()     # Tell it to start processing events
