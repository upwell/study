#!/usr/bin/env python

import urllib

def GetLocation(encodedStr):
    segnum = int(encodedStr[0])
    data = encodedStr[1:]
    seglen = len(data) / segnum
    extralen = len(data) % segnum
    segs = []
    i = 0

    while i < extralen:
        start = (seglen + 1) * i
        segs.append(data[start:(start + seglen + 1)])
        i += 1

    i = extralen
    while i < segnum:
        start = seglen * (i-extralen) + (seglen+1) * extralen
        segs.append(data[start:(start+seglen)])
        i += 1

    i = 0
    decodedStr = ""
    while i < len(segs[0]):
        j = 0
        while j < len(segs):
            if i < len(segs[j]):
                decodedStr = decodedStr + segs[j][i]
            j += 1
        i += 1

    decodedStr = urllib.unquote(decodedStr)
    rtnStr = ""
    i = 0
    while i < len(decodedStr):
        if decodedStr[i] == '^':
            rtnStr = rtnStr + "0";
        else:
            rtnStr = rtnStr + decodedStr[i]
        i += 1

    rtnStr = rtnStr.replace('+', ' ');
    return rtnStr

def test():
    #sample = '8h2xt72E1%2ptFi%%8%7583t%a22%27E3p2mFF25%73%Fi24F25743f.331%E25A1n58%51_.%.e165E62m'
    sample = '4h%2Fxit85256F12186E63pt3Ffi.%%9FE4%%%7%31783tA%3an25845%52565%_3.p%2.meFE%%42E5E9E515m'
    print GetLocation(sample)

if __name__ == '__main__':
    test()
