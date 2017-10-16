type  test1.txt
.\regEngine abcdef test1.txt
.\regEngine "(abcdef)|abc" test1.txt
.\regEngine a*b+c?cdef test1.txt
.\regEngine abcd([0-9]+) test1.txt
.\regEngine [A-Z]bcd[\w][a-z][0-9]G test1.txt
.\regEngine [\w]+@[a-z]+.com test1.txt
