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
    . "time"
)


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

const Host = string("10.64.75.194")
const Port = string("1344")

var request_map map[string]Request
var req_data chan ReqData
var resp_data chan RespData

var done_chan chan bool

var TOTAL_REQ_NUM uint32 = 10000000

//var net_lock sync.Mutex
var map_lock sync.Mutex

func getMd5(in uint32) string {
    hash := md5.New()
    hash.Write([]byte(string(in)))

    return fmt.Sprintf("%x", hash.Sum(nil))
}

func prepareRequest() {
    var i uint32 = 0
    for {
        if i >= TOTAL_REQ_NUM {
            fmt.Println("request are all ready")
            return
        }

        url := "http://www.google.com"
        uuid := getMd5(i)

        http_str := fmt.Sprintf("GET %s HTTP/1.0\r\n\r\n", url)
        http_len := len(http_str)

        req_str := fmt.Sprintf("REQMOD icap://%s:%s/REQ-Service ICAP/1.0\r\n" + "Host:%s:%s\r\n" + "Allow: 204\r\n" + "X-Session-ID: %s\r\n" + "Encapsulated: req-hdr=0, null-body=%d\r\n\r\n" + "%s", Host, Port, Host, Port, uuid, http_len, http_str)

        req_data <- ReqData{req_str, uuid}
        i++
    }
}

func sendRequest(conn net.Conn) {
    var i uint32 = 0
    for {
        if i >= TOTAL_REQ_NUM {
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
        if i >= TOTAL_REQ_NUM {
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
        if i >= TOTAL_REQ_NUM {
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

        if i >= TOTAL_REQ_NUM {
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
        if i >= TOTAL_REQ_NUM {
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

    average := total / int64(TOTAL_REQ_NUM)

    fmt.Printf("Average [%d us]\nLongest [%d us]\n", average, longest)
}

func usage() {
    fmt.Printf("Usage: ./%s <req_num>\n", os.Args[0])
    flag.PrintDefaults()
    os.Exit(2)
}

func main() {
    flag.Usage = usage
    flag.Parse()

    args := flag.Args()
    if len(args) != 1 {
        fmt.Printf("<req_num> missing, use default value [%d]\n", TOTAL_REQ_NUM)
    } else {
        val, err := strconv.ParseInt(args[0], 10, 32)
        if(err != nil) {
            fmt.Printf("parse [%s] error, use default value [%d]\n", args[0], TOTAL_REQ_NUM)
        } else {
            TOTAL_REQ_NUM = uint32(val)
            fmt.Printf("peform number of request [%d]\n", TOTAL_REQ_NUM)
        }
    }

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

    tps := float64(TOTAL_REQ_NUM) / g_period
    fmt.Printf("TPS is [%f] in [%f s] [%d ns]\n", tps, g_period, g_ns)
}

