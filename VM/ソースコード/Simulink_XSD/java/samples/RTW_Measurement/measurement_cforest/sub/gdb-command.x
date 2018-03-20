target remote localhost:50004
file measure
load measure
# dummy load PE2,PE3,PE4
load measure 0x88000000
load measure 0x90000000
load measure 0x98000000

break exit

c
q
