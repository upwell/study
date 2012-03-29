package main

import (
    "fmt"
    "net"
    "bufio"
    "strings"
    "crypto/md5"
    . "time"
)


type Request struct {
    url string
    guid string
    start Time
    end Time
    respond bool
}

const Host = string("10.64.75.194")
const Port = string("1344")

var request_map map[string]Request
var req_data chan string
var resp_data chan string

var done_chan chan bool

var TOTAL_REQ_NUM uint32 = 1000000

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

        url := "http://www.baidu.com"
        uuid := getMd5(i)
        starttime := Now()
        endtime := starttime
        respond := false

        request := Request{
            url, uuid, starttime, endtime, respond,
        }

        request_map[uuid] = request

        http_str := fmt.Sprintf("GET %s HTTP/1.0\r\n\r\n", url)
        http_len := len(http_str)

        req_str := fmt.Sprintf("REQMOD icap://%s:%s/REQ-Service ICAP/1.0\r\n" + "Host:%s:%s\r\n" + "Allow: 204\r\n" + "X-Session-ID: %s\r\n" + "Encapsulated: req-hdr=0, null-body=%d\r\n\r\n" + "%s", Host, Port, Host, Port, uuid, http_len, http_str)

        req_data <- req_str
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

        str_data := <-req_data
        cnt, err := conn.Write([]byte(str_data))
        if err != nil {
            fmt.Println("write error %d", cnt)
        }
        i++
    }
}

func receiveResponse(conn net.Conn)  {
    var i uint32 = 0
    reader := bufio.NewReader(conn)
    var data string;

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
            resp_data <- data
            data = ""
        }
    }
}

func parseResponse() {
    var i uint32 = 0
    for {
        if i >= TOTAL_REQ_NUM {
            fmt.Println("parse all response done")
            done_chan <- true
            return
        }

        resp_str := <-resp_data
        fmt.Sprintf("%s", resp_str)
        i++
    }
}

func main() {
    request_map = make(map[string]Request)
    req_data = make(chan string)
    resp_data = make(chan string)
    done_chan = make(chan bool)

    /*
    raddr, err := net.ResolveTCPAddr("tcp", Host + ":" + Port)
    if err != nil {
        fmt.Println("resolve failed")
    }
    */

    conn, err := net.Dial("tcp", Host + ":" + Port)
    if err != nil {
        fmt.Println("connect failed")
    }

    go prepareRequest()
    go sendRequest(conn)
    go receiveResponse(conn)
    go parseResponse()

    done := <-done_chan
    fmt.Println(done)
}

