package main

import (
    "fmt"
    "log"
    "io/ioutil"
    "regexp"
    "time"
    "os"
    "runtime/pprof"
)

var exps = []string {
    "Twain",
    "(?i)Twain",
    "[a-z]shing",
    "Huck[a-zA-Z]+|Saw[a-zA-Z]+",
    "\\b\\w+nn\\b",
    "[a-q][^u-z]{13}x",
    "Tom|Sawyer|Huckleberry|Finn",
    "(?i)Tom|Sawyer|Huckleberry|Finn",
    ".{0,2}(Tom|Sawyer|Huckleberry|Finn)",
    ".{2,4}(Tom|Sawyer|Huckleberry|Finn)",
    "Tom.{10,25}river|river.{10,25}Tom",
    "[a-zA-Z]+ing",
    "\\s[a-zA-Z]{0,12}ing\\s",
    "([A-Za-z]awyer|[A-Za-z]inn)\\s",
    "[\"'][^\"']{0,30}[?!\\.][\"']",
    "\u221E|\u2713",
    "\\p{Sm}",                       // any mathematical symbol
}

func main() {
    content, err := ioutil.ReadFile("data/mtent12.txt")
    if err != nil {
        log.Fatal(err)
    }

    var patterns []*regexp.Regexp 
    content_str := string(content)

    start := time.Now()
    for _, exp := range(exps) {
        //fmt.Println(exp, "->", regexp.QuoteMeta(exp))
        r , err := regexp.Compile(exp)
        //r , err := regexp.CompilePOSIX(exp)
        if err != nil {
            log.Fatal("Compile err")
            continue
        }
        patterns = append(patterns, r)
    }
    fmt.Println("Compile: ", time.Since(start))

    f, _ := os.Create("profile_file") 
    pprof.StartCPUProfile(f)
    defer pprof.StopCPUProfile()

    start2 := time.Now()
    for i, p := range(patterns) {
        start = time.Now()
        //x := p.FindAll(content, len(content))
       // x := p.FindAllIndex(content, len(content))
        x := p.FindAllStringIndex(content_str, len(content_str))
        fmt.Printf("%02d: find:%-08d takes: %s \n", i, len(x),  time.Since(start))
    }
    fmt.Println("takes: ", time.Since(start2))
}
