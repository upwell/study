package main

import (
    "fmt"
    "net"
    "bufio"
    "strings"
    "crypto/md5"
    "sync"
    "os"
    "flag"
    "strconv"
    "io/ioutil"
    "encoding/json"
    "math/rand"
    . "time"
)

type CfgMsg struct {
    IcapHost string
    IcapPort string
    ConnNum int
    ReqNum string
    IcapReq []CfgReq
}

type CfgReq struct {
    HttpUrl string
    IsWithUser bool
    Username string
}

type Request struct {
    start Time
    end Time
    respond bool
}

type ReqData struct {
    data string
    guid string
}

type RespData struct {
    data string
    end Time
}

var Host string = "10.64.75.194"
var Port string = "1344"
var ConnNum int = 1
var TotalReqNum uint32 = 10000000

var Cfg CfgMsg

var request_map map[string]Request
var req_data chan ReqData
var resp_data chan RespData
var done_chan chan bool


//var net_lock sync.Mutex
var map_lock sync.Mutex

func getMd5(in uint32) string {
    hash := md5.New()
    hash.Write([]byte(string(in)))

    return fmt.Sprintf("%x", hash.Sum(nil))
}

func getCfgReq() CfgReq {
    var req CfgReq

    req_len := len(Cfg.IcapReq)
    if req_len > 0 {
        req = Cfg.IcapReq[rand.Int() % req_len]
    } else {
        req = CfgReq { "http://www.google.com", false, "" }
    }

    return req
}

func prepareRequest() {
    var i uint32 = 0
    for {
        if i >= TotalReqNum {
            fmt.Println("request are all ready")
            return
        }

        req := getCfgReq()

        url := req.HttpUrl
        uuid := getMd5(i)

        http_str := fmt.Sprintf("GET %s HTTP/1.0\r\n\r\n", url)
        http_len := len(http_str)

        req_str := fmt.Sprintf("REQMOD icap://%s:%s/REQ-Service ICAP/1.0\r\n" + "Host:%s:%s\r\n" + "Allow: 204\r\n" + "X-Session-ID: %s\r\n", Host, Port, Host, Port, uuid)

        if req.IsWithUser {
            req_str += "X-Subscriber-Type: 0\r\nX-Subscriber-Data: " + req.Username + "\r\n"
        }

        req_str += fmt.Sprintf("Encapsulated: req-hdr=0, null-body=%d\r\n\r\n" + "%s", http_len, http_str)

        req_data <- ReqData{req_str, uuid}
        i++
    }
}

func sendRequest(conn net.Conn) {
    var i uint32 = 0
    for {
        if i >= TotalReqNum {
            fmt.Println("send request completed")
            return
        }

        reqd := <-req_data
        cnt, err := conn.Write([]byte(reqd.data))

        start_time := Now()
        end_time := start_time

        request := Request{ start_time, end_time, false }
        map_lock.Lock()
        request_map[reqd.guid] = request
        map_lock.Unlock()

        if err != nil {
            fmt.Println("write error %d", cnt)
        }
        i++
    }
}

func receiveResponse(conn net.Conn)  {
    var i uint32 = 0
    reader := bufio.NewReader(conn)
    data := ""

    for {
        if i >= TotalReqNum {
            fmt.Println("receive response completed");
            return
        }

        line, err := reader.ReadString('\n')
        if err != nil {
            fmt.Println("read error")
            return
        }

        data += line
        if strings.Contains(data, "\r\n\r\n") {
            i++
            resp_data <- RespData{data, Now()}
            data = ""
        }
    }
}

func sendRequestNew(conn *net.TCPConn) {
    var i uint32 = 0
    for {
        if i >= TotalReqNum {
            fmt.Println("send request completed")
            return
        }

        reqd := <-req_data
        //net_lock.Lock()
        cnt, err := conn.Write([]byte(reqd.data))
        //net_lock.Unlock()

        start_time := Now()
        end_time := start_time

        request := Request{ start_time, end_time, false }

        map_lock.Lock()
        request_map[reqd.guid] = request
        map_lock.Unlock()

        if err != nil {
            fmt.Println("write error %d", cnt)
        }
        i++
    }
}

func receiveResponseNew(conn *net.TCPConn)  {
    var i uint32 = 0
    var data string = ""

    for {
        buff := make([]byte, 4096)

        if i >= TotalReqNum {
            fmt.Println("receive response completed");
            return
        }

        //net_lock.Lock()
        n, err := conn.Read(buff)
        //net_lock.Unlock()
        if err != nil {
            fmt.Println("read error");
            return
        }

        data += string(buff[0:n])

        for strings.Contains(data, "\r\n\r\n") {
            i++
            pos := strings.Index(data, "\r\n\r\n")
            tmp_data := data[0:pos+4]
            data = data[pos+4:]
            resp_data <- RespData{tmp_data, Now()}
        }
    }
}

