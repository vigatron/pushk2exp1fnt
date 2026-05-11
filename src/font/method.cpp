#include "method.hpp"


const uint8_t method[256] = {
            opN,        // 0   N
            opH,        // 1   H
            opH,        // 2   H
            opH,        // 3   H
            opV,        // 4   V
            opH,        // 5   H
            opH,        // 6   H
            opV,        // 7   V
            opV,        // 8   V
            opV,        // 9   V
            opV,        // 10  V
            opN,        // 11  N
            opH,        // 12  H
            opN,        // 13
            opN,        // 14
            opV,        // 15
            opV,        // 16  >
            opV,        // 17  <
            opV,        // 18
            opH,        // 19
            opN,        // 20
            opN,        // 21
            opH,        // 22
            opV,        // 23
            opH,        // 24
            opH,        // 25
            opV,        // 26
            opV,        // 27
            opN,        // 28
            opV,        // 29
            opH,        // 30
            opH,        // 31

            opN,        // 32
            opH,        // 33  !
            opH,        // 34  "
            opH,        // 35  #
            opV,        // 36  $
            opN,        // 37  %
            opN,        // 38  &
            opN,        // 39  `
            opV,        // 40  (
            opV,        // 41  )
            opV,        // 42  *
            opV,        // 43  +
            opN,        // 44  ,
            opH,        // 45  -
            opN,        // 46  .
            opN,        // 47  /
            opN,        // 48  0
            opV,        // 49  1
            opN,        // 50  2
            opV,        // 51  3
            opN,        // 52  4
            opN,        // 53  5
            opN,        // 54  6
            opN,        // 55  7
            opV,        // 56  8
            opN,        // 57  9
            opV,        // 58  :
            opV,        // 59  ;
            opV,        // 60  <
            opV,        // 61  =
            opV,        // 62  >
            opN,        // 63  ?

            opN,        // 64  @
            opH,        // 65  A
            opV,        // 66  B
            opV,        // 67  C
            opV,        // 68  D
            opV,        // 69  E
            opV,        // 70  F
            opN,        // 71  G

            opV,        // 72  H
            opV,        // 73  I
            opN,        // 74  J
            opV,        // 75  K
            opN,        // 76  L
            opH,        // 77  M
            opN,        // 78  N
            opV,        // 79  O


            opV,        // 80  P
            opV,        // 81  Q
            opN,        // 82  R
            opN,        // 83  S
            opV,        // 84  T
            opH,        // 85  U
            opH,        // 86  V
            opH,        // 87  W

            opV,        // 88  X
            opH,        // 89  Y
            opN,        // 90  Z
            opV,        // 91  [
            opN,        // 92  
            opV,        // 93  ]
            opH,        // 94  ^
            opH,        // 95  _

            opN,        // 96   `
            opN,        // 97   a
            opN,        // 98   b
            opV,        // 99   c
            opN,        // 100  d
            opN,        // 101  e
            opN,        // 102  f
            opN,        // 103  g

            opN,        // 104  h
            opV,        // 105  i
            opN,        // 106  j
            opV,        // 107  k
            opV,        // 108  l
            opH,        // 109  m
            opH,        // 110  n
            opV,        // 111  o

            opN,        // 112  p
            opN,        // 113  q
            opN,        // 114  r
            opN,        // 115  s
            opN,        // 116  t
            opH,        // 117  u
            opH,        // 118  v
            opH,        // 119  w

            opV,        // 120  x
            opN,        // 121  y
            opN,        // 122  z
            opV,        // 123  {
            opV,        // 124  |
            opV,        // 125  }
            opN,        // 126  ~
            opH,        // 127  


            opN,        // 128  A
            opN,        // 129  Б
            opV,        // 130  В
            opV,        // 131  Г
            opH,        // 132  Д
            opV,        // 133  Е
            opV,        // 134  Ж
            opV,        // 135  З
            opH,        // 136  И
            opH,        // 137  Й
            opV,        // 138  К
            opN,        // 139
            opH,        // 140  М
            opV,        // 141  Н
            opV,        // 142  О
            opV,        // 143  П
            opN,        // 144  Р
            opV,        // 145  С
            opH,        // 146  Т
            opN,        // 147  У
            opH,        // 148  Ф
            opV,        // 149  Х
            opH,        // 150  Ц
            opN,        // 151  Ч
            opH,        // 152  Ш
            opH,        // 153  Щ
            opN,        // 154  Ъ
            opH,        // 155  Ы
            opN,        // 156  Ь
            opV,        // 157  Э
            opV,        // 158  Ю
            opN,        // 159  Я

            opN,        // 160  а
            opN,        // 161  б
            opV,        // 162  в
            opV,        // 163  г
            opH,        // 164  д
            opV,        // 165  е
            opV,        // 166  ж
            opV,        // 167  з
            opH,        // 168  и
            opH,        // 169  й
            opV,        // 170  к
            opN,        // 171  л
            opH,        // 172  м
            opV,        // 173  н
            opV,        // 174  о
            opV,        // 175  п
            opN,        // 176  sh1
            opN,        // 177  sh2
            opN,        // 178  sh3
            opG,        // 179  ps  : U+D
            opG,        // 180  ps  : U+D+L
            opG,        // 181  ps  : U+D+LL
            opG,        // 182  ps  : UU+DD+L
            opG,        // 183  ps  : DD+L
            opG,        // 184  ps  : D+LL
            opG,        // 185  ps  : UU+DD+LL
            opG,        // 186  ps  : UU+DD
            opG,        // 187  ps  : DD+LL
            opG,        // 188  ps  : UU+LL
            opG,        // 189  ps  : UU+L
            opG,        // 190  ps  : U+LL
            opG,        // 191  ps  : D+L

            opG,        // 192  ps  : U+R
            opG,        // 193  ps  : U+L+R
            opG,        // 194  ps
            opG,        // 195  ps
            opG,        // 196  ps
            opG,        // 197  ps  +
            opG,        // 198  ps
            opG,        // 199  ps
            opG,        // 200  ps
            opG,        // 201  ps
            opG,        // 202  ps
            opG,        // 203  ps
            opG,        // 204  ps
            opG,        // 205  ps
            opG,        // 206  ps  ++
            opG,        // 207  ps
            opG,        // 208  ps
            opG,        // 209  ps
            opG,        // 210  ps
            opG,        // 211  ps
            opG,        // 212  ps
            opG,        // 213  ps
            opG,        // 214  ps
            opG,        // 215  ps
            opG,        // 216  ps
            opG,        // 217  ps
            opG,        // 218  pseudo last
            opN,        // 219  sys2
            opH,        // 220  sys2
            opV,        // 221  sys2
            opV,        // 222  sys2
            opH,        // 223  sys2

            opN,        // 224 р
            opV,        // 225 с
            opV,        // 226 т
            opN,        // 227 у
            opV,        // 228 ф
            opV,        // 229 х
            opN,        // 230 ц
            opN,        // 231 ч
            opH,        // 232 ш
            opN,        // 233 щ
            opN,        // 234 ъ
            opN,        // 235 ы
            opN,        // 236 ь
            opV,        // 237 э
            opV,        // 238 ю
            opN,        // 239 я
            opN,        // 240   Ё
            opN,        // 241   ё
            opV,        // 242 >
            opV,        // 243 <
            opN,        // 244
            opN,        // 245
            opV,        // 246 
            opN,        // 247
            opH,        // 248
            opN,        // 249
            opN,        // 250
            opN,        // 251
            opN,        // 252
            opN,        // 253
            opN,        // 254
            opN         // 255

};


uint8_t getmethod(uint8_t symn) {
    return method[symn];
}
