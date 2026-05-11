

def markdownline3(str1, str2, str3):
    r = "| " + str1 + " | " + str2 + " | " + str3 + " |"
    return r

def markdownline4(str1, str2, str3, str4):
    r = "| " + str1 + " | " + str2 + " | " + str3 + " | " + str4 + " |"
    return r

def markdownsep(cols):
    r = ""
    r += '|'
    for i in range(cols):
        r+= "-----|"
    return r

def gen_mrkd_tbl3(hdr : list[str], arr1, arr2, arr3 ) -> list[str]:
    r = []
    r.append(markdownline3(hdr[0],hdr[1],hdr[2]))
    r.append(markdownsep(len(hdr)))
    for i in range(len(arr1)):
        r.append( markdownline3(arr1[i], arr2[i], arr3[i] ) )
    return r

def gen_mrkd_tbl4(hdr : list[str], arr1, arr2, arr3, arr4 ) -> list[str]:
    r = []
    r.append(markdownline4(hdr[0],hdr[1],hdr[2],hdr[3]))
    r.append(markdownsep(len(hdr)))
    for i in range(len(arr1)):
        r.append(markdownline4(arr1[i],arr2[i],arr3[i],arr4[i]))
    return r


def gen_mrkd_arch( jret ):
    r = []
    return r