func getSessionID(resp_str string) string {
    rslt := ""
    pos := strings.Index(resp_str, "X-Session-ID")
    if(pos != -1) {
        resp_str = resp_str[pos:]
        pos = strings.Index(resp_str, "\r\n")
        if(pos != -1) {
            resp_str = resp_str[0:pos]
            keyval := strings.Split(resp_str, ": ")
            if len(keyval) < 2 {
                fmt.Println(resp_str)
            }
            rslt = keyval[1]
        }
    }

    return rslt
}

func parseResponse() {
    var i uint32 = 0
    for {
        if i >= TotalReqNum {
            fmt.Println("parse all response done")
            done_chan <- true
            return
        }

        respd := <-resp_data
        val := getSessionID(respd.data)
        if val != "" {
            map_lock.Lock()
            if request,ok := request_map[val]; ok {
                request.respond = true
                request.end = respd.end
                request_map[val] = request
            } else {
                fmt.Println("parseResponse: no data for session " + val + " - " + respd.data)
            }
            map_lock.Unlock()
        }
        i++
    }
}

func analyzeResult() {
    var total int64 = 0
    var longest int64 = 0

    for _, req := range request_map {
        time := (req.end.UnixNano() - req.start.UnixNano()) / 1e3

        if time > longest {
            longest = time
        }

        total += time
    }

    average := total / int64(TotalReqNum)

    fmt.Printf("Average [%d us]\nLongest [%d us]\n", average, longest)
}

func usage() {
    fmt.Printf("Usage: ./%s <req_num>\n", os.Args[0])
    flag.PrintDefaults()
    os.Exit(2)
}

func isNotEmpty(val string) bool {
    return val != ""
}

func parseCfg(cfgfile string) {
    fmt.Println("parse config file", cfgfile)

    filedata, err := ioutil.ReadFile(cfgfile)
    if err != nil {
        fmt.Println("read file error", err)
        os.Exit(2)
    }

    err = json.Unmarshal(filedata, &Cfg)
    if err != nil {
        fmt.Println("parse config error", err)
        os.Exit(2)
    }

    if isNotEmpty(Cfg.IcapHost) {
        Host = Cfg.IcapHost
    }
    fmt.Println("icap host is", Host)

    if isNotEmpty(Cfg.IcapPort) {
        Port = Cfg.IcapPort
    }
    fmt.Println("icap port is", Port)

    if Cfg.ConnNum > 0 {
        ConnNum = Cfg.ConnNum
    }
    fmt.Println("conn number is", ConnNum)

    if isNotEmpty(Cfg.ReqNum) {
        tmpval, err := strconv.ParseInt(Cfg.ReqNum, 10, 32)
        if err != nil {
            fmt.Println("parse int error", err)
        } else {
            TotalReqNum = uint32(tmpval)
        }
    }
    fmt.Println("total request number is", TotalReqNum)
}

func main() {
    var cfg_file string

    flag.Usage = usage
    flag.StringVar(&cfg_file, "f", "icap.json", "running config file with json format")
    flag.Parse()

    parseCfg(cfg_file)

    request_map = make(map[string]Request)
    req_data = make(chan ReqData, 1024)
    resp_data = make(chan RespData, 1024)
    done_chan = make(chan bool)

    raddr, err := net.ResolveTCPAddr("tcp", Host + ":" + Port)
    if err != nil {
        fmt.Println("resolve failed")
    }

    conn, err := net.DialTCP("tcp", nil, raddr)
    //conn, err := net.Dial("tcp", Host + ":" + Port)
    if err != nil {
        fmt.Println("connect failed")
    }

    g_start := Now()

    go prepareRequest()
    //go sendRequest(conn)
    //go receiveResponse(conn)
    go sendRequestNew(conn)
    go receiveResponseNew(conn)
    go parseResponse()

    done := <-done_chan
    fmt.Println(done)
    g_end := Now()

    analyzeResult()

    g_ns := (g_end.UnixNano() - g_start.UnixNano())
    g_period := float64(g_ns) / float64(1e9)

    tps := float64(TotalReqNum) / g_period
    fmt.Printf("TPS is [%f] in [%f s] [%d ns]\n", tps, g_period, g_ns)
}

