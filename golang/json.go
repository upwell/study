package main

import (
    "fmt"
    "encoding/json"
    "os"
    "io/ioutil"
)

type Message struct {
    IcapHost string
    IcapPort string
    ConnNum int
    ReqNum string
    IcapReq []Request
}

type Request struct {
    HttpUrl string
    IsWithUser bool
    Username string
}

func main() {
    var filename string

    if(len(os.Args) == 2) {
        filename = os.Args[1]
    } else {
        fmt.Println("invalid input")
        os.Exit(2)
    }

    fmt.Println("filename is", filename)

    filedata, err := ioutil.ReadFile(filename)
    if err != nil {
        fmt.Println("read file error", err);
        return
    }

    var m Message

    err = json.Unmarshal(filedata, &m)
    if err != nil {
        fmt.Println("parse json error", err);
        return
    }

    fmt.Println(m.IcapHost)
}
