package main

import (
    "fmt"
    "crypto/md5"
)

func main() {
    h := md5.New()

    for i := 0; i < 10; i++ {
        h.Write([]byte(string(i)))
        rslt := fmt.Sprintf("%x", h.Sum(nil))
        fmt.Println(rslt)
    }
}
