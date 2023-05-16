        lw      0       2       val        load reg1 with 5 (symbolic address)
        sw      0       3       seven      load reg2 with -1 (numeric address)
        sw      0       1       one
        lw      0       5       six
        noop
        nor     1       2       4
        nor     5       4       2
        add     3       3       2          decrement reg1
        add     2       2       2          decrement reg1
        add     4       3       3          decrement reg1
        nor     3       5       5
        lw      3       2       3
        lw      2       2       1
        noop
        halt
seven   .fill   23
one     .fill   0
val     .fill   32
result  .fill   12
six     .fill   -56
