package main

import (
    "fmt"
    "net"
    "os"
    "io/ioutil"
    "time"
)

var Host string = "192.168.75.2"
var Port string = "8180"

var ConnNum int = 20
var RequestNum int = 100000
var Thinktime time.Duration = 20 * time.Millisecond

var done_chan []chan bool
var data []byte

func recvResponse(index int, conn *net.TCPConn, req_num int) {
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

    done_chan[index] <- true
}

func sendRequest(index int, raddr *net.TCPAddr, req_num int) {
    conn, err := net.DialTCP("tcp", nil, raddr)
    if err != nil {
        fmt.Println("connect failed")
        return
    }

    go recvResponse(index, conn, req_num)

    for i := 0; i < req_num; i++ {
        cnt, err := conn.Write(data)
        if err != nil {
            fmt.Println("write error", cnt)
            break
        }

        time.Sleep(Thinktime)
    }

    fmt.Println("all requests sent")
}

func main() {
    filedata, err := ioutil.ReadFile("test.data")
    if err != nil {
        fmt.Println("read file test.data error");
        os.Exit(3)
    }

    done_chan = make([]chan bool, ConnNum)

    data = filedata

    raddr, err := net.ResolveTCPAddr("tcp", Host + ":" + Port)
    if err != nil {
        fmt.Println("resolve failed")
    }

    index := 0
    for index := 0; index < ConnNum; index++ {
        go sendRequest(index, raddr, RequestNum)
    }

    for i := 0; i < ConnNum; i++ {
        <- done_chan[i]
    }

    fmt.Println("done")

    return
}

