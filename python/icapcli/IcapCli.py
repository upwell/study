import socket

class IcapReq(object):
    'icap request entry class'
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.conn = None

    def set_url(self, url, sid):
        self.url = url
        self.sid = sid
        self.inc_acc = False

    def set_url_acc(self, url, sid, acctype, account):
        self.url = url
        self.sid = sid
        self.acctype = acctype
        self.account = account
        self.inc_acc = True

    def get_conn(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((self.host, self.port))

        self.conn = sock

    def close_conn(self):
        self.conn.close()

    def get_req_str(self):
        str = 'REQMOD icap://%s:%d/REQ-Service ICAP/1.0\r\n' % (self.host, self.port)
        str += 'Host: %s:%d\r\n' % (self.host, self.port)
        str += 'Allow: 204\r\n'
        str += 'X-Session-ID: %s\r\n' % (self.sid)

        if self.inc_acc:
            str += 'X-Subscriber-Type: %s\r\n' % (self.acctype)
            str += 'X-Subscriber-Data: %s\r\n' % (self.account)

        http_str = 'GET %s HTTP/1.0\r\n' % (self.url)
        str += 'Encapsulated: req-hdr=0, null-body=%d\r\n' % len(http_str)
        str += '\r\n'

        str += http_str
        str += '\r\n'
        return str

    def issue_req(self):
        if self.conn is None:
            self.get_conn()
        sock = self.conn
        sock.sendall(self.get_req_str())
        data = sock.recv(2048)
        return data

def test():
    req = IcapReq('10.64.75.193', 1344)
    req.set_url('http://www.baidu.com/', '00000000000000000')
    data = req.issue_req()
    print "===== ICAP Response =====\n%s" % data
    req.set_url('http://www.qq.com/', '11111111111111111')
    data = req.issue_req()
    print "===== ICAP Response =====\n%s" % data
    req.close_conn()

if __name__ == '__main__':
    test()
