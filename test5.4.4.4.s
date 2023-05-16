        lw      0       1       five    load reg1 with 5 (symbolic address)
        lw      1       5       3       load reg2 with -1 (numeric address)
        lw      0       4       neg1
        lw      1       3       stAddr
start   add     1       2       1       decrement reg1
sdfa    nor     1       4       5
        beq     0       5       0       goto end of program when reg1==0
        lw      0       5       next
        sw      3       3       2
        add     1       5       2
        sw      3       4       5
        jalr    1       1       1
        nor     3       2       5
done    halt                            end of program
five    .fill   5
what    .fill   673
next    .fill   -123
neg1    .fill   -1
