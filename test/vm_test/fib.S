addi sp, sp, 512 # init stack
addi a0, a0, 10 # calc fib(10)
jal ra, fib
addi a1, a0, 0
addi a0, x0, 3
ecall # print result
addi a0, x0, 0
ecall # exit
fib:
    addi sp,sp,-8
    sw ra,0(sp)
    sw a0,4(sp)
    add t0,x0,x0 # t0=0
    beq t0,a0,ret_zero # argument n in register a0
    addi t0,t0,1 # t0=1
    beq t0,a0,ret_one
    addi a0,a0,-1 # calc (n-1)
    jal ra,fib # call fib(n-1)
    add t0,x0,a0 # t0=fib(n-1)
    lw a0,4(sp) # restore old n
    sw t0,4(sp) # backup intermediate result
    addi a0,a0,-2 # calc (n-2)
    jal ra,fib # call fib(n-2)
    lw t0,4(sp) # restore intermediate result
    add a0,t0,a0 #return value=fib(n-1)+fib(n-2)
    beq x0,x0,exit #unconditional jump 
ret_zero:
    add a0,x0,x0 #a0 for return value
    beq x0,x0,exit #unconditional jump
ret_one:
    addi a0,x0,1 #a0 for return value
exit:
    lw ra,0(sp)
    addi sp,sp,8
    jalr x0, ra, 0