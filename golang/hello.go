package main

import "fmt"
import "strings"

func main() {
    sum := 0;
    for i := 0; i < 10; i++ {
        sum += i
    }
    fmt.Println(sum)

    str := "I am in beijing"
    strs := strings.Fields(str)
    for _, value := range strs {
        fmt.Println(value)
    }
}
