package main

import (
    "fmt"
    "net"
    "os"
    "io/ioutil"
    "time"
)

var Host string = "10.64.75.199"
var Port string = "8180"

var Thinktime time.Duration = 0 * time.Millisecond

var done_chan chan bool

var data []byte

func recvResponse(conn *net.TCPConn, req_num int) {
    total_bytes := req_num * 42
    recv_bytes := 0
    for {
        buff := make([]byte, 4096)

        n, err := conn.Read(buff)
        if err != nil {
            fmt.Println("read error", n);
            break;
        }
        recv_bytes += n

        if recv_bytes == total_bytes {
            fmt.Println("all data received")
            break
        }
    }

    done_chan <- true
}

func sendRequest(raddr *net.TCPAddr, req_num int) {
    conn, err := net.DialTCP("tcp", nil, raddr)
    if err != nil {
        fmt.Println("connect failed")
        return
    }

    go recvResponse(conn, req_num)

    i := 0
    for {
        cnt, err := conn.Write(data)
        if err != nil {
            fmt.Println("write error", cnt)
            break
        }

        time.Sleep(Thinktime)
        i++
        if i >= req_num {
            fmt.Println("all requests sent")
            break
        }
    }
}

func main() {

    filedata, err := ioutil.ReadFile("test.data")
    if err != nil {
        fmt.Println("read file test.data error");
        os.Exit(3)
    }

    done_chan = make(chan bool)

    data = filedata

    request_num := 10000

    raddr, err := net.ResolveTCPAddr("tcp", Host + ":" + Port)
    if err != nil {
        fmt.Println("resolve failed")
    }

    go sendRequest(raddr, request_num)

    done := <-done_chan
    fmt.Println(done)

    return
}

